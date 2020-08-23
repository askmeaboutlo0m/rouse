/*
 * Copyright (c) 2019, 2020 askmeaboutloom
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
 * Emscripten links to all OpenGL functions statically. While loading the
 * functions dynamically still works due to emulation thereof, they don't
 * recommend it for performance reasons, function pointers require additional
 * validation in WebAssembly. So we'll be special-casing this a bunch.
 */
#ifdef __EMSCRIPTEN__
#   define GL_GLES_PROTOTYPES 1
#else
#   define GL_GLES_PROTOTYPES 0
#endif

#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

/* Don't load standard GL functions dynamically in Emscripten. */
#ifdef __EMSCRIPTEN__
#   define R_GL_PROC_LIST
#else
#   define R_GL_PROC_LIST \
        R_GL_PROC_X(PFNGLACTIVETEXTUREPROC, glActiveTexture) \
        R_GL_PROC_X(PFNGLATTACHSHADERPROC, glAttachShader) \
        R_GL_PROC_X(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation) \
        R_GL_PROC_X(PFNGLBINDBUFFERPROC, glBindBuffer) \
        R_GL_PROC_X(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer) \
        R_GL_PROC_X(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer) \
        R_GL_PROC_X(PFNGLBINDTEXTUREPROC, glBindTexture) \
        R_GL_PROC_X(PFNGLBLENDCOLORPROC, glBlendColor) \
        R_GL_PROC_X(PFNGLBLENDEQUATIONPROC, glBlendEquation) \
        R_GL_PROC_X(PFNGLBLENDEQUATIONSEPARATEPROC, glBlendEquationSeparate) \
        R_GL_PROC_X(PFNGLBLENDFUNCPROC, glBlendFunc) \
        R_GL_PROC_X(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate) \
        R_GL_PROC_X(PFNGLBUFFERDATAPROC, glBufferData) \
        R_GL_PROC_X(PFNGLBUFFERSUBDATAPROC, glBufferSubData) \
        R_GL_PROC_X(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus) \
        R_GL_PROC_X(PFNGLCLEARPROC, glClear) \
        R_GL_PROC_X(PFNGLCLEARCOLORPROC, glClearColor) \
        R_GL_PROC_X(PFNGLCLEARDEPTHFPROC, glClearDepthf) \
        R_GL_PROC_X(PFNGLCLEARSTENCILPROC, glClearStencil) \
        R_GL_PROC_X(PFNGLCOLORMASKPROC, glColorMask) \
        R_GL_PROC_X(PFNGLCOMPILESHADERPROC, glCompileShader) \
        R_GL_PROC_X(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D) \
        R_GL_PROC_X(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, glCompressedTexSubImage2D) \
        R_GL_PROC_X(PFNGLCOPYTEXIMAGE2DPROC, glCopyTexImage2D) \
        R_GL_PROC_X(PFNGLCOPYTEXSUBIMAGE2DPROC, glCopyTexSubImage2D) \
        R_GL_PROC_X(PFNGLCREATEPROGRAMPROC, glCreateProgram) \
        R_GL_PROC_X(PFNGLCREATESHADERPROC, glCreateShader) \
        R_GL_PROC_X(PFNGLCULLFACEPROC, glCullFace) \
        R_GL_PROC_X(PFNGLDELETEBUFFERSPROC, glDeleteBuffers) \
        R_GL_PROC_X(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers) \
        R_GL_PROC_X(PFNGLDELETEPROGRAMPROC, glDeleteProgram) \
        R_GL_PROC_X(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers) \
        R_GL_PROC_X(PFNGLDELETESHADERPROC, glDeleteShader) \
        R_GL_PROC_X(PFNGLDELETETEXTURESPROC, glDeleteTextures) \
        R_GL_PROC_X(PFNGLDEPTHFUNCPROC, glDepthFunc) \
        R_GL_PROC_X(PFNGLDEPTHMASKPROC, glDepthMask) \
        R_GL_PROC_X(PFNGLDEPTHRANGEFPROC, glDepthRangef) \
        R_GL_PROC_X(PFNGLDETACHSHADERPROC, glDetachShader) \
        R_GL_PROC_X(PFNGLDISABLEPROC, glDisable) \
        R_GL_PROC_X(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray) \
        R_GL_PROC_X(PFNGLDRAWARRAYSPROC, glDrawArrays) \
        R_GL_PROC_X(PFNGLDRAWELEMENTSPROC, glDrawElements) \
        R_GL_PROC_X(PFNGLENABLEPROC, glEnable) \
        R_GL_PROC_X(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray) \
        R_GL_PROC_X(PFNGLFINISHPROC, glFinish) \
        R_GL_PROC_X(PFNGLFLUSHPROC, glFlush) \
        R_GL_PROC_X(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer) \
        R_GL_PROC_X(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D) \
        R_GL_PROC_X(PFNGLFRONTFACEPROC, glFrontFace) \
        R_GL_PROC_X(PFNGLGENBUFFERSPROC, glGenBuffers) \
        R_GL_PROC_X(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap) \
        R_GL_PROC_X(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers) \
        R_GL_PROC_X(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers) \
        R_GL_PROC_X(PFNGLGENTEXTURESPROC, glGenTextures) \
        R_GL_PROC_X(PFNGLGETACTIVEATTRIBPROC, glGetActiveAttrib) \
        R_GL_PROC_X(PFNGLGETACTIVEUNIFORMPROC, glGetActiveUniform) \
        R_GL_PROC_X(PFNGLGETATTACHEDSHADERSPROC, glGetAttachedShaders) \
        R_GL_PROC_X(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation) \
        R_GL_PROC_X(PFNGLGETBOOLEANVPROC, glGetBooleanv) \
        R_GL_PROC_X(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv) \
        R_GL_PROC_X(PFNGLGETERRORPROC, glGetError) \
        R_GL_PROC_X(PFNGLGETFLOATVPROC, glGetFloatv) \
        R_GL_PROC_X(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, glGetFramebufferAttachmentParameteriv) \
        R_GL_PROC_X(PFNGLGETINTEGERVPROC, glGetIntegerv) \
        R_GL_PROC_X(PFNGLGETPROGRAMIVPROC, glGetProgramiv) \
        R_GL_PROC_X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog) \
        R_GL_PROC_X(PFNGLGETRENDERBUFFERPARAMETERIVPROC, glGetRenderbufferParameteriv) \
        R_GL_PROC_X(PFNGLGETSHADERIVPROC, glGetShaderiv) \
        R_GL_PROC_X(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog) \
        R_GL_PROC_X(PFNGLGETSHADERPRECISIONFORMATPROC, glGetShaderPrecisionFormat) \
        R_GL_PROC_X(PFNGLGETSHADERSOURCEPROC, glGetShaderSource) \
        R_GL_PROC_X(PFNGLGETSTRINGPROC, glGetString) \
        R_GL_PROC_X(PFNGLGETTEXPARAMETERFVPROC, glGetTexParameterfv) \
        R_GL_PROC_X(PFNGLGETTEXPARAMETERIVPROC, glGetTexParameteriv) \
        R_GL_PROC_X(PFNGLGETUNIFORMFVPROC, glGetUniformfv) \
        R_GL_PROC_X(PFNGLGETUNIFORMIVPROC, glGetUniformiv) \
        R_GL_PROC_X(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation) \
        R_GL_PROC_X(PFNGLGETVERTEXATTRIBFVPROC, glGetVertexAttribfv) \
        R_GL_PROC_X(PFNGLGETVERTEXATTRIBIVPROC, glGetVertexAttribiv) \
        R_GL_PROC_X(PFNGLGETVERTEXATTRIBPOINTERVPROC, glGetVertexAttribPointerv) \
        R_GL_PROC_X(PFNGLHINTPROC, glHint) \
        R_GL_PROC_X(PFNGLISBUFFERPROC, glIsBuffer) \
        R_GL_PROC_X(PFNGLISENABLEDPROC, glIsEnabled) \
        R_GL_PROC_X(PFNGLISFRAMEBUFFERPROC, glIsFramebuffer) \
        R_GL_PROC_X(PFNGLISPROGRAMPROC, glIsProgram) \
        R_GL_PROC_X(PFNGLISRENDERBUFFERPROC, glIsRenderbuffer) \
        R_GL_PROC_X(PFNGLISSHADERPROC, glIsShader) \
        R_GL_PROC_X(PFNGLISTEXTUREPROC, glIsTexture) \
        R_GL_PROC_X(PFNGLLINEWIDTHPROC, glLineWidth) \
        R_GL_PROC_X(PFNGLLINKPROGRAMPROC, glLinkProgram) \
        R_GL_PROC_X(PFNGLPIXELSTOREIPROC, glPixelStorei) \
        R_GL_PROC_X(PFNGLPOLYGONOFFSETPROC, glPolygonOffset) \
        R_GL_PROC_X(PFNGLREADPIXELSPROC, glReadPixels) \
        R_GL_PROC_X(PFNGLRELEASESHADERCOMPILERPROC, glReleaseShaderCompiler) \
        R_GL_PROC_X(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage) \
        R_GL_PROC_X(PFNGLSAMPLECOVERAGEPROC, glSampleCoverage) \
        R_GL_PROC_X(PFNGLSCISSORPROC, glScissor) \
        R_GL_PROC_X(PFNGLSHADERBINARYPROC, glShaderBinary) \
        R_GL_PROC_X(PFNGLSHADERSOURCEPROC, glShaderSource) \
        R_GL_PROC_X(PFNGLSTENCILFUNCPROC, glStencilFunc) \
        R_GL_PROC_X(PFNGLSTENCILFUNCSEPARATEPROC, glStencilFuncSeparate) \
        R_GL_PROC_X(PFNGLSTENCILMASKPROC, glStencilMask) \
        R_GL_PROC_X(PFNGLSTENCILMASKSEPARATEPROC, glStencilMaskSeparate) \
        R_GL_PROC_X(PFNGLSTENCILOPPROC, glStencilOp) \
        R_GL_PROC_X(PFNGLSTENCILOPSEPARATEPROC, glStencilOpSeparate) \
        R_GL_PROC_X(PFNGLTEXIMAGE2DPROC, glTexImage2D) \
        R_GL_PROC_X(PFNGLTEXPARAMETERFPROC, glTexParameterf) \
        R_GL_PROC_X(PFNGLTEXPARAMETERFVPROC, glTexParameterfv) \
        R_GL_PROC_X(PFNGLTEXPARAMETERIPROC, glTexParameteri) \
        R_GL_PROC_X(PFNGLTEXPARAMETERIVPROC, glTexParameteriv) \
        R_GL_PROC_X(PFNGLTEXSUBIMAGE2DPROC, glTexSubImage2D) \
        R_GL_PROC_X(PFNGLUNIFORM1FPROC, glUniform1f) \
        R_GL_PROC_X(PFNGLUNIFORM1FVPROC, glUniform1fv) \
        R_GL_PROC_X(PFNGLUNIFORM1IPROC, glUniform1i) \
        R_GL_PROC_X(PFNGLUNIFORM1IVPROC, glUniform1iv) \
        R_GL_PROC_X(PFNGLUNIFORM2FPROC, glUniform2f) \
        R_GL_PROC_X(PFNGLUNIFORM2FVPROC, glUniform2fv) \
        R_GL_PROC_X(PFNGLUNIFORM2IPROC, glUniform2i) \
        R_GL_PROC_X(PFNGLUNIFORM2IVPROC, glUniform2iv) \
        R_GL_PROC_X(PFNGLUNIFORM3FPROC, glUniform3f) \
        R_GL_PROC_X(PFNGLUNIFORM3FVPROC, glUniform3fv) \
        R_GL_PROC_X(PFNGLUNIFORM3IPROC, glUniform3i) \
        R_GL_PROC_X(PFNGLUNIFORM3IVPROC, glUniform3iv) \
        R_GL_PROC_X(PFNGLUNIFORM4FPROC, glUniform4f) \
        R_GL_PROC_X(PFNGLUNIFORM4FVPROC, glUniform4fv) \
        R_GL_PROC_X(PFNGLUNIFORM4IPROC, glUniform4i) \
        R_GL_PROC_X(PFNGLUNIFORM4IVPROC, glUniform4iv) \
        R_GL_PROC_X(PFNGLUNIFORMMATRIX2FVPROC, glUniformMatrix2fv) \
        R_GL_PROC_X(PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv) \
        R_GL_PROC_X(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv) \
        R_GL_PROC_X(PFNGLUSEPROGRAMPROC, glUseProgram) \
        R_GL_PROC_X(PFNGLVALIDATEPROGRAMPROC, glValidateProgram) \
        R_GL_PROC_X(PFNGLVERTEXATTRIB1FPROC, glVertexAttrib1f) \
        R_GL_PROC_X(PFNGLVERTEXATTRIB1FVPROC, glVertexAttrib1fv) \
        R_GL_PROC_X(PFNGLVERTEXATTRIB2FPROC, glVertexAttrib2f) \
        R_GL_PROC_X(PFNGLVERTEXATTRIB2FVPROC, glVertexAttrib2fv) \
        R_GL_PROC_X(PFNGLVERTEXATTRIB3FPROC, glVertexAttrib3f) \
        R_GL_PROC_X(PFNGLVERTEXATTRIB3FVPROC, glVertexAttrib3fv) \
        R_GL_PROC_X(PFNGLVERTEXATTRIB4FPROC, glVertexAttrib4f) \
        R_GL_PROC_X(PFNGLVERTEXATTRIB4FVPROC, glVertexAttrib4fv) \
        R_GL_PROC_X(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer) \
        R_GL_PROC_X(PFNGLVIEWPORTPROC, glViewport)
