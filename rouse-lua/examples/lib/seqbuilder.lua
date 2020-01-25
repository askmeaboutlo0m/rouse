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
    return self
end


function TweenBuilder:to_value1(name, type, a)
    if is_number(a) then
        return type.fixed(a)
    elseif is_function(a) then
        return type.custom(a)
    else
        arg_types_error(name, a)
    end
end

function TweenBuilder:to_value2(name, type, a, b)
    if is_number(a) and is_number(b) then
        return type.between(a, b)
    else
        arg_types_error(name, a, b)
    end
end

function TweenBuilder:to_value(name, type, ...)
    local args = {...}
    local argc = #args
    if argc == 1 then
        return self:to_value1(name, type, args[1])
    elseif argc == 2 then
        return self:to_value2(name, type, args[1], args[2])
    else
        arg_count_error(name, argc)
    end
end

for name, aliases in pairs {origin_x = {"ox"}, origin_y = {"oy"}, pos_x = {"x"},
                            pos_y = {"y"}, scale_x = {"sx"}, scale_y = {"sy"},
                            skew_x = {"kx"}, skew_y = {"ky"}, angle = {"n"},
                            base_x = {"bx"}, base_y = {"by"}, rotation = {"r"},
                            rel_x = {"rx"}, rel_y = {"ry"}} do
    local tween_name = "sprite_" .. name
    TweenBuilder[name] = function (self, ...)
        local topic = self.topic
        local value = self:to_value(name, R.TweenFloat, ...)
        return self:add_attribute(function (tween)
            tween[tween_name](tween, topic, value)
        end)
    end
    for i = 1, #aliases do
        TweenBuilder[aliases[i]] = TweenBuilder[name]
    end
end

function TweenBuilder:field(key, ...)
    local topic = self.topic
    local value = self:to_value("field", R.TweenFloat, ...)
    return self:add_attribute(function (tween)
        tween:field(topic, key, value)
    end)
end


function TweenBuilder:scale(...)
    local topic = self.topic
    local value = self:to_value("scale", R.TweenScale, ...)
    return self:add_attribute(function (tween)
        tween:sprite_scale(topic, value)
    end)
end

TweenBuilder.s = TweenBuilder.scale


function TweenBuilder:ease(ease)
    self._ease = ease
    return self
end

function TweenBuilder:apply()
    self.apply_on_start = true
    return self
end


function TweenBuilder:apply_attributes(attributes)
    local apply_seq = SeqBuilder.new(self.parent.scene)
    apply_seq:add_step(function (sequence)
        local tween = sequence:tween_fixed(0.0, nil)
        for i, attribute in ipairs(attributes) do
            attribute(tween)
        end
        return tween:build()
    end)
    apply_seq:start()
end

function TweenBuilder:add_to_sequence(make_tween)
    local ease       = self._ease
    local attributes = self.attributes

    if self.apply_on_start then
        self:apply_attributes(attributes)
    end

    return self.parent:add_step(function (sequence)
        local tween = make_tween(sequence, ease)
        for i, attribute in ipairs(attributes) do
            attribute(tween)
        end
        return tween:build()
    end)
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
    if argc == 1 then
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
        arg_count_error("delay", argc)
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
