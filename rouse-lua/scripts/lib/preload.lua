-- Copyright (c) 2019, 2020 askmeaboutloom
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

local PreloadScene <const> = class()
local asset_types  <const> = {
    "font", "image", "model", "json", "texture", "sound",
}

local function get_empty_loaded()
    local loaded = {}
    for _, type in ipairs(asset_types) do
        loaded[type] = {}
    end
    return loaded
end

function PreloadScene.extract_key_base(path)
    return string.match(path, "([^/%.]+)%.[^/]+$")
end

function PreloadScene.extract_key_dir_base(path)
    return string.match(path, "([^/%.]+/[^/%.]+)%.[^/]+$")
end

function PreloadScene.extract_key_path(path)
    return string.match(path, "^(.*)%.[^/]+$")
end

function PreloadScene:init(args)
    self.nvg                = args.nvg or R.Nvg.new(0)
    self.ms                 = args.ms or 10
    self.extract_key        = args.extract_key or self.extract_key_base
    self.on_done            = args.on_done or error("No on_done callback given")
    self.loaded             = get_empty_loaded()
    self.current            = 1
    self.bytes              = 0
    self.fetch_bytes        = 0
    self.temp_bytes         = 0
    self.bitmap_image_flags = args.bitmap_image_flags

    if args.packfile and not args.packfiles then
        R.warn("The 'packfile' argument is deprecated, use 'packfiles' instead")
        args.packfiles = {args.packfile}
    end

    self.resources, self.total_bytes = self:parse_resources(
            args.resources or error("No resources to load given"))

    self.fetching = R.platform == "emscripten"
    if self.fetching then
        self.fetched    = 0
        local packfiles = self:get_packfiles(args.packfiles)
        if packfiles then
            self.fetch_total_bytes = self:get_total_packfile_bytes(packfiles)
            self:fetch_next_pack(packfiles)
        else
            self.fetch_total_bytes = self.total_bytes
            self:fetch_next_resource()
        end
    else
        self.fetched = #self.resources
    end

    self.outer_color   = args.outer_color   or R.Nvg.rgbaf(0.0, 0.0, 0.0, 0.6)
    self.outer_ratio_x = args.outer_ratio_x or 0.9
    self.outer_ratio_y = args.outer_ratio_y or 0.1
    self.inner_ratio_y = args.inner_ratio_y or 0.8
    self.inner_color   = args.inner_color   or R.Nvg.rgb(255, 255, 255)
    self.roundness     = args.roundness     or 10.0
end

local function maybe_call(value_or_function)
    return is_function(value_or_function)
       and value_or_function()
        or value_or_function
end

function PreloadScene:parse_resources(lines_or_function)
    local lines       = maybe_call(lines_or_function)
    local resources   = {}
    local total_bytes = 0
    for i, line in ipairs(lines) do
        local trimmed      = string.trim(line)
        local digits, path = string.match(trimmed, "^(%d+)%s+(.+)$")
        if digits and path then
            local bytes  = tonumber(digits)
            total_bytes  = total_bytes + bytes
            resources[i] = {path, bytes}
        else
            errorf("Can't parse resource path: '%s'", trimmed)
        end
    end
    return resources, total_bytes
end

function PreloadScene:get_packfiles(packfiles_or_function)
    if packfiles_or_function then
       return maybe_call(packfiles_or_function)
   end
end

function PreloadScene:get_total_packfile_bytes(packfiles)
    local total_bytes = 0
    for i, packfile in ipairs(packfiles) do
        total_bytes = total_bytes + packfile.bytes
    end
    return total_bytes
end


function PreloadScene:fetch_next_pack(packfiles)
    if #packfiles > 0 then
        local packfile    = table.remove(packfiles, 1)
        local on_progress = function (bytes)
            self.temp_bytes = bytes
        end
        local on_done = function ()
            self.temp_bytes  = 0
            self.fetch_bytes = self.fetch_bytes + packfile.bytes
            self:fetch_next_pack(packfiles)
        end
        R.fetch_pack(packfile.url, on_progress, on_done)
    else
        self.fetched = #self.resources
    end
end

