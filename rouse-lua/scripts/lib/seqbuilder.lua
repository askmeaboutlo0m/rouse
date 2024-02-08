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

local function arg_count_error(name, argc)
    error("Can't build " .. name .. " step from " .. argc .. " arguments")
end

local function arg_types_error(name, ...)
    local types = {}
    for i, arg in ipairs({...}) do
        table.insert(types, type(arg))
    end
    error("Can't build " .. name .. " step with argument types (" ..
          table.concat(types, ", ") .. ")")
end


local ApplyFloat = {}

function ApplyFloat.fixed(value)
    return value
end

function ApplyFloat.between(a, b)
    return R.rand_between(a, b)
end

function ApplyFloat.custom(fn)
    return fn()
end

local ApplyColor = {}

function ApplyColor.fixed(value)
    return value
end

function ApplyColor.between(a, b)
    return R.Nvg.rgbaf(R.rand_between(a.r, b.r), R.rand_between(a.g, b.g),
                       R.rand_between(a.b, b.b), R.rand_between(a.a, b.a))
end

function ApplyColor.custom(fn)
    return fn()
end


local SeqBuilder   = class()
local TweenBuilder = class()

function TweenBuilder:init(parent, topic, ease)
    self.attributes = {}
    self.parent     = parent
    self.topic      = topic
    self._ease      = ease
end

function TweenBuilder:add_attribute(attribute)
    table.insert(self.attributes, attribute)
end

function TweenBuilder:add_application(application)
    table.insert(self.applications, application)
end


function TweenBuilder:to_value1(name, type, is_type, convert, a)
    if is_type(a) then
        if convert then
            return type.fixed(convert(a))
        else
            return type.fixed(a)
        end
    elseif is_function(a) then
        if convert then
            return type.custom(function ()
                return convert(a())
            end)
        else
            return type.custom(a)
        end
    else
        arg_types_error(name, a)
    end
end

function TweenBuilder:to_value2(name, type, is_type, convert, a, b)
    if is_type(a) and is_type(b) then
        if convert then
            return type.between(convert(a), convert(b))
        else
            return type.between(a, b)
        end
    else
        arg_types_error(name, a, b)
    end
end

function TweenBuilder:to_value(name, type, is_type, convert, ...)
    local args = {...}
    local argc = #args
    if argc == 1 then
        return self:to_value1(name, type, is_type, convert, args[1])
    elseif argc == 2 then
        return self:to_value2(name, type, is_type, convert, args[1], args[2])
    else
        arg_count_error(name, argc)
    end
end

function TweenBuilder:to_float(name, type, ...)
    return self:to_value(name, type, is_number, nil, ...)
end

local function is_nvg_color(x)
    return is_userdata(x, "NVGcolor")
end

function TweenBuilder:to_v4_from_color(name, type, ...)
    return self:to_value(name, type, is_nvg_color, function (color)
        return R.V4.new(color.r, color.g, color.b, color.a)
    end, ...)
end

local function make_tween_functions(prefix, with_topic, specs)
    for name, aliases in pairs(specs) do
        local tween_name = prefix .. name
        TweenBuilder[name] = function (self, ...)
            local topic = self.topic
            local value = self:to_float(name, R.TweenFloat, ...)
            self:add_attribute(function (tween)
                tween[tween_name](tween, topic, value)
            end)
            if self.applications then
                local applied_value = self:to_float(name, ApplyFloat, ...)
                self:add_application(function ()
                    topic[name] = applied_value
                end)
            end
            return self
        end

        for i = 1, #aliases do
            TweenBuilder[aliases[i]] = TweenBuilder[name]
        end

        if with_topic then
            local name_with = name .. "_with"
            TweenBuilder[name_with] = function(self, topic, ...)
                local value = self:to_float(name, R.TweenFloat, ...)
                self:add_attribute(function (tween)
                    tween[tween_name](tween, topic, value)
                end)
                if self.applications then
                    local applied_value = self:to_float(name, ApplyFloat, ...)
                    self:add_application(function ()
                        topic[name] = applied_value
                    end)
                end
                return self
            end
            for i = 1, #aliases do
                TweenBuilder[aliases[i] .. "_with"] = TweenBuilder[name_with]
            end
        end
    end
end

make_tween_functions("sprite_", true, {
    origin_x = {"ox"}, origin_y = {"oy"}, pos_x = {"x"},
    pos_y = {"y"}, scale_x = {"sx"}, scale_y = {"sy"},
    skew_x = {"kx"}, skew_y = {"ky"}, angle = {"n"},
    alpha = {"a"}, colorize = {"z"}, base_x = {"bx"}, base_y = {"by"},
    rotation = {"r"}, rel_x = {"rx"}, rel_y = {"ry"},
})

