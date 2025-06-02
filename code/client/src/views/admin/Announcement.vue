<template>
  <div class="announcement-container">
    <el-card>
      <template #header>
        <div class="card-header">
          <h2>公告系统</h2>
          <el-button @click="showCreateDialog" icon="Plus">新建公告</el-button>
        </div>
      </template>

      <!-- 筛选条件 -->
      <el-form :inline="true" :model="filterForm" class="filter-form">
        <el-form-item label="置顶状态">
          <el-select v-model="filterForm.isTop" placeholder="请选择">
            <el-option label="全部" value="" />
            <el-option label="置顶" value="true" />
            <el-option label="未置顶" value="false" />
          </el-select>
        </el-form-item>
        <el-form-item>
          <el-button type="primary" @click="fetchAnnouncements">查询</el-button>
        </el-form-item>
      </el-form>

      <!-- 公告列表 -->
      <el-table 
        :data="announcements" 
        border 
        :stripe="true"
        :loading="loading"
      >
        <el-table-column prop="announcementId" label="公告ID" width="80" />
        <el-table-column prop="title" label="标题" />
        <el-table-column prop="publishTime" label="发布时间">
          <template #default="scope">
            {{ formatDate(scope.row.publishTime) }}
          </template>
        </el-table-column>
        <el-table-column prop="isTop" label="置顶">
          <template #default="scope">
            <el-tag v-if="scope.row.isTop" type="success">是</el-tag>
            <el-tag v-else>否</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="操作" width="150">
          <template #default="scope">
            <el-button 
              @click="editAnnouncement(scope.row)"
              size="small"
              type="primary"
            >
              编辑
            </el-button>
            <el-button 
              @click="deleteAnnouncement(scope.row.announcementId)"
              size="small"
              type="danger"
            >
              删除
            </el-button>
          </template>
        </el-table-column>
      </el-table>

      <el-pagination
        v-model:current-page="currentPage"
        v-model:page-size="pageSize"
        :page-sizes="[10, 20, 30, 50]"
        layout="total, sizes, prev, pager, next"
        :total="total"
        @size-change="handleSizeChange"
        @current-change="handleCurrentChange"
      />
    </el-card>

    <!-- 新建/编辑公告对话框 -->
    <el-dialog
      v-model="dialogVisible"
      :title="isEdit ? '编辑公告' : '新建公告'"
      width="60%"
    >
      <el-form :model="form" label-width="100px" :rules="rules" ref="formRef">
        <el-form-item label="标题" prop="title">
          <el-input v-model="form.title" placeholder="请输入公告标题" />
        </el-form-item>
        <el-form-item label="内容" prop="content">
          <!-- 实际项目中应该使用富文本编辑器 -->
          <el-input
            v-model="form.content"
            type="textarea"
            :rows="8"
            placeholder="请输入公告内容"
          />
        </el-form-item>
        <el-form-item label="生效时间" prop="effectiveTime">
          <el-date-picker
            v-model="form.effectiveTime"
            type="datetime"
            placeholder="选择生效时间"
            value-format="YYYY-MM-DD HH:mm:ss"
          />
        </el-form-item>
        <el-form-item label="过期时间" prop="expireTime">
          <el-date-picker
            v-model="form.expireTime"
            type="datetime"
            placeholder="选择过期时间"
            value-format="YYYY-MM-DD HH:mm:ss"
          />
        </el-form-item>
        <el-form-item label="置顶" prop="isTop">
          <el-switch v-model="form.isTop" />
        </el-form-item>
      </el-form>
      <template #footer>
        <span class="dialog-footer">
          <el-button @click="dialogVisible = false">取消</el-button>
          <el-button type="primary" @click="submitAnnouncement">
            {{ isEdit ? '更新' : '发布' }}
          </el-button>
        </span>
      </template>
    </el-dialog>
  </div>
</template>

<script setup>
import { ref, reactive } from 'vue'
import { ElMessage, ElMessageBox } from 'element-plus'
import request from '../../utils/request'