function PreloadScene:fetch_next_resource()
    local next_to_fetch = self.fetched + 1
    if next_to_fetch <= #self.resources then
        local path, bytes = table.unpack(self.resources[next_to_fetch])
        local on_progress = function (bytes)
            self.temp_bytes = bytes
        end
        local on_done = function ()
            self.temp_bytes  = 0
            self.fetch_bytes = self.fetch_bytes + bytes
            self.fetched     = next_to_fetch
            self:fetch_next_resource()
        end
        R.fetch(path, path, on_progress, on_done)
    end
end


function PreloadScene:get_bitmap_image_flags(path)
    local flags_or_function = self.bitmap_image_flags
    if flags_or_function then
        if is_function(flags_or_function) then
            return flags_or_function(path)
        else
            return flags_or_function
        end
    else
        return 0
    end
end


PreloadScene.load_type_lua  = false
PreloadScene.load_type_frag = false
PreloadScene.load_type_vert = false

function PreloadScene:load_type_json(key, path)
    return "json", path
end

function PreloadScene:load_type_ogg(key, path)
    if R.al_enabled then
        return "sound", R.Al.Buffer.from_file(path)
    else
        return false, "sound not enabled"
    end
end

function PreloadScene:load_type_png(key, path)
    return "image", R.BitmapImage.from_file(self.nvg, path,
                                            self:get_bitmap_image_flags(path))
end

function PreloadScene:load_type_rvg(key, path)
    return "image", R.VectorImage.from_file(path)
end

function PreloadScene:load_type_rmodel(key, path)
    return "model", R.Model.from_file(path)
end

function PreloadScene:load_type_ttf(key, path)
    return "font", self.nvg:create_font(key, path)
end

PreloadScene["load_type_texture.png"] = function (self, key, path)
    return "texture", R.GL.Texture.new(path)
end

function PreloadScene:load_resource(path)
    local suffix = string.match(path, "%.([^/]+)$")
    local loader = self["load_type_" .. suffix]
    if loader then
        local key                     = self.extract_key(path)
        local prefix, asset_or_reason = loader(self, key, path)
        if prefix then
            if self.loaded[prefix][key] then
                R.warn("Duplicate asset '%s': %s", key, path)
            else
                self.loaded[prefix][key] = asset_or_reason
            end
        else
            R.debug("Not loading '%s': %s", path, asset_or_reason)
        end
    elseif loader ~= false then
        -- A `false` loader means it's intentionally not loaded.
        -- For example, Lua files just need to be fetched for
        -- Emscripten, the loading happens explicitly later.
        -- A `nil` loader will trigger this warning though, since
        -- it's some kind of file extension we don't understand.
        R.warn("Don't know how to load a '%s': %s", suffix, path)
    end
end

function PreloadScene:load_next_resource()
    if self.current <= self.fetched then
        local path, bytes = table.unpack(self.resources[self.current])
        if path then
            self.current = self.current + 1
            self:load_resource(string.trim(path))
            self.bytes = self.bytes + bytes
            return false
        else
            return true
        end
    else
        return true
    end
end

function PreloadScene:on_tick(rendered)
    local limit = SDL.ticks + self.ms
    local done

    repeat
        done = self:load_next_resource()
    until done or SDL.ticks >= limit

    if self.current > #self.resources then
        R.Scene.next(function (scene)
            return self.on_done(scene, self)
        end)
    end
end


function PreloadScene:calculate_ratio(bytes, total_bytes)
    return total_bytes and total_bytes > 0
       and math.min(bytes / math.max(total_bytes, 1), 1.0)
        or 1.0
end

function PreloadScene:on_render()
    R.Viewport.reset()
    R.GL.clear(1.0, 1.0, 1.0, 1.0, 0.0, 0)

    local nvg = self.nvg
    local vp  = R.Viewport.window()
    nvg:begin_frame(vp.w, vp.h, 1.0)

    local fetch_ratio = self:calculate_ratio(self.fetch_bytes + self.temp_bytes,
                                             self.fetch_total_bytes)
    local outer_w = vp.w * self.outer_ratio_x
    local fetch_w = outer_w * fetch_ratio
    local outer_h = vp.h * self.outer_ratio_y
    local outer_x = (vp.w - outer_w) / 2.0
    local outer_y = (vp.h - outer_h) / 2.0

    nvg:fill_color(self.outer_color)
    nvg:begin_path()
    nvg:rounded_rect(outer_x, outer_y, fetch_w, outer_h, self.roundness)
    nvg:fill()

    local ratio   = self:calculate_ratio(self.bytes, self.total_bytes)
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
