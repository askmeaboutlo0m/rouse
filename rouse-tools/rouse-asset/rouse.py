#!/usr/bin/env python
# Copyright (c) 2019 askmeaboutloom
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
from gimpfu    import *
from gimpenums import *
import json
import os
import re
import sys
import traceback


class Rouse:
    def __init__(self, image, dirname):
        self.total   = 0
        self.count   = 0
        self.image   = image
        self.tree    = []
        self.dirname = dirname

        if not os.path.exists(dirname):
            os.makedirs(dirname)

        for child in self.image.layers:
            self.remove_comments(child)

        for child in self.image.layers:
            self.recurse_layers(child, self.tree)
        sys.stderr.write("\n")

        with open(os.path.join(self.dirname, "tree.json"), "w") as fh:
            json.dump(self.tree, fh, sort_keys=True, indent=4,
                      separators=(",", ": "))


    def remove_comments(self, layer):
        if re.search(r'^\s*//', layer.name):
            self.image.remove_layer(layer)
        else:
            self.total += 1
            if hasattr(layer, "layers"):
                for child in layer.layers:
                    self.remove_comments(child)


    def dump_image(self, layer, info):
        img = gimp.Image(info["width"], info["height"], self.image.base_type)
        lyr = pdb.gimp_layer_new_from_drawable(layer, img)
        img.add_layer(lyr, 0)

        message = "[png] {0}/{1}".format(self.count, self.total)
        blanks  = " " * len(message)
        sys.stderr.write("\r" + blanks + "\r" + message)

        pdb.file_png_save(img, img.active_layer, info["png"],
                          info["png"], 0, 9, 0, 0, 0, 0, 0)
        gimp.delete(img)


    def recurse_layers(self, layer, tree):
        match = re.search(r'^\s*(@?)\s*(.+)\s*$', layer.name.strip())
        if not match:
            raise ValueError("can't handle layer name '{0}'".format(layer.name))

        name   = "{0}{1}".format(*match.group(1, 2))
        raster = bool(re.search(r'!svg', name))
        vector = bool(re.search(r'=svg', name))
        if raster:
            name = re.sub(r'\s*!svg\s*', "", name)
        if vector:
            name = re.sub(r'\s*=svg\s*', "", name)

        safe = "{0:0>3}-{1}".format(self.count, re.sub(r'\W', "_", name))
        self.count += 1

        info = {
            "name"     : name,
            "png"      : os.path.join(self.dirname, "{0}.png".format(safe)),
            "raster"   : raster,
            "vector"   : vector,
            "frames"   : bool(match.group(1)),
            "x"        : layer.offsets[0],
            "y"        : layer.offsets[1],
            "width"    : layer.width,
            "height"   : layer.height,
            "children" : [],
        }

        if hasattr(layer, "layers"):
            for child in layer.layers:
                self.recurse_layers(child, info["children"])

        self.image.active_layer = layer
        self.dump_image(layer, info)
        tree.append(info)

        if not pdb.gimp_item_is_group(layer):
            pdb.gimp_selection_all(self.image)
            pdb.gimp_edit_clear(layer)
            pdb.gimp_selection_none(self.image)


def rouse(image, drawable):
    try:
        Rouse(image, os.environ["ROUSE_DIRNAME"])
        print("\nrouse ok\n")
    except Exception as e:
        traceback.print_exc(e)


register(
    "python_fu_rouse",
    "Rouse",
    "",
    "askmeaboutloom",
    "",
    "2019",
    "<Image>/File/Create/Rouse",
    "*",
    [],
    [],
    rouse,
)

main()
