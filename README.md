# NAME

rouse - cross-platform interactive animation engine

# SYNOPSIS

Flash Player is dead and I still want to make interactive animations. This is my replacement.

The main library is written in C and there's Lua bindings on top of that. It's confirmed to run on Linux, Windows and, thanks to Emscripten, in just about any browser you can get your hands on, even on mobile devices. It should also work natively on macOS, iOS and Android, but I haven't gotten around to trying that.

# DESCRIPTION

There's a few sub-projects here:

* [rouse](rouse) contains the main library.

* [rouse-lua](rouse-lua) has the Lua bindings.

* [rouse-tools](rouse-tools) is a collection of miscellaneous scripts.

# KNOWN ISSUES

This thing is still severely undercooked. Docs are missing.

I want to add some scripts to make building easier. Something to build in debug mode, release mode, cross-compile to Windows and do an Emscripten build.

A script to generate a new animation project would also be very useful. While on a native platform it's simple, making it compile with Emscripten is non-trivial.

There should probably be a link to a Discord server or something where people can go to ask questions if they actually use this. Otherwise I may end up getting enough direct communication that I can't help everyone.

# LICENSE

Copyright 2019 askmeaboutloom. Licensed under the MIT license, see [the LICENSE file](LICENSE) for details.

Licenses for third-party software are marked where applicable.