make_tween_functions("al_source_", true, {
    gain               = {"source_gain"},
    pitch              = {"source_pitch"},
    pos_x              = {"source_pos_x", "source_x"},
    pos_y              = {"source_pos_y", "source_y"},
    pos_z              = {"source_pos_z", "source_z"},
    velocity_x         = {"source_velocity_x"},
    velocity_y         = {"source_velocity_y"},
    velocity_z         = {"source_velocity_z"},
    reference_distance = {"source_reference_distance"},
})

-- FIXME: apply doesn't work for these, since they're not fields on the topic.
make_tween_functions("al_", true, {
    listener_gain       = {},
    listener_pos_x      = {"listener_x"},
    listener_pos_y      = {"listener_y"},
    listener_pos_z      = {"listener_z"},
    listener_velocity_x = {},
    listener_velocity_y = {},
    listener_velocity_z = {},
})

function TweenBuilder:field(key, ...)
    return self:field_with(key, self.topic, ...)
end

function TweenBuilder:field_with(key, topic, ...)
    local value = self:to_float("field", R.TweenFloat, ...)
    self:add_attribute(function (tween)
        tween:field(topic, key, value)
    end)
    if self.applications then
        local applied_value = self:to_float("field", ApplyFloat, ...)
        self:add_application(function ()
            topic[key] = applied_value
        end)
    end
    return self
end


function TweenBuilder:tint(...)
    return self:tint_with(self.topic, ...)
end

function TweenBuilder:tint_with(topic, ...)
    local topic = self.topic
    local value = self:to_v4_from_color("tint", R.TweenV4, ...)
    self:add_attribute(function (tween)
        tween:sprite_tint(topic, value)
    end)
    if self.applications then
        local applied_value = self:to_float("tint", ApplyColor, ...)
        self:add_application(function ()
            topic.tint = applied_value
        end)
    end
    return self
end

TweenBuilder.color = TweenBuilder.tint
TweenBuilder.c     = TweenBuilder.tint


function TweenBuilder:scale(...)
    return self:scale_with(self.topic, ...)
end

function TweenBuilder:scale_with(topic, ...)
    local value = self:to_float("scale", R.TweenScale, ...)
    self:add_attribute(function (tween)
        tween:sprite_scale(topic, value)
    end)
    if self.applications then
        local applied_value = self:to_float("scale", ApplyFloat, ...)
        self:add_application(function ()
            topic.scales = applied_value
        end)
    end
    return self
end

TweenBuilder.s = TweenBuilder.scale


function TweenBuilder:ease(ease)
    self._ease = ease
    return self
end

TweenBuilder.e = TweenBuilder.ease


function TweenBuilder:apply()
    self.applications = {}
    return self
end


function TweenBuilder:apply_attributes()
    for i, application in ipairs(self.applications) do
        application()
    end
--    local apply_seq = SeqBuilder.new(self.parent.scene)
--    apply_seq:add_step(function (sequence)
--        local tween = sequence:tween_fixed(0.0, nil)
--        for i, attribute in ipairs(attributes) do
--            attribute(tween)
--        end
--        return tween:build()
--    end)
--    apply_seq:start()
end

function TweenBuilder:add_to_sequence(make_tween)
    local ease       = self._ease
    local attributes = self.attributes

    if self.applications then
        self:apply_attributes()
    end

    return self.parent:add_step(function (sequence)
        local tween = make_tween(sequence, ease)
        for i, attribute in ipairs(attributes) do
            attribute(tween)
        end
        return tween:build()
    end)
end

function TweenBuilder:time0()
    return self:time1(0.0)
end

function TweenBuilder:time1(a)
    if is_number(a) then
        return self:add_to_sequence(function (sequence, ease)
            return sequence:tween_fixed(a, ease)
        end)
    elseif is_function(a) then
        return self:add_to_sequence(function (sequence, ease)
            return sequence:tween_custom(a, ease)
        end)
    else
        arg_types_error("time", a)
    end
end

function TweenBuilder:time2(a, b)
    if is_number(a) and is_number(b) then
        return self:add_to_sequence(function (sequence, ease)
            return sequence:tween_between(a, b, ease)
        end)
    else
        arg_types_error("time", a, b)
    end
end

function TweenBuilder:time(...)
    local args = {...}
    local argc = #args
    if argc == 0 then
        return self:time0()
    elseif argc == 1 then
        return self:time1(args[1])
    elseif argc == 2 then
        return self:time2(args[1], args[2])
    else
        arg_count_error("time", argc)
    end
end

TweenBuilder.t = TweenBuilder.time


function SeqBuilder:init(scene, topic, ease)
    self.scene = scene
    self.topic = topic
    self.ease  = ease
    self.steps = {}
