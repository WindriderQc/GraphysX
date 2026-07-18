import { dirname, resolve } from "node:path";
import { fileURLToPath } from "node:url";
import { defineConfig } from "vite";

const here = dirname(fileURLToPath(import.meta.url));

export default defineConfig({
  build: {
    outDir: "output/vehicle-pack-inspector-build",
    emptyOutDir: true,
    rollupOptions: {
      input: resolve(here, "vehicle-pack-inspector.html")
    }
  }
});
