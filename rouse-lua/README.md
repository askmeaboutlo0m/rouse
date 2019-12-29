# NAME

rouse-lua - Lua scripting on top of rouse

# SYNOPSIS

Build rouse first. Then build this, passing `-DROUSE_EXPORT_FILE=path/to/rouse/build/directory/Rouse.cmake` to CMake so it can take the exports from there.

Then try running the resulting `lrouse` executable with `--main=examples/ease.lua`.

# DESCRIPTION

These are [Lua](https://www.lua.org/) bindings for rouse. They use Lua 5.3.5 right now.

Since it's annoying to write these bindings entirely by hand, there's a preprocessor dubbed XL in the [xlua directory](xlua), inspired by [Perl's XS](https://perldoc.perl.org/perlxs.html). It's very ugly and weird, but it reduces the amount of code required by a huge amount.

There's some support Lua code in [examples/lib](examples/lib), which provides a nicer and more fluent interface on top of the raw C bindings.

# KNOWN ISSUES

The bindings are missing a bunch of stuff. Especially events aren't implemented fully.

The XL parser and generator code are *awful*. They really need to be cleaned up.

# LICENSE

Copyright 2019 askmeaboutloom. Licensed under the MIT license, see [the LICENSE file](../LICENSE) for details.

The copy of Lua 5.3.5 included is third-party software, see its [LICENSE](lib/rouse/lua_5_3_5/LICENSE) for details.
