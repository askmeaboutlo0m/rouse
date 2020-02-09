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

local SeqBuilder = dofile("scripts/lib/seqbuilder.lua")
local SceneBase  = class()

function SceneBase:init(args)
    self.scene          = args.scene  or error("no scene given")
    self.nvg            = args.nvg    or error("no nvg given")
    self.width          = args.width  or error("no width given")
    self.height         = args.height or error("no height given")
    self.assets         = args.assets or {}
    self.frame_renderer = args.frame_renderer
    self.frame_buffer   = args.frame_buffer
    self.root           = R.Sprite.new("root")
    self.clear_color    = R.Nvg.rgbaf(1.0, 1.0, 1.0, 1.0)
    self.sprites        = {}
    R.Viewport.set_window(self.width, self.height)
    self:init_render_function()
end

function SceneBase:init_render_function()
    local nvg    = self.nvg
    local width  = self.width
    local height = self.height
    local root   = self.root

    if self.frame_renderer and self.frame_buffer then
        local fr    = self.frame_renderer
        local fb    = self.frame_buffer
        local fb_w  = fb.width
        local fb_h  = fb.height
        local fb_vp = R.Viewport.new(0, 0, fb_w, fb_h)

        self.render = function ()
            fb:bind()
            local r, g, b, a = self.clear_color:unpack()
            R.GL.clear(r, g, b, a, 0.0, 0)
            root:draw(nvg, width, height, fb_w, fb_h)
            fb:unbind()
            R.GL.clear(r, g, b, a, 0.0, 0)
            fr:draw(fb)
        end
    else
        self.render = function ()
            local r, g, b, a = self.clear_color:unpack()
            R.GL.clear(r, g, b, a, 0.0, 0)
            local vp = self.viewport or self:reset_viewport()
            root:draw(nvg, width, height, vp.w, vp.h)
        end
    end
end


function SceneBase:on_event(event)
    if event.type == SDL.EventType.WINDOWEVENT and
       event.window.event == SDL.WindowEventType.SIZE_CHANGED then
        self.viewport = nil
    end
end

function SceneBase:reset_viewport()
    R.Viewport.reset()
    local vp      = R.Viewport.window()
    self.viewport = vp
    return vp
end

function SceneBase:on_render()
    self.render()
end

function SceneBase:asset(prefix, key)
    local a = self.assets[prefix] or errorf(
        "Asset prefix '%s' not found (looking for '%s')", prefix, key)
    return a[key] or errorf("Asset '%s' not found (in %s)", key, prefix)
end

function SceneBase:font_asset(key)
    return self:asset("font", key)
end

function SceneBase:image_asset(key)
    return self:asset("image", key)
end

function SceneBase:json_asset(key)
    return self:asset("json", key)
end


function SceneBase:sprite(...)
    local retvals = {}
    for i, name in ipairs {...} do
        retvals[i] = self.sprites[name] or errorf("Sprite '%s' not found", name)
    end
    return table.unpack(retvals)
end


function SceneBase:figure_out_content(name, content)
    if content == nil then
        return name and self:image_asset(name) or nil
    elseif content == false then
        return nil
    elseif type(content) == "string" then
        return self:image_asset(content)
    else
        return content
    end
end

function SceneBase:figure_out_parent(parent)
    if parent == nil then
        return self.root
    elseif parent == false then
        return nil
    elseif type(parent) == "string" then
        return self:sprite(parent)
    else
        return parent
    end
end

