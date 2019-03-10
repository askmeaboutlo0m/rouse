/*
 * Copyright (c) 2019 askmeaboutloom
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
#include <stdnoreturn.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../common.h"
#include "gl.h"


int   R_gl_max_vertex_attribs = -1;
float R_gl_max_anisotropy     = -1.0f;


static bool initialized = false;

void R_gl_init(void)
{
    if (initialized) {
        R_die("Attempt to call R_gl_init twice");
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        R_die("glewInit failed");
    }

    R_GL_CLEAR_ERROR();
    R_GL(glClearColor, 1.0f, 1.0f, 1.0f, 0.0f);

    R_GL(glGetIntegerv, GL_MAX_VERTEX_ATTRIBS, &R_gl_max_vertex_attribs);
    if (GLEW_EXT_texture_filter_anisotropic) {
        R_GL(glGetFloatv, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &R_gl_max_anisotropy);
    }

#define R_GL_DEBUGV(NAME, FMT) R_debug("%s: " #NAME "=" FMT, __func__, NAME)
    R_GL_DEBUGV(R_gl_max_vertex_attribs, "%d");
    R_GL_DEBUGV(R_gl_max_anisotropy,     "%f");
#undef R_GL_DEBUGV
}


void R_gl_die (unsigned int err, const char *where)
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


void R_gl_clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                GLclampf depth, GLint stencil)
{
    R_GL_CLEAR_ERROR();
    R_GL(glClearColor, r, g, b, a);
    R_GL(glClearDepthf, depth);
    R_GL(glClearStencil, stencil);
    R_GL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


static unsigned int parse_shader(unsigned int type, const char *source)
{
    unsigned int shader = glCreateShader(type);
    R_GL_CHECK_ERROR("glCreateShader");

    char       *buffer = source[0] == '<' ? R_slurp(source + 1, NULL) : NULL;
    const char *code   = buffer ? buffer : source;
    R_GL(glShaderSource, shader, 1, &code, NULL);
    R_GL(glCompileShader, shader);
    free(buffer);

    return shader;
}

static void dump_info_log(
        void (*get_info_log)(unsigned int, int, int *, char *),
        unsigned int handle, int log_length)
{
    char *buffer = R_malloc(log_length);

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

    unsigned int program = glCreateProgram();
    R_GL_CHECK_ERROR("glCreateProgram");

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
        R_MAGIC_INIT_TYPE(R_TextureOptions)
        .format     = R_TEXTURE_FORMAT_RGBA,
        .min_filter = GL_LINEAR,
        .mag_filter = GL_LINEAR,
        .index      = 0,
    };
}

static SDL_Surface *load_surface(const char *path, Uint32 pixel_format)
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

unsigned int R_gl_texture_new(const char *path, R_TextureOptions *options)
{
    R_MAGIC_CHECK(options);

    unsigned int internal_format, format, type;
    Uint32       pixel_format;
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

    SDL_Surface *surface = load_surface(path, pixel_format);

    GLuint texture;
    R_GL(glActiveTexture, GL_TEXTURE0 + options->index);
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
    R_GL(glActiveTexture, GL_TEXTURE0 + index);
    R_GL(glBindTexture, GL_TEXTURE_2D, texture);
    R_GL(glUniform1i, location, index);
}


int R_gl_uniform_location(unsigned int program, const char *name)
{
    R_GL_CLEAR_ERROR();
    int location = glGetUniformLocation(program, name);
    R_GL_CHECK_ERROR("glGetUniformLocation");
    if (location == -1) {
        R_die("No uniform location for '%s'", name);
    }
    return location;
}
