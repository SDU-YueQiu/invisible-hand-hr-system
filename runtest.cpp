#include <gtest/gtest.h>
#include "../server/Config/config.h"
#include "../server/Model/adminUser.h"
#include "../server/Model/announcement.h"
#include "../server/Main/main.h"

// ��ʼ������
class SystemIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ��ʼ�����û�ģ������
        config = Config();
        adminUser = AdminUser("admin", "password");
        announcement = Announcement();
    }

    void TearDown() override {
        // ������Դ
    }

    Config config;
    AdminUser adminUser;
    Announcement announcement;
};

// �������ü���
TEST_F(SystemIntegrationTest, ConfigLoadTest) {
    ASSERT_TRUE(config.loadConfig("../config/config.json"));
}

// ���Թ���Ա�û�������Ȩ��
TEST_F(SystemIntegrationTest, AdminUserTest) {
    EXPECT_EQ(adminUser.getUsername(), "admin");
    EXPECT_TRUE(adminUser.verifyPassword("password"));

    adminUser.setPermission("read", true);
    EXPECT_TRUE(adminUser.hasPermission("read"));
}

// ���Թ���ģ��
TEST_F(SystemIntegrationTest, AnnouncementTest) {
    announcement.setTitle("Test Announcement");
    announcement.setContent("This is a test announcement.");

    EXPECT_EQ(announcement.getTitle(), "Test Announcement");
    EXPECT_EQ(announcement.getContent(), "This is a test announcement.");

    announcement.publish();
    EXPECT_TRUE(announcement.isPublished());
}

// ����ϵͳ��ģ��
TEST_F(SystemIntegrationTest, MainModuleTest) {
    // ģ��ϵͳ��ʼ��
    ASSERT_NO_THROW(main::initializeSystem());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

