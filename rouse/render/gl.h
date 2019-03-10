/*
 * gl.h - OpenGL functionality
 *
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

/*
 * Macros for OpenGL error checking, which has a rather annoying interface.
 * In a release build, you probably want to disable this checking by defining
 * `ROUSE_GL_UNCHECKED`. In WebGL, you'll get error checking anyway I think.
 */
#ifdef ROUSE_GL_UNCHECKED
#   define R_GL_CLEAR_ERROR()      ((void) 0)
#   define R_GL_CHECK_ERROR(WHERE) ((void) 0)
#   define R_GL(FUNC, ...)         FUNC(__VA_ARGS__)
#else
/*
 * Clear any piled up errors and warn about each one. Every function that does
 * error checking should make sure to clear errors beforehand, otherwise those
 * leftover errors can cause it to fail through no fault of its own. If you get
 * these warnings, it should mean you (or something else) did an unchecked call
 * somewhere, which failed but nobody collected its reported error.
 */
#   define R_GL_CLEAR_ERROR() do { \
           GLenum glerror; \
           while ((glerror = glGetError()) != GL_NO_ERROR) { \
               R_gl_warn(glerror, "R_GL_CLEAR_ERROR"); \
           } \
       } while (0)
/*
 * Check if there's an OpenGL error laying around and `R_gl_die` if there is.
 * The `WHERE` parameter should be a string telling you the context, e.g. which
 * GL function you tried to call, so that it can be included in the message.
 */
#   define R_GL_CHECK_ERROR(WHERE) do { \
           GLenum glerror = glGetError(); \
           if (glerror != GL_NO_ERROR) { \
               R_gl_die(glerror, WHERE); \
           } \
       } while (0)
/*
 * Run the given OpenGL function and check for errors. So for example, instead
 * of `glActiveTexture(GL_TEXTURE0)`, use `R_GL(glActiveTexture, GLTEXTURE0)`.
 * This only works for OpenGL functions with a `void` result and at least one
 * parameter (which is most of them). Otherwise, you need to do a manual check
 * by using `R_GL_CHECK_ERROR` after your function.
 */
#   define R_GL(FUNC, ...) do { \
           FUNC(__VA_ARGS__); \
           R_GL_CHECK_ERROR(#FUNC); \
       } while (0)
#endif


/*
 * Various global OpenGL options retrieved on `R_gl_init` follow.
 * Don't assign to these, they're supposed to be read-only.
 */

/*
 * `GL_MAX_VERTEX_ATTRIBS`, the maximum number of vertex attributes.
 */
extern int R_gl_max_vertex_attribs;
/*
 * `GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT`, the highest anisotropic filtering
 * available. If you don't have the `ext_texture_filter_anisotropic` extension,
 * then this will be -1.0f.
 */
extern float R_gl_max_anisotropy;


/* Initialize GL stuff. Gets called by `R_main`, so don't do it yourself. */
void R_gl_init(void);

/*
 * `R_die`s and `R_warn`s with a description for the given OpenGL error code.
 * The `where` will be included in the message for context.
 */
void R_gl_die (unsigned int err, const char *where);
void R_gl_warn(unsigned int err, const char *where);

/* Returns a static string description for the given GL error. */
const char *R_gl_strerror(unsigned int err);

/* Do a `glClear` with the given clear color, depth and stencil values. */
void R_gl_clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                GLclampf depth, GLint stencil);

/*
 * You probably want `R_gl_program_new` instead, but this function loads a
 * single shader of the given `type` (basically, either `GL_VERTEX_SHADER` or
 * `GL_FRAGMENT_SHADER`, since only OpenGL ES 2.0 is supported) from the
 * `source`, which can either be a file path marked by a preceding `<` or the
 * shader source code itself.
 */
unsigned int R_gl_shader_new(unsigned int type, const char *source);
/*
 * Load a program with the given `vert`ex and `frag`ment shaders. If either of
 * those arguments starts with a `<`, the rest of the string is taken as a path
 * to a file that contains the shader source. Otherwise, the string itself is
 * taken as the shader source code. Will `R_die` if something goes wrong during
 * compilation. Any info log produced by the compilation is passed to `R_warn`.
 */
unsigned int R_gl_program_new(const char *vert, const char *frag);
/*
 * Free the program you previously loaded. Freeing `0` does nothing, just like
 * freeing a null pointer doesn't. Other errors will call `R_die` though.
 */
void R_gl_program_free(unsigned int program);


typedef enum R_TextureFormat {
    R_TEXTURE_FORMAT_RGBA,
} R_TextureFormat;

typedef struct R_TextureOptions {
    R_MAGIC_FIELD
    /* Pixel format of the texture. Default is `R_TEXTURE_FORMAT_RGBA`. */
    R_TextureFormat format;
    /* Texture filters, default is `GL_LINEAR` */
    unsigned int mag_filter, min_filter;
    /*
     * The slot index to use for loading the texture, default is 0 for
     * `GL_TEXTURE0`. Higher numbers `n` stand for `GL_TEXTUREn`.
     */
    int index;
} R_TextureOptions;

/* Get the default set of texture options. */
R_TextureOptions R_texture_options(void);

/*
 * Load a 2D texture from the given file `path` and return the handle to it.
 * Use `R_texture_options` to get a default set of texture options, manipulate
 * them as desired and pass a pointer to them here.
 *
 * The image is converted to the appropriate pixel format for you, but it's
 * not flipped or expanded to powers of two. If you need that, you need to do
 * it yourself beforehand.
 *
 * The texture slot you specify will be used to load the image, so if you had
 * something in it before it will be clobbered. Although if you're loading
 * textures in the middle of rendering stuff then you should mabye rethink
 * what you're doing.
 */
unsigned int R_gl_texture_new(const char *path, R_TextureOptions *options);

/* Deletes a texture you loaded via e.g. `R_gl_texture_new`. */
void R_gl_texture_free(unsigned int texture);

/*
 * Activate the texture slot specified by `index` (0 means `GL_TEXTURE0`,
 * 1 means `GL_TEXTURE1` and so on), bind the 2D `texture` to it and then stuff
 * it into the given uniform `location` (which should be a `sampler2D`).
 */
void R_gl_texture_bind(int index, unsigned int texture, int location);

/*
 * Fetch the location of a uniform by `name` in the given shader `program`.
 * Will `R_die` if that uniform doesn't exist.
 *
 * Shader compilers tend to optimize away uniforms that you never use. So if
 * you get an error telling you that a uniform doesn't exist even though it
 * clearly does in the source code, that might be the reason why.
 */
int R_gl_uniform_location(unsigned int program, const char *name);
