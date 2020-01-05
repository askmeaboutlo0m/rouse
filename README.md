# NAME

rouse - cross-platform interactive animation engine

# SYNOPSIS

Flash Player is dead and I still want to make interactive animations. This is my replacement.

The main library is written in C and there's Lua bindings on top of that. It's confirmed to run on Linux, Windows and, thanks to Emscripten, in just about any browser you can get your hands on, even on mobile devices. It should also work natively on macOS, iOS and Android, but I haven't gotten around to trying that.

Check out a sample animation here: <https://askmeaboutlo0m.github.io/rouse-sampleanim/index.html>

# DESCRIPTION

There's a few sub-projects here:

* [rouse](rouse) contains the main library.

* [rouse-lua](rouse-lua) has the Lua bindings.

* [rouse-tools](rouse-tools) is a collection of miscellaneous scripts.

## Discord

If you actually want to use this and have questions or need help, there's a rouse-devel Discord server you can join:

<https://discord.gg/fhCtYK2>

## Building

This project uses [CMake](https://cmake.org/). You need version 3.14 or newer.

Perl version 5.20 is required for several code generation scripts involved. It should come with most Linux distributions and macOS.

You need SDL2 and SDL2\_image. Those should be available in your package manager. You also need [cglm](https://github.com/recp/cglm), which you probably need to build from source.

I've only tried building this on Linux with gcc and clang. It should work on other systems as well, but probably needs adjustments.

Check out [the metamake script](metamake), it's what works for me. It uses [Ninja](https://ninja-build.org/) as the CMake generation target, so either install that (via your package manager ideally) or replace it with `make` calls. It supports the following build modes:

* `metamake debug` to build for Linux in debug mode.

* `metamake release` for Linux release mode.

* `metamake emscripten` to build a browser version. You need [Emscripten](https://emscripten.org/) and set the `EMPREFIX` environment variable to where you installed cglm.

* `metamake windows` to cross-compile to Windows. This requires [Docker](https://www.docker.com/), but nothing else, as it'll bootstrap itself.

# KNOWN ISSUES

This thing is still severely undercooked. Docs are missing.

# LICENSE

Code is copyright 2019, 2020 askmeaboutloom. Licensed under the MIT license, see [the LICENSE file](LICENSE) for details.

Images and models are copyright 2019, 2020 askmeaboutloom, licensed under a [Creative Commons Attribution 4.0 International license (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/).

Licenses for third-party software and other works are marked where applicable.