#endif

/*
 * These *do* have to be dynamic in Emscripten. Or rather, they don't exist,
 * EXT_multisampled_render_to_texture is unsupported and there's no static
 * version to link to. If more extensions are added, this may need adjustment.
 */
#define R_GL_PROC_EXT_LIST \
    R_GL_PROC_X(PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC, glFramebufferTexture2DMultisampleEXT) \
    R_GL_PROC_X(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC, glRenderbufferStorageMultisampleEXT)

#define R_GL_PROC_X(TYPE, NAME) extern TYPE NAME;
R_GL_PROC_LIST
R_GL_PROC_EXT_LIST
#undef R_GL_PROC_X

#ifdef R_GL_DEFINE_FALLBACKS
#   define R_GL_DEFINE_FALLBACK(TYPE, NAME, ARGS, RETVAL) \
        static TYPE GL_APIENTRY R_ ## NAME ## _fallback ARGS \
        { \
            R_warn("OpenGL procedure " #NAME " was not loaded"); \
            return RETVAL; \
        }

#   ifdef __EMSCRIPTEN__
#       define R_GL_FALLBACK(...) /* nothing, these are linked statically */
#   else
#       define R_GL_FALLBACK(...) R_GL_DEFINE_FALLBACK(__VA_ARGS__)
#   endif
#   define R_GL_FALLBACK_EXT(...) R_GL_DEFINE_FALLBACK(__VA_ARGS__)

R_GL_FALLBACK(void, glActiveTexture, (R_UNUSED GLenum texture),)
R_GL_FALLBACK(void, glAttachShader, (R_UNUSED GLuint program, R_UNUSED GLuint shader),)
R_GL_FALLBACK(void, glBindAttribLocation, (R_UNUSED GLuint program, R_UNUSED GLuint index, R_UNUSED const GLchar *name),)
R_GL_FALLBACK(void, glBindBuffer, (R_UNUSED GLenum target, R_UNUSED GLuint buffer),)
R_GL_FALLBACK(void, glBindFramebuffer, (R_UNUSED GLenum target, R_UNUSED GLuint framebuffer),)
R_GL_FALLBACK(void, glBindRenderbuffer, (R_UNUSED GLenum target, R_UNUSED GLuint renderbuffer),)
R_GL_FALLBACK(void, glBindTexture, (R_UNUSED GLenum target, R_UNUSED GLuint texture),)
R_GL_FALLBACK(void, glBlendColor, (R_UNUSED GLfloat red, R_UNUSED GLfloat green, R_UNUSED GLfloat blue, R_UNUSED GLfloat alpha),)
R_GL_FALLBACK(void, glBlendEquation, (R_UNUSED GLenum mode),)
R_GL_FALLBACK(void, glBlendEquationSeparate, (R_UNUSED GLenum modeRGB, R_UNUSED GLenum modeAlpha),)
R_GL_FALLBACK(void, glBlendFunc, (R_UNUSED GLenum sfactor, R_UNUSED GLenum dfactor),)
R_GL_FALLBACK(void, glBlendFuncSeparate, (R_UNUSED GLenum sfactorRGB, R_UNUSED GLenum dfactorRGB, R_UNUSED GLenum sfactorAlpha, R_UNUSED GLenum dfactorAlpha),)
R_GL_FALLBACK(void, glBufferData, (R_UNUSED GLenum target, R_UNUSED GLsizeiptr size, R_UNUSED const void *data, R_UNUSED GLenum usage),)
R_GL_FALLBACK(void, glBufferSubData, (R_UNUSED GLenum target, R_UNUSED GLintptr offset, R_UNUSED GLsizeiptr size, R_UNUSED const void *data),)
R_GL_FALLBACK(GLenum, glCheckFramebufferStatus, (R_UNUSED GLenum target), 0)
R_GL_FALLBACK(void, glClear, (R_UNUSED GLbitfield mask),)
R_GL_FALLBACK(void, glClearColor, (R_UNUSED GLfloat red, R_UNUSED GLfloat green, R_UNUSED GLfloat blue, R_UNUSED GLfloat alpha),)
R_GL_FALLBACK(void, glClearDepthf, (R_UNUSED GLfloat d),)
R_GL_FALLBACK(void, glClearStencil, (R_UNUSED GLint s),)
R_GL_FALLBACK(void, glColorMask, (R_UNUSED GLboolean red, R_UNUSED GLboolean green, R_UNUSED GLboolean blue, R_UNUSED GLboolean alpha),)
R_GL_FALLBACK(void, glCompileShader, (R_UNUSED GLuint shader),)
R_GL_FALLBACK(void, glCompressedTexImage2D, (R_UNUSED GLenum target, R_UNUSED GLint level, R_UNUSED GLenum internalformat, R_UNUSED GLsizei width, R_UNUSED GLsizei height, R_UNUSED GLint border, R_UNUSED GLsizei imageSize, R_UNUSED const void *data),)
R_GL_FALLBACK(void, glCompressedTexSubImage2D, (R_UNUSED GLenum target, R_UNUSED GLint level, R_UNUSED GLint xoffset, R_UNUSED GLint yoffset, R_UNUSED GLsizei width, R_UNUSED GLsizei height, R_UNUSED GLenum format, R_UNUSED GLsizei imageSize, R_UNUSED const void *data),)
R_GL_FALLBACK(void, glCopyTexImage2D, (R_UNUSED GLenum target, R_UNUSED GLint level, R_UNUSED GLenum internalformat, R_UNUSED GLint x, R_UNUSED GLint y, R_UNUSED GLsizei width, R_UNUSED GLsizei height, R_UNUSED GLint border),)
R_GL_FALLBACK(void, glCopyTexSubImage2D, (R_UNUSED GLenum target, R_UNUSED GLint level, R_UNUSED GLint xoffset, R_UNUSED GLint yoffset, R_UNUSED GLint x, R_UNUSED GLint y, R_UNUSED GLsizei width, R_UNUSED GLsizei height),)
R_GL_FALLBACK(GLuint, glCreateProgram, (void), 0)
R_GL_FALLBACK(GLuint, glCreateShader, (R_UNUSED GLenum type), 0)
R_GL_FALLBACK(void, glCullFace, (R_UNUSED GLenum mode),)
R_GL_FALLBACK(void, glDeleteBuffers, (R_UNUSED GLsizei n, R_UNUSED const GLuint *buffers),)
R_GL_FALLBACK(void, glDeleteFramebuffers, (R_UNUSED GLsizei n, R_UNUSED const GLuint *framebuffers),)
R_GL_FALLBACK(void, glDeleteProgram, (R_UNUSED GLuint program),)
R_GL_FALLBACK(void, glDeleteRenderbuffers, (R_UNUSED GLsizei n, R_UNUSED const GLuint *renderbuffers),)
R_GL_FALLBACK(void, glDeleteShader, (R_UNUSED GLuint shader),)
R_GL_FALLBACK(void, glDeleteTextures, (R_UNUSED GLsizei n, R_UNUSED const GLuint *textures),)
R_GL_FALLBACK(void, glDepthFunc, (R_UNUSED GLenum func),)
R_GL_FALLBACK(void, glDepthMask, (R_UNUSED GLboolean flag),)
R_GL_FALLBACK(void, glDepthRangef, (R_UNUSED GLfloat n, R_UNUSED GLfloat f),)
R_GL_FALLBACK(void, glDetachShader, (R_UNUSED GLuint program, R_UNUSED GLuint shader),)
R_GL_FALLBACK(void, glDisable, (R_UNUSED GLenum cap),)
R_GL_FALLBACK(void, glDisableVertexAttribArray, (R_UNUSED GLuint index),)
R_GL_FALLBACK(void, glDrawArrays, (R_UNUSED GLenum mode, R_UNUSED GLint first, R_UNUSED GLsizei count),)
R_GL_FALLBACK(void, glDrawElements, (R_UNUSED GLenum mode, R_UNUSED GLsizei count, R_UNUSED GLenum type, R_UNUSED const void *indices),)
R_GL_FALLBACK(void, glEnable, (R_UNUSED GLenum cap),)
R_GL_FALLBACK(void, glEnableVertexAttribArray, (R_UNUSED GLuint index),)
R_GL_FALLBACK(void, glFinish, (void),)
R_GL_FALLBACK(void, glFlush, (void),)
R_GL_FALLBACK(void, glFramebufferRenderbuffer, (R_UNUSED GLenum target, R_UNUSED GLenum attachment, R_UNUSED GLenum renderbuffertarget, R_UNUSED GLuint renderbuffer),)
R_GL_FALLBACK(void, glFramebufferTexture2D, (R_UNUSED GLenum target, R_UNUSED GLenum attachment, R_UNUSED GLenum textarget, R_UNUSED GLuint texture, R_UNUSED GLint level),)
R_GL_FALLBACK(void, glFrontFace, (R_UNUSED GLenum mode),)
R_GL_FALLBACK(void, glGenBuffers, (R_UNUSED GLsizei n, R_UNUSED GLuint *buffers),)
R_GL_FALLBACK(void, glGenerateMipmap, (R_UNUSED GLenum target),)
R_GL_FALLBACK(void, glGenFramebuffers, (R_UNUSED GLsizei n, R_UNUSED GLuint *framebuffers),)
R_GL_FALLBACK(void, glGenRenderbuffers, (R_UNUSED GLsizei n, R_UNUSED GLuint *renderbuffers),)
R_GL_FALLBACK(void, glGenTextures, (R_UNUSED GLsizei n, R_UNUSED GLuint *textures),)
R_GL_FALLBACK(void, glGetActiveAttrib, (R_UNUSED GLuint program, R_UNUSED GLuint index, R_UNUSED GLsizei bufSize, R_UNUSED GLsizei *length, R_UNUSED GLint *size, R_UNUSED GLenum *type, R_UNUSED GLchar *name),)
R_GL_FALLBACK(void, glGetActiveUniform, (R_UNUSED GLuint program, R_UNUSED GLuint index, R_UNUSED GLsizei bufSize, R_UNUSED GLsizei *length, R_UNUSED GLint *size, R_UNUSED GLenum *type, R_UNUSED GLchar *name),)
R_GL_FALLBACK(void, glGetAttachedShaders, (R_UNUSED GLuint program, R_UNUSED GLsizei maxCount, R_UNUSED GLsizei *count, R_UNUSED GLuint *shaders),)
R_GL_FALLBACK(GLint, glGetAttribLocation, (R_UNUSED GLuint program, R_UNUSED const GLchar *name), -1)
R_GL_FALLBACK(void, glGetBooleanv, (R_UNUSED GLenum pname, R_UNUSED GLboolean *data),)
R_GL_FALLBACK(void, glGetBufferParameteriv, (R_UNUSED GLenum target, R_UNUSED GLenum pname, R_UNUSED GLint *params),)
R_GL_FALLBACK(GLenum, glGetError, (void), GL_NO_ERROR)
R_GL_FALLBACK(void, glGetFloatv, (R_UNUSED GLenum pname, R_UNUSED GLfloat *data),)
R_GL_FALLBACK(void, glGetFramebufferAttachmentParameteriv, (R_UNUSED GLenum target, R_UNUSED GLenum attachment, R_UNUSED GLenum pname, R_UNUSED GLint *params),)
R_GL_FALLBACK(void, glGetIntegerv, (R_UNUSED GLenum pname, R_UNUSED GLint *data),)
R_GL_FALLBACK(void, glGetProgramiv, (R_UNUSED GLuint program, R_UNUSED GLenum pname, R_UNUSED GLint *params),)
R_GL_FALLBACK(void, glGetProgramInfoLog, (R_UNUSED GLuint program, R_UNUSED GLsizei bufSize, R_UNUSED GLsizei *length, R_UNUSED GLchar *infoLog),)
R_GL_FALLBACK(void, glGetRenderbufferParameteriv, (R_UNUSED GLenum target, R_UNUSED GLenum pname, R_UNUSED GLint *params),)
R_GL_FALLBACK(void, glGetShaderiv, (R_UNUSED GLuint shader, R_UNUSED GLenum pname, R_UNUSED GLint *params),)
R_GL_FALLBACK(void, glGetShaderInfoLog, (R_UNUSED GLuint shader, R_UNUSED GLsizei bufSize, R_UNUSED GLsizei *length, R_UNUSED GLchar *infoLog),)
R_GL_FALLBACK(void, glGetShaderPrecisionFormat, (R_UNUSED GLenum shadertype, R_UNUSED GLenum precisiontype, R_UNUSED GLint *range, R_UNUSED GLint *precision),)
R_GL_FALLBACK(void, glGetShaderSource, (R_UNUSED GLuint shader, R_UNUSED GLsizei bufSize, R_UNUSED GLsizei *length, R_UNUSED GLchar *source),)
R_GL_FALLBACK(const GLubyte *GL_APIENTRY, glGetString, (R_UNUSED GLenum name), NULL)
R_GL_FALLBACK(void, glGetTexParameterfv, (R_UNUSED GLenum target, R_UNUSED GLenum pname, R_UNUSED GLfloat *params),)
R_GL_FALLBACK(void, glGetTexParameteriv, (R_UNUSED GLenum target, R_UNUSED GLenum pname, R_UNUSED GLint *params),)
R_GL_FALLBACK(void, glGetUniformfv, (R_UNUSED GLuint program, R_UNUSED GLint location, R_UNUSED GLfloat *params),)
R_GL_FALLBACK(void, glGetUniformiv, (R_UNUSED GLuint program, R_UNUSED GLint location, R_UNUSED GLint *params),)
R_GL_FALLBACK(GLint, glGetUniformLocation, (R_UNUSED GLuint program, R_UNUSED const GLchar *name), -1)
R_GL_FALLBACK(void, glGetVertexAttribfv, (R_UNUSED GLuint index, R_UNUSED GLenum pname, R_UNUSED GLfloat *params),)
R_GL_FALLBACK(void, glGetVertexAttribiv, (R_UNUSED GLuint index, R_UNUSED GLenum pname, R_UNUSED GLint *params),)
R_GL_FALLBACK(void, glGetVertexAttribPointerv, (R_UNUSED GLuint index, R_UNUSED GLenum pname, R_UNUSED void **pointer),)
R_GL_FALLBACK(void, glHint, (R_UNUSED GLenum target, R_UNUSED GLenum mode),)
R_GL_FALLBACK(GLboolean, glIsBuffer, (R_UNUSED GLuint buffer), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsEnabled, (R_UNUSED GLenum cap), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsFramebuffer, (R_UNUSED GLuint framebuffer), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsProgram, (R_UNUSED GLuint program), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsRenderbuffer, (R_UNUSED GLuint renderbuffer), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsShader, (R_UNUSED GLuint shader), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsTexture, (R_UNUSED GLuint texture), GL_FALSE)
R_GL_FALLBACK(void, glLineWidth, (R_UNUSED GLfloat width),)
R_GL_FALLBACK(void, glLinkProgram, (R_UNUSED GLuint program),)
R_GL_FALLBACK(void, glPixelStorei, (R_UNUSED GLenum pname, R_UNUSED GLint param),)
R_GL_FALLBACK(void, glPolygonOffset, (R_UNUSED GLfloat factor, R_UNUSED GLfloat units),)
R_GL_FALLBACK(void, glReadPixels, (R_UNUSED GLint x, R_UNUSED GLint y, R_UNUSED GLsizei width, R_UNUSED GLsizei height, R_UNUSED GLenum format, R_UNUSED GLenum type, R_UNUSED void *pixels),)
R_GL_FALLBACK(void, glReleaseShaderCompiler, (void),)
R_GL_FALLBACK(void, glRenderbufferStorage, (R_UNUSED GLenum target, R_UNUSED GLenum internalformat, R_UNUSED GLsizei width, R_UNUSED GLsizei height),)
R_GL_FALLBACK(void, glSampleCoverage, (R_UNUSED GLfloat value, R_UNUSED GLboolean invert),)
R_GL_FALLBACK(void, glScissor, (R_UNUSED GLint x, R_UNUSED GLint y, R_UNUSED GLsizei width, R_UNUSED GLsizei height),)
R_GL_FALLBACK(void, glShaderBinary, (R_UNUSED GLsizei count, R_UNUSED const GLuint *shaders, R_UNUSED GLenum binaryformat, R_UNUSED const void *binary, R_UNUSED GLsizei length),)
R_GL_FALLBACK(void, glShaderSource, (R_UNUSED GLuint shader, R_UNUSED GLsizei count, R_UNUSED const GLchar *const*string, R_UNUSED const GLint *length),)
R_GL_FALLBACK(void, glStencilFunc, (R_UNUSED GLenum func, R_UNUSED GLint ref, R_UNUSED GLuint mask),)
R_GL_FALLBACK(void, glStencilFuncSeparate, (R_UNUSED GLenum face, R_UNUSED GLenum func, R_UNUSED GLint ref, R_UNUSED GLuint mask),)
R_GL_FALLBACK(void, glStencilMask, (R_UNUSED GLuint mask),)
R_GL_FALLBACK(void, glStencilMaskSeparate, (R_UNUSED GLenum face, R_UNUSED GLuint mask),)
R_GL_FALLBACK(void, glStencilOp, (R_UNUSED GLenum fail, R_UNUSED GLenum zfail, R_UNUSED GLenum zpass),)
R_GL_FALLBACK(void, glStencilOpSeparate, (R_UNUSED GLenum face, R_UNUSED GLenum sfail, R_UNUSED GLenum dpfail, R_UNUSED GLenum dppass),)
R_GL_FALLBACK(void, glTexImage2D, (R_UNUSED GLenum target, R_UNUSED GLint level, R_UNUSED GLint internalformat, R_UNUSED GLsizei width, R_UNUSED GLsizei height, R_UNUSED GLint border, R_UNUSED GLenum format, R_UNUSED GLenum type, R_UNUSED const void *pixels),)
R_GL_FALLBACK(void, glTexParameterf, (R_UNUSED GLenum target, R_UNUSED GLenum pname, R_UNUSED GLfloat param),)
R_GL_FALLBACK(void, glTexParameterfv, (R_UNUSED GLenum target, R_UNUSED GLenum pname, R_UNUSED const GLfloat *params),)
R_GL_FALLBACK(void, glTexParameteri, (R_UNUSED GLenum target, R_UNUSED GLenum pname, R_UNUSED GLint param),)
R_GL_FALLBACK(void, glTexParameteriv, (R_UNUSED GLenum target, R_UNUSED GLenum pname, R_UNUSED const GLint *params),)
R_GL_FALLBACK(void, glTexSubImage2D, (R_UNUSED GLenum target, R_UNUSED GLint level, R_UNUSED GLint xoffset, R_UNUSED GLint yoffset, R_UNUSED GLsizei width, R_UNUSED GLsizei height, R_UNUSED GLenum format, R_UNUSED GLenum type, R_UNUSED const void *pixels),)
R_GL_FALLBACK(void, glUniform1f, (R_UNUSED GLint location, R_UNUSED GLfloat v0),)
R_GL_FALLBACK(void, glUniform1fv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED const GLfloat *value),)
R_GL_FALLBACK(void, glUniform1i, (R_UNUSED GLint location, R_UNUSED GLint v0),)
R_GL_FALLBACK(void, glUniform1iv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED const GLint *value),)
R_GL_FALLBACK(void, glUniform2f, (R_UNUSED GLint location, R_UNUSED GLfloat v0, R_UNUSED GLfloat v1),)
R_GL_FALLBACK(void, glUniform2fv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED const GLfloat *value),)
R_GL_FALLBACK(void, glUniform2i, (R_UNUSED GLint location, R_UNUSED GLint v0, R_UNUSED GLint v1),)
R_GL_FALLBACK(void, glUniform2iv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED const GLint *value),)
R_GL_FALLBACK(void, glUniform3f, (R_UNUSED GLint location, R_UNUSED GLfloat v0, R_UNUSED GLfloat v1, R_UNUSED GLfloat v2),)
R_GL_FALLBACK(void, glUniform3fv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED const GLfloat *value),)
R_GL_FALLBACK(void, glUniform3i, (R_UNUSED GLint location, R_UNUSED GLint v0, R_UNUSED GLint v1, R_UNUSED GLint v2),)
R_GL_FALLBACK(void, glUniform3iv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED const GLint *value),)
R_GL_FALLBACK(void, glUniform4f, (R_UNUSED GLint location, R_UNUSED GLfloat v0, R_UNUSED GLfloat v1, R_UNUSED GLfloat v2, R_UNUSED GLfloat v3),)
R_GL_FALLBACK(void, glUniform4fv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED const GLfloat *value),)
R_GL_FALLBACK(void, glUniform4i, (R_UNUSED GLint location, R_UNUSED GLint v0, R_UNUSED GLint v1, R_UNUSED GLint v2, R_UNUSED GLint v3),)
R_GL_FALLBACK(void, glUniform4iv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED const GLint *value),)
R_GL_FALLBACK(void, glUniformMatrix2fv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED GLboolean transpose, R_UNUSED const GLfloat *value),)
R_GL_FALLBACK(void, glUniformMatrix3fv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED GLboolean transpose, R_UNUSED const GLfloat *value),)
R_GL_FALLBACK(void, glUniformMatrix4fv, (R_UNUSED GLint location, R_UNUSED GLsizei count, R_UNUSED GLboolean transpose, R_UNUSED const GLfloat *value),)
R_GL_FALLBACK(void, glUseProgram, (R_UNUSED GLuint program),)
R_GL_FALLBACK(void, glValidateProgram, (R_UNUSED GLuint program),)
R_GL_FALLBACK(void, glVertexAttrib1f, (R_UNUSED GLuint index, R_UNUSED GLfloat x),)
R_GL_FALLBACK(void, glVertexAttrib1fv, (R_UNUSED GLuint index, R_UNUSED const GLfloat *v),)
R_GL_FALLBACK(void, glVertexAttrib2f, (R_UNUSED GLuint index, R_UNUSED GLfloat x, R_UNUSED GLfloat y),)
R_GL_FALLBACK(void, glVertexAttrib2fv, (R_UNUSED GLuint index, R_UNUSED const GLfloat *v),)
R_GL_FALLBACK(void, glVertexAttrib3f, (R_UNUSED GLuint index, R_UNUSED GLfloat x, R_UNUSED GLfloat y, R_UNUSED GLfloat z),)
R_GL_FALLBACK(void, glVertexAttrib3fv, (R_UNUSED GLuint index, R_UNUSED const GLfloat *v),)
R_GL_FALLBACK(void, glVertexAttrib4f, (R_UNUSED GLuint index, R_UNUSED GLfloat x, R_UNUSED GLfloat y, R_UNUSED GLfloat z, R_UNUSED GLfloat w),)
R_GL_FALLBACK(void, glVertexAttrib4fv, (R_UNUSED GLuint index, R_UNUSED const GLfloat *v),)
R_GL_FALLBACK(void, glVertexAttribPointer, (R_UNUSED GLuint index, R_UNUSED GLint size, R_UNUSED GLenum type, R_UNUSED GLboolean normalized, R_UNUSED GLsizei stride, R_UNUSED const void *pointer),)
R_GL_FALLBACK(void, glViewport, (R_UNUSED GLint x, R_UNUSED GLint y, R_UNUSED GLsizei width, R_UNUSED GLsizei height),)
R_GL_FALLBACK_EXT(void, glRenderbufferStorageMultisampleEXT, (R_UNUSED GLenum target, R_UNUSED GLsizei samples, R_UNUSED GLenum internalformat, R_UNUSED GLsizei width, R_UNUSED GLsizei height),)
R_GL_FALLBACK_EXT(void, glFramebufferTexture2DMultisampleEXT, (R_UNUSED GLenum target, R_UNUSED GLenum attachment, R_UNUSED GLenum textarget, R_UNUSED GLuint texture, R_UNUSED GLint level, R_UNUSED GLsizei samples),)
#   undef R_GL_DEFINE_FALLBACK
#endif
