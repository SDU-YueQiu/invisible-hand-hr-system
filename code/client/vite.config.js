import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import path from 'path'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
      '@': path.resolve(__dirname, 'src')
    }
  },
  server: {
    host: true, // 允许外部访问
    port: 5173, // 确保端口与内网穿透配置一致
    allowedHosts: [
      'frp-pet.com' // 添加允许访问的域名
    ]
  }
  // server: {
  //   proxy: {
  //     '/api/v1': {
  //       target: 'http://localhost:3001',
  //       changeOrigin: true,
  //       rewrite: (path) => path.replace(/^\/api\/v1/, '')
  //     }
  //   }
  // }
})

