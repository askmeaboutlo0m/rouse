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

local SceneSetup  = dofile("examples/lib/scenesetup.lua")
local SampleSetup = class(SceneSetup)

function SampleSetup:build()
    self:json("sampleanim")

    self:edit("sampleanim"):pivots(0.5, 0.9)

    self:edit("left_ear" ):pivots(0.8, 0.8)
    self:edit("right_ear"):pivots(0.2, 0.8)

    self:edit( "left_eye"):pivots(0.5, 0.5)
    self:edit("right_eye"):pivots(0.5, 0.5)

    self:edit( "left_iris"):pivots(0.5, 0.5)
    self:edit("right_iris"):pivots(0.5, 0.5)

    self:edit("hair" ):pivots(0.5, 0.5)
    self:edit("nose" ):pivots(0.5, 0.5)
    self:edit("mouth"):pivots(0.5, 0.1)

    self:edit( "left_cheek"):pivots(0.6, 0.4)
    self:edit("right_cheek"):pivots(0.4, 0.4)
end

return SampleSetup
