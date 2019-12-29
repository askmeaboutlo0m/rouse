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

local SetupSpec = class()

function SetupSpec:init(setup, spec)
    self.setup = setup
    self.spec  = spec
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


local SceneSetup = class()

function SceneSetup:init(scene)
    self.scene   = scene
    self.specs   = {}
    self.indexes = {}
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

    if name and self.indexes[name] then
        errorf("Duplicate sprite spec '%s'", name)
    end

    self.specs[index] = spec
    if name then
        self.indexes[name] = index
    end
end

function SceneSetup:find_spec(name)
    local index = self.indexes[name]
        or errorf("Sprite spec '%s' not found", name)
    return self.specs[index]
end


function SceneSetup:json(key)
    local json = self.scene:json_asset(key)
    for i, spec in ipairs(json) do
        self:add_spec(spec)
    end
end

function SceneSetup:edit(key)
    local spec = self:find_spec(key)
    return SetupSpec.new(self, spec)
end


return SceneSetup