end

function SeqBuilder:add_step(step)
    table.insert(self.steps, step)
    return self
end


function SeqBuilder:delay1(a)
    if is_number(a) then
        return self:add_step(function (sequence)
            sequence:delay_fixed(a)
        end)
    elseif is_function(a) then
        return self:add_step(function (sequence)
            sequence:delay_custom(a)
        end)
    else
        arg_types_error("delay", a)
    end
end

function SeqBuilder:delay2(a, b)
    if is_number(a) and is_number(b) then
        return self:add_step(function (sequence)
            sequence:delay_between(a, b)
        end)
    else
        arg_types_error("delay", a, b)
    end
end

function SeqBuilder:delay(...)
    local args = {...}
    local argc = #args
    if argc == 1 then
        return self:delay1(args[1])
    elseif argc == 2 then
        return self:delay2(args[1], args[2])
    else
        arg_count_error("delay", argc)
    end
end


function SeqBuilder:call1(a)
    if is_function(a) then
        return self:add_step(function (sequence)
            sequence:call(a)
        end)
    elseif is_thread(a) then
        return self:add_step(function (sequence)
            sequence:call(function ()
                local ok, msg = coroutine.resume(a)
                if not ok then
                    error(msg)
                end
            end)
        end)
    else
        arg_types_error("call", a)
    end
end

function SeqBuilder:call(...)
    local args = {...}
    local argc = #args
    if argc == 1 then
        return self:call1(args[1])
    else
        arg_count_error("call", argc)
    end
end


function SeqBuilder:frame1(a)
    local scene = self.scene
    local topic = self.topic
    if is_function(a) then
        return self:call1(function ()
            local content = a()
            scene:frame(topic, content)
        end)
    else
        return self:call1(function ()
            scene:frame(topic, a)
        end)
    end
end

function SeqBuilder:frame2(a, b)
    return self:frame1(a):delay(b)
end

function SeqBuilder:frame(...)
    local args = {...}
    local argc = #args
    if argc == 1 then
        return self:frame1(args[1])
    elseif argc == 2 then
        return self:frame2(args[1], args[2])
    else
        arg_count_error("frame", argc)
    end
end


function SeqBuilder:make_tween(topic, ease)
    return TweenBuilder.new(self, topic or self.topic, ease or self.ease)
end

function SeqBuilder:tween1(a)
    if is_userdata(a) or a == nil then
        return self:make_tween(a, nil, nil)
    elseif is_function(a) or is_string(a) then
        return self:make_tween(nil, a, nil)
    else
        arg_types_error("tween", a)
    end
end

function SeqBuilder:tween2(a, b)
    if (is_userdata(a) or a == nil) and
       (is_function(b) or is_string(b) or b == nil) then
        return self:make_tween(a, b, nil)
    else
        arg_types_error("tween", a, b)
    end
end

function SeqBuilder:tween(...)
    local args = {...}
    local argc = #args
    if argc == 0 then
        return self:make_tween(nil, nil, nil)
    elseif argc == 1 then
        return self:tween1(args[1])
    elseif argc == 2 then
        return self:tween2(args[1], args[2])
    else
        arg_count_error("tween", argc)
    end
end

function SeqBuilder:apply(...)
    return self:tween(...):apply()
end


function SeqBuilder:build_sequence(laps, on_done)
    local sequence = R.Sequence.new()
    for i, step in ipairs(self.steps) do
        step(sequence)
    end
    return sequence:run(self.scene.scene, laps, on_done)
end

function SeqBuilder:start_or_loop0(name)
    return self:build_sequence(-1, nil)
end

function SeqBuilder:start_or_loop1(name, a)
    if is_function(a) or a == nil then
        return self:build_sequence(-1, a)
    elseif is_integer(a) then
        return self:build_sequence(a, nil)
    else
        arg_types_error(name, a)
    end
end

function SeqBuilder:start_or_loop2(name, a, b)
    if is_integer(a) and (is_function(b) or b == nil) then
        return self:build_sequence(a, b)
    else
        arg_types_error(name, a, b)
    end
end

function SeqBuilder:start_or_loop(name, ...)
    local args = {...}
    local argc = #args
    if argc == 0 then
        return self:start_or_loop0(name)
    elseif argc == 1 then
        return self:start_or_loop1(name, args[1])
    elseif argc == 2 then
        return self:start_or_loop2(name, args[1], args[2])
    else
        arg_count_error(name, argc)
    end
end

function SeqBuilder:loop(...)
    return self:start_or_loop("loop", ...)
end

function SeqBuilder:start(...)
    return self:start_or_loop("start", 1, ...)
end


return SeqBuilder
