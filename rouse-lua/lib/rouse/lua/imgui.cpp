/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
/*
 * Copyright (c) 2021 - 2022 askmeaboutloom
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
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

extern "C" {

#include "lua_inc.h"
#include "intern.h"
#include "util.h"

static int imgui_quit_handler_id = 0;

static void imgui_quit(void)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

static void imgui_on_quit(R_UNUSED void *arg)
{
    imgui_quit();
}


struct BeginStackEntry {
    const char *begin_name;
    int end_id;
    void (*end_fn)(void);
};

static ImVector<BeginStackEntry> begin_stack;

void R_lua_imgui_begin(const char *begin_name, int end_id, void (*end_fn)(void))
{
    BeginStackEntry entry = {begin_name, end_id, end_fn};
    begin_stack.push_back(entry);
}

void R_lua_imgui_end(lua_State *L, const char *end_name, int end_id)
{
    if (begin_stack.empty()) {
        R_LUA_DIE(L, "Unexpected %s, there's nothing to end", end_name);
    }
    const BeginStackEntry &entry = begin_stack.back();
    if (entry.end_id == end_id) {
        begin_stack.pop_back();
    }
    else  {
        R_LUA_DIE(L, "Unexpected %s to end %s", end_name, entry.begin_name);
    }
}


static bool imgui_have_frame = false;

static void close_begin_stack(void)
{
    for (int i = begin_stack.size(); i > 0; --i) {
        const BeginStackEntry &entry = begin_stack[i - 1];
        R_warn("Closing %s", entry.begin_name);
        entry.end_fn();
    }
    begin_stack.clear();
}



static int imgui_init_xl(lua_State *L)
{
    bool RETVAL;
    if (imgui_quit_handler_id == 0) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO &io    = ImGui::GetIO();
        io.IniFilename = NULL;

        int type = lua_gettop(L) == 0 ? LUA_TNONE : lua_type(L, 1);
        if (type == LUA_TNONE || type == LUA_TNIL) {
            ImGui::StyleColorsDark();
        }
        else if (type == LUA_TSTRING) {
            const char *style = lua_tostring(L, 1);
            if (R_str_equal(style, "dark")) {
                ImGui::StyleColorsDark();
            }
            else if (R_str_equal(style, "light")) {
                ImGui::StyleColorsLight();
            }
            else if (R_str_equal(style, "classic")) {
                ImGui::StyleColorsClassic();
            }
            else {
                R_LUA_DIE(L, "Unknown ImGui style '%s'", style);
            }
        }
        else {
            lua_pushvalue(L, 1);
            if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
                ImGui::DestroyContext();
                lua_error(L);
            }
        }

        ImGui_ImplSDL2_InitForOpenGL(R_window, R_glcontext);
        ImGui_ImplOpenGL3_Init("#version 100");
        imgui_quit_handler_id = R_quit_handler_add(imgui_on_quit, NULL, 20000);
        RETVAL                = true;
    }
    else {
        RETVAL = false;
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int imgui_shutdown_xl(lua_State *L)
{
    bool RETVAL;
    XL_UNUSED(L);
    if (imgui_quit_handler_id == 0) {
        RETVAL = false;
    }
    else {
        if (imgui_have_frame) {
            close_begin_stack();
            R_warn("Discarding previous ImGui frame");
            ImGui::EndFrame();
            imgui_have_frame = false;
        }
        R_quit_handler_remove(imgui_quit_handler_id, NULL);
        imgui_quit_handler_id = 0;
        imgui_quit();
        RETVAL = true;
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int imgui_stylecolorsdark_xl(lua_State *L)
{
    XL_UNUSED(L);
    ImGui::StyleColorsDark();
    return 0;
}

static int imgui_stylecolorslight_xl(lua_State *L)
{
    XL_UNUSED(L);
    ImGui::StyleColorsLight();
    return 0;
}

static int imgui_stylecolorsclassic_xl(lua_State *L)
{
    XL_UNUSED(L);
    ImGui::StyleColorsClassic();
    return 0;
}

static int imgui_processevent_xl(lua_State *L)
{
    SDL_Event *event = R_CPPCAST(SDL_Event *, XL_checkutype(L, 1, "SDL_Event"));
    ImGui_ImplSDL2_ProcessEvent(event);
    return 0;
}

static int imgui_newframe_xl(lua_State *L)
{
    XL_UNUSED(L);
    if (imgui_have_frame) {
        close_begin_stack();
        R_warn("Discarding previous ImGui frame");
        ImGui::EndFrame();
    }
    else {
        imgui_have_frame = true;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    return 0;
}

static int imgui_render_xl(lua_State *L)
{
    XL_UNUSED(L);
    if (imgui_have_frame) {
        close_begin_stack();
        ImGui::Render();
        imgui_have_frame = false;
    }
    else {
        R_LUA_DIE(L, "No ImGui frame to render");
    }
    return 0;
}

static int imgui_renderdrawdata_xl(lua_State *L)
{
    XL_UNUSED(L);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return 0;
}


static int init(lua_State *L);
int ImGuiLua_InitImGui(lua_State *L);

int R_lua_imgui_init(lua_State *L)
{
    init(L);
    lua_pushcfunction(L, ImGuiLua_InitImGui);
    lua_pushglobaltable(L);
    lua_call(L, 1, 0);
    return 0;
}

}

#ifdef __cplusplus
extern "C" {
#endif

static luaL_Reg imgui_function_registry_xl[] = {
    {"Init", imgui_init_xl},
    {"NewFrame", imgui_newframe_xl},
    {"ProcessEvent", imgui_processevent_xl},
    {"Render", imgui_render_xl},
    {"RenderDrawData", imgui_renderdrawdata_xl},
    {"Shutdown", imgui_shutdown_xl},
    {"StyleColorsClassic", imgui_stylecolorsclassic_xl},
    {"StyleColorsDark", imgui_stylecolorsdark_xl},
    {"StyleColorsLight", imgui_stylecolorslight_xl},
    {NULL, NULL},
};

static int init(lua_State *L)
{
    XL_initfunctions(L, imgui_function_registry_xl, "ImGui", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
