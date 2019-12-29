dofile("examples/lib/common.lua")

local GRAPH_WIDTH   = 600
local GRAPH_HEIGHT  = 600
local GRAPH_SAMPLES = (GRAPH_WIDTH / 3)

local easings = {
    "linear",
    "bounce_in",
    "bounce_out",
    "bounce_in_out",
    "cubic_in",
    "cubic_out",
    "cubic_in_out",
    "elastic_in",
    "elastic_out",
    "elastic_in_out",
    "back_in",
    "back_out",
    "back_in_out",
    "quint_in",
    "quint_out",
    "quint_in_out",
    "quad_in",
    "quad_out",
    "quad_in_out",
    "quart_in",
    "quart_out",
    "quart_in_out",
    "sine_in",
    "sine_out",
    "sine_in_out",
    "expo_in",
    "expo_out",
    "expo_in_out",
}

local SceneBase = dofile("examples/lib/scenebase.lua")
local EaseScene = class(SceneBase)

function EaseScene:init(scene)
    self.super:init(scene, R.Nvg.new(0))
    self.index   = 0
    self.counter = 0
    self:add_easing_graph()
    self:add_text_fields()
end


local function draw_graph_background(nvg, w, h)
    nvg:fill_color(R.Nvg.rgb(225, 225, 225))
    nvg:begin_path()
    nvg:rect(0.0, 0.0, w, h)
    nvg:fill()
end

local function draw_graph_frame(nvg, w, h)
    nvg:stroke_width(4.0)
    nvg:stroke_color(R.Nvg.rgb(0, 0, 0))
    nvg:fill_color(R.Nvg.rgb(0, 0, 0))
    -- y axis
    nvg:begin_path()
    nvg:move_to(0.0, h + 14.0)
    nvg:line_to(0.0, 0.0)
    nvg:stroke()
    -- top arrow
    nvg:begin_path()
    nvg:move_to( 0.0, -14.0)
    nvg:line_to(-8.0,   0.0)
    nvg:line_to( 8.0,   0.0)
    nvg:line_to( 0.0, -14.0)
    nvg:fill()
    -- x axis
    nvg:begin_path()
    nvg:move_to(-14.0, h)
    nvg:line_to(w, h)
    nvg:stroke()
    -- right arrow
    nvg:begin_path()
    nvg:move_to(w + 14.0, h)
    nvg:line_to(w, h - 8.0)
    nvg:line_to(w, h + 8.0)
    nvg:line_to(w + 14.0, h)
    nvg:fill()
end

local function draw_graph_line(nvg, w, h, sample_count, ease)
    local s   = GRAPH_SAMPLES
    local c   = math.min(sample_count, s)
    local d   = sample_count > s and sample_count - s or 0
    local dst = math.floor(ease(c / s) * s)
    local src = math.floor(ease(d / s) * s)

    nvg:stroke_width(4.0)
    nvg:stroke_color(R.Nvg.rgb(255, 0, 0))
    nvg:begin_path()
    for i = src, dst do
        local f = i / s
        local x = w * f
        local y = h - h * ease(f)
        if i == src then
            nvg:move_to(x, y)
        else
            nvg:line_to(x, y)
        end
    end
    nvg:stroke()
end

function EaseScene:add_easing_graph()
    local sprite = R.Sprite.new("graph")
    sprite.x = (1920.0 - GRAPH_WIDTH ) * 0.5
    sprite.y = (1080.0 - GRAPH_HEIGHT) * 0.5

    sprite.content = function (nvg, matrix)
        nvg:set_transform(matrix)
        local w, h = GRAPH_WIDTH, GRAPH_HEIGHT
        draw_graph_background(nvg, w, h)
        draw_graph_frame(nvg, w, h)
        draw_graph_line(nvg, w, h, self.counter, R.Ease[easings[self.index + 1]])
    end

    self.root:add_child(sprite)
end


function EaseScene:add_text_fields()
    local nvg  = self.nvg
    local font = self.nvg:create_font("DejaVuSansMono",
            "../rouse/test/data/dejavu/DejaVuSansMono.ttf")

    local parent = self.root
    local function make_field(name, x, y)
        local field = R.TextField.new {
            font  = font,
            size  = 64.0,
            align = R.Nvg.Align.CENTER | R.Nvg.Align.MIDDLE,
        }

        local sprite   = R.Sprite.new(name)
        sprite.x       = x
        sprite.y       = y
        sprite.content = field
        parent:add_child(sprite)

        return field
    end

    self.name_field   = make_field("name",   (1920.0 - GRAPH_WIDTH) * 0.250, 1080.0 * 0.5)
    self.number_field = make_field("number", (1920.0 + GRAPH_WIDTH) * 0.625, 1080.0 * 0.5)
end


local function format_float(f)
    local s = string.format("%f", f)
    if not s:find(".", 1, true) then
        s = s .. "."
    end
    s = s .. "00"
    return s:match("^-?%d+%.%d%d")
end

function EaseScene:on_tick(rendered)
    self.counter = self.counter + 1
    if self.counter > GRAPH_SAMPLES * 2 then
        self.counter = 0
        self.index   = (self.index + 1) % #easings
    end

    if rendered then
        local easing_name = easings[self.index + 1]
        self.name_field.string = easing_name

        local s = GRAPH_SAMPLES;
        local t = (self.counter % GRAPH_SAMPLES) / s
        local u = R.Ease[easing_name](t)
        local c = self.counter > GRAPH_SAMPLES and 1.0 - t or t
        local e = self.counter > GRAPH_SAMPLES and 1.0 - u or u
        self.number_field.string = string.format("f(%s) = %s", format_float(c),
                                                               format_float(e))
    end
end


return simple_main_args {
    title    = "ease",
    on_scene = EaseScene.new,
}