const announcements = ref([])
const loading = ref(true)
const currentPage = ref(1)
const pageSize = ref(10)
const total = ref(0)

const filterForm = ref({
  isTop: ''
})

// 表单相关
const dialogVisible = ref(false)
const isEdit = ref(false)
const form = reactive({
  announcementId: null,
  title: '',
  content: '',
  effectiveTime: '',
  expireTime: '',
  isTop: false
})

// 表单验证规则
const rules = {
  title: [
    { required: true, message: '请输入公告标题', trigger: 'blur' },
    { min: 2, max: 100, message: '标题长度在2到100个字符之间', trigger: 'blur' }
  ],
  content: [
    { required: true, message: '请输入公告内容', trigger: 'blur' }
  ]
}

// 获取公告列表
const fetchAnnouncements = async () => {
  try {
    loading.value = true
    const res = await request.get('/admin/announcements', {
      params: {
        page: currentPage.value,
        limit: pageSize.value,
        isTop: filterForm.value.isTop
      }
    })
    
    if (res.success) {
      announcements.value = res.data.announcements
      total.value = res.data.totalItems
    }
  } catch (error) {
    console.error('获取公告列表失败', error)
    ElMessage.error('获取公告列表失败')
  } finally {
    loading.value = false
  }
}

// 显示新建公告对话框
const showCreateDialog = () => {
  resetForm()
  isEdit.value = false
  dialogVisible.value = true
}

// 编辑公告
const editAnnouncement = (announcement) => {
  Object.assign(form, announcement)
  isEdit.value = true
  dialogVisible.value = true
}

// 删除公告
const deleteAnnouncement = async (announcementId) => {
  try {
    await ElMessageBox.confirm(
      '此操作将永久删除该公告，是否继续？',
      '警告',
      {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }
    )

    const res = await request.delete(`/admin/announcements/${announcementId}`)

    if (res.success) {
      ElMessage.success('公告删除成功')
      fetchAnnouncements()
    }
  } catch (error) {
    if (error !== 'cancel') {
      console.error('删除失败', error)
      ElMessage.error('删除失败')
    }
  }
}

// 提交公告
const submitAnnouncement = async () => {
  try {
    // 这里可以添加表单验证逻辑
    if (!form.title.trim() || !form.content.trim()) {
      ElMessage.warning('请填写完整信息')
      return
    }

    if (isEdit.value) {
      // 更新公告
      const res = await request.put(`/admin/announcements/${form.announcementId}`, {
        ...form
      })

      if (res.success) {
        ElMessage.success('公告更新成功')
        dialogVisible.value = false
        fetchAnnouncements()
      }
    } else {
      // 创建公告
      const res = await request.post('/admin/announcements', {
        ...form
      })

      if (res.success) {
        ElMessage.success('公告发布成功')
        dialogVisible.value = false
        fetchAnnouncements()
      }
    }
  } catch (error) {
    console.error('操作失败', error)
    ElMessage.error('操作失败')
  }
}

// 重置表单
const resetForm = () => {
  form.announcementId = null
  form.title = ''
  form.content = ''
  form.effectiveTime = ''
  form.expireTime = ''
  form.isTop = false
}

// 分页处理
const handleSizeChange = (newSize) => {
  pageSize.value = newSize
  fetchAnnouncements()
}

const handleCurrentChange = (newPage) => {
  currentPage.value = newPage
  fetchAnnouncements()
}

// 格式化日期
const formatDate = (dateString) => {
  if (!dateString) return '未知'
  const date = new Date(dateString)
  return date.toLocaleString('zh-CN', {
    year: 'numeric',
    month: '2-digit',
    day: '2-digit',
    hour: '2-digit',
    minute: '2-digit'
  })
}

// 初始化加载数据
fetchAnnouncements()
</script>

<style scoped>
.announcement-container {
  padding: 20px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.filter-form {
  margin-bottom: 20px;
}
</style>