import { fileURLToPath } from "node:url";
import { defineConfig } from "vite";

export default defineConfig({
  build: {
    outDir: "output/suzanne-fidelity-build",
    emptyOutDir: true,
    rollupOptions: {
      input: fileURLToPath(new URL("./suzanne-ascii-preview.html", import.meta.url))
    }
  }
});
