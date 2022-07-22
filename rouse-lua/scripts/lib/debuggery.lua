-- Copyright (c) 2020 - 2022 askmeaboutloom
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

local Debuggery             = class()
local DEFAULT_COLOR         = R.Nvg.rgb(220, 0, 0)
local DEFAULT_OUTLINE_COLOR = R.Nvg.rgb(255, 255, 255)

function Debuggery:init(args)
    self.scene    = args.scene   or error("no scene given")
    self.sprites  = args.sprites or args.names or {}
    self.enabled  = args.enabled ~= false
    self.mark_pos = args.mark_pos

    self.field = R.TextField.new {
        font             = args.font       or -1,
        size             = args.text_size  or 32.0,
        color            = args.text_color or DEFAULT_COLOR,
        align            = R.Nvg.Align.LEFT | R.Nvg.Align.MIDDLE,
        outline_blur     = args.text_outline_blur     or 2.0,
        outline_softness = args.text_outline_softness or 0.2,
        outline_color    = args.text_outline_color    or DEFAULT_OUTLINE_COLOR,
    }

    self.stroke_color   = args.stroke_color or DEFAULT_COLOR
    self.stroke_width   = 5.0
    self.cross_size     = 20.0
    self.sprite         = R.Sprite.new(self.sprite_name or "debuggery")
    self.sprite.content = function (nvg, matrix)
        if self.enabled then
            self:draw(nvg, matrix)
        end
    end

    if args.add_to_scene ~= false then
        self.scene.root:add_child(self.sprite)
    end
end

function Debuggery:draw(nvg, matrix)
    nvg:set_transform(matrix)
    nvg:stroke_color(self.stroke_color)
    nvg:stroke_width(self.stroke_width)
    local field    = self.field
    local mark_pos = self.mark_pos

    for i, sprite_or_name in ipairs(self.sprites) do
        local sprite = is_string(sprite_or_name)
                   and self.scene:sprite(sprite_or_name)
                   or  sprite_or_name

        local x    = sprite.world_origin_x
        local y    = sprite.world_origin_y
        local size = self.cross_size

        nvg:begin_path()
        nvg:move_to(x - size, y - size)
        nvg:line_to(x + size, y + size)
        nvg:stroke()

        nvg:begin_path()
        nvg:move_to(x + size, y - size)
        nvg:line_to(x - size, y + size)
        nvg:stroke()

        if mark_pos then
            nvg:begin_path()
            nvg:move_to(sprite.world_pos_x, sprite.world_pos_y)
            nvg:line_to(x, y)
            nvg:stroke()
        end

        local name = sprite.name
        if name then
            field.string = sprite.name
            field.x      = x + size
            field.y      = y
            field:draw(nvg, matrix)
        end
    end
end

return Debuggery
