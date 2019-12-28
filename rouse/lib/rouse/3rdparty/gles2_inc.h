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

#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-parameter"
R_GL_FALLBACK(void, glActiveTexture, (GLenum texture),)
R_GL_FALLBACK(void, glAttachShader, (GLuint program, GLuint shader),)
R_GL_FALLBACK(void, glBindAttribLocation, (GLuint program, GLuint index, const GLchar *name),)
R_GL_FALLBACK(void, glBindBuffer, (GLenum target, GLuint buffer),)
R_GL_FALLBACK(void, glBindFramebuffer, (GLenum target, GLuint framebuffer),)
R_GL_FALLBACK(void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer),)
R_GL_FALLBACK(void, glBindTexture, (GLenum target, GLuint texture),)
R_GL_FALLBACK(void, glBlendColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha),)
R_GL_FALLBACK(void, glBlendEquation, (GLenum mode),)
R_GL_FALLBACK(void, glBlendEquationSeparate, (GLenum modeRGB, GLenum modeAlpha),)
R_GL_FALLBACK(void, glBlendFunc, (GLenum sfactor, GLenum dfactor),)
R_GL_FALLBACK(void, glBlendFuncSeparate, (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha),)
R_GL_FALLBACK(void, glBufferData, (GLenum target, GLsizeiptr size, const void *data, GLenum usage),)
R_GL_FALLBACK(void, glBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const void *data),)
R_GL_FALLBACK(GLenum, glCheckFramebufferStatus, (GLenum target), 0)
R_GL_FALLBACK(void, glClear, (GLbitfield mask),)
R_GL_FALLBACK(void, glClearColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha),)
R_GL_FALLBACK(void, glClearDepthf, (GLfloat d),)
R_GL_FALLBACK(void, glClearStencil, (GLint s),)
R_GL_FALLBACK(void, glColorMask, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha),)
R_GL_FALLBACK(void, glCompileShader, (GLuint shader),)
R_GL_FALLBACK(void, glCompressedTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data),)
R_GL_FALLBACK(void, glCompressedTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data),)
R_GL_FALLBACK(void, glCopyTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border),)
R_GL_FALLBACK(void, glCopyTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height),)
R_GL_FALLBACK(GLuint, glCreateProgram, (void), 0)
R_GL_FALLBACK(GLuint, glCreateShader, (GLenum type), 0)
R_GL_FALLBACK(void, glCullFace, (GLenum mode),)
R_GL_FALLBACK(void, glDeleteBuffers, (GLsizei n, const GLuint *buffers),)
R_GL_FALLBACK(void, glDeleteFramebuffers, (GLsizei n, const GLuint *framebuffers),)
R_GL_FALLBACK(void, glDeleteProgram, (GLuint program),)
R_GL_FALLBACK(void, glDeleteRenderbuffers, (GLsizei n, const GLuint *renderbuffers),)
R_GL_FALLBACK(void, glDeleteShader, (GLuint shader),)
R_GL_FALLBACK(void, glDeleteTextures, (GLsizei n, const GLuint *textures),)
R_GL_FALLBACK(void, glDepthFunc, (GLenum func),)
R_GL_FALLBACK(void, glDepthMask, (GLboolean flag),)
R_GL_FALLBACK(void, glDepthRangef, (GLfloat n, GLfloat f),)
R_GL_FALLBACK(void, glDetachShader, (GLuint program, GLuint shader),)
R_GL_FALLBACK(void, glDisable, (GLenum cap),)
R_GL_FALLBACK(void, glDisableVertexAttribArray, (GLuint index),)
R_GL_FALLBACK(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count),)
R_GL_FALLBACK(void, glDrawElements, (GLenum mode, GLsizei count, GLenum type, const void *indices),)
R_GL_FALLBACK(void, glEnable, (GLenum cap),)
R_GL_FALLBACK(void, glEnableVertexAttribArray, (GLuint index),)
R_GL_FALLBACK(void, glFinish, (void),)
R_GL_FALLBACK(void, glFlush, (void),)
R_GL_FALLBACK(void, glFramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer),)
R_GL_FALLBACK(void, glFramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level),)
R_GL_FALLBACK(void, glFrontFace, (GLenum mode),)
R_GL_FALLBACK(void, glGenBuffers, (GLsizei n, GLuint *buffers),)
R_GL_FALLBACK(void, glGenerateMipmap, (GLenum target),)
R_GL_FALLBACK(void, glGenFramebuffers, (GLsizei n, GLuint *framebuffers),)
R_GL_FALLBACK(void, glGenRenderbuffers, (GLsizei n, GLuint *renderbuffers),)
R_GL_FALLBACK(void, glGenTextures, (GLsizei n, GLuint *textures),)
R_GL_FALLBACK(void, glGetActiveAttrib, (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name),)
R_GL_FALLBACK(void, glGetActiveUniform, (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name),)
R_GL_FALLBACK(void, glGetAttachedShaders, (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders),)
R_GL_FALLBACK(GLint, glGetAttribLocation, (GLuint program, const GLchar *name), -1)
R_GL_FALLBACK(void, glGetBooleanv, (GLenum pname, GLboolean *data),)
R_GL_FALLBACK(void, glGetBufferParameteriv, (GLenum target, GLenum pname, GLint *params),)
R_GL_FALLBACK(GLenum, glGetError, (void), GL_NO_ERROR)
R_GL_FALLBACK(void, glGetFloatv, (GLenum pname, GLfloat *data),)
R_GL_FALLBACK(void, glGetFramebufferAttachmentParameteriv, (GLenum target, GLenum attachment, GLenum pname, GLint *params),)
R_GL_FALLBACK(void, glGetIntegerv, (GLenum pname, GLint *data),)
R_GL_FALLBACK(void, glGetProgramiv, (GLuint program, GLenum pname, GLint *params),)
R_GL_FALLBACK(void, glGetProgramInfoLog, (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog),)
R_GL_FALLBACK(void, glGetRenderbufferParameteriv, (GLenum target, GLenum pname, GLint *params),)
R_GL_FALLBACK(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint *params),)
R_GL_FALLBACK(void, glGetShaderInfoLog, (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog),)
R_GL_FALLBACK(void, glGetShaderPrecisionFormat, (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision),)
R_GL_FALLBACK(void, glGetShaderSource, (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source),)
R_GL_FALLBACK(const GLubyte *GL_APIENTRY, glGetString, (GLenum name), NULL)
R_GL_FALLBACK(void, glGetTexParameterfv, (GLenum target, GLenum pname, GLfloat *params),)
R_GL_FALLBACK(void, glGetTexParameteriv, (GLenum target, GLenum pname, GLint *params),)
R_GL_FALLBACK(void, glGetUniformfv, (GLuint program, GLint location, GLfloat *params),)
R_GL_FALLBACK(void, glGetUniformiv, (GLuint program, GLint location, GLint *params),)
R_GL_FALLBACK(GLint, glGetUniformLocation, (GLuint program, const GLchar *name), -1)
R_GL_FALLBACK(void, glGetVertexAttribfv, (GLuint index, GLenum pname, GLfloat *params),)
R_GL_FALLBACK(void, glGetVertexAttribiv, (GLuint index, GLenum pname, GLint *params),)
R_GL_FALLBACK(void, glGetVertexAttribPointerv, (GLuint index, GLenum pname, void **pointer),)
R_GL_FALLBACK(void, glHint, (GLenum target, GLenum mode),)
R_GL_FALLBACK(GLboolean, glIsBuffer, (GLuint buffer), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsEnabled, (GLenum cap), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsFramebuffer, (GLuint framebuffer), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsProgram, (GLuint program), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsRenderbuffer, (GLuint renderbuffer), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsShader, (GLuint shader), GL_FALSE)
R_GL_FALLBACK(GLboolean, glIsTexture, (GLuint texture), GL_FALSE)
R_GL_FALLBACK(void, glLineWidth, (GLfloat width),)
R_GL_FALLBACK(void, glLinkProgram, (GLuint program),)
R_GL_FALLBACK(void, glPixelStorei, (GLenum pname, GLint param),)
R_GL_FALLBACK(void, glPolygonOffset, (GLfloat factor, GLfloat units),)
R_GL_FALLBACK(void, glReadPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels),)
R_GL_FALLBACK(void, glReleaseShaderCompiler, (void),)
R_GL_FALLBACK(void, glRenderbufferStorage, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height),)
R_GL_FALLBACK(void, glSampleCoverage, (GLfloat value, GLboolean invert),)
R_GL_FALLBACK(void, glScissor, (GLint x, GLint y, GLsizei width, GLsizei height),)
R_GL_FALLBACK(void, glShaderBinary, (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length),)
R_GL_FALLBACK(void, glShaderSource, (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length),)
R_GL_FALLBACK(void, glStencilFunc, (GLenum func, GLint ref, GLuint mask),)
R_GL_FALLBACK(void, glStencilFuncSeparate, (GLenum face, GLenum func, GLint ref, GLuint mask),)
R_GL_FALLBACK(void, glStencilMask, (GLuint mask),)
R_GL_FALLBACK(void, glStencilMaskSeparate, (GLenum face, GLuint mask),)
R_GL_FALLBACK(void, glStencilOp, (GLenum fail, GLenum zfail, GLenum zpass),)
R_GL_FALLBACK(void, glStencilOpSeparate, (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass),)
R_GL_FALLBACK(void, glTexImage2D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels),)
R_GL_FALLBACK(void, glTexParameterf, (GLenum target, GLenum pname, GLfloat param),)
R_GL_FALLBACK(void, glTexParameterfv, (GLenum target, GLenum pname, const GLfloat *params),)
R_GL_FALLBACK(void, glTexParameteri, (GLenum target, GLenum pname, GLint param),)
R_GL_FALLBACK(void, glTexParameteriv, (GLenum target, GLenum pname, const GLint *params),)
R_GL_FALLBACK(void, glTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels),)
R_GL_FALLBACK(void, glUniform1f, (GLint location, GLfloat v0),)
R_GL_FALLBACK(void, glUniform1fv, (GLint location, GLsizei count, const GLfloat *value),)
R_GL_FALLBACK(void, glUniform1i, (GLint location, GLint v0),)
R_GL_FALLBACK(void, glUniform1iv, (GLint location, GLsizei count, const GLint *value),)
R_GL_FALLBACK(void, glUniform2f, (GLint location, GLfloat v0, GLfloat v1),)
R_GL_FALLBACK(void, glUniform2fv, (GLint location, GLsizei count, const GLfloat *value),)
R_GL_FALLBACK(void, glUniform2i, (GLint location, GLint v0, GLint v1),)
R_GL_FALLBACK(void, glUniform2iv, (GLint location, GLsizei count, const GLint *value),)
R_GL_FALLBACK(void, glUniform3f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2),)
R_GL_FALLBACK(void, glUniform3fv, (GLint location, GLsizei count, const GLfloat *value),)
R_GL_FALLBACK(void, glUniform3i, (GLint location, GLint v0, GLint v1, GLint v2),)
R_GL_FALLBACK(void, glUniform3iv, (GLint location, GLsizei count, const GLint *value),)
R_GL_FALLBACK(void, glUniform4f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3),)
R_GL_FALLBACK(void, glUniform4fv, (GLint location, GLsizei count, const GLfloat *value),)
R_GL_FALLBACK(void, glUniform4i, (GLint location, GLint v0, GLint v1, GLint v2, GLint v3),)
R_GL_FALLBACK(void, glUniform4iv, (GLint location, GLsizei count, const GLint *value),)
R_GL_FALLBACK(void, glUniformMatrix2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),)
R_GL_FALLBACK(void, glUniformMatrix3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),)
R_GL_FALLBACK(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value),)
R_GL_FALLBACK(void, glUseProgram, (GLuint program),)
R_GL_FALLBACK(void, glValidateProgram, (GLuint program),)
R_GL_FALLBACK(void, glVertexAttrib1f, (GLuint index, GLfloat x),)
R_GL_FALLBACK(void, glVertexAttrib1fv, (GLuint index, const GLfloat *v),)
R_GL_FALLBACK(void, glVertexAttrib2f, (GLuint index, GLfloat x, GLfloat y),)
R_GL_FALLBACK(void, glVertexAttrib2fv, (GLuint index, const GLfloat *v),)
R_GL_FALLBACK(void, glVertexAttrib3f, (GLuint index, GLfloat x, GLfloat y, GLfloat z),)
R_GL_FALLBACK(void, glVertexAttrib3fv, (GLuint index, const GLfloat *v),)
R_GL_FALLBACK(void, glVertexAttrib4f, (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w),)
R_GL_FALLBACK(void, glVertexAttrib4fv, (GLuint index, const GLfloat *v),)
R_GL_FALLBACK(void, glVertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer),)
R_GL_FALLBACK(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height),)
R_GL_FALLBACK_EXT(void, glRenderbufferStorageMultisampleEXT, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height),)
R_GL_FALLBACK_EXT(void, glFramebufferTexture2DMultisampleEXT, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples),)
#   pragma GCC diagnostic pop
#   undef R_GL_DEFINE_FALLBACK
#endif
