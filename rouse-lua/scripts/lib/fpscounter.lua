-- Copyright (c) 2019 - 2022 askmeaboutloom
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

local FpsCounter = class()

function FpsCounter:init(nvg, font_id, len)
    self.index  = 0
    self.len    = len or 100
    self.last   = SDL.ticks
    self.sum    = 0
    self.frames = {}
    if nvg then
        self.text_field = R.TextField.new {
            font             = font_id,
            size             = 32.0,
            color            = R.Nvg.color(0xffffff),
            align            = R.Nvg.Align.LEFT | R.Nvg.Align.TOP,
            outline_blur     = 2.0,
            outline_softness = 0.2,
            string           =
                "you need to call my on_frame in your on_render method",
        }
        self.sprite         = R.Sprite.new("fps")
        self.sprite.content = self.text_field
    end
end

function FpsCounter:update()
    local index = self.index + 1
    local now   = SDL.ticks
    local delta = now - self.last
    self.last   = now
    self.sum    = self.sum + delta - (self.frames[index] or 0)

    self.frames[index] = delta
    self.index         = index % self.len
end

function FpsCounter:calculate()
    local div = self.sum / #self.frames
    if div == 0.0 then
        return nil
    else
        return math.floor(1000.0 / div + 0.5)
    end
end

function FpsCounter:get_fps_string()
    local fps = self:calculate()
    return fps and string.format("%d fps", fps) or "inf fps"
end

function FpsCounter:on_frame()
    self:update()
    self.text_field.string = self:get_fps_string()
end

return FpsCounter
