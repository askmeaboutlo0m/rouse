/*
 * Copyright (c) 2019 - 2022 askmeaboutloom
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <cglm/struct.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <rouse_config.h>
#include "../common.h"
#include "../dl.h"
#include "gl.h"

#define R_GL_DEFINE_FALLBACKS
#include "../3rdparty/gles2_inc.h"


bool R_GL_EXT_texture_filter_anisotropic;
bool R_GL_EXT_multisampled_render_to_texture;

int   R_gl_max_vertex_attribs  = -1;
float R_gl_max_anisotropy      = -1.0f;
int   R_gl_max_texture_samples = 1;

static bool initialized = false;

#define R_GL_PROC_X(TYPE, NAME) TYPE NAME = R_ ## NAME ## _fallback;
R_GL_PROC_LIST
R_GL_PROC_EXT_LIST
#undef R_GL_PROC_X

#define GET_GL_PROC(TYPE, NAME) do { \
        void *_proc = SDL_GL_GetProcAddress(#NAME); \
        if (_proc) { \
            R_debug("got proc address: " #TYPE " " #NAME " at %p", _proc); \
            R_DL_ASSIGN(NAME, _proc); \
        } \
        else { \
            R_warn("did not find proc address for " #TYPE " " #NAME); \
        } \
    } while (0)

static void load_gl_procs(void)
{
    R_debug("populating gl proc function pointers");
#   define R_GL_PROC_X(TYPE, NAME) GET_GL_PROC(TYPE, NAME);
    R_GL_PROC_LIST
#   undef R_GL_PROC_X
    R_GL_CLEAR_ERROR();
}

static void load_ext_multisampled_render_to_texture_procs(void)
{
    GET_GL_PROC(PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC,
                glFramebufferTexture2DMultisampleEXT);
    GET_GL_PROC(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC,
                glRenderbufferStorageMultisampleEXT);
#   undef R_GL_PROC_X
    R_GL_CLEAR_ERROR();
}


static const char *get_gl_string(unsigned int name)
{
    const unsigned char *value = glGetString(name);
    R_GL_CLEAR_ERROR();
    return value ? (const char *)value : "(NULL)";
}

static void dump_gl_info(void)
{
#define R_GL_DUMP_STRING(LOG, NAME) LOG(#NAME ": %s", get_gl_string(NAME))
    R_GL_DUMP_STRING(R_info,  GL_VENDOR);
    R_GL_DUMP_STRING(R_info,  GL_RENDERER);
    R_GL_DUMP_STRING(R_info,  GL_VERSION);
    R_GL_DUMP_STRING(R_info,  GL_SHADING_LANGUAGE_VERSION);
    R_GL_DUMP_STRING(R_debug, GL_EXTENSIONS);
#undef R_GL_DUMP_STRING
}

static void check_gl_extensions(void)
{
    R_debug("checking GL extensions");
#   define CHECK_GL_EXTENSION(NAME) do { \
            SDL_bool _supported = SDL_GL_ExtensionSupported(#NAME); \
            R_ ## NAME = _supported; \
            R_info(#NAME " is %ssupported", _supported ? "" : "NOT "); \
            R_GL_CLEAR_ERROR(); \
        } while (0)
    CHECK_GL_EXTENSION(GL_EXT_texture_filter_anisotropic);
    CHECK_GL_EXTENSION(GL_EXT_multisampled_render_to_texture);
#   undef CHECK_GL_EXTENSION
}

static void get_gl_values(void)
{
#define R_GL_DEBUGV(NAME, FMT) R_debug("%s: " #NAME "=" FMT, __func__, NAME)
    R_debug("checking for maximum vertex attributes");
    R_GL(glGetIntegerv, GL_MAX_VERTEX_ATTRIBS, &R_gl_max_vertex_attribs);
    R_GL_DEBUGV(R_gl_max_vertex_attribs, "%d");

    R_debug("checking for maximum anisotropic filtering");
    if (R_GL_EXT_texture_filter_anisotropic) {
        R_GL(glGetFloatv, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &R_gl_max_anisotropy);
    }
    R_GL_DEBUGV(R_gl_max_anisotropy, "%f");

    R_debug("checking for maximum texture samples");
    if (R_GL_EXT_multisampled_render_to_texture) {
        int samples; /* Let's make sure this is at least 1. */
        R_GL(glGetIntegerv, GL_MAX_SAMPLES_EXT, &samples);
        R_gl_max_texture_samples = R_MAX(samples, 1);
        load_ext_multisampled_render_to_texture_procs();
    }
    R_GL_DEBUGV(R_gl_max_texture_samples, "%d");
