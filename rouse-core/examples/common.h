#include <rouse.h>

R_Scene *common_init(void *(*init_fn )(void *), void *user,
                     void (*event_fn )(void *, SDL_Event *),
                     void (*tick_fn  )(void *, bool),
                     void (*render_fn)(void *, R_Camera *),
                     void (*free_fn  )(void *));
