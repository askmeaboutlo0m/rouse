/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
/*
 * Copyright (c) 2019, 2021 askmeaboutloom
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
#include <rouse.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"


static int r_framebufferoptions_width_newindex_xl(lua_State *L)
{
    R_FrameBufferOptions *self = R_CPPCAST(R_FrameBufferOptions *, XL_checkutype(L, 1, "R_FrameBufferOptions"));
    int VALUE = XL_checkint(L, 2);
    self->width = VALUE;
    return 0;
}

static int r_framebufferoptions_height_newindex_xl(lua_State *L)
{
    R_FrameBufferOptions *self = R_CPPCAST(R_FrameBufferOptions *, XL_checkutype(L, 1, "R_FrameBufferOptions"));
    int VALUE = XL_checkint(L, 2);
    self->height = VALUE;
    return 0;
}

static int r_framebufferoptions_samples_newindex_xl(lua_State *L)
{
    R_FrameBufferOptions *self = R_CPPCAST(R_FrameBufferOptions *, XL_checkutype(L, 1, "R_FrameBufferOptions"));
    int VALUE = XL_checkint(L, 2);
    self->samples = VALUE;
    return 0;
}

static int r_framebufferoptions_filter_newindex_xl(lua_State *L)
{
    R_FrameBufferOptions *self = R_CPPCAST(R_FrameBufferOptions *, XL_checkutype(L, 1, "R_FrameBufferOptions"));
    const char *VALUE = luaL_checkstring(L, 2);
    int filter;
    if (R_str_equal(VALUE, "nearest")) {
        filter = GL_NEAREST;
    }
    else if (R_str_equal(VALUE, "linear")) {
        filter = GL_LINEAR;
    }
    else {
        R_LUA_DIE(L, "Unknown frame buffer filter '%s'", VALUE);
    }
    self->min_filter = self->mag_filter = filter;
    return 0;
}


static R_FrameBufferAttachmentType get_attachment_type(
    lua_State *L, const char *what, const char *value)
{
    if (R_str_equal(value, "none")) {
        return R_FRAME_BUFFER_ATTACHMENT_NONE;
    }
    else if (R_str_equal(value, "buffer")) {
        return R_FRAME_BUFFER_ATTACHMENT_BUFFER;
    }
    else if (R_str_equal(value, "texture")) {
        return R_FRAME_BUFFER_ATTACHMENT_TEXTURE;
    }
    else {
        R_LUA_DIE(L, "Unknown frame buffer %s attachment type '%s'", what, value);
    }
}


static int r_framebufferoptions_color_type_newindex_xl(lua_State *L)
{
    R_FrameBufferOptions *self = R_CPPCAST(R_FrameBufferOptions *, XL_checkutype(L, 1, "R_FrameBufferOptions"));
    const char *VALUE = luaL_checkstring(L, 2);
    self->color_type = get_attachment_type(L, "color", VALUE);
    return 0;
}

static int r_framebufferoptions_depth_type_newindex_xl(lua_State *L)
{
    R_FrameBufferOptions *self = R_CPPCAST(R_FrameBufferOptions *, XL_checkutype(L, 1, "R_FrameBufferOptions"));
    const char *VALUE = luaL_checkstring(L, 2);
    self->depth_type = get_attachment_type(L, "depth", VALUE);
    return 0;
}

static int r_framebufferoptions_stencil_type_newindex_xl(lua_State *L)
{
    R_FrameBufferOptions *self = R_CPPCAST(R_FrameBufferOptions *, XL_checkutype(L, 1, "R_FrameBufferOptions"));
    const char *VALUE = luaL_checkstring(L, 2);
    self->stencil_type = get_attachment_type(L, "stencil", VALUE);
    return 0;
}

static int r_framebufferoptions_method_set_xl(lua_State *L)
{
    return XL_setfromtable(L, "R_FrameBufferOptions", 1, 2);
}

static int r_framebuffer_new_xl(lua_State *L)
{
    R_FrameBuffer *RETVAL;
    int argc = lua_gettop(L);
    if (argc == 1) {
        R_FrameBufferOptions *options = XL_pushnewutypeuv(
                L, (R_FrameBufferOptions[1]){R_frame_buffer_options()},
                sizeof(R_FrameBufferOptions), "R_FrameBufferOptions", 0);
        lua_getfield(L, -1, "set");
        lua_pushvalue(L, -2);
        lua_pushvalue(L, 1);
        lua_call(L, 2, 0);
        RETVAL = R_frame_buffer_new(options);
    }
    else {
        R_LUA_DIE(L, "Wrong number of arguments to R.FrameBuffer.new");
    }
    XL_pushnewpptypeuv(L, RETVAL, "R_FrameBuffer", 0);
    return 1;
}

static int r_framebuffer_new_2d_xl(lua_State *L)
{
    R_FrameBuffer *RETVAL;
    int argc = lua_gettop(L);
    if (argc == 1) {
        R_FrameBufferOptions *options = XL_pushnewutypeuv(
                L, (R_FrameBufferOptions[1]){R_frame_buffer_2d_options(0, 0)},
                sizeof(R_FrameBufferOptions), "R_FrameBufferOptions", 0);
        lua_getfield(L, -1, "set");
        lua_pushvalue(L, -2);
        lua_pushvalue(L, 1);
        lua_call(L, 2, 0);
        RETVAL = R_frame_buffer_new(options);
    }
    else if (argc == 2) {
        int width  = XL_checkint(L, 1);
        int height = XL_checkint(L, 2);
        RETVAL     = R_frame_buffer_2d_new(width, height);
    }
    else {
        R_LUA_DIE(L, "Wrong number of arguments to R.FrameBuffer.new_2d");
    }
    XL_pushnewpptypeuv(L, RETVAL, "R_FrameBuffer", 0);
    return 1;
}

static int r_framebuffer_method_gc_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype_nullable(L, 1, "R_FrameBuffer"));
    R_frame_buffer_free(self);
    return 0;
}

static int r_framebuffer_width_index_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    int RETVAL;
    R_MAGIC_CHECK(R_FrameBuffer, self);
    RETVAL = self->width;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_framebuffer_height_index_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    int RETVAL;
    R_MAGIC_CHECK(R_FrameBuffer, self);
    RETVAL = self->height;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_framebuffer_real_width_index_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    int RETVAL;
    R_MAGIC_CHECK(R_FrameBuffer, self);
    RETVAL = self->real_width;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_framebuffer_real_height_index_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    int RETVAL;
    R_MAGIC_CHECK(R_FrameBuffer, self);
    RETVAL = self->real_height;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_framebuffer_handle_index_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    unsigned int RETVAL;
    R_MAGIC_CHECK(R_FrameBuffer, self);
    RETVAL = self->handle;
    XL_pushuint(L, RETVAL);
    return 1;
}

static int r_framebuffer_color_index_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    unsigned int RETVAL;
    R_MAGIC_CHECK(R_FrameBuffer, self);
    RETVAL = self->color;
    XL_pushuint(L, RETVAL);
    return 1;
}

static int r_framebuffer_depth_index_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    unsigned int RETVAL;
    R_MAGIC_CHECK(R_FrameBuffer, self);
    RETVAL = self->depth;
    XL_pushuint(L, RETVAL);
    return 1;
}

static int r_framebuffer_stencil_index_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    unsigned int RETVAL;
    R_MAGIC_CHECK(R_FrameBuffer, self);
    RETVAL = self->stencil;
    XL_pushuint(L, RETVAL);
    return 1;
}

static int r_framebuffer_method_bind_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    R_frame_buffer_bind(self);
    return 0;
}

static int r_framebuffer_method_unbind_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
    XL_UNUSED(self);
    R_frame_buffer_unbind();
    return 0;
}

static int r_framebuffer_unbind_xl(lua_State *L)
{
    XL_UNUSED(L);
    R_frame_buffer_unbind();
    return 0;
}

static int r_framebuffer_method_write_to_stdout_xl(lua_State *L)
{
    R_FrameBuffer *self = R_CPPCAST(R_FrameBuffer *, XL_checkpptype(L, 1, "R_FrameBuffer"));
#ifdef __EMSCRIPTEN__
    /* You're not gonna record video in Emscripten, so this isn't allowed. */
    XL_UNUSED(self);
    R_LUA_DIE(L, "write_to_stdout not supported in Emscripten");