#undef R_GL_DEBUGV
}

void R_gl_init(void)
{
    if (initialized) {
        R_die("Attempt to call R_gl_init twice");
    }

    load_gl_procs();
    dump_gl_info();
    check_gl_extensions();
    get_gl_values();

    R_debug("setting initial clear color");
    R_GL(glClearColor, 1.0f, 1.0f, 1.0f, 0.0f);
}


void R_gl_die(unsigned int err, const char *where)
{
    R_die("OpenGL error %d (%s) in %s", err,
          R_gl_strerror(err), where ? where : "");
}

void R_gl_warn(unsigned int err, const char *where)
{
    R_warn("OpenGL error %d (%s) in %s", err,
           R_gl_strerror(err), where ? where : "");
}


const char *R_gl_strerror(unsigned int err)
{
    switch (err) {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "unknown GL error";
    }
}


void R_gl_clear(float r, float g, float b, float a,
                float depth, int stencil)
{
    R_GL_CLEAR_ERROR();
    R_GL(glClearColor, r, g, b, a);
    R_GL(glClearDepthf, depth);
    R_GL(glClearStencil, stencil);
    R_GL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


static unsigned int parse_shader(unsigned int type, const char *source)
{
    unsigned int shader = R_GL_ASSIGN(glCreateShader, type);
    char       *buffer  = source[0] == '<' ? R_slurp(source + 1, NULL) : NULL;
    const char *code    = buffer ? buffer : source;
    R_GL(glShaderSource, shader, 1, &code, NULL);
    R_GL(glCompileShader, shader);
    free(buffer);

    return shader;
}

static void dump_info_log(
        void GL_APIENTRY (*get_info_log)(unsigned int, int, int *, char *),
        unsigned int handle, int log_length)
{
    char *buffer = R_malloc(R_int2size(log_length));

    int buflen;
    R_GL(get_info_log, handle, log_length, &buflen, buffer);

    if (buflen > 0) {
        R_warn("%.*s", buflen, buffer);
    }

    free(buffer);
}


static void dump_shader_info_log(unsigned int shader)
{
    int log_length;
    R_GL(glGetShaderiv, shader, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        dump_info_log(glGetShaderInfoLog, shader, log_length);
    }
}

static int get_shader_compile_status(unsigned int shader)
{
    int compile_status;
    R_GL(glGetShaderiv, shader, GL_COMPILE_STATUS, &compile_status);
    return compile_status;
}

unsigned int R_gl_shader_new(unsigned int type, const char *source)
{
    R_GL_CLEAR_ERROR();

    unsigned int shader = parse_shader(type, source);
    dump_shader_info_log(shader);

    if (!get_shader_compile_status(shader)) {
        R_die("Can't compile shader: %s", source);
    }

    return shader;
}


static void dump_program_info_log(unsigned int program)
{
    int log_length;
    R_GL(glGetProgramiv, program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        dump_info_log(glGetProgramInfoLog, program, log_length);
    }
}

static int get_program_link_status(unsigned int program)
{
    int link_status;
    R_GL(glGetProgramiv, program, GL_LINK_STATUS, &link_status);
    return link_status;
}

unsigned int R_gl_program_new(const char *vert, const char *frag)
{
    R_GL_CLEAR_ERROR();

    unsigned int program = R_GL_ASSIGN_0(glCreateProgram);
    unsigned int vshader = R_gl_shader_new(GL_VERTEX_SHADER,   vert);
    unsigned int fshader = R_gl_shader_new(GL_FRAGMENT_SHADER, frag);

    R_GL(glAttachShader, program, vshader);
    R_GL(glAttachShader, program, fshader);
    R_GL(glLinkProgram, program);

    dump_program_info_log(program);

    if (!get_program_link_status(program)) {
        R_die("Can't link program with shaders '%s' and '%s'", vert, frag);
    }

    R_GL(glDetachShader, program, vshader);
    R_GL(glDetachShader, program, fshader);
    R_GL(glDeleteShader, vshader);
    R_GL(glDeleteShader, fshader);

    return program;
}

void R_gl_program_free(unsigned int program)
{
    if (program) {
        R_GL_CLEAR_ERROR();
        R_GL(glDeleteProgram, program);
    }
}


R_TextureOptions R_texture_options(void)
{
    return (R_TextureOptions){
        R_MAGIC_INIT(R_TextureOptions)
        .format     = R_TEXTURE_FORMAT_RGBA,
        .min_filter = GL_LINEAR,
        .mag_filter = GL_LINEAR,
        .index      = 0,
    };
}

SDL_Surface *R_surface_load(const char *path, uint32_t pixel_format)
{
    SDL_Surface *original_surface = IMG_Load(path);
    if (!original_surface) {
        R_die("Can't load image '%s': %s", path, IMG_GetError());
    }

    SDL_Surface *converted_surface = SDL_ConvertSurfaceFormat(original_surface,
                                                              pixel_format, 0);
    if (!converted_surface) {
        R_die("Can't convert surface: %s", SDL_GetError());
    }

    SDL_FreeSurface(original_surface);
    return converted_surface;
}

unsigned int R_gl_texture_new(const char *path, const R_TextureOptions *options)
{
    R_MAGIC_CHECK(R_TextureOptions, options);

    int          internal_format;
    unsigned int format, type;
    uint32_t     pixel_format;
    switch (options->format) {
        case R_TEXTURE_FORMAT_RGBA:
            internal_format = GL_RGBA;
            format          = GL_RGBA;
            type            = GL_UNSIGNED_BYTE;
            pixel_format    = SDL_PIXELFORMAT_ABGR8888;
            break;
        default:
            R_die("Unknown texture format: %u", options->format);
    }

    SDL_Surface *surface = R_surface_load(path, pixel_format);

    unsigned int texture;
    R_GL(glActiveTexture, GL_TEXTURE0 + R_int2uint(options->index));
    R_GL(glGenTextures, 1, &texture);
    R_GL(glBindTexture, GL_TEXTURE_2D, texture);

    R_GL(glTexImage2D, GL_TEXTURE_2D, 0, internal_format, surface->w,
                       surface->h, 0, format, type, surface->pixels);

    SDL_FreeSurface(surface);

    R_GL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
         options->mag_filter);
    R_GL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
         options->min_filter);

    return texture;
}

void R_gl_texture_free(unsigned int texture)
{
    R_GL_CLEAR_ERROR();
    R_GL(glDeleteTextures, 1, &texture);
}

void R_gl_texture_bind(int index, unsigned int texture, int location)
{
    R_GL_CLEAR_ERROR();
    R_GL(glActiveTexture, GL_TEXTURE0 + R_int2uint(index));
    R_GL(glBindTexture, GL_TEXTURE_2D, texture);
    R_GL(glUniform1i, location, index);
}


int R_gl_uniform_location(unsigned int program, const char *name)
{
    R_GL_CLEAR_ERROR();
    int location = R_GL_ASSIGN(glGetUniformLocation, program, name);
    if (location == -1) {
        R_die("No uniform location for '%s'", name);
    }
    return location;
}
