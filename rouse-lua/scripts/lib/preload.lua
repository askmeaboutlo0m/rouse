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

    if R.get_platform() == "emscripten" then
        self.fetched = 0
        self:fetch_next_resource()
    else
        self.fetched = #args.resources
    end

    self.outer_color   = args.outer_color   or R.Nvg.rgbaf(0.0, 0.0, 0.0, 0.6)
    self.outer_ratio_x = args.outer_ratio_x or 0.9
    self.outer_ratio_y = args.outer_ratio_y or 0.1
    self.inner_ratio_y = args.inner_ratio_y or 0.8
    self.inner_color   = args.inner_color   or R.Nvg.rgb(255, 255, 255)
    self.roundness     = args.roundness     or 10.0
end


function PreloadScene:fetch_next_resource()
    local next_to_fetch = self.fetched + 1
    if next_to_fetch <= #self.resources then
        local path = self.resources[next_to_fetch]
        R.fetch(path, path, function ()
            self.fetched = next_to_fetch
            self:fetch_next_resource()
        end)
    end
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
    if self.current <= self.fetched then
        local path = self.resources[self.current]
        if path then
            self.current = self.current + 1
            self:load_resource(string.trim(path))
            return false
        else
            return true
        end
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

    if self.current > #self.resources then
        R.Scene.next(function (scene)
            return self.on_done(scene, self)
        end)
    end
end


function PreloadScene:on_render()
    R.Viewport.reset()
    R.GL.clear(1.0, 1.0, 1.0, 1.0, 0.0, 0)

    local nvg = self.nvg
    local vp  = R.Viewport.window()
    nvg:begin_frame(vp.w, vp.h, 1.0)

    local outer_w = vp.w * self.outer_ratio_x
    local outer_h = vp.h * self.outer_ratio_y
    local outer_x = (vp.w - outer_w) / 2.0
    local outer_y = (vp.h - outer_h) / 2.0

    nvg:fill_color(self.outer_color)
    nvg:begin_path()
    nvg:rounded_rect(outer_x, outer_y, outer_w, outer_h, self.roundness)
    nvg:fill()

    local ratio   = self.current / math.max(#self.resources, 1.0)
    local inner_h = outer_h * self.inner_ratio_y
    local delta   = outer_h - inner_h
    local inner_w = (outer_w - delta) * ratio
    local inner_x = outer_x + delta / 2.0
    local inner_y = outer_y + delta / 2.0

    nvg:fill_color(self.inner_color)
    nvg:begin_path()
    nvg:rounded_rect(inner_x, inner_y, inner_w, inner_h, self.roundness)
    nvg:fill()

    nvg:end_frame()
end


return PreloadScene
