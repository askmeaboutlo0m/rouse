#include <rouse.h>

/*
 * Set this to a true value to see a comparision between the R_ease_* and
 * glm_ease_* functions. Red is rouse, blue is cglm. Many are the same, some are
 * slightly different and I think one cglm easing is broken. I like my easings
 * the way they are though, so the rouse functions are here to stay.
 */
#define COMPARE_WITH_GLM 0

#define GRAPH_WIDTH   600
#define GRAPH_HEIGHT  600
#define GRAPH_SAMPLES (GRAPH_WIDTH / 3)

typedef struct SceneData {
    R_Nvg           *nvg;
    R_Canvas        *canvas;
    R_FrameBuffer   *fb;
    R_FrameRenderer *fr;
    R_TextField     *name_field;
    R_TextField     *number_field;
    int             index;
    int             counter;
} SceneData;

typedef struct EasingEntry {
    const char *name;
    R_EaseFn   ease;
    R_EaseFn   glm;
} EasingEntry;

static EasingEntry easings[] = {
    {"linear",         R_ease_linear,         glm_ease_linear      },
    {"bounce_in",      R_ease_bounce_in,      glm_ease_bounce_in   },
    {"bounce_out",     R_ease_bounce_out,     glm_ease_bounce_out  },
    {"bounce_in_out",  R_ease_bounce_in_out,  glm_ease_bounce_inout},
    {"cubic_in",       R_ease_cubic_in,       glm_ease_cubic_in    },
    {"cubic_out",      R_ease_cubic_out,      glm_ease_cubic_out   },
    {"cubic_in_out",   R_ease_cubic_in_out,   glm_ease_cubic_inout },
    {"elastic_in",     R_ease_elastic_in,     glm_ease_elast_in    },
    {"elastic_out",    R_ease_elastic_out,    glm_ease_elast_out   },
    {"elastic_in_out", R_ease_elastic_in_out, glm_ease_elast_inout },
    {"back_in",        R_ease_back_in,        glm_ease_back_in     },
    {"back_out",       R_ease_back_out,       glm_ease_back_out    },
    {"back_in_out",    R_ease_back_in_out,    glm_ease_back_inout  },
    {"quint_in",       R_ease_quint_in,       glm_ease_quint_in    },
    {"quint_out",      R_ease_quint_out,      glm_ease_quint_out   },
    {"quint_in_out",   R_ease_quint_in_out,   glm_ease_quint_inout },
    {"quad_in",        R_ease_quad_in,        glm_ease_quad_in     },
    {"quad_out",       R_ease_quad_out,       glm_ease_quad_out    },
    {"quad_in_out",    R_ease_quad_in_out,    glm_ease_quad_inout  },
    {"quart_in",       R_ease_quart_in,       glm_ease_quart_in    },
    {"quart_out",      R_ease_quart_out,      glm_ease_quart_out   },
    {"quart_in_out",   R_ease_quart_in_out,   glm_ease_quart_inout },
    {"sine_in",        R_ease_sine_in,        glm_ease_sine_in     },
    {"sine_out",       R_ease_sine_out,       glm_ease_sine_out    },
    {"sine_in_out",    R_ease_sine_in_out,    glm_ease_sine_inout  },
    {"expo_in",        R_ease_expo_in,        glm_ease_exp_in      },
    {"expo_out",       R_ease_expo_out,       glm_ease_exp_out     },
    {"expo_in_out",    R_ease_expo_in_out,    glm_ease_exp_inout   },
};


static R_TextField *make_field(const char *name, float x, float y,
                               R_Sprite *parent, R_Nvg *nvg)
{
    R_TextField *field = R_text_field_new();
    field->align = NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE;
    field->font  = nvgFindFont(R_nvg_context(nvg), "DejaVuSansMono");
    field->size  = 64;

    R_Sprite *sprite = R_sprite_new(name);
    R_sprite_transform_at(sprite, 0)->pos = R_v2(x, y);
    R_sprite_draw_text_field(sprite, field);
    R_sprite_child_add_noinc(parent, sprite);

    return field;
}

static void add_text_fields(SceneData *sd)
{
    NVGcontext *ctx = R_nvg_context(sd->nvg);
    nvgCreateFont(ctx, "DejaVuSansMono", "test/data/dejavu/DejaVuSansMono.ttf");

    R_Sprite *parent = R_canvas_sprite(sd->canvas);
    sd->name_field   = make_field("name", (1920.0f - GRAPH_WIDTH) * 0.25f,
                                  1080.0f * 0.5f, parent, sd->nvg);
    sd->number_field = make_field("number", (1920.0f + GRAPH_WIDTH) * 0.625f,
                                  1080.0f * 0.5f, parent, sd->nvg);
}


static void draw_graph_background(NVGcontext *ctx, float w, float h)
{
    nvgFillColor(ctx, nvgRGB(225, 225, 225));
    nvgBeginPath(ctx);
    nvgRect(ctx, 0.0f, 0.0f, w, h);
    nvgFill(ctx);
}

