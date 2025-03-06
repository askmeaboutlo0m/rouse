#!/usr/bin/env python
# SPDX-License-Identifier: MIT
import json
import krita
import os
import re


class Rouse:
    def __init__(self, dirname):
        self.total = 0
        self.count = 0
        self.export_count = 0
        self.document = krita.Krita.instance().activeDocument()
        self.tree = []
        self.dirname = dirname

        if not os.path.exists(dirname):
            os.makedirs(dirname)

        for child in self.document.topLevelNodes():
            self.remove_comments(child)

        for child in reversed(self.document.topLevelNodes()):
            self.recurse_layers(child, self.tree)

        with open(os.path.join(self.dirname, "tree.json"), "w") as fh:
            json.dump(self.tree, fh, sort_keys=True, indent=4, separators=(",", ": "))

    def remove_comments(self, node):
        name = str(node.name())
        if re.search(r"^\s*//", name):
            node.remove()
        else:
            self.total += 1
            for child in node.childNodes():
                self.remove_comments(child)

    def dump_image(self, node, info):
        self.export_count += 1
        print("[png] {0}/{1}".format(self.export_count, self.total))

        children = []
        if node.type() == "grouplayer":
            bounds = node.bounds()
        else:
            cloned = node.clone()
            bounds = cloned.bounds()
            pos = cloned.position()
            cloned.move(pos.x() - bounds.x(), pos.y() - bounds.y())
            for child in cloned.childNodes():
                child.remove()
            children.append(cloned)

        document = krita.Krita.instance().createDocument(
            bounds.width(), bounds.height(), node.name(), "RGBA", "U8", "", 64.0
        )
        document.setBatchmode(True)
        document.rootNode().setChildNodes(children)

        exportConfiguration = krita.InfoObject()
        exportConfiguration.setProperties(
            {
                "alpha": True,
                "compression": 9,
                "forceSRGB": True,
                "indexed": False,
                "interlaced": False,
                "saveSRGBProfile": False,
                "transparencyFillcolor": [255, 255, 255],
            }
        )
        document.exportImage(info["png"], exportConfiguration)

        document.close()

    def recurse_layers(self, node, tree):
        match = re.search(r"^\s*(@?)\s*(.+)\s*$", str(node.name()).strip())
        if not match:
            raise ValueError("can't handle layer name '{0}'".format(node.name()))

        name = "{0}{1}".format(*match.group(1, 2))
        raster = bool(re.search(r"!svg", name))
        vector = bool(re.search(r"=svg", name))
        if raster:
            name = re.sub(r"\s*!svg\s*", "", name)
        if vector:
            name = re.sub(r"\s*=svg\s*", "", name)

        isolate_match = re.search(r"\s*isolate=(\w+)\s*", name)
        if isolate_match:
            isolate = isolate_match.group(1)
            name = re.sub(r"\s*isolate=\w+\s*", "", name)
        else:
            isolate = None

        alpha_match = re.search(r"\s*alpha=([0-9]+\.[0-9]+)\s*", name)
        if alpha_match:
            alpha = float(alpha_match.group(1))
            name = re.sub(r"\s*alpha=([0-9]+\.[0-9]+)\s*", "", name)
        else:
            alpha = None

        safe = "{0:0>3}-{1}".format(self.count, re.sub(r"\W", "_", name))
        self.count += 1

        bounds = node.bounds()
        info = {
            "name": name,
            "png": os.path.join(self.dirname, "{0}.png".format(safe)),
            "raster": raster,
            "vector": vector,
            "isolate": isolate,
            "alpha": alpha,
            "frames": bool(match.group(1)),
            "x": bounds.x(),
            "y": bounds.y(),
            "width": bounds.width(),
            "height": bounds.height(),
            "children": [],
        }

        for child in reversed(node.childNodes()):
            self.recurse_layers(child, info["children"])

        self.dump_image(node, info)
        tree.append(info)


def rouse():
    Rouse(os.environ["ROUSE_DIRNAME"])


class RouseExtension(krita.Extension):

    def __init__(self, parent):
        super().__init__(parent)

    def setup(self):
        pass

    def createActions(self, window):
        action = window.createAction("rouse", "Rouse")
        action.triggered.connect(rouse)


instance = krita.Krita.instance()
instance.addExtension(RouseExtension(instance))