#else
    R_frame_buffer_write(self, stdout);
#endif
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

static int r_framebuffer_index_anchor_xl;
static int r_framebuffer_index_xl(lua_State *L)
{
    return XL_index(L, "R_FrameBuffer", &r_framebuffer_index_anchor_xl, 1, 2);
}

static int r_framebufferoptions_index_xl(lua_State *L)
{
    return XL_index_fallback(L, "R_FrameBufferOptions", 1, 2);
}

int r_framebufferoptions_newindex_anchor_xl;
static int r_framebufferoptions_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_FrameBufferOptions", &r_framebufferoptions_newindex_anchor_xl, 1, 2, 3);
}

static luaL_Reg r_framebuffer_function_registry_xl[] = {
    {"new", r_framebuffer_new_xl},
    {"new_2d", r_framebuffer_new_2d_xl},
    {"unbind", r_framebuffer_unbind_xl},
    {NULL, NULL},
};

static luaL_Reg r_framebuffer_index_registry_xl[] = {
    {"color", r_framebuffer_color_index_xl},
    {"depth", r_framebuffer_depth_index_xl},
    {"handle", r_framebuffer_handle_index_xl},
    {"height", r_framebuffer_height_index_xl},
    {"real_height", r_framebuffer_real_height_index_xl},
    {"real_width", r_framebuffer_real_width_index_xl},
    {"stencil", r_framebuffer_stencil_index_xl},
    {"width", r_framebuffer_width_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_framebuffer_method_registry_xl[] = {
    {"__gc", r_framebuffer_method_gc_xl},
    {"__index", r_framebuffer_index_xl},
    {"bind", r_framebuffer_method_bind_xl},
    {"unbind", r_framebuffer_method_unbind_xl},
    {"write_to_stdout", r_framebuffer_method_write_to_stdout_xl},
    {NULL, NULL},
};

static luaL_Reg r_framebufferoptions_method_registry_xl[] = {
    {"__index", r_framebufferoptions_index_xl},
    {"__newindex", r_framebufferoptions_newindex_xl},
    {"set", r_framebufferoptions_method_set_xl},
    {NULL, NULL},
};

static luaL_Reg r_framebufferoptions_newindex_registry_xl[] = {
    {"color_type", r_framebufferoptions_color_type_newindex_xl},
    {"depth_type", r_framebufferoptions_depth_type_newindex_xl},
    {"filter", r_framebufferoptions_filter_newindex_xl},
    {"height", r_framebufferoptions_height_newindex_xl},
    {"samples", r_framebufferoptions_samples_newindex_xl},
    {"stencil_type", r_framebufferoptions_stencil_type_newindex_xl},
    {"width", r_framebufferoptions_width_newindex_xl},
    {NULL, NULL},
};

int R_lua_frame_buffer_init(lua_State *L)
{
    XL_initmetatable(L, "R_FrameBuffer", r_framebuffer_method_registry_xl);
    XL_initmetatable(L, "R_FrameBufferOptions", r_framebufferoptions_method_registry_xl);
    XL_initindextable(L, &r_framebuffer_index_anchor_xl, r_framebuffer_index_registry_xl);
    XL_initnewindextable(L, &r_framebufferoptions_newindex_anchor_xl, r_framebufferoptions_newindex_registry_xl);
    XL_initfunctions(L, r_framebuffer_function_registry_xl, "R", "FrameBuffer", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
