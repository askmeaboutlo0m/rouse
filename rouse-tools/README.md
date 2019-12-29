# NAME

rouse-tools - miscellaneous toolage around rouse

# SYNOPSIS

At the time of writing, this is basically my asset pipeline. It's pretty ugly stuff, but maybe someone can find something useful here.

This needs documentation and probably some cleanup.

# DESCRIPTION

There's the following tools here:

* [rouse-asset](rouse-asset)

Scripts to pull apart layered images (e.g. PSD files) into separate PNG and SVG assets, with an accompanying JSON file describing how to reassemble them. Uses GIMP for image manipulation, potrace to trace bitmaps to vector images and svgo to strip down the resulting SVGs.

* [rouse-svg](rouse-svg)

Transforms a subset of SVG images into rouse's internal vector image format. It can also generate C code with direct NanoVG calls, but I don't think that's particularly useful really.

# KNOWN ISSUES

This is terrible, undocumented, hacky code. I'm very happy with it, but it'll probably only bring you grief.

# LICENSE

Copyright 2019 askmeaboutloom. Licensed under the MIT license, see [the LICENSE file](../LICENSE) for details.
