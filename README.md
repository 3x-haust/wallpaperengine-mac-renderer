# wallpaperengine-mac-renderer

macOS port of [Almamu/linux-wallpaperengine](https://github.com/Almamu/linux-wallpaperengine) — an OpenGL reimplementation of Wallpaper Engine's scene renderer.

This fork exists to power scene (`scene.pkg`) playback in [Workshop Wallpaper Bridge](https://github.com/3x-haust/workshop-wallpaper-bridge), a macOS menu-bar app that plays locally copied Wallpaper Engine wallpapers as desktop backgrounds. The renderer runs **offscreen only** here: it records a scene into frames that the app encodes into a looping video wallpaper. No visible window, no desktop integration of its own.

All credit for the scene engine itself goes to [@Almamu](https://github.com/Almamu) and the upstream contributors. See the [upstream README](https://github.com/Almamu/linux-wallpaperengine#readme) for the original project, full feature docs, and Linux usage.

## What this fork changes

- **macOS build** (Apple Silicon, OpenGL 4.1 core): CMake builds against Homebrew deps; X11/Wayland/CEF paths are compiled out in the scene-only target.
- **Strict-GL correctness fixes** that Apple's OpenGL front-end surfaces but Mesa tolerates: shader macro/helper redefinition conflicts with the real Wallpaper Engine assets, FBO format parsing (`rgba16161616f` float buffers), texture-size detection for `composelayer` bridge objects (fixes half-screen bloom blobs and blurry effect passes), and final-blit UV clamping.
- **Scene-faithful HDR pipeline**: scenes authored with `hdr: true` composite in float buffers on every platform and are tonemapped once at the final blit with an exponential knee, so bloom/shine thresholds see the brightness values the scene was authored against.
- **Offscreen recording**:
  - `--record-dir <dir>` — deterministic PNG frame sequence (fixed `1/fps` timestep).
  - `--record-raw <path>` — tightly-packed RGBA frames streamed to a file or FIFO through a triple-buffered writer thread, for piping straight into an encoder (prints a `RECORD_RAW format=rgba width=W height=H fps=F frames=N` header). About 4x faster than the PNG path.
  - `--record-fps`, `--record-seconds`, exact `--window WxH` output size (no Retina doubling), clean exit code for scripting.
  - `--record-exclude-live` — skip live-only elements (e.g. clocks) when baking a loop.
- **Debugging aids**: `--dump-passes <dir>` (per-pass FBO PNGs), `--gl-debug` (glGetError/FBO-status sweeps).
- Requires the real Wallpaper Engine `assets` folder (from your own Windows install) via `--assets-dir`; missing assets abort with the exact file list instead of rendering black.

## Building (macOS)

```bash
brew install cmake lz4 sdl2 ffmpeg glfw glew glm mpv freetype
cmake -B build/macos-scene -DCMAKE_BUILD_TYPE=Release
cmake --build build/macos-scene
```

## Recording a scene

```bash
./output/wwb-scene-renderer \
  --window 0x0x1920x1080 --silent --no-audio-processing --disable-mouse \
  --record-raw /tmp/frames.rgba --record-fps 30 --record-seconds 20 \
  --assets-dir "/path/to/wallpaper_engine/assets" \
  "/path/to/workshop/scene-folder"

ffmpeg -f rawvideo -pix_fmt rgba -s 1920x1080 -r 30 -i /tmp/frames.rgba \
  -c:v libx264 -pix_fmt yuv420p -crf 18 scene.mp4
```

## License

GPL-3.0, same as upstream. This repository publishes the complete corresponding source for the renderer binary bundled with Workshop Wallpaper Bridge releases.
