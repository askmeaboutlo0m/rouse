#define NANOVG_GLES2
#include "nanovg/nanovg.h"
/*
 * nanovg_gl.h uses GLuint, which in the end is just a silly typedef for an
 * unsigned int. Including massive GL headers just for that one type would be
 * pretty excessive, so let's just #define it away. In the unlikely case it's
 * already been #defined, just include the header without doing anything.
 */
#ifndef GLuint
#   define GLuint unsigned int
#   include "nanovg/nanovg_gl.h"
#   undef GLuint
#else
#   include "nanovg/nanovg_gl.h"
#endif
