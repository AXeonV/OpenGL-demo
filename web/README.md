# Web build

This is the original C++ scene compiled to WebAssembly and WebGL 2. The cube positions, vertex data, light trajectories, material values, procedural shader formulas, and camera calculations are shared with the desktop version.

## Build

Install and activate the official Emscripten SDK **6.0.12**, then run from the repository root:

```sh
node scripts/build-web.mjs
```

Alternatively, specify the SDK without changing the terminal environment:

```sh
node scripts/build-web.mjs --emsdk /path/to/emsdk
```

`--out /path/to/preview` writes a complete standalone page to another directory. Serve it over HTTP; opening the HTML via `file://` cannot load WebAssembly and the asset package reliably.

The default output is the root `index.html` plus `web/app.js`, `web/app.wasm`, and `web/app.data`. Keep these generated assets with the repository so the homepage's existing project synchronization can publish the demo without requiring a C++ toolchain on the web server. Rebuild them after changing C++ or GLSL, then commit the source and build outputs together before syncing the homepage.

## Compatibility changes

- `GLPlatform.h` selects GLES 3 for Web and the existing GLAD headers for desktop.
- The same frame body runs in Emscripten's browser main loop. Its state survives between callbacks; desktop retains the normal GLFW loop.
- Controls retain the desktop mappings: mouse look, WASD movement, wheel zoom, Left Ctrl cursor capture, F11 fullscreen, and Escape to stop the demo. Browsers require an initial canvas click before capturing the mouse; an embedded browser may prohibit capture. The page contains only the canvas. Canvas resizing and focus loss are handled by the Web platform layer.
- Only build-cache copies of legacy-encoded sources are converted to UTF-8. Original files keep their encoding.
- Only generated shader copies use GLSL ES 3.00, explicit precision/output, and portable float syntax. The cube face-coordinate helper receives a fallback for its previously undefined no-return path. The original shader files and procedural rendering formulas are unchanged.

No threads, external CDN resources, or backend are required. The page can be served from a GitHub Pages subdirectory or the Gallery iframe.
