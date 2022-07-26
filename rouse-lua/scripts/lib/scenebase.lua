-- Copyright (c) 2019, 2020, 2021 askmeaboutloom
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

local SeqBuilder = redofile("scripts/lib/seqbuilder.lua")
local SceneBase  = class()

function SceneBase:init(args)
    self.scene          = args.scene   or error("no scene given")
    self.nvg            = args.nvg     or error("no nvg given")
    self.width          = args.width   or error("no width given")
    self.height         = args.height  or error("no height given")
    self.assets         = args.assets  or {}
    self.sources        = args.sources or {}
    self.frame_renderer = args.frame_renderer
    self.frame_buffer   = args.frame_buffer
    self.root           = R.Sprite.new("root")
    self.clear_color    = args.clear_color or R.Nvg.rgbaf(1.0, 1.0, 1.0, 1.0)
    self.sprites        = {}
    if args.imgui_enabled then
        ImGui.Init()
        self.imgui_enabled = true
    end
    R.Viewport.set_window(self.width, self.height)
    self:init_render_function()
end

function SceneBase:init_render_function()
    local nvg      = self.nvg
    local width    = self.width
    local height   = self.height
    local root     = self.root
    local imgui    = self.imgui_enabled
    local fr       = self.frame_renderer
    local fb       = self.frame_buffer
    local indirect = fb and fr
    local dump     = rawget(_G, "DUMP_FRAME_BUFFER_EACH_FRAME")
    self.render    = function ()
        if imgui then
            ImGui.NewFrame()
        end

        local w, h
        if indirect then
            fb:bind()
            w, h = fb.width, fb.height
        else
            local vp = self.viewport or self:reset_viewport()
            w, h     = vp.w, vp.h
        end

        self:before_render(nvg, width, height, w, h)
        local r, g, b, a = self.clear_color:unpack()
        R.GL.clear(r, g, b, a, 0.0, 0)
        root:draw(nvg, width, height, w, h)

        if imgui then
            ImGui.Render()
            ImGui.RenderDrawData()
        end

        if indirect then
            fb:unbind()
            R.GL.clear(r, g, b, a, 0.0, 0)
            fr:draw(fb)
            if dump then
                fb:write_to_stdout()
            end
        end
    end
end


function SceneBase:on_free()
    if self.imgui_enabled then
        ImGui.Shutdown()
    end
end


local KEYDOWN      = SDL.EventType.KEYDOWN
local WINDOWEVENT  = SDL.EventType.WINDOWEVENT
local SIZE_CHANGED = SDL.WindowEventType.SIZE_CHANGED
local RETURN       = SDL.Scancode.RETURN
local RETURN2      = SDL.Scancode.RETURN2
local ALT          = SDL.Keymod.ALT

function SceneBase:on_event(event)
    local event_type = event.type
    if event_type == WINDOWEVENT then
        if event.window.event == SIZE_CHANGED then
            self.viewport = nil
        end
    elseif event_type == KEYDOWN then
        local key      = event.key
        local scancode = key.scancode
        if (scancode == RETURN or scancode == RETURN2) and (key.mod & ALT) ~= 0 then
            SDL.toggle_fullscreen_desktop()
        end
    end

    if self.imgui_enabled then
        ImGui.ProcessEvent(event)
    end
end

function SceneBase.register_event_handlers(class, reg)
    local event_handlers = rawget(class, "event_handlers")
    if not event_handlers then
        event_handlers       = {}
        class.event_handlers = event_handlers
    end

    for key, handler in pairs(reg) do
        local type
        if key == "ANY" then
            type = "ANY"
        elseif is_string(key) and SDL.EventType[key] then
            type = SDL.EventType[key]
        elseif is_integer(key) and SDL.EventType[key] then
            type = key
        end

        if type then
            event_handlers[type] = handler
        else
            error("Can't register unknown event type '" .. tostring(key) .. "'")
        end
    end

    if not rawget(class, "on_event") then
        class.on_event = function (self, event)
            self.super.on_event(self, event)

            local any_handler = event_handlers["ANY"]
            if any_handler then
                if is_string(any_handler) then
                    self[any_handler](self, event)
                else
                    any_handler(self, event)
                end
            end

            local handler = event_handlers[event.type]
            if handler then
                if is_string(handler) then
                    self[handler](self, event.inner_event)
                else
                    handler(self, event.inner_event)
                end
            end
        end
    end
end


function SceneBase:reset_viewport()
    R.Viewport.reset()
    local vp      = R.Viewport.window()
    self.viewport = vp
    return vp
end

function SceneBase:before_render()
    -- Nothing, override me.
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

function SceneBase:model_asset(key)
    return self:asset("model", key)
end

function SceneBase:sound_asset(key)
    return self:asset("sound", key)
end

function SceneBase:texture_asset(key)
    return self:asset("texture", key)
end


function SceneBase:sprite(...)
    local retvals = {}
    for i, name in ipairs {...} do
        retvals[i] = self.sprites[name] or errorf("Sprite '%s' not found", name)
    end
    return table.unpack(retvals)
end

