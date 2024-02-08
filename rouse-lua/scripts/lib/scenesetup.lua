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

local SetupSpec = class()

function SetupSpec:init(setup, spec)
    self.setup = setup
    self.spec  = spec
end

function SetupSpec:like(name)
    local other = self.setup:find_spec(name)
    for key, value in pairs(other) do
        if key ~= "name" then
            self.spec[key] = value
        end
    end
    return self
end

function SetupSpec:unchecked_parent(name)
    self.spec.parent = name
    return self
end

function SetupSpec:parent(name)
    self.setup:find_spec(name)
    return self:unchecked_parent(name)
end

function SetupSpec:track(name)
    self.spec.track = name
    return self
end


function SetupSpec:before(name)
    local target_index = self.setup:find_index(name)
    self.setup:move(self.spec.name, target_index)
    return self
end

function SetupSpec:after(name)
    local target_index = self.setup:find_index(name)
    self.setup:move(self.spec.name, target_index + 1)
    return self
end


function SetupSpec:content(content)
    self.spec.content = content
    return self
end

function SetupSpec:x(x)
    self.spec.x = x
    return self
end

function SetupSpec:y(y)
    self.spec.y = y
    return self
end

function SetupSpec:pos(x, y)
    return self:x(x):y(y)
end

function SetupSpec:world_x(world_x)
    self.spec.world_x = world_x
    return self
end

function SetupSpec:world_y(world_y)
    self.spec.world_y = world_y
    return self
end

function SetupSpec:world_pos(world_x, world_y)
    return self:world_x(world_x):world_y(world_y)
end

function SetupSpec:pivot_x(pivot_x)
    self.spec.pivot_x = pivot_x
    return self
end

function SetupSpec:pivot_y(pivot_y)
    self.spec.pivot_y = pivot_y
    return self
end

function SetupSpec:pivots(pivot_x, pivot_y)
    return self:pivot_x(pivot_x):pivot_y(pivot_y)
end

function SetupSpec:scale_x(scale_x)
    self.spec.scale_x = scale_x
    return self
end

function SetupSpec:scale_y(scale_y)
    self.spec.scale_y = scale_y
    return self
end

function SetupSpec:scales(scale_x, scale_y)
    return self:scale_x(scale_x):scale_y(scale_y)
end

function SetupSpec:scale(scale)
    return self:scales(scale, scale)
end

function SetupSpec:angle(angle)
    self.spec.angle = angle
    return self
end

function SetupSpec:rotation(rotation)
    return self:angle(math.rad(rotation))
end

function SetupSpec:alpha(alpha)
    self.spec.alpha = alpha
    return self
end

function SetupSpec:colorize(colorize)
    self.spec.colorize = colorize
    return self
end

function SetupSpec:gradient_map(gradient_map)
    self.spec.gradient_map = gradient_map
    return self
end


local SceneSetup = class()

function SceneSetup:init(scene)
    self.scene = scene
    self.specs = {}
end

function SceneSetup.assemble(cls, scene)
    local self = cls.new(scene)
    self:build()
    self:add_to_scene()
end

function SceneSetup:build()
    error("SceneSetup:build must be overridden")
end

function SceneSetup:add_to_scene()
    for i, spec in ipairs(self.specs) do
        self.scene:add(spec)
    end
end


function SceneSetup:add_spec(spec)
    local index = #self.specs + 1
    local name  = spec.name

    if name and self:search_index(name) then
        errorf("Duplicate sprite spec '%s'", name)
    end

    self.specs[index] = spec
end

function SceneSetup:search_index(name)
    for i = 1, #self.specs do
        if name == self.specs[i].name then
            return i
        end
    end
    return nil
end

function SceneSetup:find_index(name)
    return self:search_index(name) or errorf("Sprite spec '%s' not found", name)
end

function SceneSetup:find_spec(name)
    return self.specs[self:find_index(name)]
end


function SceneSetup:move(name, target_index)
    local source_index = self:find_index(name)
    if source_index < target_index then
        target_index = target_index - 1
    end
    local spec = table.remove(self.specs, source_index)
    table.insert(self.specs, target_index, spec)
end


function SceneSetup:json(key, modify_spec)
    local json = self.scene:json_asset(key)
    for i, spec in ipairs(json) do
        if modify_spec then
            spec = modify_spec(spec)
        end
        if spec then
            self:add_spec(spec)
        end
    end
end

function SceneSetup:json_prefixed(key, prefix)
    if not prefix then
        prefix = key .. "/"
    end
    return self:json(prefix .. key, function (spec)
        if spec.content then
            spec.content = prefix .. spec.content
        end
        return spec
    end)
end

function SceneSetup:add(name)
    local spec = {name = name}
    table.insert(self.specs, spec)
    return SetupSpec.new(self, spec)
end

function SceneSetup:edit(name)
    local spec = self:find_spec(name)
    return SetupSpec.new(self, spec)
end

function SceneSetup:remove(name)
    local target_index = self:find_index(name)
    return table.remove(self.specs, target_index)
end


return SceneSetup