function SceneBase:add(spec)
    local name = spec.name
    if name and self.sprites[name] then
        errorf("Duplicate sprite name '%s'", name)
    end

    local sprite = R.Sprite.new(name)
    if name then
        self.sprites[name] = sprite
    end

    local content = self:figure_out_content(name, spec.content)
    if content then
        sprite.content = content
    end

    local parent = self:figure_out_parent(spec.parent)
    if parent then
        local index = spec.index
        if index then
            parent:add_child_at(sprite, index)
        else
            parent:add_child(sprite)
        end
    end

    local x = spec.world_x or spec.x or 0.0
    local y = spec.world_y or spec.y or 0.0

    if (spec.world_x or spec.world_y) and parent then
        x = x - parent.world_pos_x
        y = y - parent.world_pos_y
    end

    if spec.angle    then sprite.angle    = spec.angle    end
    if spec.rotation then sprite.rotation = spec.rotation end

    local scale_x = spec.scale_x or spec.scale
    local scale_y = spec.scale_y or spec.scale
    if scale_x then sprite.scale_x = scale_x end
    if scale_y then sprite.scale_y = scale_y end

    if spec.pivot_x then
        local origin_x  = content.width * spec.pivot_x
        sprite.origin_x = origin_x
        if spec.pos_from_origin or spec.x_from_origin then
            x = x + origin_x
        end
    end

    if spec.pivot_y then
        local origin_y  = content.height * spec.pivot_y
        sprite.origin_y = origin_y
        if spec.pos_from_origin or spec.y_from_origin then
            y = y + origin_y
        end
    end

    sprite.x = x
    sprite.y = y

    sprite:reset_base()

    if spec.track then
        sprite:track(self:maybe_sprite(spec.track))
    end

    return sprite
end

function SceneBase:remove(sprite_or_name)
    local sprite, name
    if type(sprite_or_name) == "string" then
        sprite = self:sprite(sprite_or_name)
        name   = sprite_or_name
    else
        sprite = sprite_or_name
        name   = sprite.name
    end
    sprite:orphan()
    if name then
        self.sprites[name] = nil
    end
end


function SceneBase:maybe_look_up(object_or_name, lookup_fn)
    if type(object_or_name) == "string" then
        return lookup_fn(self, object_or_name)
    else
        return object_or_name
    end
end

function SceneBase:maybe_sprite(sprite_or_name)
    return self:maybe_look_up(sprite_or_name, self.sprite)
end

function SceneBase:maybe_image_asset(asset_or_name)
    return self:maybe_look_up(asset_or_name, self.image_asset)
end

function SceneBase:frame(sprite_or_name, asset_or_name)
    local sprite   = self:maybe_sprite(sprite_or_name)
    local asset    = self:maybe_image_asset(asset_or_name)
    sprite.content = asset
    return sprite, asset
end

function SceneBase:seq(topic_or_name, ...)
    local topic = self:maybe_sprite(topic_or_name)
    return SeqBuilder.new(self, topic, ...)
end


function SceneBase:kill_all()
    self.scene:kill_all()
end

function SceneBase:kill_by_id(id)
    self.scene:kill_by_id(id)
end


local ErrorScene = class(SceneBase)

function ErrorScene:init(scene, nvg, assets, next_scene_fn)
    self.super.init(self, {
        scene  = scene,
        nvg    = nvg,
        assets = assets,
        width  = 1280,
        height = 720,
    })
    self.clear_color       = R.Nvg.rgbaf(1.0, 0.0, 0.0, 1.0)
    self.next_scene_fn     = next_scene_fn
    self.waiting_for_input = true
    self.input_event_types = {
        [SDL.EventType.CONTROLLERBUTTONDOWN] = true,
        [SDL.EventType.FINGERDOWN]           = true,
        [SDL.EventType.JOYBUTTONDOWN]        = true,
        [SDL.EventType.KEYDOWN]              = true,
        [SDL.EventType.MOUSEBUTTONDOWN]      = true,
    }
end

function ErrorScene:on_event(event)
    self.super.on_event(self, event)
    if self.waiting_for_input and self.input_event_types[event.type] then
        self:next_scene(self.next_scene_fn)
        self.waiting_for_input = false
    end
end

function SceneBase:next_scene(next_or_path)
    local next_scene_fn
    if type(next_or_path) == "string" then
        next_scene_fn = function (...)
            local scene_class = dofile(next_or_path)
            return scene_class.new(...)
        end
    else
        next_scene_fn = next_or_path
    end

    R.Scene.next(function (scene)
        local success, result = pcall(function ()
            return next_scene_fn {
                scene          = scene,
                nvg            = self.nvg,
                assets         = self.assets,
                frame_renderer = self.frame_renderer,
                frame_buffer   = self.frame_buffer,
            }
        end)
        if success then
            return result
        else
            R.warn("Error setting scene: %s", result)
            return ErrorScene.new(scene, self.nvg, self.assets, next_scene_fn)
        end
    end)
end


return SceneBase