function SceneBase:maybe_sprite(...)
    local retvals = {}
    for i, name in ipairs {...} do
        retvals[i] = self.sprites[name] or false
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

    local x = spec.world_x or spec.x or spec.pos and spec.pos.x or 0.0
    local y = spec.world_y or spec.y or spec.pos and spec.pos.y or 0.0

    if (spec.world_x or spec.world_y) and parent then
        x = x - parent.world_pos_x
        y = y - parent.world_pos_y
    end

    if spec.angle    then sprite.angle    = spec.angle    end
    if spec.rotation then sprite.rotation = spec.rotation end
    if spec.alpha    then sprite.alpha    = spec.alpha    end

    local scale_x = spec.scale_x or spec.scale
    local scale_y = spec.scale_y or spec.scale
    if scale_x then sprite.scale_x = scale_x end
    if scale_y then sprite.scale_y = scale_y end

    local function set_origin_x(origin_x)
        sprite.origin_x = origin_x
        if spec.pos_from_origin or spec.x_from_origin then
            x = x + origin_x
        end
    end
    if spec.origin_x then
        set_origin_x(spec.origin_x)
    elseif spec.pivot_x then
        set_origin_x(content.width * spec.pivot_x)
    end

    local function set_origin_y(origin_y)
        sprite.origin_y = origin_y
        if spec.pos_from_origin or spec.y_from_origin then
            y = y + origin_y
        end
    end
    if spec.origin_y then
        set_origin_y(spec.origin_y)
    elseif spec.pivot_y then
        set_origin_y(content.height * spec.pivot_y)
    end

    sprite.x = x
    sprite.y = y

    sprite:reset_base()

    if spec.track then
        sprite:track(self:maybe_look_up_sprite(spec.track))
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

function SceneBase:maybe_look_up_sprite(sprite_or_name)
    return self:maybe_look_up(sprite_or_name, self.sprite)
end

function SceneBase:maybe_look_up_image_asset(asset_or_name)
    return self:maybe_look_up(asset_or_name, self.image_asset)
end

function SceneBase:frame(sprite_or_name, asset_or_name)
    local sprite   = self:maybe_look_up_sprite(sprite_or_name)
    local asset    = self:maybe_look_up_image_asset(asset_or_name)
    sprite.content = asset
    return sprite, asset
end

function SceneBase:seq(topic_or_name, ...)
    local topic = self:maybe_look_up_sprite(topic_or_name)
    return SeqBuilder.new(self, topic, ...)
end

function SceneBase:delay(...)
    local args = {...}
    local last = table.remove(args)
    return self:seq()
        :delay(table.unpack(args))
        :start(last)
end


function SceneBase:kill_all()
    self.scene:kill_all()
end

function SceneBase:kill_by_id(id)
    if id then
        self.scene:kill_by_id(id)
    end
end


SceneBase.have_sound = R.al_enabled

if SceneBase.have_sound then
    function SceneBase:allocate_sound_sources(count)
        local sources   = self.sources
        local allocated = {}
        local i         = 1

        while i <= #sources and #allocated < count do
            local source = sources[i]
            if not source.playing then
                table.insert(allocated, source)
            end
            i = i + 1
        end

        while #allocated < count do
            local source = R.Al.Source.new()
            table.insert(sources,   source)
            table.insert(allocated, source)
        end

        return table.unpack(allocated)
    end

    function SceneBase:maybe_look_up_sound(sound_or_name)
        return self:maybe_look_up(sound_or_name, self.sound_asset)
    end

    SceneBase.source_defaults = {
        gain     = 1.0,
        pos      = R.V3.new(0.0, 0.0, 0.0),
        velocity = R.V3.new(0.0, 0.0, 0.0),
        looping  = false,
    }

    function SceneBase:play_sound_from(source, sound_or_name, args)
        local sound   = self:maybe_look_up_sound(sound_or_name)
        source.buffer = sound
        source:set(SceneBase.source_defaults)
        if args then
            source:set(args)
        end
        source:play()
    end

    function SceneBase:play_sound(sound_or_name, args)
        local source = self:allocate_sound_sources(1)
        self:play_sound_from(source, sound_or_name, args)
        return source
    end

    function SceneBase:stop_sound(source)
        if source then
            source:stop()
        end
        return source
    end

    function SceneBase:stop_all_sounds()
        for i, source in ipairs(self.sources) do
            source:stop()
        end
    end

    function SceneBase:stop_sounds_on_scene_switch()
        for i, source in ipairs(self.sources) do
            if not source.cross_scene then
                source:stop()
            end
        end
    end
else
    function SceneBase:allocate_sound_sources(count)
        error("OpenAL is not enabled")
    end

    function SceneBase:maybe_look_up_sound(sound_or_name)
        error("OpenAL is not enabled")
    end

    function SceneBase:play_sound_from(source, sound_or_name, args)
        -- nothing
    end

    function SceneBase:play_sound(sound_or_name, args)
        return nil
    end

    function SceneBase:stop_sound(source)
        return nil
    end

    function SceneBase:stop_all_sounds()
        -- nothing
    end

    function SceneBase:stop_sounds_on_scene_switch()
        -- nothing
    end
end


local ErrorScene = class(SceneBase)

function ErrorScene:init(scene, nvg, assets, sources, frame_renderer,
                         frame_buffer, next_scene_fn)
    self.super.init(self, {
        scene          = scene,
        nvg            = nvg,
        assets         = assets,
        sources        = sources,
        frame_renderer = frame_renderer,
        frame_buffer   = frame_buffer,
        width          = 1280,
        height         = 720,
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
            local scene_class = redofile(next_or_path)
            return scene_class.new(...)
        end
    else
        next_scene_fn = next_or_path
    end

    self:stop_sounds_on_scene_switch()

    R.Scene.next(function (scene)
        local success, result = pcall(function ()
            return next_scene_fn {
                scene          = scene,
                nvg            = self.nvg,
                assets         = self.assets,
                sources        = self.sources,
                frame_renderer = self.frame_renderer,
                frame_buffer   = self.frame_buffer,
            }
        end)
        if success then
            return result
        else
            R.warn("Error setting scene: %s", result)
            return ErrorScene.new(scene, self.nvg, self.assets, self.sources,
                                  self.frame_renderer, self.frame_buffer,
                                  next_scene_fn)
        end
    end)
end


return SceneBase
