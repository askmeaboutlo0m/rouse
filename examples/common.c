#include "common.h"

#define MOVE_FORWARD  (1 << 0)
#define MOVE_BACKWARD (1 << 1)
#define MOVE_LEFT     (1 << 2)
#define MOVE_RIGHT    (1 << 3)
#define MOVE_UP       (1 << 4)
#define MOVE_DOWN     (1 << 5)
#define LOOK_UP       (1 << 6)
#define LOOK_DOWN     (1 << 7)
#define LOOK_LEFT     (1 << 8)
#define LOOK_RIGHT    (1 << 9)


typedef struct CommonData {
    R_FrameBuffer   *fb;
    R_FrameRenderer *fr;
    R_FirstPerson   *fp;
    R_Camera        *camera;
    R_Input         *input;
    R_V3            speed;
    unsigned int    input_flags;
    struct {
        void *data;
        void (*on_event )(void *, SDL_Event *);
        void (*on_tick  )(void *, bool);
        void (*on_render)(void *, R_Camera *);
        void (*on_free  )(void *);
    } custom;
} CommonData;


static void on_key(SDL_KeyboardEvent *key, R_UserData context, R_UserData user)
{
    CommonData *cd = context.data;
    if (key->type == SDL_KEYDOWN) {
        cd->input_flags |= user.u;
    }
    else {
        cd->input_flags &= ~user.u;
    }
}

static void on_event(R_Scene *scene, SDL_Event *event)
{
    R_debug("on_event %d", event->type);
    CommonData *cd = scene->user.data;
    R_input_handle_event(cd->input, event, scene->user);

    if (cd->custom.on_event) {
        cd->custom.on_event(cd->custom.data, event);
    }
}


static void move(CommonData *cd)
{
    float forward = 0.0f;
    if (cd->input_flags & MOVE_FORWARD)  forward += 1.0f;
    if (cd->input_flags & MOVE_BACKWARD) forward -= 1.0f;

    float right = 0.0f;
    if (cd->input_flags & MOVE_LEFT)  right -= 1.0f;
    if (cd->input_flags & MOVE_RIGHT) right += 1.0f;

    float up = 0.0f;
    if (cd->input_flags & MOVE_UP)   up += 1.0f;
    if (cd->input_flags & MOVE_DOWN) up -= 1.0f;

#   define MOVE(AXIS, DIRECTION) \
       cd->speed.AXIS = cd->speed.AXIS * 0.9f + DIRECTION * 0.05f
    MOVE(x, forward);
    MOVE(y, right);
    MOVE(z, up);
#   undef MOVE_AXIS

    R_first_person_move(cd->fp, cd->speed.x, cd->speed.y, cd->speed.z);
}

static void look(CommonData *cd)
{
    float h = 0.0f;
    if (cd->input_flags & LOOK_LEFT ) h += 1.0f;
    if (cd->input_flags & LOOK_RIGHT) h -= 1.0f;

    float v = 0.0f;
    if (cd->input_flags & LOOK_UP  ) v += 1.0f;
    if (cd->input_flags & LOOK_DOWN) v -= 1.0f;

    R_first_person_look(cd->fp, h * 0.02f, v * 0.02f);
}

static void on_tick(R_Scene *scene, bool rendered)
{
    R_debug("on_tick%s", rendered ? " (rendered)" : "");
    CommonData *cd = scene->user.data;
    move(cd);
    look(cd);

    if (cd->custom.on_tick) {
        cd->custom.on_tick(cd->custom.data, rendered);
    }
}


static void on_render(R_Scene *scene)
{
    R_debug("on_render");
    CommonData *cd = scene->user.data;

    R_first_person_apply(cd->fp, cd->camera);
    R_frame_buffer_bind(cd->fb);

    if (cd->custom.on_render) {
        cd->custom.on_render(cd->custom.data, cd->camera);
    }

    R_frame_buffer_unbind();
    R_gl_clear(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0);
    R_frame_renderer_draw(cd->fr, cd->fb);
}


static void on_free(R_Scene *scene)
{
    CommonData *cd = scene->user.data;

    if (cd->custom.on_free) {
        cd->custom.on_free(cd->custom.data);
    }

    R_input_free(cd->input);
    R_first_person_free(cd->fp);
    R_camera_free(cd->camera);
    R_frame_renderer_free(cd->fr);
    R_frame_buffer_free(cd->fb);
    free(cd);
}


R_Scene *common_init(void *(*init_fn )(void *), void *user,
                     void (*event_fn )(void *, SDL_Event *),
                     void (*tick_fn  )(void *, bool),
                     void (*render_fn)(void *, R_Camera *),
                     void (*free_fn  )(void *))
{
    R_FrameBufferOptions fbo = R_frame_buffer_options();
    fbo.width      = 1920;
    fbo.height     = 1080;
    fbo.color_type = R_FRAME_BUFFER_ATTACHMENT_TEXTURE;
    fbo.depth_type = R_FRAME_BUFFER_ATTACHMENT_BUFFER;

    CommonData *cd  = R_NEW(cd);
    cd->fb          = R_frame_buffer_new(&fbo);
    cd->fr          = R_frame_renderer_new();
    cd->fp          = R_first_person_new(R_v3(0.0f, 0.0f, -20.0f), 0.0f, 0.0f);
    cd->camera      = R_camera_new_perspective(R_to_rad(60.0f), 16.0f / 9.0f,
                                               0.1f, 1000.0f);
    cd->input       = R_input_new();
    cd->speed       = R_v3_zero();
    cd->input_flags = 0;

    cd->custom.data      = init_fn(user);
    cd->custom.on_event  = event_fn;
    cd->custom.on_tick   = tick_fn;
    cd->custom.on_render = render_fn;
    cd->custom.on_free   = free_fn;

#   define BIND_KEY(KEY, FLAG) \
        R_input_key_bind(cd->input, SDL_SCANCODE_ ## KEY, on_key, \
                         R_user_uint(FLAG), NULL)

    BIND_KEY(W,      MOVE_FORWARD);
    BIND_KEY(A,      MOVE_LEFT);
    BIND_KEY(S,      MOVE_BACKWARD);
    BIND_KEY(D,      MOVE_RIGHT);
    BIND_KEY(SPACE,  MOVE_UP);
    BIND_KEY(LSHIFT, MOVE_DOWN);
    BIND_KEY(Q,      LOOK_LEFT);
    BIND_KEY(E,      LOOK_RIGHT);
    BIND_KEY(R,      LOOK_UP);
    BIND_KEY(F,      LOOK_DOWN);

#   undef BIND_KEY

    R_Scene *scene   = R_scene_new();
    scene->on_event  = on_event;
    scene->on_tick   = on_tick;
    scene->on_render = on_render;
    scene->on_free   = on_free;
    scene->user.data = cd;
    return scene;
}
