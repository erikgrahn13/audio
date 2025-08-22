import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
 server: {
    fs: {
      allow: ['../../'] // <-- make sure the path to your widgets folder is allowed
    }
  },
  optimizeDeps: {
    include: ['juce-framework-frontend']
  }
  
})