static void draw_graph_frame(NVGcontext *ctx, float w, float h)
{
    nvgStrokeWidth(ctx, 4.0f);
    nvgStrokeColor(ctx, nvgRGB(0, 0, 0));
    nvgFillColor(ctx, nvgRGB(0, 0, 0));
    /* y axis */
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, 0.0f, h + 14.0f);
    nvgLineTo(ctx, 0.0f, 0.0f);
    nvgStroke(ctx);
    /* top arrow */
    nvgBeginPath(ctx);
    nvgMoveTo(ctx,  0.0f, -14.0f);
    nvgLineTo(ctx, -8.0f,   0.0f);
    nvgLineTo(ctx,  8.0f,   0.0f);
    nvgLineTo(ctx,  0.0f, -14.0f);
    nvgFill(ctx);
    /* x axis */
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, -14.0f, h);
    nvgLineTo(ctx, w, h);
    nvgStroke(ctx);
    /* right arrow */
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, w + 14.0f, h);
    nvgLineTo(ctx, w, h - 8.0f);
    nvgLineTo(ctx, w, h + 8.0f);
    nvgLineTo(ctx, w + 14.0f, h);
    nvgFill(ctx);
}

static void draw_graph_line(NVGcontext *ctx, float w, float h, int sample_count,
                            R_EaseFn ease, NVGcolor color)
{
    int   s   = GRAPH_SAMPLES;
    float sf  = GRAPH_SAMPLES;
    int   c   = R_MIN(sample_count, s);
    int   d   = sample_count > s ? sample_count - s : 0;
    int   dst = R_float2int(ease(R_int2float(c) / sf) * sf);
    int   src = R_float2int(ease(R_int2float(d) / sf) * sf);

    nvgStrokeWidth(ctx, 4.0f);
    nvgStrokeColor(ctx, color);
    nvgBeginPath(ctx);
    for (int i = src; i <= dst; ++i) {
        float f = R_int2float(i) / sf;
        float x = w * f;
        float y = h - h * ease(f);
        if (i == src) {
            nvgMoveTo(ctx, x, y);
        }
        else {
            nvgLineTo(ctx, x, y);
        }
    }
    nvgStroke(ctx);
}

static void draw_graph(NVGcontext *ctx, const float matrix[static 6],
                       R_UserData user)
{
    SceneData   *sd = user.data;
    EasingEntry *ee = &easings[sd->index];
    R_nvg_transform_set(ctx, matrix);
    float w = GRAPH_WIDTH;
    float h = GRAPH_HEIGHT;
    draw_graph_background(ctx, w, h);
    draw_graph_frame(ctx, w, h);
    draw_graph_line(ctx, w, h, sd->counter, ee->ease, nvgRGB(255, 0, 0));
#if COMPARE_WITH_GLM
    draw_graph_line(ctx, w, h, sd->counter, ee->glm,  nvgRGB(0, 0, 255));
#endif
}

static void add_easing_graph(SceneData *sd)
{
    R_Sprite *sprite = R_sprite_new("graph");
    R_sprite_transform_at(sprite, 0)->pos = R_v2(
            (1920.0f - GRAPH_WIDTH) * 0.5f, (1080.0f - GRAPH_HEIGHT) * 0.5f);
    R_sprite_draw_fn(sprite, draw_graph, NULL, R_user_data(sd));
    R_sprite_child_add_noinc(R_canvas_sprite(sd->canvas), sprite);
}


static void on_tick(R_Scene *scene, R_UNUSED bool rendered)
{
    SceneData *sd = scene->user.data;
    if (++sd->counter > GRAPH_SAMPLES * 2) {
        sd->counter = 0;
        sd->index   = (sd->index + 1) % R_size2int(R_LENGTH(easings));
    }

    R_String *name = sd->name_field->string;
    R_string_clear(name);
    R_string_append(name, easings[sd->index].name);

    float s = GRAPH_SAMPLES;
    float t = R_int2float(sd->counter % GRAPH_SAMPLES) / s;
    float u = easings[sd->index].ease(t);
    float c = sd->counter > GRAPH_SAMPLES ? 1.0f - t : t;
    float e = sd->counter > GRAPH_SAMPLES ? 1.0f - u : u;
    R_String *number = sd->number_field->string;
    R_string_clear(number);
    R_string_printf(number, "f(%.2f) = %.2f", c, e);
}

static void on_render(R_Scene *scene)
{
    SceneData     *sd = scene->user.data;
    R_FrameBuffer *fb = sd->fb;

    R_frame_buffer_bind(fb);
    R_gl_clear(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0);
    R_canvas_render(sd->canvas, sd->nvg, fb->width, fb->height);

    R_frame_buffer_unbind();
    R_gl_clear(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0);
    R_frame_renderer_draw(sd->fr, fb);
}

static void on_free(R_Scene *scene)
{
    SceneData *sd = scene->user.data;
    R_text_field_decref(sd->name_field);
    R_text_field_decref(sd->number_field);
    R_frame_renderer_free(sd->fr);
    R_frame_buffer_free(sd->fb);
    R_canvas_free(sd->canvas);
    R_nvg_decref(sd->nvg);
    free(sd);
}

static SceneData *init_scene_data(void)
{
    SceneData *sd = R_NEW(sd);
    sd->nvg       = R_nvg_new(0);
    sd->canvas    = R_canvas_new(1920, 1080);
    sd->fb        = R_canvas_frame_buffer_new(sd->canvas);
    sd->fr        = R_frame_renderer_new(true);
    sd->index     = 0;
    sd->counter   = 0;
    add_easing_graph(sd);
    add_text_fields(sd);
    return sd;
}

static R_Scene *init_scene(R_UNUSED void *user)
{
    R_Scene *scene   = R_scene_new();
    scene->user      = R_user_data(init_scene_data());
    scene->on_tick   = on_tick;
    scene->on_render = on_render;
    scene->on_free   = on_free;
    return scene;
}

int main(R_UNUSED int argc, R_UNUSED char **argv)
{
    R_MainArgs args   = R_main_args(init_scene, NULL);
    args.window.title = "ease";
    R_main(&args);
    return 0;
}
