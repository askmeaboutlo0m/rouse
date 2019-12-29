-- Copyright (c) 2019 askmeaboutloom
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.

local PreloadScene = class()


function PreloadScene:init(args)
    self.nvg       = args.nvg or R.Nvg.new(0)
    self.ms        = args.ms or 10
    self.resources = args.resources or error("No resources to load given")
    self.on_done   = args.on_done   or error("No on_done callback given")
    self.loaded    = {font = {}, image = {}, json = {}}
    self.current   = 1
end


function PreloadScene:load_type_json(key, path)
    return "json", R.Json.parse_file(path)
end

function PreloadScene:load_type_png(key, path)
    return "image", R.BitmapImage.from_file(self.nvg, path, 0)
end

function PreloadScene:load_type_rvg(key, path)
    return "image", R.VectorImage.from_file(path)
end

function PreloadScene:load_type_ttf(key, path)
    return "font", self.nvg:create_font(key, path)
end

function PreloadScene:load_resource(path)
    local key, suffix = string.match(path, "([^/]+)%.([^%./]+)$")
    local loader      = self["load_type_" .. suffix]
    if loader then
        local prefix, asset = loader(self, key, path)
        if self.loaded[prefix][key] then
            R.warn("Duplicate asset '%s': %s", key, path)
        else
            self.loaded[prefix][key] = asset
        end
    else
        R.warn("Don't know how to load a '%s': %s", suffix, path)
    end
end

function PreloadScene:load_next_resource()
    local path = self.resources[self.current]
    if path then
        self.current = self.current + 1
        self:load_resource(string.trim(path))
        return false
    else
        return true
    end
end

function PreloadScene:on_tick(rendered)
    local limit = SDL.get_ticks() + self.ms
    local done

    repeat
        done = self:load_next_resource()
    until done or SDL.get_ticks() >= limit

    if done then
        R.Scene.next(function (scene)
            return self.on_done(scene, self)
        end)
    end
end


function PreloadScene:on_render()
    R.Viewport.reset()
    R.GL.clear(0.0, 0.0, 0.0, 1.0, 0.0, 0)

    local nvg = self.nvg
    nvg:begin_frame(1920.0, 1080.0, 1.0)
    nvg:begin_path()

    local ratio = self.current / math.max(#self.resources, 1)
    nvg:fill_color(R.Nvg.rgb(255, 255, 255))
    nvg:rect(0.0, 0.0, 1920.0 * ratio, 1080.0)

    nvg:fill()
    nvg:end_frame()
end


return PreloadScene
