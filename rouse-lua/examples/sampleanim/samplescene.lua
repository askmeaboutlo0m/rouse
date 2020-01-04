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

local SceneBase   = dofile("examples/lib/scenebase.lua")
local FpsCounter  = dofile("examples/lib/fpscounter.lua")
local Debuggery   = dofile("examples/lib/debuggery.lua")
local SampleSetup = dofile("samplesetup.lua")
local SampleScene = class(SceneBase)


function SampleScene:init(args)
    self.super.init(self, table.merge(args, {width = 1920, height = 1080}))
    SampleSetup:assemble(self)

    self:seq("sampleanim", "quad_in_out")
        :tween():rotation(-15.0, 15.0):apply():time(1.0, 3.0)
        :loop()

    self:seq("left_ear", "quad_in_out")
        :tween():rotation(0.0, 20.0):apply():time(1.0, 3.0)
        :loop()

    self:seq("right_ear", "quad_in_out")
        :tween():rotation(0.0, 20.0):apply():time(1.0, 3.0)
        :loop()

    local nose   = self:sprite("nose")
    local turn_x = 40.0
    local turn_y = 30.0

    self:seq(nose, "quad_in_out")
        :tween()
            :x(nose.x - turn_x, nose.x + turn_x)
            :apply()
            :time(0.5, 3.0)
        :loop()

    self:seq(nose, "quad_in_out")
        :tween()
            :y(nose.y - turn_y, nose.y + turn_y)
            :apply()
            :time(0.5, 3.0)
        :loop()

    local left_iris = self:sprite("left_iris")
    local look_x    = 40.0
    local look_y    = 90.0

    self:seq(left_iris, "quad_in_out")
        :tween()
            :x(left_iris.x - look_x, left_iris.x + look_x)
            :apply()
            :time(2.0, 4.0)
        :loop()

    self:seq(left_iris, "quad_in_out")
        :tween()
            :y(left_iris.y - look_y, left_iris.y + look_y)
            :apply()
            :time(2.0, 4.0)
        :loop()

    self:seq()
        :delay(6.0, 12.0)
        :call(function () self:blink() end)
        :loop()

    local font = self:font_asset("DejaVuSansMono")

    self.fps_counter = FpsCounter.new(self.nvg, font)
    self.root:add_child(self.fps_counter.sprite)

    self.debug = Debuggery.new {
        scene    = self,
        font     = font,
        enabled  = false,
        mark_pos = false,
        names    = {
            "sampleanim", "nose", "mouth", "hair", "left_ear", "right_ear",
            "left_cheek", "right_cheek", "left_iris", "right_iris",
        },
    }

    self.pre_render = self:make_pre_render()

    if DUMP_FRAME_BUFFER_EACH_FRAME then
        local fb = self.frame_buffer
        self.on_render = function (self)
            SampleScene.on_render(self)
            fb:write_to_stdout()
        end
    end
end


function SampleScene:blink()
    local function set_eye(i)
        local ll_sprite = self:sprite( "left_eye_lid")
        local rl_sprite = self:sprite("right_eye_lid")
        local ll_asset  = self:image_asset( "left_eye_lid" .. i)
        local rl_asset  = self:image_asset("right_eye_lid" .. i)
        return function ()
            self:frame(ll_sprite, ll_asset)
            self:frame(rl_sprite, rl_asset)
        end
    end

    local interval = R.rand_between(0.04, 0.06)

    self:seq("left_eye", "quad_in_out")
        :tween():scale_y(0.92, 0.97):time(interval * 4.0)
        :tween():scale_y(1.01, 1.04):time(interval * 4.0)
        :tween():scale_y(1.00):time(interval * 2.0)
        :start()

    self:seq()
        :call(set_eye(2)):delay(interval)
        :call(set_eye(3)):delay(interval)
        :call(set_eye(4)):delay(interval)
        :call(set_eye(5)):delay(interval * 2.0)
        :call(set_eye(4)):delay(interval)
        :call(set_eye(3)):delay(interval)
        :call(set_eye(2)):delay(interval)
        :call(set_eye(1)):delay(interval)
        :start()
end


function SampleScene:reload()
    R.info("reloading")
    self:next_scene("samplescene.lua")
end

function SampleScene:on_event(event)
    self.super.on_event(self, event)
    if event.type == SDL.EventType.KEYDOWN then
        local scancode = event.key.scancode
        if scancode == SDL.Scancode.Q then
            SDL.set_windowed()
        elseif scancode == SDL.Scancode.W then
            SDL.set_fullscreen_desktop()
        elseif scancode == SDL.Scancode.R then
            self:reload()
        elseif scancode == SDL.Scancode.D then
            self.debug.enabled = not self.debug.enabled
        elseif scancode == SDL.Scancode.F then
            local fps_sprite = self.fps_counter.sprite
            if fps_sprite.parent then
                fps_sprite:orphan()
            else
                self.root:add_child(fps_sprite)
            end
        elseif scancode == SDL.Scancode.M then
            self.debug.mark_pos = not self.debug.mark_pos
        end
    end
end


function SampleScene:make_pre_render()
    local nose, mouth, hair       = self:sprite("nose", "mouth", "hair")
    local left_ear,   right_ear   = self:sprite("left_ear",   "right_ear"  )
    local left_eye,   right_eye   = self:sprite("left_eye",   "right_eye"  )
    local left_cheek, right_cheek = self:sprite("left_cheek", "right_cheek")
    local left_iris,  right_iris  = self:sprite("left_iris",  "right_iris" )

    local nose_orig, mouth_orig, hair_orig  = nose.pos, mouth.pos, hair.pos
    local left_ear_orig,   right_ear_orig   = left_ear  .pos, right_ear  .pos
    local left_eye_orig,   right_eye_orig   = left_eye  .pos, right_eye  .pos
    local left_cheek_orig, right_cheek_orig = left_cheek.pos, right_cheek.pos
    local left_iris_orig,  right_iris_orig  = left_iris .pos, right_iris .pos

    local fps_counter = self.fps_counter

    return function ()
        local turn = nose.pos - nose_orig
        hair       .pos = 0.4 * turn  +        hair_orig
        mouth      .pos = turn *  0.5 +       mouth_orig
        left_eye   .pos = turn *  0.6 +    left_eye_orig
        right_eye  .pos = turn *  0.6 +   right_eye_orig
        left_ear   .pos = turn * -0.9 +    left_ear_orig
        right_ear  .pos = turn * -0.9 +   right_ear_orig
        left_cheek .pos = turn *  0.4 +  left_cheek_orig
        right_cheek.pos = turn *  0.4 + right_cheek_orig

        right_eye.scale_y = left_eye.scale_y
        right_iris.pos    = left_iris.pos - left_iris_orig + right_iris_orig

        fps_counter:on_frame()
    end
end


function SampleScene:on_render()
    self.pre_render()
    self.render()
end


return SampleScene
