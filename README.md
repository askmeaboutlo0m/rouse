# NAME

rouse - a library for interactive animation

# SYNOPSIS

See `examples` for now I guess.

# DESCRIPTION

This is my replacement for Flash Player going away. It uses OpenGL ES 2.0 and runs on just about anything.

## Building

This uses CMake. Dependencies are [cglm](https://github.com/recp/cglm), SDL2 and SDL2\_image. You'll probably have to build cglm yourself, the others are generally available pre-built.

* Debug build:

```sh
cmake -B build -G Ninja
```

* Release build:

```sh
cmake -B buildrelease -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DUSE_ASAN=OFF \
    -DBUILD_WITH_MAGIC=OFF \
    -DBUILD_WITH_GL_CHECKS=OFF \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
```

* Emscripten release build, spews some warnings during linking about `-Bstatic` and `-Bdynamic`, but seems to work otherwise:

```sh
EMPREFIX=/path/to/prefix
emcmake cmake -B buildem -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=cross/emscripten.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DUSE_ASAN=OFF \
    -DUSE_PKGCONFIG=OFF \
    -DBUILD_WITH_MAGIC=OFF \
    -DBUILD_WITH_GL_CHECKS=OFF \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON \
    -DCGLM_CFLAGS="-Isystem$EMPREFIX/include" \
    -DCGLM_LDFLAGS="-L$EMPREFIX/lib" \
    -DCGLM_LIBS='-lm libcglm.a' \
    -DSDL2_CFLAGS=-sUSE_SDL=2 \
    -DSDL2_LDFLAGS=-sUSE_SDL=2 \
    -DSDL2_IMAGE_CFLAGS='-sUSE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"]' \
    -DSDL2_IMAGE_LDFLAGS='-sUSE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"]' \
    -DCMAKE_C_COMPILER="$(pwd)/cross/emccwrap" \
    -DCMAKE_C_COMPILER_AR="$(which emar)" \
    -DCMAKE_C_COMPILER_RANLIB="$(which emranlib)"
```

## Recording

You can dump framebuffers to stdout and pipe the result into ffmpeg to make an in-engine recording. To do so, set `R_max_ticks_before_render` to `1` and call `R_frame_buffer_write` every frame. Don't output anything else on stdout of course.

The command you need to pipe this into is something like:

```sh
DIMENSIONS=1920x1080
FRAMERATE=60
OUTPUT="recording_$(date '+%Y-%m-%dT%H:%M:%S').mp4"
./your-program | ffmpeg -f rawvideo -pix_fmt bgr32 -s:v "$DIMENSIONS" \
                        -r "$FRAMERATE" \ -i pipe:0 -c:v libx264 \
                        -pix_fmt yuv420p "$OUTPUT"
```

# KNOWN ISSUES

This thing is totally not done, particularly the 3D parts. Use at your own risk.

The mixture of a Flash-esque 2D library and a really low-level 3D library is a bit strange. Maybe that should be torn apart sometime.

# LICENSE

Copyright 2019 askmeaboutloom. Licensed under the MIT license, see [the LICENSE file](LICENSE) for details.

Licenses for third-party software can be found in the respective files under the directory [lib/rouse/3rdparty](lib/rouse/3rdparty).

The license for the DejaVu fonts used for tests and examples can be found in [test/data/dejavu/LICENSE](test/data/dejavu/LICENSE).
