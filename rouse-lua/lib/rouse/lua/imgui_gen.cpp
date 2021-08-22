/*
 * This file is auto-generated.
 *
 * The generator is Copyright (c) 2021 askmeaboutloom.
 *
 * That generator is based on cimgui <https://github.com/cimgui/cimgui>,
 * which is Copyright (c) 2015 Stephan Dilly.
 *
 * That is based on Dear Imgui <https://github.com/ocornut/imgui>, which is
 * Copyright (c) 2014-2021 Omar Cornut.
 *
 * They are all licensed under the terms of the MIT license:
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
#include <imgui.h>
#include <rouse.h>

extern "C" {
#include "lua_inc.h"
#include "intern.h"
#include "util.h"
}

#define IMGUILUA_SETUP(L) do { \
        /* put your custom setup code here */ \
    } while (0)

extern "C" void R_lua_imgui_begin(const char *begin_name, int end_id, void (*end_fn)(void));

#define IMGUILUA_BEGIN(L, BEGIN, END, END_ID) do { \
        R_lua_imgui_begin(#BEGIN, END_ID, END); \
    } while (0)

extern "C" void R_lua_imgui_end(lua_State *L, const char *end_name, int end_id);

#define IMGUILUA_END(L, END, END_ID, ...) do { \
        R_lua_imgui_end(L, #END, END_ID); \
    } while (0)

static ImVec2 ImGuiLua_ToImVec2(lua_State *L, int index)
{
    R_V2 rouse_v2 = *static_cast<R_V2 *>(luaL_checkudata(L, index, "R_V2"));
    return ImVec2(rouse_v2.x, rouse_v2.y);
}

static void ImGuiLua_PushImVec2(lua_State *L, const ImVec2 &im_vec2)
{
    R_V2 rouse_v2 = R_v2(im_vec2.x, im_vec2.y);
    XL_pushnewutypeuv(L, &rouse_v2, sizeof(R_V2), "R_V2", 0);
}

static ImVec4 ImGuiLua_ToImVec4(lua_State *L, int index)
{
    R_V4 rouse_v4 = *static_cast<R_V4 *>(luaL_checkudata(L, index, "R_V4"));
    return ImVec4(rouse_v4.x, rouse_v4.y, rouse_v4.z, rouse_v4.w);
}

static void ImGuiLua_PushImVec4(lua_State *L, const ImVec4 &im_vec4)
{
    R_V4 rouse_v4 = R_v4(im_vec4.x, im_vec4.y, im_vec4.z, im_vec4.w);
    XL_pushnewutypeuv(L, &rouse_v4, sizeof(R_V4), "R_V4", 0);
}

// ----------------------------------------------------------------------------

#define IMGUILUA_ENUM(X)     /* nothing, just a marker for the generator */
#define IMGUILUA_FUNCTION(X) /* nothing, just a marker for the generator */

static void ImGuiLua_SetEnumValue(lua_State *L, const char *name, int value)
{
    lua_pushinteger(L, static_cast<lua_Integer>(value));
    lua_setfield(L, -2, name);
}

static void ImGuiLua_GetOrCreateTable(lua_State *L, const char *name)
{
    lua_pushstring(L, name);
    int type = lua_rawget(L, -2);
    switch (type) {
    case LUA_TTABLE:
        break;
    case LUA_TNIL:
    case LUA_TNONE:
        lua_pop(L, 1);
        lua_newtable(L);
        lua_pushstring(L, name);
        lua_pushvalue(L, -2);
        lua_rawset(L, -4);
        break;
    default:
        lua_pop(L, 1);
        luaL_error(L, "Value '%s' should be a table, but is %s",
                   name, lua_typename(L, type));
    }
}

static void ImGuiLua_InitMetaTable(lua_State *L, const char *tname, luaL_Reg *methods)
{
    luaL_newmetatable(L, tname);
    if (methods) {
        luaL_setfuncs(L, methods, 0);
    }
    lua_pop(L, 1);
}


static int ImGuiLua_BoolRefIndex(lua_State *L)
{
    bool *ref = static_cast<bool *>(luaL_checkudata(L, 1, "ImGuiLua_BoolRef"));
    lua_pushliteral(L, "value");
    if (lua_rawequal(L, 2, -1)) {
        lua_pushboolean(L, *ref);
    }
    else {
        lua_pushnil(L);
    }
    return 1;
}

static int ImGuiLua_BoolRefNewindex(lua_State *L)
{
    bool *ref = static_cast<bool *>(luaL_checkudata(L, 1, "ImGuiLua_BoolRef"));
    lua_pushliteral(L, "value");
    if (lua_rawequal(L, 2, -1)) {
        bool value = lua_toboolean(L, 3);
        *ref = value;
        return 0;
    }
    else {
        const char *key = lua_tostring(L, 2);
        return luaL_error(L, "Attempt to assign to nonexistent field '%s'", key);
    }
}

static luaL_Reg ImGuiLua_BoolRefMethods[] = {
    {"__index", ImGuiLua_BoolRefIndex},
    {"__newindex", ImGuiLua_BoolRefNewindex},
    {NULL, NULL},
};

IMGUILUA_FUNCTION(ImGui::Bool)
static int ImGuiLua_Bool(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    bool value = ARGC == 0 ? false : lua_toboolean(L, 1);
    bool *ref = static_cast<bool *>(lua_newuserdatauv(L, sizeof(*ref), 0));
    luaL_setmetatable(L, "ImGuiLua_BoolRef");
    *ref = value;
    return 1;
}


static int ImGuiLua_IntRefIndex(lua_State *L)
{
    int *ref = static_cast<int *>(luaL_checkudata(L, 1, "ImGuiLua_IntRef"));
    lua_pushliteral(L, "value");
    if (lua_rawequal(L, 2, -1)) {
        lua_pushinteger(L, static_cast<lua_Integer>(*ref));
    }
    else {
        lua_pushnil(L);
    }
    return 1;
}

static int ImGuiLua_IntRefNewindex(lua_State *L)
{
    int *ref = static_cast<int *>(luaL_checkudata(L, 1, "ImGuiLua_IntRef"));
    lua_pushliteral(L, "value");
    if (lua_rawequal(L, 2, -1)) {
        int value = static_cast<int>(lua_tointeger(L, 3));
        *ref = value;
        return 0;
    }
    else {
        const char *key = lua_tostring(L, 2);
        return luaL_error(L, "Attempt to assign to nonexistent field '%s'", key);
    }
}

static luaL_Reg ImGuiLua_IntRefMethods[] = {
    {"__index", ImGuiLua_IntRefIndex},
    {"__newindex", ImGuiLua_IntRefNewindex},
    {NULL, NULL},
};

IMGUILUA_FUNCTION(ImGui::Int)
static int ImGuiLua_Int(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int value = ARGC == 0 ? 0 : lua_tointeger(L, 1);
    int *ref = static_cast<int *>(lua_newuserdatauv(L, sizeof(*ref), 0));
    luaL_setmetatable(L, "ImGuiLua_IntRef");
    *ref = value;
    return 1;
}


static int ImGuiLua_FloatRefIndex(lua_State *L)
{
    float *ref = static_cast<float *>(luaL_checkudata(L, 1, "ImGuiLua_FloatRef"));
    lua_pushliteral(L, "value");
    if (lua_rawequal(L, 2, -1)) {
        lua_pushnumber(L, static_cast<lua_Number>(*ref));
    }
    else {
        lua_pushnil(L);
    }
    return 1;
}

static int ImGuiLua_FloatRefNewindex(lua_State *L)
{
    float *ref = static_cast<float *>(luaL_checkudata(L, 1, "ImGuiLua_FloatRef"));
    lua_pushliteral(L, "value");
    if (lua_rawequal(L, 2, -1)) {
        float value = static_cast<float>(lua_tonumber(L, 3));
        *ref = value;
        return 0;
    }
    else {
        const char *key = lua_tostring(L, 2);
        return luaL_error(L, "Attempt to assign to nonexistent field '%s'", key);
    }
}

static luaL_Reg ImGuiLua_FloatRefMethods[] = {
    {"__index", ImGuiLua_FloatRefIndex},
    {"__newindex", ImGuiLua_FloatRefNewindex},
    {NULL, NULL},
};

IMGUILUA_FUNCTION(ImGui::Float)
static int ImGuiLua_Float(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    float value = ARGC == 0 ? 0.0f : static_cast<float>(lua_tonumber(L, 1));
    float *ref = static_cast<float *>(lua_newuserdatauv(L, sizeof(*ref), 0));
    luaL_setmetatable(L, "ImGuiLua_FloatRef");
    *ref = value;
    return 1;
}


static void ImGuiLua_InitTypes(lua_State *L)
{
    ImGuiLua_InitMetaTable(L, "ImGuiLua_BoolRef", ImGuiLua_BoolRefMethods);
    ImGuiLua_InitMetaTable(L, "ImGuiLua_IntRef", ImGuiLua_IntRefMethods);
    ImGuiLua_InitMetaTable(L, "ImGuiLua_FloatRef", ImGuiLua_FloatRefMethods);
}


IMGUILUA_ENUM(ImDrawFlags)
static void ImGuiLua_RegisterEnumImDrawFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Closed", 1 << 0);
    ImGuiLua_SetEnumValue(L, "RoundCornersTopLeft", 1 << 4);
    ImGuiLua_SetEnumValue(L, "RoundCornersTopRight", 1 << 5);
    ImGuiLua_SetEnumValue(L, "RoundCornersBottomLeft", 1 << 6);
    ImGuiLua_SetEnumValue(L, "RoundCornersBottomRight", 1 << 7);
    ImGuiLua_SetEnumValue(L, "RoundCornersNone", 1 << 8);
    ImGuiLua_SetEnumValue(L, "RoundCornersTop", ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight);
    ImGuiLua_SetEnumValue(L, "RoundCornersBottom", ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight);
    ImGuiLua_SetEnumValue(L, "RoundCornersLeft", ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersTopLeft);
    ImGuiLua_SetEnumValue(L, "RoundCornersRight", ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopRight);
    ImGuiLua_SetEnumValue(L, "RoundCornersAll", ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight);
    ImGuiLua_SetEnumValue(L, "RoundCornersDefault_", ImDrawFlags_RoundCornersAll);
    ImGuiLua_SetEnumValue(L, "RoundCornersMask_", ImDrawFlags_RoundCornersAll | ImDrawFlags_RoundCornersNone);
}

IMGUILUA_ENUM(ImDrawListFlags)
static void ImGuiLua_RegisterEnumImDrawListFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "AntiAliasedLines", 1 << 0);
    ImGuiLua_SetEnumValue(L, "AntiAliasedLinesUseTex", 1 << 1);
    ImGuiLua_SetEnumValue(L, "AntiAliasedFill", 1 << 2);
    ImGuiLua_SetEnumValue(L, "AllowVtxOffset", 1 << 3);
}

IMGUILUA_ENUM(ImFontAtlasFlags)
static void ImGuiLua_RegisterEnumImFontAtlasFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "NoPowerOfTwoHeight", 1 << 0);
    ImGuiLua_SetEnumValue(L, "NoMouseCursors", 1 << 1);
    ImGuiLua_SetEnumValue(L, "NoBakedLines", 1 << 2);
}

IMGUILUA_ENUM(ImGuiBackendFlags)
static void ImGuiLua_RegisterEnumImGuiBackendFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "HasGamepad", 1 << 0);
    ImGuiLua_SetEnumValue(L, "HasMouseCursors", 1 << 1);
    ImGuiLua_SetEnumValue(L, "HasSetMousePos", 1 << 2);
    ImGuiLua_SetEnumValue(L, "RendererHasVtxOffset", 1 << 3);
}

IMGUILUA_ENUM(ImGuiButtonFlags)
static void ImGuiLua_RegisterEnumImGuiButtonFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "MouseButtonLeft", 1 << 0);
    ImGuiLua_SetEnumValue(L, "MouseButtonRight", 1 << 1);
    ImGuiLua_SetEnumValue(L, "MouseButtonMiddle", 1 << 2);
    ImGuiLua_SetEnumValue(L, "MouseButtonMask_", ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle);
    ImGuiLua_SetEnumValue(L, "MouseButtonDefault_", ImGuiButtonFlags_MouseButtonLeft);
}

IMGUILUA_ENUM(ImGuiCol)
static void ImGuiLua_RegisterEnumImGuiCol(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "Text", 0);
    ImGuiLua_SetEnumValue(L, "TextDisabled", 1);
    ImGuiLua_SetEnumValue(L, "WindowBg", 2);
    ImGuiLua_SetEnumValue(L, "ChildBg", 3);
    ImGuiLua_SetEnumValue(L, "PopupBg", 4);
    ImGuiLua_SetEnumValue(L, "Border", 5);
    ImGuiLua_SetEnumValue(L, "BorderShadow", 6);
    ImGuiLua_SetEnumValue(L, "FrameBg", 7);
    ImGuiLua_SetEnumValue(L, "FrameBgHovered", 8);
    ImGuiLua_SetEnumValue(L, "FrameBgActive", 9);
    ImGuiLua_SetEnumValue(L, "TitleBg", 10);
    ImGuiLua_SetEnumValue(L, "TitleBgActive", 11);
    ImGuiLua_SetEnumValue(L, "TitleBgCollapsed", 12);
    ImGuiLua_SetEnumValue(L, "MenuBarBg", 13);
    ImGuiLua_SetEnumValue(L, "ScrollbarBg", 14);
    ImGuiLua_SetEnumValue(L, "ScrollbarGrab", 15);
    ImGuiLua_SetEnumValue(L, "ScrollbarGrabHovered", 16);
    ImGuiLua_SetEnumValue(L, "ScrollbarGrabActive", 17);
    ImGuiLua_SetEnumValue(L, "CheckMark", 18);
    ImGuiLua_SetEnumValue(L, "SliderGrab", 19);
    ImGuiLua_SetEnumValue(L, "SliderGrabActive", 20);
    ImGuiLua_SetEnumValue(L, "Button", 21);
    ImGuiLua_SetEnumValue(L, "ButtonHovered", 22);
    ImGuiLua_SetEnumValue(L, "ButtonActive", 23);
    ImGuiLua_SetEnumValue(L, "Header", 24);
    ImGuiLua_SetEnumValue(L, "HeaderHovered", 25);
    ImGuiLua_SetEnumValue(L, "HeaderActive", 26);
    ImGuiLua_SetEnumValue(L, "Separator", 27);
    ImGuiLua_SetEnumValue(L, "SeparatorHovered", 28);
    ImGuiLua_SetEnumValue(L, "SeparatorActive", 29);
    ImGuiLua_SetEnumValue(L, "ResizeGrip", 30);
    ImGuiLua_SetEnumValue(L, "ResizeGripHovered", 31);
    ImGuiLua_SetEnumValue(L, "ResizeGripActive", 32);
    ImGuiLua_SetEnumValue(L, "Tab", 33);
    ImGuiLua_SetEnumValue(L, "TabHovered", 34);
    ImGuiLua_SetEnumValue(L, "TabActive", 35);
    ImGuiLua_SetEnumValue(L, "TabUnfocused", 36);
    ImGuiLua_SetEnumValue(L, "TabUnfocusedActive", 37);
    ImGuiLua_SetEnumValue(L, "PlotLines", 38);
    ImGuiLua_SetEnumValue(L, "PlotLinesHovered", 39);
    ImGuiLua_SetEnumValue(L, "PlotHistogram", 40);
    ImGuiLua_SetEnumValue(L, "PlotHistogramHovered", 41);
    ImGuiLua_SetEnumValue(L, "TableHeaderBg", 42);
    ImGuiLua_SetEnumValue(L, "TableBorderStrong", 43);
    ImGuiLua_SetEnumValue(L, "TableBorderLight", 44);
    ImGuiLua_SetEnumValue(L, "TableRowBg", 45);
    ImGuiLua_SetEnumValue(L, "TableRowBgAlt", 46);
    ImGuiLua_SetEnumValue(L, "TextSelectedBg", 47);
    ImGuiLua_SetEnumValue(L, "DragDropTarget", 48);
    ImGuiLua_SetEnumValue(L, "NavHighlight", 49);
    ImGuiLua_SetEnumValue(L, "NavWindowingHighlight", 50);
    ImGuiLua_SetEnumValue(L, "NavWindowingDimBg", 51);
    ImGuiLua_SetEnumValue(L, "ModalWindowDimBg", 52);
    ImGuiLua_SetEnumValue(L, "COUNT", 53);
}

IMGUILUA_ENUM(ImGuiColorEditFlags)
static void ImGuiLua_RegisterEnumImGuiColorEditFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "NoAlpha", 1 << 1);
    ImGuiLua_SetEnumValue(L, "NoPicker", 1 << 2);
    ImGuiLua_SetEnumValue(L, "NoOptions", 1 << 3);
    ImGuiLua_SetEnumValue(L, "NoSmallPreview", 1 << 4);
    ImGuiLua_SetEnumValue(L, "NoInputs", 1 << 5);
    ImGuiLua_SetEnumValue(L, "NoTooltip", 1 << 6);
    ImGuiLua_SetEnumValue(L, "NoLabel", 1 << 7);
    ImGuiLua_SetEnumValue(L, "NoSidePreview", 1 << 8);
    ImGuiLua_SetEnumValue(L, "NoDragDrop", 1 << 9);
    ImGuiLua_SetEnumValue(L, "NoBorder", 1 << 10);
    ImGuiLua_SetEnumValue(L, "AlphaBar", 1 << 16);
    ImGuiLua_SetEnumValue(L, "AlphaPreview", 1 << 17);
    ImGuiLua_SetEnumValue(L, "AlphaPreviewHalf", 1 << 18);
    ImGuiLua_SetEnumValue(L, "HDR", 1 << 19);
    ImGuiLua_SetEnumValue(L, "DisplayRGB", 1 << 20);
    ImGuiLua_SetEnumValue(L, "DisplayHSV", 1 << 21);
    ImGuiLua_SetEnumValue(L, "DisplayHex", 1 << 22);
    ImGuiLua_SetEnumValue(L, "Uint8", 1 << 23);
    ImGuiLua_SetEnumValue(L, "Float", 1 << 24);
    ImGuiLua_SetEnumValue(L, "PickerHueBar", 1 << 25);
    ImGuiLua_SetEnumValue(L, "PickerHueWheel", 1 << 26);
    ImGuiLua_SetEnumValue(L, "InputRGB", 1 << 27);
    ImGuiLua_SetEnumValue(L, "InputHSV", 1 << 28);
    ImGuiLua_SetEnumValue(L, "DefaultOptions_", ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar);
    ImGuiLua_SetEnumValue(L, "DisplayMask_", ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
    ImGuiLua_SetEnumValue(L, "DataTypeMask_", ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_Float);
    ImGuiLua_SetEnumValue(L, "PickerMask_", ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_PickerHueBar);
    ImGuiLua_SetEnumValue(L, "InputMask_", ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_InputHSV);
}

IMGUILUA_ENUM(ImGuiComboFlags)
static void ImGuiLua_RegisterEnumImGuiComboFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "PopupAlignLeft", 1 << 0);
    ImGuiLua_SetEnumValue(L, "HeightSmall", 1 << 1);
    ImGuiLua_SetEnumValue(L, "HeightRegular", 1 << 2);
    ImGuiLua_SetEnumValue(L, "HeightLarge", 1 << 3);
    ImGuiLua_SetEnumValue(L, "HeightLargest", 1 << 4);
    ImGuiLua_SetEnumValue(L, "NoArrowButton", 1 << 5);
    ImGuiLua_SetEnumValue(L, "NoPreview", 1 << 6);
    ImGuiLua_SetEnumValue(L, "HeightMask_", ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest);
}

IMGUILUA_ENUM(ImGuiCond)
static void ImGuiLua_RegisterEnumImGuiCond(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Always", 1 << 0);
    ImGuiLua_SetEnumValue(L, "Once", 1 << 1);
    ImGuiLua_SetEnumValue(L, "FirstUseEver", 1 << 2);
    ImGuiLua_SetEnumValue(L, "Appearing", 1 << 3);
}

IMGUILUA_ENUM(ImGuiConfigFlags)
static void ImGuiLua_RegisterEnumImGuiConfigFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "NavEnableKeyboard", 1 << 0);
    ImGuiLua_SetEnumValue(L, "NavEnableGamepad", 1 << 1);
    ImGuiLua_SetEnumValue(L, "NavEnableSetMousePos", 1 << 2);
    ImGuiLua_SetEnumValue(L, "NavNoCaptureKeyboard", 1 << 3);
    ImGuiLua_SetEnumValue(L, "NoMouse", 1 << 4);
    ImGuiLua_SetEnumValue(L, "NoMouseCursorChange", 1 << 5);
    ImGuiLua_SetEnumValue(L, "IsSRGB", 1 << 20);
    ImGuiLua_SetEnumValue(L, "IsTouchScreen", 1 << 21);
}

IMGUILUA_ENUM(ImGuiDataType)
static void ImGuiLua_RegisterEnumImGuiDataType(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "S8", 0);
    ImGuiLua_SetEnumValue(L, "U8", 1);
    ImGuiLua_SetEnumValue(L, "S16", 2);
    ImGuiLua_SetEnumValue(L, "U16", 3);
    ImGuiLua_SetEnumValue(L, "S32", 4);
    ImGuiLua_SetEnumValue(L, "U32", 5);
    ImGuiLua_SetEnumValue(L, "S64", 6);
    ImGuiLua_SetEnumValue(L, "U64", 7);
    ImGuiLua_SetEnumValue(L, "Float", 8);
    ImGuiLua_SetEnumValue(L, "Double", 9);
    ImGuiLua_SetEnumValue(L, "COUNT", 10);
}

IMGUILUA_ENUM(ImGuiDir)
static void ImGuiLua_RegisterEnumImGuiDir(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", -1);
    ImGuiLua_SetEnumValue(L, "Left", 0);
    ImGuiLua_SetEnumValue(L, "Right", 1);
    ImGuiLua_SetEnumValue(L, "Up", 2);
    ImGuiLua_SetEnumValue(L, "Down", 3);
    ImGuiLua_SetEnumValue(L, "COUNT", 4);
}

IMGUILUA_ENUM(ImGuiDragDropFlags)
static void ImGuiLua_RegisterEnumImGuiDragDropFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "SourceNoPreviewTooltip", 1 << 0);
    ImGuiLua_SetEnumValue(L, "SourceNoDisableHover", 1 << 1);
    ImGuiLua_SetEnumValue(L, "SourceNoHoldToOpenOthers", 1 << 2);
    ImGuiLua_SetEnumValue(L, "SourceAllowNullID", 1 << 3);
    ImGuiLua_SetEnumValue(L, "SourceExtern", 1 << 4);
    ImGuiLua_SetEnumValue(L, "SourceAutoExpirePayload", 1 << 5);
    ImGuiLua_SetEnumValue(L, "AcceptBeforeDelivery", 1 << 10);
    ImGuiLua_SetEnumValue(L, "AcceptNoDrawDefaultRect", 1 << 11);
    ImGuiLua_SetEnumValue(L, "AcceptNoPreviewTooltip", 1 << 12);
    ImGuiLua_SetEnumValue(L, "AcceptPeekOnly", ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
}

IMGUILUA_ENUM(ImGuiFocusedFlags)
static void ImGuiLua_RegisterEnumImGuiFocusedFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "ChildWindows", 1 << 0);
    ImGuiLua_SetEnumValue(L, "RootWindow", 1 << 1);
    ImGuiLua_SetEnumValue(L, "AnyWindow", 1 << 2);
    ImGuiLua_SetEnumValue(L, "RootAndChildWindows", ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows);
}

IMGUILUA_ENUM(ImGuiHoveredFlags)
static void ImGuiLua_RegisterEnumImGuiHoveredFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "ChildWindows", 1 << 0);
    ImGuiLua_SetEnumValue(L, "RootWindow", 1 << 1);
    ImGuiLua_SetEnumValue(L, "AnyWindow", 1 << 2);
    ImGuiLua_SetEnumValue(L, "AllowWhenBlockedByPopup", 1 << 3);
    ImGuiLua_SetEnumValue(L, "AllowWhenBlockedByActiveItem", 1 << 5);
    ImGuiLua_SetEnumValue(L, "AllowWhenOverlapped", 1 << 6);
    ImGuiLua_SetEnumValue(L, "AllowWhenDisabled", 1 << 7);
    ImGuiLua_SetEnumValue(L, "RectOnly", ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped);
    ImGuiLua_SetEnumValue(L, "RootAndChildWindows", ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows);
}

IMGUILUA_ENUM(ImGuiInputTextFlags)
static void ImGuiLua_RegisterEnumImGuiInputTextFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "CharsDecimal", 1 << 0);
    ImGuiLua_SetEnumValue(L, "CharsHexadecimal", 1 << 1);
    ImGuiLua_SetEnumValue(L, "CharsUppercase", 1 << 2);
    ImGuiLua_SetEnumValue(L, "CharsNoBlank", 1 << 3);
    ImGuiLua_SetEnumValue(L, "AutoSelectAll", 1 << 4);
    ImGuiLua_SetEnumValue(L, "EnterReturnsTrue", 1 << 5);
    ImGuiLua_SetEnumValue(L, "CallbackCompletion", 1 << 6);
    ImGuiLua_SetEnumValue(L, "CallbackHistory", 1 << 7);
    ImGuiLua_SetEnumValue(L, "CallbackAlways", 1 << 8);
    ImGuiLua_SetEnumValue(L, "CallbackCharFilter", 1 << 9);
    ImGuiLua_SetEnumValue(L, "AllowTabInput", 1 << 10);
    ImGuiLua_SetEnumValue(L, "CtrlEnterForNewLine", 1 << 11);
    ImGuiLua_SetEnumValue(L, "NoHorizontalScroll", 1 << 12);
    ImGuiLua_SetEnumValue(L, "AlwaysOverwrite", 1 << 13);
    ImGuiLua_SetEnumValue(L, "ReadOnly", 1 << 14);
    ImGuiLua_SetEnumValue(L, "Password", 1 << 15);
    ImGuiLua_SetEnumValue(L, "NoUndoRedo", 1 << 16);
    ImGuiLua_SetEnumValue(L, "CharsScientific", 1 << 17);
    ImGuiLua_SetEnumValue(L, "CallbackResize", 1 << 18);
    ImGuiLua_SetEnumValue(L, "CallbackEdit", 1 << 19);
}

IMGUILUA_ENUM(ImGuiKeyModFlags)
static void ImGuiLua_RegisterEnumImGuiKeyModFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Ctrl", 1 << 0);
    ImGuiLua_SetEnumValue(L, "Shift", 1 << 1);
    ImGuiLua_SetEnumValue(L, "Alt", 1 << 2);
    ImGuiLua_SetEnumValue(L, "Super", 1 << 3);
}

IMGUILUA_ENUM(ImGuiKey)
static void ImGuiLua_RegisterEnumImGuiKey(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "Tab", 0);
    ImGuiLua_SetEnumValue(L, "LeftArrow", 1);
    ImGuiLua_SetEnumValue(L, "RightArrow", 2);
    ImGuiLua_SetEnumValue(L, "UpArrow", 3);
    ImGuiLua_SetEnumValue(L, "DownArrow", 4);
    ImGuiLua_SetEnumValue(L, "PageUp", 5);
    ImGuiLua_SetEnumValue(L, "PageDown", 6);
    ImGuiLua_SetEnumValue(L, "Home", 7);
    ImGuiLua_SetEnumValue(L, "End", 8);
    ImGuiLua_SetEnumValue(L, "Insert", 9);
    ImGuiLua_SetEnumValue(L, "Delete", 10);
    ImGuiLua_SetEnumValue(L, "Backspace", 11);
    ImGuiLua_SetEnumValue(L, "Space", 12);
    ImGuiLua_SetEnumValue(L, "Enter", 13);
    ImGuiLua_SetEnumValue(L, "Escape", 14);
    ImGuiLua_SetEnumValue(L, "KeyPadEnter", 15);
    ImGuiLua_SetEnumValue(L, "A", 16);
    ImGuiLua_SetEnumValue(L, "C", 17);
    ImGuiLua_SetEnumValue(L, "V", 18);
    ImGuiLua_SetEnumValue(L, "X", 19);
    ImGuiLua_SetEnumValue(L, "Y", 20);
    ImGuiLua_SetEnumValue(L, "Z", 21);
    ImGuiLua_SetEnumValue(L, "COUNT", 22);
}

IMGUILUA_ENUM(ImGuiMouseButton)
static void ImGuiLua_RegisterEnumImGuiMouseButton(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "Left", 0);
    ImGuiLua_SetEnumValue(L, "Right", 1);
    ImGuiLua_SetEnumValue(L, "Middle", 2);
    ImGuiLua_SetEnumValue(L, "COUNT", 5);
}

IMGUILUA_ENUM(ImGuiMouseCursor)
static void ImGuiLua_RegisterEnumImGuiMouseCursor(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", -1);
    ImGuiLua_SetEnumValue(L, "Arrow", 0);
    ImGuiLua_SetEnumValue(L, "TextInput", 1);
    ImGuiLua_SetEnumValue(L, "ResizeAll", 2);
    ImGuiLua_SetEnumValue(L, "ResizeNS", 3);
    ImGuiLua_SetEnumValue(L, "ResizeEW", 4);
    ImGuiLua_SetEnumValue(L, "ResizeNESW", 5);
    ImGuiLua_SetEnumValue(L, "ResizeNWSE", 6);
    ImGuiLua_SetEnumValue(L, "Hand", 7);
    ImGuiLua_SetEnumValue(L, "NotAllowed", 8);
    ImGuiLua_SetEnumValue(L, "COUNT", 9);
}

IMGUILUA_ENUM(ImGuiNavInput)
static void ImGuiLua_RegisterEnumImGuiNavInput(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "Activate", 0);
    ImGuiLua_SetEnumValue(L, "Cancel", 1);
    ImGuiLua_SetEnumValue(L, "Input", 2);
    ImGuiLua_SetEnumValue(L, "Menu", 3);
    ImGuiLua_SetEnumValue(L, "DpadLeft", 4);
    ImGuiLua_SetEnumValue(L, "DpadRight", 5);
    ImGuiLua_SetEnumValue(L, "DpadUp", 6);
    ImGuiLua_SetEnumValue(L, "DpadDown", 7);
    ImGuiLua_SetEnumValue(L, "LStickLeft", 8);
    ImGuiLua_SetEnumValue(L, "LStickRight", 9);
    ImGuiLua_SetEnumValue(L, "LStickUp", 10);
    ImGuiLua_SetEnumValue(L, "LStickDown", 11);
    ImGuiLua_SetEnumValue(L, "FocusPrev", 12);
    ImGuiLua_SetEnumValue(L, "FocusNext", 13);
    ImGuiLua_SetEnumValue(L, "TweakSlow", 14);
    ImGuiLua_SetEnumValue(L, "TweakFast", 15);
    ImGuiLua_SetEnumValue(L, "KeyMenu_", 16);
    ImGuiLua_SetEnumValue(L, "KeyLeft_", 17);
    ImGuiLua_SetEnumValue(L, "KeyRight_", 18);
    ImGuiLua_SetEnumValue(L, "KeyUp_", 19);
    ImGuiLua_SetEnumValue(L, "KeyDown_", 20);
    ImGuiLua_SetEnumValue(L, "COUNT", 21);
    ImGuiLua_SetEnumValue(L, "InternalStart_", ImGuiNavInput_KeyMenu_);
}

IMGUILUA_ENUM(ImGuiPopupFlags)
static void ImGuiLua_RegisterEnumImGuiPopupFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "MouseButtonLeft", 0);
    ImGuiLua_SetEnumValue(L, "MouseButtonRight", 1);
    ImGuiLua_SetEnumValue(L, "MouseButtonMiddle", 2);
    ImGuiLua_SetEnumValue(L, "MouseButtonMask_", 0x1F);
    ImGuiLua_SetEnumValue(L, "MouseButtonDefault_", 1);
    ImGuiLua_SetEnumValue(L, "NoOpenOverExistingPopup", 1 << 5);
    ImGuiLua_SetEnumValue(L, "NoOpenOverItems", 1 << 6);
    ImGuiLua_SetEnumValue(L, "AnyPopupId", 1 << 7);
    ImGuiLua_SetEnumValue(L, "AnyPopupLevel", 1 << 8);
    ImGuiLua_SetEnumValue(L, "AnyPopup", ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel);
}

IMGUILUA_ENUM(ImGuiSelectableFlags)
static void ImGuiLua_RegisterEnumImGuiSelectableFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "DontClosePopups", 1 << 0);
    ImGuiLua_SetEnumValue(L, "SpanAllColumns", 1 << 1);
    ImGuiLua_SetEnumValue(L, "AllowDoubleClick", 1 << 2);
    ImGuiLua_SetEnumValue(L, "Disabled", 1 << 3);
    ImGuiLua_SetEnumValue(L, "AllowItemOverlap", 1 << 4);
}

IMGUILUA_ENUM(ImGuiSliderFlags)
static void ImGuiLua_RegisterEnumImGuiSliderFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "AlwaysClamp", 1 << 4);
    ImGuiLua_SetEnumValue(L, "Logarithmic", 1 << 5);
    ImGuiLua_SetEnumValue(L, "NoRoundToFormat", 1 << 6);
    ImGuiLua_SetEnumValue(L, "NoInput", 1 << 7);
    ImGuiLua_SetEnumValue(L, "InvalidMask_", 0x7000000F);
}

IMGUILUA_ENUM(ImGuiSortDirection)
static void ImGuiLua_RegisterEnumImGuiSortDirection(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Ascending", 1);
    ImGuiLua_SetEnumValue(L, "Descending", 2);
}

IMGUILUA_ENUM(ImGuiStyleVar)
static void ImGuiLua_RegisterEnumImGuiStyleVar(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "Alpha", 0);
    ImGuiLua_SetEnumValue(L, "WindowPadding", 1);
    ImGuiLua_SetEnumValue(L, "WindowRounding", 2);
    ImGuiLua_SetEnumValue(L, "WindowBorderSize", 3);
    ImGuiLua_SetEnumValue(L, "WindowMinSize", 4);
    ImGuiLua_SetEnumValue(L, "WindowTitleAlign", 5);
    ImGuiLua_SetEnumValue(L, "ChildRounding", 6);
    ImGuiLua_SetEnumValue(L, "ChildBorderSize", 7);
    ImGuiLua_SetEnumValue(L, "PopupRounding", 8);
    ImGuiLua_SetEnumValue(L, "PopupBorderSize", 9);
    ImGuiLua_SetEnumValue(L, "FramePadding", 10);
    ImGuiLua_SetEnumValue(L, "FrameRounding", 11);
    ImGuiLua_SetEnumValue(L, "FrameBorderSize", 12);
    ImGuiLua_SetEnumValue(L, "ItemSpacing", 13);
    ImGuiLua_SetEnumValue(L, "ItemInnerSpacing", 14);
    ImGuiLua_SetEnumValue(L, "IndentSpacing", 15);
    ImGuiLua_SetEnumValue(L, "CellPadding", 16);
    ImGuiLua_SetEnumValue(L, "ScrollbarSize", 17);
    ImGuiLua_SetEnumValue(L, "ScrollbarRounding", 18);
    ImGuiLua_SetEnumValue(L, "GrabMinSize", 19);
    ImGuiLua_SetEnumValue(L, "GrabRounding", 20);
    ImGuiLua_SetEnumValue(L, "TabRounding", 21);
    ImGuiLua_SetEnumValue(L, "ButtonTextAlign", 22);
    ImGuiLua_SetEnumValue(L, "SelectableTextAlign", 23);
    ImGuiLua_SetEnumValue(L, "COUNT", 24);
}

IMGUILUA_ENUM(ImGuiTabBarFlags)
static void ImGuiLua_RegisterEnumImGuiTabBarFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Reorderable", 1 << 0);
    ImGuiLua_SetEnumValue(L, "AutoSelectNewTabs", 1 << 1);
    ImGuiLua_SetEnumValue(L, "TabListPopupButton", 1 << 2);
    ImGuiLua_SetEnumValue(L, "NoCloseWithMiddleMouseButton", 1 << 3);
    ImGuiLua_SetEnumValue(L, "NoTabListScrollingButtons", 1 << 4);
    ImGuiLua_SetEnumValue(L, "NoTooltip", 1 << 5);
    ImGuiLua_SetEnumValue(L, "FittingPolicyResizeDown", 1 << 6);
    ImGuiLua_SetEnumValue(L, "FittingPolicyScroll", 1 << 7);
    ImGuiLua_SetEnumValue(L, "FittingPolicyMask_", ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll);
    ImGuiLua_SetEnumValue(L, "FittingPolicyDefault_", ImGuiTabBarFlags_FittingPolicyResizeDown);
}

IMGUILUA_ENUM(ImGuiTabItemFlags)
static void ImGuiLua_RegisterEnumImGuiTabItemFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "UnsavedDocument", 1 << 0);
    ImGuiLua_SetEnumValue(L, "SetSelected", 1 << 1);
    ImGuiLua_SetEnumValue(L, "NoCloseWithMiddleMouseButton", 1 << 2);
    ImGuiLua_SetEnumValue(L, "NoPushId", 1 << 3);
    ImGuiLua_SetEnumValue(L, "NoTooltip", 1 << 4);
    ImGuiLua_SetEnumValue(L, "NoReorder", 1 << 5);
    ImGuiLua_SetEnumValue(L, "Leading", 1 << 6);
    ImGuiLua_SetEnumValue(L, "Trailing", 1 << 7);
}

IMGUILUA_ENUM(ImGuiTableBgTarget)
static void ImGuiLua_RegisterEnumImGuiTableBgTarget(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "RowBg0", 1);
    ImGuiLua_SetEnumValue(L, "RowBg1", 2);
    ImGuiLua_SetEnumValue(L, "CellBg", 3);
}

IMGUILUA_ENUM(ImGuiTableColumnFlags)
static void ImGuiLua_RegisterEnumImGuiTableColumnFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Disabled", 1 << 0);
    ImGuiLua_SetEnumValue(L, "DefaultHide", 1 << 1);
    ImGuiLua_SetEnumValue(L, "DefaultSort", 1 << 2);
    ImGuiLua_SetEnumValue(L, "WidthStretch", 1 << 3);
    ImGuiLua_SetEnumValue(L, "WidthFixed", 1 << 4);
    ImGuiLua_SetEnumValue(L, "NoResize", 1 << 5);
    ImGuiLua_SetEnumValue(L, "NoReorder", 1 << 6);
    ImGuiLua_SetEnumValue(L, "NoHide", 1 << 7);
    ImGuiLua_SetEnumValue(L, "NoClip", 1 << 8);
    ImGuiLua_SetEnumValue(L, "NoSort", 1 << 9);
    ImGuiLua_SetEnumValue(L, "NoSortAscending", 1 << 10);
    ImGuiLua_SetEnumValue(L, "NoSortDescending", 1 << 11);
    ImGuiLua_SetEnumValue(L, "NoHeaderLabel", 1 << 12);
    ImGuiLua_SetEnumValue(L, "NoHeaderWidth", 1 << 13);
    ImGuiLua_SetEnumValue(L, "PreferSortAscending", 1 << 14);
    ImGuiLua_SetEnumValue(L, "PreferSortDescending", 1 << 15);
    ImGuiLua_SetEnumValue(L, "IndentEnable", 1 << 16);
    ImGuiLua_SetEnumValue(L, "IndentDisable", 1 << 17);
    ImGuiLua_SetEnumValue(L, "IsEnabled", 1 << 24);
    ImGuiLua_SetEnumValue(L, "IsVisible", 1 << 25);
    ImGuiLua_SetEnumValue(L, "IsSorted", 1 << 26);
    ImGuiLua_SetEnumValue(L, "IsHovered", 1 << 27);
    ImGuiLua_SetEnumValue(L, "WidthMask_", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_WidthFixed);
    ImGuiLua_SetEnumValue(L, "IndentMask_", ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_IndentDisable);
    ImGuiLua_SetEnumValue(L, "StatusMask_", ImGuiTableColumnFlags_IsEnabled | ImGuiTableColumnFlags_IsVisible | ImGuiTableColumnFlags_IsSorted | ImGuiTableColumnFlags_IsHovered);
    ImGuiLua_SetEnumValue(L, "NoDirectResize_", 1 << 30);
}

IMGUILUA_ENUM(ImGuiTableFlags)
static void ImGuiLua_RegisterEnumImGuiTableFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Resizable", 1 << 0);
    ImGuiLua_SetEnumValue(L, "Reorderable", 1 << 1);
    ImGuiLua_SetEnumValue(L, "Hideable", 1 << 2);
    ImGuiLua_SetEnumValue(L, "Sortable", 1 << 3);
    ImGuiLua_SetEnumValue(L, "NoSavedSettings", 1 << 4);
    ImGuiLua_SetEnumValue(L, "ContextMenuInBody", 1 << 5);
    ImGuiLua_SetEnumValue(L, "RowBg", 1 << 6);
    ImGuiLua_SetEnumValue(L, "BordersInnerH", 1 << 7);
    ImGuiLua_SetEnumValue(L, "BordersOuterH", 1 << 8);
    ImGuiLua_SetEnumValue(L, "BordersInnerV", 1 << 9);
    ImGuiLua_SetEnumValue(L, "BordersOuterV", 1 << 10);
    ImGuiLua_SetEnumValue(L, "BordersH", ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuterH);
    ImGuiLua_SetEnumValue(L, "BordersV", ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV);
    ImGuiLua_SetEnumValue(L, "BordersInner", ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH);
    ImGuiLua_SetEnumValue(L, "BordersOuter", ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersOuterH);
    ImGuiLua_SetEnumValue(L, "Borders", ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter);
    ImGuiLua_SetEnumValue(L, "NoBordersInBody", 1 << 11);
    ImGuiLua_SetEnumValue(L, "NoBordersInBodyUntilResize", 1 << 12);
    ImGuiLua_SetEnumValue(L, "SizingFixedFit", 1 << 13);
    ImGuiLua_SetEnumValue(L, "SizingFixedSame", 2 << 13);
    ImGuiLua_SetEnumValue(L, "SizingStretchProp", 3 << 13);
    ImGuiLua_SetEnumValue(L, "SizingStretchSame", 4 << 13);
    ImGuiLua_SetEnumValue(L, "NoHostExtendX", 1 << 16);
    ImGuiLua_SetEnumValue(L, "NoHostExtendY", 1 << 17);
    ImGuiLua_SetEnumValue(L, "NoKeepColumnsVisible", 1 << 18);
    ImGuiLua_SetEnumValue(L, "PreciseWidths", 1 << 19);
    ImGuiLua_SetEnumValue(L, "NoClip", 1 << 20);
    ImGuiLua_SetEnumValue(L, "PadOuterX", 1 << 21);
    ImGuiLua_SetEnumValue(L, "NoPadOuterX", 1 << 22);
    ImGuiLua_SetEnumValue(L, "NoPadInnerX", 1 << 23);
    ImGuiLua_SetEnumValue(L, "ScrollX", 1 << 24);
    ImGuiLua_SetEnumValue(L, "ScrollY", 1 << 25);
    ImGuiLua_SetEnumValue(L, "SortMulti", 1 << 26);
    ImGuiLua_SetEnumValue(L, "SortTristate", 1 << 27);
    ImGuiLua_SetEnumValue(L, "SizingMask_", ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_SizingStretchSame);
}

IMGUILUA_ENUM(ImGuiTableRowFlags)
static void ImGuiLua_RegisterEnumImGuiTableRowFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Headers", 1 << 0);
}

IMGUILUA_ENUM(ImGuiTreeNodeFlags)
static void ImGuiLua_RegisterEnumImGuiTreeNodeFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "Selected", 1 << 0);
    ImGuiLua_SetEnumValue(L, "Framed", 1 << 1);
    ImGuiLua_SetEnumValue(L, "AllowItemOverlap", 1 << 2);
    ImGuiLua_SetEnumValue(L, "NoTreePushOnOpen", 1 << 3);
    ImGuiLua_SetEnumValue(L, "NoAutoOpenOnLog", 1 << 4);
    ImGuiLua_SetEnumValue(L, "DefaultOpen", 1 << 5);
    ImGuiLua_SetEnumValue(L, "OpenOnDoubleClick", 1 << 6);
    ImGuiLua_SetEnumValue(L, "OpenOnArrow", 1 << 7);
    ImGuiLua_SetEnumValue(L, "Leaf", 1 << 8);
    ImGuiLua_SetEnumValue(L, "Bullet", 1 << 9);
    ImGuiLua_SetEnumValue(L, "FramePadding", 1 << 10);
    ImGuiLua_SetEnumValue(L, "SpanAvailWidth", 1 << 11);
    ImGuiLua_SetEnumValue(L, "SpanFullWidth", 1 << 12);
    ImGuiLua_SetEnumValue(L, "NavLeftJumpsBackHere", 1 << 13);
    ImGuiLua_SetEnumValue(L, "CollapsingHeader", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog);
}

IMGUILUA_ENUM(ImGuiViewportFlags)
static void ImGuiLua_RegisterEnumImGuiViewportFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "IsPlatformWindow", 1 << 0);
    ImGuiLua_SetEnumValue(L, "IsPlatformMonitor", 1 << 1);
    ImGuiLua_SetEnumValue(L, "OwnedByApp", 1 << 2);
}

IMGUILUA_ENUM(ImGuiWindowFlags)
static void ImGuiLua_RegisterEnumImGuiWindowFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "NoTitleBar", 1 << 0);
    ImGuiLua_SetEnumValue(L, "NoResize", 1 << 1);
    ImGuiLua_SetEnumValue(L, "NoMove", 1 << 2);
    ImGuiLua_SetEnumValue(L, "NoScrollbar", 1 << 3);
    ImGuiLua_SetEnumValue(L, "NoScrollWithMouse", 1 << 4);
    ImGuiLua_SetEnumValue(L, "NoCollapse", 1 << 5);
    ImGuiLua_SetEnumValue(L, "AlwaysAutoResize", 1 << 6);
    ImGuiLua_SetEnumValue(L, "NoBackground", 1 << 7);
    ImGuiLua_SetEnumValue(L, "NoSavedSettings", 1 << 8);
    ImGuiLua_SetEnumValue(L, "NoMouseInputs", 1 << 9);
    ImGuiLua_SetEnumValue(L, "MenuBar", 1 << 10);
    ImGuiLua_SetEnumValue(L, "HorizontalScrollbar", 1 << 11);
    ImGuiLua_SetEnumValue(L, "NoFocusOnAppearing", 1 << 12);
    ImGuiLua_SetEnumValue(L, "NoBringToFrontOnFocus", 1 << 13);
    ImGuiLua_SetEnumValue(L, "AlwaysVerticalScrollbar", 1 << 14);
    ImGuiLua_SetEnumValue(L, "AlwaysHorizontalScrollbar", 1<< 15);
    ImGuiLua_SetEnumValue(L, "AlwaysUseWindowPadding", 1 << 16);
    ImGuiLua_SetEnumValue(L, "NoNavInputs", 1 << 18);
    ImGuiLua_SetEnumValue(L, "NoNavFocus", 1 << 19);
    ImGuiLua_SetEnumValue(L, "UnsavedDocument", 1 << 20);
    ImGuiLua_SetEnumValue(L, "NoNav", ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus);
    ImGuiLua_SetEnumValue(L, "NoDecoration", ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    ImGuiLua_SetEnumValue(L, "NoInputs", ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus);
    ImGuiLua_SetEnumValue(L, "NavFlattened", 1 << 23);
    ImGuiLua_SetEnumValue(L, "ChildWindow", 1 << 24);
    ImGuiLua_SetEnumValue(L, "Tooltip", 1 << 25);
    ImGuiLua_SetEnumValue(L, "Popup", 1 << 26);
    ImGuiLua_SetEnumValue(L, "Modal", 1 << 27);
    ImGuiLua_SetEnumValue(L, "ChildMenu", 1 << 28);
}


// function ImGui::AcceptDragDropPayload

// const ImGuiPayload* ImGui_AcceptDragDropPayload(const char*, ImGuiDragDropFlags): Unknown return type 'const ImGuiPayload*' (const ImGuiPayload*)


// function ImGui::AlignTextToFramePadding

IMGUILUA_FUNCTION(ImGui::AlignTextToFramePadding)
static int ImGui_AlignTextToFramePadding(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::AlignTextToFramePadding();
    return 0;
}


// function ImGui::ArrowButton

IMGUILUA_FUNCTION(ImGui::ArrowButton)
static int ImGui_ArrowButton(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    int dir;
    dir = static_cast<int>(luaL_checkinteger(L, 2));
    bool RETVAL = ImGui::ArrowButton(str_id,dir);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::Begin

IMGUILUA_FUNCTION(ImGui::Begin)
static int ImGui_Begin(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 3);
    }
    const char *name;
    name = lua_tostring(L, 1);
    bool *p_open;
    if (ARGC < 2) {
        p_open = NULL;
    }
    else {
        if (lua_type(L, 2) == LUA_TNIL) {
            p_open = NULL;
        }
        else {
            p_open = static_cast<bool *>(luaL_checkudata(L, 2, "ImGuiLua_BoolRef"));
        }
    }
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    bool RETVAL = ImGui::Begin(name,p_open,flags);
    IMGUILUA_BEGIN(L, ImGui::Begin, ImGui::End, 1);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginChild

static int ImGui_BeginChild_uv2bi(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 4);
    }
    unsigned int id;
    id = static_cast<unsigned int>(luaL_checkinteger(L, 1));
    ImVec2 size;
    if (ARGC < 2) {
        size = ImVec2(0,0);
    }
    else {
        size = ImGuiLua_ToImVec2(L, 2);
    }
    bool border;
    if (ARGC < 3) {
        border = false;
    }
    else {
        border = lua_toboolean(L, 3);
    }
    int flags;
    if (ARGC < 4) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 4));
    }
    bool RETVAL = ImGui::BeginChild(id,size,border,flags);
    IMGUILUA_BEGIN(L, ImGui::BeginChild, ImGui::EndChild, 2);
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int ImGui_BeginChild_sv2bi(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 4);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    ImVec2 size;
    if (ARGC < 2) {
        size = ImVec2(0,0);
    }
    else {
        size = ImGuiLua_ToImVec2(L, 2);
    }
    bool border;
    if (ARGC < 3) {
        border = false;
    }
    else {
        border = lua_toboolean(L, 3);
    }
    int flags;
    if (ARGC < 4) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 4));
    }
    bool RETVAL = ImGui::BeginChild(str_id,size,border,flags);
    IMGUILUA_BEGIN(L, ImGui::BeginChild, ImGui::EndChild, 2);
    lua_pushboolean(L, RETVAL);
    return 1;
}

IMGUILUA_FUNCTION(ImGui::ImGui_BeginChild)
static int ImGui_BeginChild(lua_State *L)
{
    if (lua_type(L, 1) == LUA_TNUMBER) {
        return ImGui_BeginChild_uv2bi(L);
    }
    else {
        return ImGui_BeginChild_sv2bi(L);
    }
}


// function ImGui::BeginChildFrame

IMGUILUA_FUNCTION(ImGui::BeginChildFrame)
static int ImGui_BeginChildFrame(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    unsigned int id;
    id = static_cast<unsigned int>(luaL_checkinteger(L, 1));
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 2);
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    bool RETVAL = ImGui::BeginChildFrame(id,size,flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginChildFrame, ImGui::EndChildFrame, 3);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginCombo

IMGUILUA_FUNCTION(ImGui::BeginCombo)
static int ImGui_BeginCombo(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    const char *label;
    label = lua_tostring(L, 1);
    const char *preview_value;
    preview_value = lua_tostring(L, 2);
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    bool RETVAL = ImGui::BeginCombo(label,preview_value,flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginCombo, ImGui::EndCombo, 4);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginDragDropSource

IMGUILUA_FUNCTION(ImGui::BeginDragDropSource)
static int ImGui_BeginDragDropSource(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int flags;
    if (ARGC < 1) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 1));
    }
    bool RETVAL = ImGui::BeginDragDropSource(flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginDragDropSource, ImGui::EndDragDropSource, 5);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginDragDropTarget

IMGUILUA_FUNCTION(ImGui::BeginDragDropTarget)
static int ImGui_BeginDragDropTarget(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::BeginDragDropTarget();
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginDragDropTarget, ImGui::EndDragDropTarget, 6);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginGroup

IMGUILUA_FUNCTION(ImGui::BeginGroup)
static int ImGui_BeginGroup(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::BeginGroup();
    IMGUILUA_BEGIN(L, ImGui::BeginGroup, ImGui::EndGroup, 7);
    return 0;
}


// function ImGui::BeginListBox

IMGUILUA_FUNCTION(ImGui::BeginListBox)
static int ImGui_BeginListBox(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *label;
    label = lua_tostring(L, 1);
    ImVec2 size;
    if (ARGC < 2) {
        size = ImVec2(0,0);
    }
    else {
        size = ImGuiLua_ToImVec2(L, 2);
    }
    bool RETVAL = ImGui::BeginListBox(label,size);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginListBox, ImGui::EndListBox, 8);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginMainMenuBar

IMGUILUA_FUNCTION(ImGui::BeginMainMenuBar)
static int ImGui_BeginMainMenuBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::BeginMainMenuBar();
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginMainMenuBar, ImGui::EndMainMenuBar, 9);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginMenu

IMGUILUA_FUNCTION(ImGui::BeginMenu)
static int ImGui_BeginMenu(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool enabled;
    if (ARGC < 2) {
        enabled = true;
    }
    else {
        enabled = lua_toboolean(L, 2);
    }
    bool RETVAL = ImGui::BeginMenu(label,enabled);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginMenu, ImGui::EndMenu, 10);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginMenuBar

IMGUILUA_FUNCTION(ImGui::BeginMenuBar)
static int ImGui_BeginMenuBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::BeginMenuBar();
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginMenuBar, ImGui::EndMenuBar, 11);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginPopup

IMGUILUA_FUNCTION(ImGui::BeginPopup)
static int ImGui_BeginPopup(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    int flags;
    if (ARGC < 2) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::BeginPopup(str_id,flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginPopup, ImGui::EndPopup, 12);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginPopupContextItem

IMGUILUA_FUNCTION(ImGui::BeginPopupContextItem)
static int ImGui_BeginPopupContextItem(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 2);
    }
    const char *str_id;
    if (ARGC < 1) {
        str_id = NULL;
    }
    else {
        str_id = lua_tostring(L, 1);
    }
    int popup_flags;
    if (ARGC < 2) {
        popup_flags = 1;
    }
    else {
        popup_flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::BeginPopupContextItem(str_id,popup_flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginPopupContextItem, ImGui::EndPopup, 12);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginPopupContextVoid

IMGUILUA_FUNCTION(ImGui::BeginPopupContextVoid)
static int ImGui_BeginPopupContextVoid(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 2);
    }
    const char *str_id;
    if (ARGC < 1) {
        str_id = NULL;
    }
    else {
        str_id = lua_tostring(L, 1);
    }
    int popup_flags;
    if (ARGC < 2) {
        popup_flags = 1;
    }
    else {
        popup_flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::BeginPopupContextVoid(str_id,popup_flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginPopupContextVoid, ImGui::EndPopup, 12);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginPopupContextWindow

IMGUILUA_FUNCTION(ImGui::BeginPopupContextWindow)
static int ImGui_BeginPopupContextWindow(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 2);
    }
    const char *str_id;
    if (ARGC < 1) {
        str_id = NULL;
    }
    else {
        str_id = lua_tostring(L, 1);
    }
    int popup_flags;
    if (ARGC < 2) {
        popup_flags = 1;
    }
    else {
        popup_flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::BeginPopupContextWindow(str_id,popup_flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginPopupContextWindow, ImGui::EndPopup, 12);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginPopupModal

IMGUILUA_FUNCTION(ImGui::BeginPopupModal)
static int ImGui_BeginPopupModal(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 3);
    }
    const char *name;
    name = lua_tostring(L, 1);
    bool *p_open;
    if (ARGC < 2) {
        p_open = NULL;
    }
    else {
        if (lua_type(L, 2) == LUA_TNIL) {
            p_open = NULL;
        }
        else {
            p_open = static_cast<bool *>(luaL_checkudata(L, 2, "ImGuiLua_BoolRef"));
        }
    }
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    bool RETVAL = ImGui::BeginPopupModal(name,p_open,flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginPopupModal, ImGui::EndPopup, 12);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginTabBar

IMGUILUA_FUNCTION(ImGui::BeginTabBar)
static int ImGui_BeginTabBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    int flags;
    if (ARGC < 2) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::BeginTabBar(str_id,flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginTabBar, ImGui::EndTabBar, 13);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginTabItem

IMGUILUA_FUNCTION(ImGui::BeginTabItem)
static int ImGui_BeginTabItem(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 3);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool *p_open;
    if (ARGC < 2) {
        p_open = NULL;
    }
    else {
        if (lua_type(L, 2) == LUA_TNIL) {
            p_open = NULL;
        }
        else {
            p_open = static_cast<bool *>(luaL_checkudata(L, 2, "ImGuiLua_BoolRef"));
        }
    }
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    bool RETVAL = ImGui::BeginTabItem(label,p_open,flags);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginTabItem, ImGui::EndTabItem, 14);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginTable

IMGUILUA_FUNCTION(ImGui::BeginTable)
static int ImGui_BeginTable(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 5) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 5);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    int column;
    column = static_cast<int>(luaL_checkinteger(L, 2));
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    ImVec2 outer_size;
    if (ARGC < 4) {
        outer_size = ImVec2(0.0f,0.0f);
    }
    else {
        outer_size = ImGuiLua_ToImVec2(L, 4);
    }
    float inner_width;
    if (ARGC < 5) {
        inner_width = 0.0f;
    }
    else {
        inner_width = static_cast<float>(luaL_checknumber(L, 5));
    }
    bool RETVAL = ImGui::BeginTable(str_id,column,flags,outer_size,inner_width);
    if (RETVAL) {
        IMGUILUA_BEGIN(L, ImGui::BeginTable, ImGui::EndTable, 15);
    }
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::BeginTooltip

IMGUILUA_FUNCTION(ImGui::BeginTooltip)
static int ImGui_BeginTooltip(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::BeginTooltip();
    IMGUILUA_BEGIN(L, ImGui::BeginTooltip, ImGui::EndTooltip, 16);
    return 0;
}


// function ImGui::Bullet

IMGUILUA_FUNCTION(ImGui::Bullet)
static int ImGui_Bullet(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::Bullet();
    return 0;
}


// function ImGui::BulletText is excluded


// function ImGui::BulletTextV is excluded


// function ImGui::Button

IMGUILUA_FUNCTION(ImGui::Button)
static int ImGui_Button(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *label;
    label = lua_tostring(L, 1);
    ImVec2 size;
    if (ARGC < 2) {
        size = ImVec2(0,0);
    }
    else {
        size = ImGuiLua_ToImVec2(L, 2);
    }
    bool RETVAL = ImGui::Button(label,size);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::CalcItemWidth

IMGUILUA_FUNCTION(ImGui::CalcItemWidth)
static int ImGui_CalcItemWidth(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::CalcItemWidth();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::CalcListClipping

IMGUILUA_FUNCTION(ImGui::CalcListClipping)
static int ImGui_CalcListClipping(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 4);
    }
    int items_count;
    items_count = static_cast<int>(luaL_checkinteger(L, 1));
    float items_height;
    items_height = static_cast<float>(luaL_checknumber(L, 2));
    int *out_items_display_start;
    out_items_display_start = static_cast<int *>(luaL_checkudata(L, 3, "ImGuiLua_IntRef"));
    int *out_items_display_end;
    out_items_display_end = static_cast<int *>(luaL_checkudata(L, 4, "ImGuiLua_IntRef"));
    ImGui::CalcListClipping(items_count,items_height,out_items_display_start,out_items_display_end);
    return 0;
}


// function ImGui::CalcTextSize

IMGUILUA_FUNCTION(ImGui::CalcTextSize)
static int ImGui_CalcTextSize(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 4);
    }
    const char *text;
    text = lua_tostring(L, 1);
    const char *text_end;
    if (ARGC < 2) {
        text_end = NULL;
    }
    else {
        text_end = lua_tostring(L, 2);
    }
    bool hide_text_after_double_hash;
    if (ARGC < 3) {
        hide_text_after_double_hash = false;
    }
    else {
        hide_text_after_double_hash = lua_toboolean(L, 3);
    }
    float wrap_width;
    if (ARGC < 4) {
        wrap_width = -1.0f;
    }
    else {
        wrap_width = static_cast<float>(luaL_checknumber(L, 4));
    }
    ImVec2 RETVAL = ImGui::CalcTextSize(text,text_end,hide_text_after_double_hash,wrap_width);
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::CaptureKeyboardFromApp

IMGUILUA_FUNCTION(ImGui::CaptureKeyboardFromApp)
static int ImGui_CaptureKeyboardFromApp(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    bool want_capture_keyboard_value;
    if (ARGC < 1) {
        want_capture_keyboard_value = true;
    }
    else {
        want_capture_keyboard_value = lua_toboolean(L, 1);
    }
    ImGui::CaptureKeyboardFromApp(want_capture_keyboard_value);
    return 0;
}


// function ImGui::CaptureMouseFromApp

IMGUILUA_FUNCTION(ImGui::CaptureMouseFromApp)
static int ImGui_CaptureMouseFromApp(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    bool want_capture_mouse_value;
    if (ARGC < 1) {
        want_capture_mouse_value = true;
    }
    else {
        want_capture_mouse_value = lua_toboolean(L, 1);
    }
    ImGui::CaptureMouseFromApp(want_capture_mouse_value);
    return 0;
}


// function ImGui::Checkbox

IMGUILUA_FUNCTION(ImGui::Checkbox)
static int ImGui_Checkbox(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool *v;
    if (lua_type(L, 2) == LUA_TNIL) {
        v = NULL;
    }
    else {
        v = static_cast<bool *>(luaL_checkudata(L, 2, "ImGuiLua_BoolRef"));
    }
    bool RETVAL = ImGui::Checkbox(label,v);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::CheckboxFlags

// bool ImGui_CheckboxFlags(const char*, unsigned int*, unsigned int): Unknown argument type 'unsigned int*' (unsigned int*)

IMGUILUA_FUNCTION(ImGui::CheckboxFlags)
static int ImGui_CheckboxFlags(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 3);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int *flags;
    flags = static_cast<int *>(luaL_checkudata(L, 2, "ImGuiLua_IntRef"));
    int flags_value;
    flags_value = static_cast<int>(luaL_checkinteger(L, 3));
    bool RETVAL = ImGui::CheckboxFlags(label,flags,flags_value);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::CloseCurrentPopup

IMGUILUA_FUNCTION(ImGui::CloseCurrentPopup)
static int ImGui_CloseCurrentPopup(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::CloseCurrentPopup();
    return 0;
}


// function ImGui::CollapsingHeader

static int ImGui_CollapsingHeader_si(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int flags;
    if (ARGC < 2) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::CollapsingHeader(label,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int ImGui_CollapsingHeader_sbPi(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool *p_visible;
    if (lua_type(L, 2) == LUA_TNIL) {
        p_visible = NULL;
    }
    else {
        p_visible = static_cast<bool *>(luaL_checkudata(L, 2, "ImGuiLua_BoolRef"));
    }
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    bool RETVAL = ImGui::CollapsingHeader(label,p_visible,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}

IMGUILUA_FUNCTION(ImGui::ImGui_CollapsingHeader)
static int ImGui_CollapsingHeader(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC == 1 || (ARGC == 2 && lua_type(L, 2) == LUA_TNUMBER)) {
        return ImGui_CollapsingHeader_si(L);
    }
    else if (ARGC == 2 || ARGC == 3) {
        return ImGui_CollapsingHeader_sbPi(L);
    }
    else {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 3);
    }
}


// function ImGui::ColorButton

IMGUILUA_FUNCTION(ImGui::ColorButton)
static int ImGui_ColorButton(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 4);
    }
    const char *desc_id;
    desc_id = lua_tostring(L, 1);
    ImVec4 col;
    col = ImGuiLua_ToImVec4(L, 2);
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    ImVec2 size;
    if (ARGC < 4) {
        size = ImVec2(0,0);
    }
    else {
        size = ImGuiLua_ToImVec2(L, 4);
    }
    bool RETVAL = ImGui::ColorButton(desc_id,col,flags,size);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::ColorConvertFloat4ToU32

IMGUILUA_FUNCTION(ImGui::ColorConvertFloat4ToU32)
static int ImGui_ColorConvertFloat4ToU32(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImVec4 in;
    in = ImGuiLua_ToImVec4(L, 1);
    ImU32 RETVAL = ImGui::ColorConvertFloat4ToU32(in);
    lua_pushinteger(L, static_cast<ImU32>(RETVAL));
    return 1;
}


// function ImGui::ColorConvertHSVtoRGB

IMGUILUA_FUNCTION(ImGui::ColorConvertHSVtoRGB)
static int ImGui_ColorConvertHSVtoRGB(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 6) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 6);
    }
    float h;
    h = static_cast<float>(luaL_checknumber(L, 1));
    float s;
    s = static_cast<float>(luaL_checknumber(L, 2));
    float v;
    v = static_cast<float>(luaL_checknumber(L, 3));
    float *out_r;
    out_r = static_cast<float *>(luaL_checkudata(L, 4, "ImGuiLua_FloatRef"));
    float *out_g;
    out_g = static_cast<float *>(luaL_checkudata(L, 5, "ImGuiLua_FloatRef"));
    float *out_b;
    out_b = static_cast<float *>(luaL_checkudata(L, 6, "ImGuiLua_FloatRef"));
    ImGui::ColorConvertHSVtoRGB(h,s,v,*out_r,*out_g,*out_b);
    return 0;
}


// function ImGui::ColorConvertRGBtoHSV

IMGUILUA_FUNCTION(ImGui::ColorConvertRGBtoHSV)
static int ImGui_ColorConvertRGBtoHSV(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 6) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 6);
    }
    float r;
    r = static_cast<float>(luaL_checknumber(L, 1));
    float g;
    g = static_cast<float>(luaL_checknumber(L, 2));
    float b;
    b = static_cast<float>(luaL_checknumber(L, 3));
    float *out_h;
    out_h = static_cast<float *>(luaL_checkudata(L, 4, "ImGuiLua_FloatRef"));
    float *out_s;
    out_s = static_cast<float *>(luaL_checkudata(L, 5, "ImGuiLua_FloatRef"));
    float *out_v;
    out_v = static_cast<float *>(luaL_checkudata(L, 6, "ImGuiLua_FloatRef"));
    ImGui::ColorConvertRGBtoHSV(r,g,b,*out_h,*out_s,*out_v);
    return 0;
}


// function ImGui::ColorConvertU32ToFloat4

IMGUILUA_FUNCTION(ImGui::ColorConvertU32ToFloat4)
static int ImGui_ColorConvertU32ToFloat4(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImU32 in;
    in = static_cast<ImU32>(luaL_checkinteger(L, 1));
    ImVec4 RETVAL = ImGui::ColorConvertU32ToFloat4(in);
    ImGuiLua_PushImVec4(L, RETVAL);
    return 1;
}


// function ImGui::ColorEdit3

// bool ImGui_ColorEdit3(const char*, float[3], ImGuiColorEditFlags): Unknown argument type 'float[3]' (float[3])


// function ImGui::ColorEdit4

// bool ImGui_ColorEdit4(const char*, float[4], ImGuiColorEditFlags): Unknown argument type 'float[4]' (float[4])


// function ImGui::ColorPicker3

// bool ImGui_ColorPicker3(const char*, float[3], ImGuiColorEditFlags): Unknown argument type 'float[3]' (float[3])


// function ImGui::ColorPicker4

// bool ImGui_ColorPicker4(const char*, float[4], ImGuiColorEditFlags, const float*): Unknown argument type 'float[4]' (float[4])


// function ImGui::Columns

IMGUILUA_FUNCTION(ImGui::Columns)
static int ImGui_Columns(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 3);
    }
    int count;
    if (ARGC < 1) {
        count = 1;
    }
    else {
        count = static_cast<int>(luaL_checkinteger(L, 1));
    }
    const char *id;
    if (ARGC < 2) {
        id = NULL;
    }
    else {
        id = lua_tostring(L, 2);
    }
    bool border;
    if (ARGC < 3) {
        border = true;
    }
    else {
        border = lua_toboolean(L, 3);
    }
    ImGui::Columns(count,id,border);
    return 0;
}


// function ImGui::Combo

// bool ImGui_Combo(const char*, int*, bool(*)(void* data,int idx,const char** out_text), void*, int, int): Unknown argument type 'bool(*)(void* data,int idx,const char** out_text)' (bool(*)(void* data,int idx,const char** out_text))

// bool ImGui_Combo(const char*, int*, const char* const[], int, int): Unknown argument type 'const char* const[]' (const char* const[])

IMGUILUA_FUNCTION(ImGui::Combo)
static int ImGui_Combo(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 3 || ARGC > 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 3, 4);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int *current_item;
    current_item = static_cast<int *>(luaL_checkudata(L, 2, "ImGuiLua_IntRef"));
    const char *items_separated_by_zeros;
    items_separated_by_zeros = lua_tostring(L, 3);
    int popup_max_height_in_items;
    if (ARGC < 4) {
        popup_max_height_in_items = -1;
    }
    else {
        popup_max_height_in_items = static_cast<int>(luaL_checkinteger(L, 4));
    }
    bool RETVAL = ImGui::Combo(label,current_item,items_separated_by_zeros,popup_max_height_in_items);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::CreateContext

// ImGuiContext* ImGui_CreateContext(ImFontAtlas*): Unknown return type 'ImGuiContext*' (ImGuiContext*)


// function ImGui::DebugCheckVersionAndDataLayout

// bool ImGui_DebugCheckVersionAndDataLayout(const char*, size_t, size_t, size_t, size_t, size_t, size_t): Unknown argument type 'size_t' (size_t)


// function ImGui::DestroyContext

// void ImGui_DestroyContext(ImGuiContext*): Unknown argument type 'ImGuiContext*' (ImGuiContext*)


// function ImGui::DragFloat

IMGUILUA_FUNCTION(ImGui::DragFloat)
static int ImGui_DragFloat(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 7) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 7);
    }
    const char *label;
    label = lua_tostring(L, 1);
    float *v;
    v = static_cast<float *>(luaL_checkudata(L, 2, "ImGuiLua_FloatRef"));
    float v_speed;
    if (ARGC < 3) {
        v_speed = 1.0f;
    }
    else {
        v_speed = static_cast<float>(luaL_checknumber(L, 3));
    }
    float v_min;
    if (ARGC < 4) {
        v_min = 0.0f;
    }
    else {
        v_min = static_cast<float>(luaL_checknumber(L, 4));
    }
    float v_max;
    if (ARGC < 5) {
        v_max = 0.0f;
    }
    else {
        v_max = static_cast<float>(luaL_checknumber(L, 5));
    }
    const char *format;
    if (ARGC < 6) {
        format = "%.3f";
    }
    else {
        format = lua_tostring(L, 6);
    }
    int flags;
    if (ARGC < 7) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 7));
    }
    bool RETVAL = ImGui::DragFloat(label,v,v_speed,v_min,v_max,format,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::DragFloat2

// bool ImGui_DragFloat2(const char*, float[2], float, float, float, const char*, ImGuiSliderFlags): Unknown argument type 'float[2]' (float[2])


// function ImGui::DragFloat3

// bool ImGui_DragFloat3(const char*, float[3], float, float, float, const char*, ImGuiSliderFlags): Unknown argument type 'float[3]' (float[3])


// function ImGui::DragFloat4

// bool ImGui_DragFloat4(const char*, float[4], float, float, float, const char*, ImGuiSliderFlags): Unknown argument type 'float[4]' (float[4])


// function ImGui::DragFloatRange2

IMGUILUA_FUNCTION(ImGui::DragFloatRange2)
static int ImGui_DragFloatRange2(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 3 || ARGC > 9) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 3, 9);
    }
    const char *label;
    label = lua_tostring(L, 1);
    float *v_current_min;
    v_current_min = static_cast<float *>(luaL_checkudata(L, 2, "ImGuiLua_FloatRef"));
    float *v_current_max;
    v_current_max = static_cast<float *>(luaL_checkudata(L, 3, "ImGuiLua_FloatRef"));
    float v_speed;
    if (ARGC < 4) {
        v_speed = 1.0f;
    }
    else {
        v_speed = static_cast<float>(luaL_checknumber(L, 4));
    }
    float v_min;
    if (ARGC < 5) {
        v_min = 0.0f;
    }
    else {
        v_min = static_cast<float>(luaL_checknumber(L, 5));
    }
    float v_max;
    if (ARGC < 6) {
        v_max = 0.0f;
    }
    else {
        v_max = static_cast<float>(luaL_checknumber(L, 6));
    }
    const char *format;
    if (ARGC < 7) {
        format = "%.3f";
    }
    else {
        format = lua_tostring(L, 7);
    }
    const char *format_max;
    if (ARGC < 8) {
        format_max = NULL;
    }
    else {
        format_max = lua_tostring(L, 8);
    }
    int flags;
    if (ARGC < 9) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 9));
    }
    bool RETVAL = ImGui::DragFloatRange2(label,v_current_min,v_current_max,v_speed,v_min,v_max,format,format_max,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::DragInt

IMGUILUA_FUNCTION(ImGui::DragInt)
static int ImGui_DragInt(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 7) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 7);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int *v;
    v = static_cast<int *>(luaL_checkudata(L, 2, "ImGuiLua_IntRef"));
    float v_speed;
    if (ARGC < 3) {
        v_speed = 1.0f;
    }
    else {
        v_speed = static_cast<float>(luaL_checknumber(L, 3));
    }
    int v_min;
    if (ARGC < 4) {
        v_min = 0;
    }
    else {
        v_min = static_cast<int>(luaL_checkinteger(L, 4));
    }
    int v_max;
    if (ARGC < 5) {
        v_max = 0;
    }
    else {
        v_max = static_cast<int>(luaL_checkinteger(L, 5));
    }
    const char *format;
    if (ARGC < 6) {
        format = "%d";
    }
    else {
        format = lua_tostring(L, 6);
    }
    int flags;
    if (ARGC < 7) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 7));
    }
    bool RETVAL = ImGui::DragInt(label,v,v_speed,v_min,v_max,format,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::DragInt2

// bool ImGui_DragInt2(const char*, int[2], float, int, int, const char*, ImGuiSliderFlags): Unknown argument type 'int[2]' (int[2])


// function ImGui::DragInt3

// bool ImGui_DragInt3(const char*, int[3], float, int, int, const char*, ImGuiSliderFlags): Unknown argument type 'int[3]' (int[3])


// function ImGui::DragInt4

// bool ImGui_DragInt4(const char*, int[4], float, int, int, const char*, ImGuiSliderFlags): Unknown argument type 'int[4]' (int[4])


// function ImGui::DragIntRange2

IMGUILUA_FUNCTION(ImGui::DragIntRange2)
static int ImGui_DragIntRange2(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 3 || ARGC > 9) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 3, 9);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int *v_current_min;
    v_current_min = static_cast<int *>(luaL_checkudata(L, 2, "ImGuiLua_IntRef"));
    int *v_current_max;
    v_current_max = static_cast<int *>(luaL_checkudata(L, 3, "ImGuiLua_IntRef"));
    float v_speed;
    if (ARGC < 4) {
        v_speed = 1.0f;
    }
    else {
        v_speed = static_cast<float>(luaL_checknumber(L, 4));
    }
    int v_min;
    if (ARGC < 5) {
        v_min = 0;
    }
    else {
        v_min = static_cast<int>(luaL_checkinteger(L, 5));
    }
    int v_max;
    if (ARGC < 6) {
        v_max = 0;
    }
    else {
        v_max = static_cast<int>(luaL_checkinteger(L, 6));
    }
    const char *format;
    if (ARGC < 7) {
        format = "%d";
    }
    else {
        format = lua_tostring(L, 7);
    }
    const char *format_max;
    if (ARGC < 8) {
        format_max = NULL;
    }
    else {
        format_max = lua_tostring(L, 8);
    }
    int flags;
    if (ARGC < 9) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 9));
    }
    bool RETVAL = ImGui::DragIntRange2(label,v_current_min,v_current_max,v_speed,v_min,v_max,format,format_max,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::DragScalar

// bool ImGui_DragScalar(const char*, ImGuiDataType, void*, float, const void*, const void*, const char*, ImGuiSliderFlags): Unknown argument type 'void*' (void*)


// function ImGui::DragScalarN

// bool ImGui_DragScalarN(const char*, ImGuiDataType, void*, int, float, const void*, const void*, const char*, ImGuiSliderFlags): Unknown argument type 'void*' (void*)


// function ImGui::Dummy

IMGUILUA_FUNCTION(ImGui::Dummy)
static int ImGui_Dummy(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 1);
    ImGui::Dummy(size);
    return 0;
}


// function ImGui::End

IMGUILUA_FUNCTION(ImGui::End)
static int ImGui_End(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::End, 1, ImGui::Begin);
    ImGui::End();
    return 0;
}


// function ImGui::EndChild

IMGUILUA_FUNCTION(ImGui::EndChild)
static int ImGui_EndChild(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndChild, 2, ImGui::BeginChild);
    ImGui::EndChild();
    return 0;
}


// function ImGui::EndChildFrame

IMGUILUA_FUNCTION(ImGui::EndChildFrame)
static int ImGui_EndChildFrame(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndChildFrame, 3, ImGui::BeginChildFrame);
    ImGui::EndChildFrame();
    return 0;
}


// function ImGui::EndCombo

IMGUILUA_FUNCTION(ImGui::EndCombo)
static int ImGui_EndCombo(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndCombo, 4, ImGui::BeginCombo);
    ImGui::EndCombo();
    return 0;
}


// function ImGui::EndDragDropSource

IMGUILUA_FUNCTION(ImGui::EndDragDropSource)
static int ImGui_EndDragDropSource(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndDragDropSource, 5, ImGui::BeginDragDropSource);
    ImGui::EndDragDropSource();
    return 0;
}


// function ImGui::EndDragDropTarget

IMGUILUA_FUNCTION(ImGui::EndDragDropTarget)
static int ImGui_EndDragDropTarget(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndDragDropTarget, 6, ImGui::BeginDragDropTarget);
    ImGui::EndDragDropTarget();
    return 0;
}


// function ImGui::EndFrame is excluded


// function ImGui::EndGroup

IMGUILUA_FUNCTION(ImGui::EndGroup)
static int ImGui_EndGroup(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndGroup, 7, ImGui::BeginGroup);
    ImGui::EndGroup();
    return 0;
}


// function ImGui::EndListBox

IMGUILUA_FUNCTION(ImGui::EndListBox)
static int ImGui_EndListBox(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndListBox, 8, ImGui::BeginListBox);
    ImGui::EndListBox();
    return 0;
}


// function ImGui::EndMainMenuBar

IMGUILUA_FUNCTION(ImGui::EndMainMenuBar)
static int ImGui_EndMainMenuBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndMainMenuBar, 9, ImGui::BeginMainMenuBar);
    ImGui::EndMainMenuBar();
    return 0;
}


// function ImGui::EndMenu

IMGUILUA_FUNCTION(ImGui::EndMenu)
static int ImGui_EndMenu(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndMenu, 10, ImGui::BeginMenu);
    ImGui::EndMenu();
    return 0;
}


// function ImGui::EndMenuBar

IMGUILUA_FUNCTION(ImGui::EndMenuBar)
static int ImGui_EndMenuBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndMenuBar, 11, ImGui::BeginMenuBar);
    ImGui::EndMenuBar();
    return 0;
}


// function ImGui::EndPopup

IMGUILUA_FUNCTION(ImGui::EndPopup)
static int ImGui_EndPopup(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndPopup, 12, ImGui::BeginPopup, ImGui::BeginPopupContextItem, ImGui::BeginPopupContextVoid, ImGui::BeginPopupContextWindow, ImGui::BeginPopupModal);
    ImGui::EndPopup();
    return 0;
}


// function ImGui::EndTabBar

IMGUILUA_FUNCTION(ImGui::EndTabBar)
static int ImGui_EndTabBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndTabBar, 13, ImGui::BeginTabBar);
    ImGui::EndTabBar();
    return 0;
}


// function ImGui::EndTabItem

IMGUILUA_FUNCTION(ImGui::EndTabItem)
static int ImGui_EndTabItem(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndTabItem, 14, ImGui::BeginTabItem);
    ImGui::EndTabItem();
    return 0;
}


// function ImGui::EndTable

IMGUILUA_FUNCTION(ImGui::EndTable)
static int ImGui_EndTable(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndTable, 15, ImGui::BeginTable);
    ImGui::EndTable();
    return 0;
}


// function ImGui::EndTooltip

IMGUILUA_FUNCTION(ImGui::EndTooltip)
static int ImGui_EndTooltip(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImGui::EndTooltip, 16, ImGui::BeginTooltip);
    ImGui::EndTooltip();
    return 0;
}


// function ImGui::GetAllocatorFunctions

// void ImGui_GetAllocatorFunctions(ImGuiMemAllocFunc*, ImGuiMemFreeFunc*, void**): Unknown argument type 'ImGuiMemAllocFunc*' (ImGuiMemAllocFunc*)


// function ImGui::GetBackgroundDrawList

// ImDrawList* ImGui_GetBackgroundDrawList(): Unknown return type 'ImDrawList*' (ImDrawList*)


// function ImGui::GetClipboardText

IMGUILUA_FUNCTION(ImGui::GetClipboardText)
static int ImGui_GetClipboardText(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    const char *RETVAL = ImGui::GetClipboardText();
    lua_pushstring(L, RETVAL);
    return 1;
}


// function ImGui::GetColorU32

static int ImGui_GetColorU32_if(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    int idx;
    idx = static_cast<int>(luaL_checkinteger(L, 1));
    float alpha_mul;
    if (ARGC < 2) {
        alpha_mul = 1.0f;
    }
    else {
        alpha_mul = static_cast<float>(luaL_checknumber(L, 2));
    }
    ImU32 RETVAL = ImGui::GetColorU32(idx,alpha_mul);
    lua_pushinteger(L, static_cast<ImU32>(RETVAL));
    return 1;
}

static int ImGui_GetColorU32_u32(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImU32 col;
    col = static_cast<ImU32>(luaL_checkinteger(L, 1));
    ImU32 RETVAL = ImGui::GetColorU32(col);
    lua_pushinteger(L, static_cast<ImU32>(RETVAL));
    return 1;
}

static int ImGui_GetColorU32_v4(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImVec4 col;
    col = ImGuiLua_ToImVec4(L, 1);
    ImU32 RETVAL = ImGui::GetColorU32(col);
    lua_pushinteger(L, static_cast<ImU32>(RETVAL));
    return 1;
}

IMGUILUA_FUNCTION(ImGui::ImGui_GetColorU32)
static int ImGui_GetColorU32(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC == 1) {
        if (lua_isnumber(L, 1)) {
            return ImGui_GetColorU32_u32(L);
        }
        else {
            return ImGui_GetColorU32_v4(L);
        }
    }
    else if (ARGC == 2) {
        return ImGui_GetColorU32_if(L);
    }
    else {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
}


// function ImGui::GetColumnIndex

IMGUILUA_FUNCTION(ImGui::GetColumnIndex)
static int ImGui_GetColumnIndex(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImGui::GetColumnIndex();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::GetColumnOffset

IMGUILUA_FUNCTION(ImGui::GetColumnOffset)
static int ImGui_GetColumnOffset(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int column_index;
    if (ARGC < 1) {
        column_index = -1;
    }
    else {
        column_index = static_cast<int>(luaL_checkinteger(L, 1));
    }
    float RETVAL = ImGui::GetColumnOffset(column_index);
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetColumnWidth

IMGUILUA_FUNCTION(ImGui::GetColumnWidth)
static int ImGui_GetColumnWidth(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int column_index;
    if (ARGC < 1) {
        column_index = -1;
    }
    else {
        column_index = static_cast<int>(luaL_checkinteger(L, 1));
    }
    float RETVAL = ImGui::GetColumnWidth(column_index);
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetColumnsCount

IMGUILUA_FUNCTION(ImGui::GetColumnsCount)
static int ImGui_GetColumnsCount(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImGui::GetColumnsCount();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::GetContentRegionAvail

IMGUILUA_FUNCTION(ImGui::GetContentRegionAvail)
static int ImGui_GetContentRegionAvail(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetContentRegionAvail();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetContentRegionMax

IMGUILUA_FUNCTION(ImGui::GetContentRegionMax)
static int ImGui_GetContentRegionMax(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetContentRegionMax();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetCurrentContext

// ImGuiContext* ImGui_GetCurrentContext(): Unknown return type 'ImGuiContext*' (ImGuiContext*)


// function ImGui::GetCursorPos

IMGUILUA_FUNCTION(ImGui::GetCursorPos)
static int ImGui_GetCursorPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetCursorPos();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetCursorPosX

IMGUILUA_FUNCTION(ImGui::GetCursorPosX)
static int ImGui_GetCursorPosX(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetCursorPosX();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetCursorPosY

IMGUILUA_FUNCTION(ImGui::GetCursorPosY)
static int ImGui_GetCursorPosY(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetCursorPosY();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetCursorScreenPos

IMGUILUA_FUNCTION(ImGui::GetCursorScreenPos)
static int ImGui_GetCursorScreenPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetCursorScreenPos();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetCursorStartPos

IMGUILUA_FUNCTION(ImGui::GetCursorStartPos)
static int ImGui_GetCursorStartPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetCursorStartPos();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetDragDropPayload

// const ImGuiPayload* ImGui_GetDragDropPayload(): Unknown return type 'const ImGuiPayload*' (const ImGuiPayload*)


// function ImGui::GetDrawData

// ImDrawData* ImGui_GetDrawData(): Unknown return type 'ImDrawData*' (ImDrawData*)


// function ImGui::GetDrawListSharedData

// ImDrawListSharedData* ImGui_GetDrawListSharedData(): Unknown return type 'ImDrawListSharedData*' (ImDrawListSharedData*)


// function ImGui::GetFont

// ImFont* ImGui_GetFont(): Unknown return type 'ImFont*' (ImFont*)


// function ImGui::GetFontSize

IMGUILUA_FUNCTION(ImGui::GetFontSize)
static int ImGui_GetFontSize(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetFontSize();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetFontTexUvWhitePixel

IMGUILUA_FUNCTION(ImGui::GetFontTexUvWhitePixel)
static int ImGui_GetFontTexUvWhitePixel(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetFontTexUvWhitePixel();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetForegroundDrawList

// ImDrawList* ImGui_GetForegroundDrawList(): Unknown return type 'ImDrawList*' (ImDrawList*)


// function ImGui::GetFrameCount

IMGUILUA_FUNCTION(ImGui::GetFrameCount)
static int ImGui_GetFrameCount(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImGui::GetFrameCount();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::GetFrameHeight

IMGUILUA_FUNCTION(ImGui::GetFrameHeight)
static int ImGui_GetFrameHeight(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetFrameHeight();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetFrameHeightWithSpacing

IMGUILUA_FUNCTION(ImGui::GetFrameHeightWithSpacing)
static int ImGui_GetFrameHeightWithSpacing(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetFrameHeightWithSpacing();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetID is excluded


// function ImGui::GetIO

// ImGuiIO* ImGui_GetIO(): Unknown return type 'ImGuiIO*' (ImGuiIO*)


// function ImGui::GetItemRectMax

IMGUILUA_FUNCTION(ImGui::GetItemRectMax)
static int ImGui_GetItemRectMax(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetItemRectMax();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetItemRectMin

IMGUILUA_FUNCTION(ImGui::GetItemRectMin)
static int ImGui_GetItemRectMin(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetItemRectMin();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetItemRectSize

IMGUILUA_FUNCTION(ImGui::GetItemRectSize)
static int ImGui_GetItemRectSize(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetItemRectSize();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetKeyIndex

IMGUILUA_FUNCTION(ImGui::GetKeyIndex)
static int ImGui_GetKeyIndex(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int imgui_key;
    imgui_key = static_cast<int>(luaL_checkinteger(L, 1));
    int RETVAL = ImGui::GetKeyIndex(imgui_key);
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::GetKeyPressedAmount

IMGUILUA_FUNCTION(ImGui::GetKeyPressedAmount)
static int ImGui_GetKeyPressedAmount(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 3);
    }
    int key_index;
    key_index = static_cast<int>(luaL_checkinteger(L, 1));
    float repeat_delay;
    repeat_delay = static_cast<float>(luaL_checknumber(L, 2));
    float rate;
    rate = static_cast<float>(luaL_checknumber(L, 3));
    int RETVAL = ImGui::GetKeyPressedAmount(key_index,repeat_delay,rate);
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::GetMainViewport

// ImGuiViewport* ImGui_GetMainViewport(): Unknown return type 'ImGuiViewport*' (ImGuiViewport*)


// function ImGui::GetMouseCursor

IMGUILUA_FUNCTION(ImGui::GetMouseCursor)
static int ImGui_GetMouseCursor(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImGui::GetMouseCursor();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::GetMouseDragDelta

IMGUILUA_FUNCTION(ImGui::GetMouseDragDelta)
static int ImGui_GetMouseDragDelta(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 2);
    }
    int button;
    if (ARGC < 1) {
        button = 0;
    }
    else {
        button = static_cast<int>(luaL_checkinteger(L, 1));
    }
    float lock_threshold;
    if (ARGC < 2) {
        lock_threshold = -1.0f;
    }
    else {
        lock_threshold = static_cast<float>(luaL_checknumber(L, 2));
    }
    ImVec2 RETVAL = ImGui::GetMouseDragDelta(button,lock_threshold);
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetMousePos

IMGUILUA_FUNCTION(ImGui::GetMousePos)
static int ImGui_GetMousePos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetMousePos();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetMousePosOnOpeningCurrentPopup

IMGUILUA_FUNCTION(ImGui::GetMousePosOnOpeningCurrentPopup)
static int ImGui_GetMousePosOnOpeningCurrentPopup(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetMousePosOnOpeningCurrentPopup();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetScrollMaxX

IMGUILUA_FUNCTION(ImGui::GetScrollMaxX)
static int ImGui_GetScrollMaxX(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetScrollMaxX();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetScrollMaxY

IMGUILUA_FUNCTION(ImGui::GetScrollMaxY)
static int ImGui_GetScrollMaxY(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetScrollMaxY();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetScrollX

IMGUILUA_FUNCTION(ImGui::GetScrollX)
static int ImGui_GetScrollX(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetScrollX();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetScrollY

IMGUILUA_FUNCTION(ImGui::GetScrollY)
static int ImGui_GetScrollY(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetScrollY();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetStateStorage

// ImGuiStorage* ImGui_GetStateStorage(): Unknown return type 'ImGuiStorage*' (ImGuiStorage*)


// function ImGui::GetStyle

// ImGuiStyle* ImGui_GetStyle(): Unknown return type 'ImGuiStyle*' (ImGuiStyle*)


// function ImGui::GetStyleColorName

IMGUILUA_FUNCTION(ImGui::GetStyleColorName)
static int ImGui_GetStyleColorName(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int idx;
    idx = static_cast<int>(luaL_checkinteger(L, 1));
    const char *RETVAL = ImGui::GetStyleColorName(idx);
    lua_pushstring(L, RETVAL);
    return 1;
}


// function ImGui::GetStyleColorVec4

// const ImVec4* ImGui_GetStyleColorVec4(ImGuiCol): Unknown return type 'const ImVec4*' (const ImVec4*)


// function ImGui::GetTextLineHeight

IMGUILUA_FUNCTION(ImGui::GetTextLineHeight)
static int ImGui_GetTextLineHeight(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetTextLineHeight();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetTextLineHeightWithSpacing

IMGUILUA_FUNCTION(ImGui::GetTextLineHeightWithSpacing)
static int ImGui_GetTextLineHeightWithSpacing(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetTextLineHeightWithSpacing();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetTime

IMGUILUA_FUNCTION(ImGui::GetTime)
static int ImGui_GetTime(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    double RETVAL = ImGui::GetTime();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetTreeNodeToLabelSpacing

IMGUILUA_FUNCTION(ImGui::GetTreeNodeToLabelSpacing)
static int ImGui_GetTreeNodeToLabelSpacing(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetTreeNodeToLabelSpacing();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetVersion

IMGUILUA_FUNCTION(ImGui::GetVersion)
static int ImGui_GetVersion(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    const char *RETVAL = ImGui::GetVersion();
    lua_pushstring(L, RETVAL);
    return 1;
}


// function ImGui::GetWindowContentRegionMax

IMGUILUA_FUNCTION(ImGui::GetWindowContentRegionMax)
static int ImGui_GetWindowContentRegionMax(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetWindowContentRegionMax();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetWindowContentRegionMin

IMGUILUA_FUNCTION(ImGui::GetWindowContentRegionMin)
static int ImGui_GetWindowContentRegionMin(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetWindowContentRegionMin();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetWindowContentRegionWidth

IMGUILUA_FUNCTION(ImGui::GetWindowContentRegionWidth)
static int ImGui_GetWindowContentRegionWidth(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetWindowContentRegionWidth();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetWindowDrawList

// ImDrawList* ImGui_GetWindowDrawList(): Unknown return type 'ImDrawList*' (ImDrawList*)


// function ImGui::GetWindowHeight

IMGUILUA_FUNCTION(ImGui::GetWindowHeight)
static int ImGui_GetWindowHeight(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetWindowHeight();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::GetWindowPos

IMGUILUA_FUNCTION(ImGui::GetWindowPos)
static int ImGui_GetWindowPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetWindowPos();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetWindowSize

IMGUILUA_FUNCTION(ImGui::GetWindowSize)
static int ImGui_GetWindowSize(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImGui::GetWindowSize();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImGui::GetWindowWidth

IMGUILUA_FUNCTION(ImGui::GetWindowWidth)
static int ImGui_GetWindowWidth(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    float RETVAL = ImGui::GetWindowWidth();
    lua_pushnumber(L, static_cast<lua_Number>(RETVAL));
    return 1;
}


// function ImGui::Image

// void ImGui_Image(ImTextureID, const ImVec2, const ImVec2, const ImVec2, const ImVec4, const ImVec4): Unknown argument type 'ImTextureID' (ImTextureID)


// function ImGui::ImageButton

// bool ImGui_ImageButton(ImTextureID, const ImVec2, const ImVec2, const ImVec2, int, const ImVec4, const ImVec4): Unknown argument type 'ImTextureID' (ImTextureID)


// function ImGui::Indent

IMGUILUA_FUNCTION(ImGui::Indent)
static int ImGui_Indent(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    float indent_w;
    if (ARGC < 1) {
        indent_w = 0.0f;
    }
    else {
        indent_w = static_cast<float>(luaL_checknumber(L, 1));
    }
    ImGui::Indent(indent_w);
    return 0;
}


// function ImGui::InputDouble

// bool ImGui_InputDouble(const char*, double*, double, double, const char*, ImGuiInputTextFlags): Unknown argument type 'double*' (double*)


// function ImGui::InputFloat

IMGUILUA_FUNCTION(ImGui::InputFloat)
static int ImGui_InputFloat(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 6) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 6);
    }
    const char *label;
    label = lua_tostring(L, 1);
    float *v;
    v = static_cast<float *>(luaL_checkudata(L, 2, "ImGuiLua_FloatRef"));
    float step;
    if (ARGC < 3) {
        step = 0.0f;
    }
    else {
        step = static_cast<float>(luaL_checknumber(L, 3));
    }
    float step_fast;
    if (ARGC < 4) {
        step_fast = 0.0f;
    }
    else {
        step_fast = static_cast<float>(luaL_checknumber(L, 4));
    }
    const char *format;
    if (ARGC < 5) {
        format = "%.3f";
    }
    else {
        format = lua_tostring(L, 5);
    }
    int flags;
    if (ARGC < 6) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 6));
    }
    bool RETVAL = ImGui::InputFloat(label,v,step,step_fast,format,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::InputFloat2

// bool ImGui_InputFloat2(const char*, float[2], const char*, ImGuiInputTextFlags): Unknown argument type 'float[2]' (float[2])


// function ImGui::InputFloat3

// bool ImGui_InputFloat3(const char*, float[3], const char*, ImGuiInputTextFlags): Unknown argument type 'float[3]' (float[3])


// function ImGui::InputFloat4

// bool ImGui_InputFloat4(const char*, float[4], const char*, ImGuiInputTextFlags): Unknown argument type 'float[4]' (float[4])


// function ImGui::InputInt

IMGUILUA_FUNCTION(ImGui::InputInt)
static int ImGui_InputInt(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 5) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 5);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int *v;
    v = static_cast<int *>(luaL_checkudata(L, 2, "ImGuiLua_IntRef"));
    int step;
    if (ARGC < 3) {
        step = 1;
    }
    else {
        step = static_cast<int>(luaL_checkinteger(L, 3));
    }
    int step_fast;
    if (ARGC < 4) {
        step_fast = 100;
    }
    else {
        step_fast = static_cast<int>(luaL_checkinteger(L, 4));
    }
    int flags;
    if (ARGC < 5) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 5));
    }
    bool RETVAL = ImGui::InputInt(label,v,step,step_fast,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::InputInt2

// bool ImGui_InputInt2(const char*, int[2], ImGuiInputTextFlags): Unknown argument type 'int[2]' (int[2])


// function ImGui::InputInt3

// bool ImGui_InputInt3(const char*, int[3], ImGuiInputTextFlags): Unknown argument type 'int[3]' (int[3])


// function ImGui::InputInt4

// bool ImGui_InputInt4(const char*, int[4], ImGuiInputTextFlags): Unknown argument type 'int[4]' (int[4])


// function ImGui::InputScalar

// bool ImGui_InputScalar(const char*, ImGuiDataType, void*, const void*, const void*, const char*, ImGuiInputTextFlags): Unknown argument type 'void*' (void*)


// function ImGui::InputScalarN

// bool ImGui_InputScalarN(const char*, ImGuiDataType, void*, int, const void*, const void*, const char*, ImGuiInputTextFlags): Unknown argument type 'void*' (void*)


// function ImGui::InputText

// bool ImGui_InputText(const char*, char*, size_t, ImGuiInputTextFlags, ImGuiInputTextCallback, void*): Unknown argument type 'char*' (char*)


// function ImGui::InputTextMultiline

// bool ImGui_InputTextMultiline(const char*, char*, size_t, const ImVec2, ImGuiInputTextFlags, ImGuiInputTextCallback, void*): Unknown argument type 'char*' (char*)


// function ImGui::InputTextWithHint

// bool ImGui_InputTextWithHint(const char*, const char*, char*, size_t, ImGuiInputTextFlags, ImGuiInputTextCallback, void*): Unknown argument type 'char*' (char*)


// function ImGui::InvisibleButton

IMGUILUA_FUNCTION(ImGui::InvisibleButton)
static int ImGui_InvisibleButton(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 2);
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    bool RETVAL = ImGui::InvisibleButton(str_id,size,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsAnyItemActive

IMGUILUA_FUNCTION(ImGui::IsAnyItemActive)
static int ImGui_IsAnyItemActive(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsAnyItemActive();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsAnyItemFocused

IMGUILUA_FUNCTION(ImGui::IsAnyItemFocused)
static int ImGui_IsAnyItemFocused(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsAnyItemFocused();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsAnyItemHovered

IMGUILUA_FUNCTION(ImGui::IsAnyItemHovered)
static int ImGui_IsAnyItemHovered(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsAnyItemHovered();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsAnyMouseDown

IMGUILUA_FUNCTION(ImGui::IsAnyMouseDown)
static int ImGui_IsAnyMouseDown(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsAnyMouseDown();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemActivated

IMGUILUA_FUNCTION(ImGui::IsItemActivated)
static int ImGui_IsItemActivated(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsItemActivated();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemActive

IMGUILUA_FUNCTION(ImGui::IsItemActive)
static int ImGui_IsItemActive(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsItemActive();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemClicked

IMGUILUA_FUNCTION(ImGui::IsItemClicked)
static int ImGui_IsItemClicked(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int mouse_button;
    if (ARGC < 1) {
        mouse_button = 0;
    }
    else {
        mouse_button = static_cast<int>(luaL_checkinteger(L, 1));
    }
    bool RETVAL = ImGui::IsItemClicked(mouse_button);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemDeactivated

IMGUILUA_FUNCTION(ImGui::IsItemDeactivated)
static int ImGui_IsItemDeactivated(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsItemDeactivated();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemDeactivatedAfterEdit

IMGUILUA_FUNCTION(ImGui::IsItemDeactivatedAfterEdit)
static int ImGui_IsItemDeactivatedAfterEdit(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsItemDeactivatedAfterEdit();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemEdited

IMGUILUA_FUNCTION(ImGui::IsItemEdited)
static int ImGui_IsItemEdited(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsItemEdited();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemFocused

IMGUILUA_FUNCTION(ImGui::IsItemFocused)
static int ImGui_IsItemFocused(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsItemFocused();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemHovered

IMGUILUA_FUNCTION(ImGui::IsItemHovered)
static int ImGui_IsItemHovered(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int flags;
    if (ARGC < 1) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 1));
    }
    bool RETVAL = ImGui::IsItemHovered(flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemToggledOpen

IMGUILUA_FUNCTION(ImGui::IsItemToggledOpen)
static int ImGui_IsItemToggledOpen(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsItemToggledOpen();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsItemVisible

IMGUILUA_FUNCTION(ImGui::IsItemVisible)
static int ImGui_IsItemVisible(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsItemVisible();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsKeyDown

IMGUILUA_FUNCTION(ImGui::IsKeyDown)
static int ImGui_IsKeyDown(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int user_key_index;
    user_key_index = static_cast<int>(luaL_checkinteger(L, 1));
    bool RETVAL = ImGui::IsKeyDown(user_key_index);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsKeyPressed

IMGUILUA_FUNCTION(ImGui::IsKeyPressed)
static int ImGui_IsKeyPressed(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    int user_key_index;
    user_key_index = static_cast<int>(luaL_checkinteger(L, 1));
    bool repeat;
    if (ARGC < 2) {
        repeat = true;
    }
    else {
        repeat = lua_toboolean(L, 2);
    }
    bool RETVAL = ImGui::IsKeyPressed(user_key_index,repeat);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsKeyReleased

IMGUILUA_FUNCTION(ImGui::IsKeyReleased)
static int ImGui_IsKeyReleased(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int user_key_index;
    user_key_index = static_cast<int>(luaL_checkinteger(L, 1));
    bool RETVAL = ImGui::IsKeyReleased(user_key_index);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsMouseClicked

IMGUILUA_FUNCTION(ImGui::IsMouseClicked)
static int ImGui_IsMouseClicked(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    int button;
    button = static_cast<int>(luaL_checkinteger(L, 1));
    bool repeat;
    if (ARGC < 2) {
        repeat = false;
    }
    else {
        repeat = lua_toboolean(L, 2);
    }
    bool RETVAL = ImGui::IsMouseClicked(button,repeat);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsMouseDoubleClicked

IMGUILUA_FUNCTION(ImGui::IsMouseDoubleClicked)
static int ImGui_IsMouseDoubleClicked(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int button;
    button = static_cast<int>(luaL_checkinteger(L, 1));
    bool RETVAL = ImGui::IsMouseDoubleClicked(button);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsMouseDown

IMGUILUA_FUNCTION(ImGui::IsMouseDown)
static int ImGui_IsMouseDown(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int button;
    button = static_cast<int>(luaL_checkinteger(L, 1));
    bool RETVAL = ImGui::IsMouseDown(button);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsMouseDragging

IMGUILUA_FUNCTION(ImGui::IsMouseDragging)
static int ImGui_IsMouseDragging(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    int button;
    button = static_cast<int>(luaL_checkinteger(L, 1));
    float lock_threshold;
    if (ARGC < 2) {
        lock_threshold = -1.0f;
    }
    else {
        lock_threshold = static_cast<float>(luaL_checknumber(L, 2));
    }
    bool RETVAL = ImGui::IsMouseDragging(button,lock_threshold);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsMouseHoveringRect

IMGUILUA_FUNCTION(ImGui::IsMouseHoveringRect)
static int ImGui_IsMouseHoveringRect(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    ImVec2 r_min;
    r_min = ImGuiLua_ToImVec2(L, 1);
    ImVec2 r_max;
    r_max = ImGuiLua_ToImVec2(L, 2);
    bool clip;
    if (ARGC < 3) {
        clip = true;
    }
    else {
        clip = lua_toboolean(L, 3);
    }
    bool RETVAL = ImGui::IsMouseHoveringRect(r_min,r_max,clip);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsMousePosValid

// bool ImGui_IsMousePosValid(const ImVec2*): Unknown argument type 'const ImVec2*' (const ImVec2*)


// function ImGui::IsMouseReleased

IMGUILUA_FUNCTION(ImGui::IsMouseReleased)
static int ImGui_IsMouseReleased(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int button;
    button = static_cast<int>(luaL_checkinteger(L, 1));
    bool RETVAL = ImGui::IsMouseReleased(button);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsPopupOpen

IMGUILUA_FUNCTION(ImGui::IsPopupOpen)
static int ImGui_IsPopupOpen(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    int flags;
    if (ARGC < 2) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::IsPopupOpen(str_id,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsRectVisible

static int ImGui_IsRectVisible_v2(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 1);
    bool RETVAL = ImGui::IsRectVisible(size);
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int ImGui_IsRectVisible_v2v2(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    ImVec2 rect_min;
    rect_min = ImGuiLua_ToImVec2(L, 1);
    ImVec2 rect_max;
    rect_max = ImGuiLua_ToImVec2(L, 2);
    bool RETVAL = ImGui::IsRectVisible(rect_min,rect_max);
    lua_pushboolean(L, RETVAL);
    return 1;
}

IMGUILUA_FUNCTION(ImGui::ImGui_IsRectVisible)
static int ImGui_IsRectVisible(lua_State *L)
{
    int ARGC = lua_gettop(L);
    switch (ARGC) {
    case 1:
        return ImGui_IsRectVisible_v2(L);
    case 2:
        return ImGui_IsRectVisible_v2v2(L);
    default:
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
}


// function ImGui::IsWindowAppearing

IMGUILUA_FUNCTION(ImGui::IsWindowAppearing)
static int ImGui_IsWindowAppearing(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsWindowAppearing();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsWindowCollapsed

IMGUILUA_FUNCTION(ImGui::IsWindowCollapsed)
static int ImGui_IsWindowCollapsed(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::IsWindowCollapsed();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsWindowFocused

IMGUILUA_FUNCTION(ImGui::IsWindowFocused)
static int ImGui_IsWindowFocused(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int flags;
    if (ARGC < 1) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 1));
    }
    bool RETVAL = ImGui::IsWindowFocused(flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::IsWindowHovered

IMGUILUA_FUNCTION(ImGui::IsWindowHovered)
static int ImGui_IsWindowHovered(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int flags;
    if (ARGC < 1) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 1));
    }
    bool RETVAL = ImGui::IsWindowHovered(flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::LabelText is excluded


// function ImGui::LabelTextV is excluded


// function ImGui::ListBox

// bool ImGui_ListBox(const char*, int*, bool(*)(void* data,int idx,const char** out_text), void*, int, int): Unknown argument type 'bool(*)(void* data,int idx,const char** out_text)' (bool(*)(void* data,int idx,const char** out_text))

// bool ImGui_ListBox(const char*, int*, const char* const[], int, int): Unknown argument type 'const char* const[]' (const char* const[])


// function ImGui::LoadIniSettingsFromDisk

IMGUILUA_FUNCTION(ImGui::LoadIniSettingsFromDisk)
static int ImGui_LoadIniSettingsFromDisk(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *ini_filename;
    ini_filename = lua_tostring(L, 1);
    ImGui::LoadIniSettingsFromDisk(ini_filename);
    return 0;
}


// function ImGui::LoadIniSettingsFromMemory

// void ImGui_LoadIniSettingsFromMemory(const char*, size_t): Unknown argument type 'size_t' (size_t)


// function ImGui::LogButtons

IMGUILUA_FUNCTION(ImGui::LogButtons)
static int ImGui_LogButtons(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::LogButtons();
    return 0;
}


// function ImGui::LogFinish

IMGUILUA_FUNCTION(ImGui::LogFinish)
static int ImGui_LogFinish(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::LogFinish();
    return 0;
}


// function ImGui::LogText

// void ImGui_LogText(const char*, ...): Unknown argument type '...' (...)


// function ImGui::LogTextV

// void ImGui_LogTextV(const char*, va_list): Unknown argument type 'va_list' (va_list)


// function ImGui::LogToClipboard

IMGUILUA_FUNCTION(ImGui::LogToClipboard)
static int ImGui_LogToClipboard(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int auto_open_depth;
    if (ARGC < 1) {
        auto_open_depth = -1;
    }
    else {
        auto_open_depth = static_cast<int>(luaL_checkinteger(L, 1));
    }
    ImGui::LogToClipboard(auto_open_depth);
    return 0;
}


// function ImGui::LogToFile

IMGUILUA_FUNCTION(ImGui::LogToFile)
static int ImGui_LogToFile(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 2);
    }
    int auto_open_depth;
    if (ARGC < 1) {
        auto_open_depth = -1;
    }
    else {
        auto_open_depth = static_cast<int>(luaL_checkinteger(L, 1));
    }
    const char *filename;
    if (ARGC < 2) {
        filename = NULL;
    }
    else {
        filename = lua_tostring(L, 2);
    }
    ImGui::LogToFile(auto_open_depth,filename);
    return 0;
}


// function ImGui::LogToTTY

IMGUILUA_FUNCTION(ImGui::LogToTTY)
static int ImGui_LogToTTY(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int auto_open_depth;
    if (ARGC < 1) {
        auto_open_depth = -1;
    }
    else {
        auto_open_depth = static_cast<int>(luaL_checkinteger(L, 1));
    }
    ImGui::LogToTTY(auto_open_depth);
    return 0;
}


// function ImGui::MemAlloc

// void* ImGui_MemAlloc(size_t): Unknown return type 'void*' (void*)


// function ImGui::MemFree

// void ImGui_MemFree(void*): Unknown argument type 'void*' (void*)


// function ImGui::MenuItem

// bool ImGui_MenuItem(const char*, const char*, bool*, bool) is excluded

IMGUILUA_FUNCTION(ImGui::MenuItem)
static int ImGui_MenuItem(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 4);
    }
    const char *label;
    label = lua_tostring(L, 1);
    const char *shortcut;
    if (ARGC < 2) {
        shortcut = NULL;
    }
    else {
        shortcut = lua_tostring(L, 2);
    }
    bool selected;
    if (ARGC < 3) {
        selected = false;
    }
    else {
        selected = lua_toboolean(L, 3);
    }
    bool enabled;
    if (ARGC < 4) {
        enabled = true;
    }
    else {
        enabled = lua_toboolean(L, 4);
    }
    bool RETVAL = ImGui::MenuItem(label,shortcut,selected,enabled);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::NewFrame is excluded


// function ImGui::NewLine

IMGUILUA_FUNCTION(ImGui::NewLine)
static int ImGui_NewLine(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::NewLine();
    return 0;
}


// function ImGui::NextColumn

IMGUILUA_FUNCTION(ImGui::NextColumn)
static int ImGui_NextColumn(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::NextColumn();
    return 0;
}


// function ImGui::OpenPopup

static int ImGui_OpenPopup_ui(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    unsigned int id;
    id = static_cast<unsigned int>(luaL_checkinteger(L, 1));
    int popup_flags;
    if (ARGC < 2) {
        popup_flags = 0;
    }
    else {
        popup_flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::OpenPopup(id,popup_flags);
    return 0;
}

static int ImGui_OpenPopup_si(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    int popup_flags;
    if (ARGC < 2) {
        popup_flags = 0;
    }
    else {
        popup_flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::OpenPopup(str_id,popup_flags);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::ImGui_OpenPopup)
static int ImGui_OpenPopup(lua_State *L)
{
    if (lua_type(L, 1) == LUA_TNUMBER) {
        return ImGui_OpenPopup_ui(L);
    }
    else {
        return ImGui_OpenPopup_si(L);
    }
}


// function ImGui::OpenPopupOnItemClick

IMGUILUA_FUNCTION(ImGui::OpenPopupOnItemClick)
static int ImGui_OpenPopupOnItemClick(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 2);
    }
    const char *str_id;
    if (ARGC < 1) {
        str_id = NULL;
    }
    else {
        str_id = lua_tostring(L, 1);
    }
    int popup_flags;
    if (ARGC < 2) {
        popup_flags = 1;
    }
    else {
        popup_flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::OpenPopupOnItemClick(str_id,popup_flags);
    return 0;
}


// function ImGui::PlotHistogram

// void ImGui_PlotHistogram(const char*, const float*, int, int, const char*, float, float, ImVec2, int): Unknown argument type 'const float*' (const float*)

// void ImGui_PlotHistogram(const char*, float(*)(void* data,int idx), void*, int, int, const char*, float, float, ImVec2): Unknown argument type 'float(*)(void* data,int idx)' (float(*)(void* data,int idx))


// function ImGui::PlotLines

// void ImGui_PlotLines(const char*, const float*, int, int, const char*, float, float, ImVec2, int): Unknown argument type 'const float*' (const float*)

// void ImGui_PlotLines(const char*, float(*)(void* data,int idx), void*, int, int, const char*, float, float, ImVec2): Unknown argument type 'float(*)(void* data,int idx)' (float(*)(void* data,int idx))


// function ImGui::PopAllowKeyboardFocus

IMGUILUA_FUNCTION(ImGui::PopAllowKeyboardFocus)
static int ImGui_PopAllowKeyboardFocus(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::PopAllowKeyboardFocus();
    return 0;
}


// function ImGui::PopButtonRepeat

IMGUILUA_FUNCTION(ImGui::PopButtonRepeat)
static int ImGui_PopButtonRepeat(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::PopButtonRepeat();
    return 0;
}


// function ImGui::PopClipRect

IMGUILUA_FUNCTION(ImGui::PopClipRect)
static int ImGui_PopClipRect(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::PopClipRect();
    return 0;
}


// function ImGui::PopFont

IMGUILUA_FUNCTION(ImGui::PopFont)
static int ImGui_PopFont(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::PopFont();
    return 0;
}


// function ImGui::PopID

IMGUILUA_FUNCTION(ImGui::PopID)
static int ImGui_PopID(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::PopID();
    return 0;
}


// function ImGui::PopItemWidth

IMGUILUA_FUNCTION(ImGui::PopItemWidth)
static int ImGui_PopItemWidth(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::PopItemWidth();
    return 0;
}


// function ImGui::PopStyleColor

IMGUILUA_FUNCTION(ImGui::PopStyleColor)
static int ImGui_PopStyleColor(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int count;
    if (ARGC < 1) {
        count = 1;
    }
    else {
        count = static_cast<int>(luaL_checkinteger(L, 1));
    }
    ImGui::PopStyleColor(count);
    return 0;
}


// function ImGui::PopStyleVar

IMGUILUA_FUNCTION(ImGui::PopStyleVar)
static int ImGui_PopStyleVar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int count;
    if (ARGC < 1) {
        count = 1;
    }
    else {
        count = static_cast<int>(luaL_checkinteger(L, 1));
    }
    ImGui::PopStyleVar(count);
    return 0;
}


// function ImGui::PopTextWrapPos

IMGUILUA_FUNCTION(ImGui::PopTextWrapPos)
static int ImGui_PopTextWrapPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::PopTextWrapPos();
    return 0;
}


// function ImGui::ProgressBar

IMGUILUA_FUNCTION(ImGui::ProgressBar)
static int ImGui_ProgressBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 3);
    }
    float fraction;
    fraction = static_cast<float>(luaL_checknumber(L, 1));
    ImVec2 size_arg;
    if (ARGC < 2) {
        size_arg = ImVec2(-FLT_MIN,0);
    }
    else {
        size_arg = ImGuiLua_ToImVec2(L, 2);
    }
    const char *overlay;
    if (ARGC < 3) {
        overlay = NULL;
    }
    else {
        overlay = lua_tostring(L, 3);
    }
    ImGui::ProgressBar(fraction,size_arg,overlay);
    return 0;
}


// function ImGui::PushAllowKeyboardFocus

IMGUILUA_FUNCTION(ImGui::PushAllowKeyboardFocus)
static int ImGui_PushAllowKeyboardFocus(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    bool allow_keyboard_focus;
    allow_keyboard_focus = lua_toboolean(L, 1);
    ImGui::PushAllowKeyboardFocus(allow_keyboard_focus);
    return 0;
}


// function ImGui::PushButtonRepeat

IMGUILUA_FUNCTION(ImGui::PushButtonRepeat)
static int ImGui_PushButtonRepeat(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    bool repeat;
    repeat = lua_toboolean(L, 1);
    ImGui::PushButtonRepeat(repeat);
    return 0;
}


// function ImGui::PushClipRect

IMGUILUA_FUNCTION(ImGui::PushClipRect)
static int ImGui_PushClipRect(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 3);
    }
    ImVec2 clip_rect_min;
    clip_rect_min = ImGuiLua_ToImVec2(L, 1);
    ImVec2 clip_rect_max;
    clip_rect_max = ImGuiLua_ToImVec2(L, 2);
    bool intersect_with_current_clip_rect;
    intersect_with_current_clip_rect = lua_toboolean(L, 3);
    ImGui::PushClipRect(clip_rect_min,clip_rect_max,intersect_with_current_clip_rect);
    return 0;
}


// function ImGui::PushFont

// void ImGui_PushFont(ImFont*): Unknown argument type 'ImFont*' (ImFont*)


// function ImGui::PushID is excluded


// function ImGui::PushItemWidth

IMGUILUA_FUNCTION(ImGui::PushItemWidth)
static int ImGui_PushItemWidth(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    float item_width;
    item_width = static_cast<float>(luaL_checknumber(L, 1));
    ImGui::PushItemWidth(item_width);
    return 0;
}


// function ImGui::PushStyleColor

static int ImGui_PushStyleColor_iu32(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int idx;
    idx = static_cast<int>(luaL_checkinteger(L, 1));
    ImU32 col;
    col = static_cast<ImU32>(luaL_checkinteger(L, 2));
    ImGui::PushStyleColor(idx,col);
    return 0;
}

static int ImGui_PushStyleColor_iv4(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int idx;
    idx = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec4 col;
    col = ImGuiLua_ToImVec4(L, 2);
    ImGui::PushStyleColor(idx,col);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::ImGui_PushStyleColor)
static int ImGui_PushStyleColor(lua_State *L)
{
    if (lua_isnumber(L, 1)) {
        return ImGui_PushStyleColor_iv4(L);
    }
    else {
        return ImGui_PushStyleColor_iu32(L);
    }
}


// function ImGui::PushStyleVar

static int ImGui_PushStyleVar_iv2(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int idx;
    idx = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec2 val;
    val = ImGuiLua_ToImVec2(L, 2);
    ImGui::PushStyleVar(idx,val);
    return 0;
}

static int ImGui_PushStyleVar_if(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int idx;
    idx = static_cast<int>(luaL_checkinteger(L, 1));
    float val;
    val = static_cast<float>(luaL_checknumber(L, 2));
    ImGui::PushStyleVar(idx,val);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::ImGui_PushStyleVar)
static int ImGui_PushStyleVar(lua_State *L)
{
    if (lua_isnumber(L, 1)) {
        return ImGui_PushStyleVar_if(L);
    }
    else {
        return ImGui_PushStyleVar_iv2(L);
    }
}


// function ImGui::PushTextWrapPos

IMGUILUA_FUNCTION(ImGui::PushTextWrapPos)
static int ImGui_PushTextWrapPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    float wrap_local_pos_x;
    if (ARGC < 1) {
        wrap_local_pos_x = 0.0f;
    }
    else {
        wrap_local_pos_x = static_cast<float>(luaL_checknumber(L, 1));
    }
    ImGui::PushTextWrapPos(wrap_local_pos_x);
    return 0;
}


// function ImGui::RadioButton

static int ImGui_RadioButton_sb(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool active;
    active = lua_toboolean(L, 2);
    bool RETVAL = ImGui::RadioButton(label,active);
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int ImGui_RadioButton_sfPi(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 3);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int *v;
    v = static_cast<int *>(luaL_checkudata(L, 2, "ImGuiLua_IntRef"));
    int v_button;
    v_button = static_cast<int>(luaL_checkinteger(L, 3));
    bool RETVAL = ImGui::RadioButton(label,v,v_button);
    lua_pushboolean(L, RETVAL);
    return 1;
}

IMGUILUA_FUNCTION(ImGui::ImGui_RadioButton)
static int ImGui_RadioButton(lua_State *L)
{
    int ARGC = lua_gettop(L);
    switch (ARGC) {
    case 2:
        return ImGui_RadioButton_sb(L);
    case 3:
        return ImGui_RadioButton_sfPi(L);
    default:
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
}


// function ImGui::Render is excluded


// function ImGui::ResetMouseDragDelta

IMGUILUA_FUNCTION(ImGui::ResetMouseDragDelta)
static int ImGui_ResetMouseDragDelta(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int button;
    if (ARGC < 1) {
        button = 0;
    }
    else {
        button = static_cast<int>(luaL_checkinteger(L, 1));
    }
    ImGui::ResetMouseDragDelta(button);
    return 0;
}


// function ImGui::SameLine

IMGUILUA_FUNCTION(ImGui::SameLine)
static int ImGui_SameLine(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 2);
    }
    float offset_from_start_x;
    if (ARGC < 1) {
        offset_from_start_x = 0.0f;
    }
    else {
        offset_from_start_x = static_cast<float>(luaL_checknumber(L, 1));
    }
    float spacing;
    if (ARGC < 2) {
        spacing = -1.0f;
    }
    else {
        spacing = static_cast<float>(luaL_checknumber(L, 2));
    }
    ImGui::SameLine(offset_from_start_x,spacing);
    return 0;
}


// function ImGui::SaveIniSettingsToDisk

IMGUILUA_FUNCTION(ImGui::SaveIniSettingsToDisk)
static int ImGui_SaveIniSettingsToDisk(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *ini_filename;
    ini_filename = lua_tostring(L, 1);
    ImGui::SaveIniSettingsToDisk(ini_filename);
    return 0;
}


// function ImGui::SaveIniSettingsToMemory

// const char* ImGui_SaveIniSettingsToMemory(size_t*): Unknown argument type 'size_t*' (size_t*)


// function ImGui::Selectable

// bool ImGui_Selectable(const char*, bool*, ImGuiSelectableFlags, const ImVec2) is excluded

IMGUILUA_FUNCTION(ImGui::Selectable)
static int ImGui_Selectable(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 4);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool selected;
    if (ARGC < 2) {
        selected = false;
    }
    else {
        selected = lua_toboolean(L, 2);
    }
    int flags;
    if (ARGC < 3) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 3));
    }
    ImVec2 size;
    if (ARGC < 4) {
        size = ImVec2(0,0);
    }
    else {
        size = ImGuiLua_ToImVec2(L, 4);
    }
    bool RETVAL = ImGui::Selectable(label,selected,flags,size);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::Separator

IMGUILUA_FUNCTION(ImGui::Separator)
static int ImGui_Separator(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::Separator();
    return 0;
}


// function ImGui::SetAllocatorFunctions

// void ImGui_SetAllocatorFunctions(ImGuiMemAllocFunc, ImGuiMemFreeFunc, void*): Unknown argument type 'ImGuiMemAllocFunc' (ImGuiMemAllocFunc)


// function ImGui::SetClipboardText

IMGUILUA_FUNCTION(ImGui::SetClipboardText)
static int ImGui_SetClipboardText(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *text;
    text = lua_tostring(L, 1);
    ImGui::SetClipboardText(text);
    return 0;
}


// function ImGui::SetColorEditOptions

IMGUILUA_FUNCTION(ImGui::SetColorEditOptions)
static int ImGui_SetColorEditOptions(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int flags;
    flags = static_cast<int>(luaL_checkinteger(L, 1));
    ImGui::SetColorEditOptions(flags);
    return 0;
}


// function ImGui::SetColumnOffset

IMGUILUA_FUNCTION(ImGui::SetColumnOffset)
static int ImGui_SetColumnOffset(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int column_index;
    column_index = static_cast<int>(luaL_checkinteger(L, 1));
    float offset_x;
    offset_x = static_cast<float>(luaL_checknumber(L, 2));
    ImGui::SetColumnOffset(column_index,offset_x);
    return 0;
}


// function ImGui::SetColumnWidth

IMGUILUA_FUNCTION(ImGui::SetColumnWidth)
static int ImGui_SetColumnWidth(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int column_index;
    column_index = static_cast<int>(luaL_checkinteger(L, 1));
    float width;
    width = static_cast<float>(luaL_checknumber(L, 2));
    ImGui::SetColumnWidth(column_index,width);
    return 0;
}


// function ImGui::SetCurrentContext

// void ImGui_SetCurrentContext(ImGuiContext*): Unknown argument type 'ImGuiContext*' (ImGuiContext*)


// function ImGui::SetCursorPos

IMGUILUA_FUNCTION(ImGui::SetCursorPos)
static int ImGui_SetCursorPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImVec2 local_pos;
    local_pos = ImGuiLua_ToImVec2(L, 1);
    ImGui::SetCursorPos(local_pos);
    return 0;
}


// function ImGui::SetCursorPosX

IMGUILUA_FUNCTION(ImGui::SetCursorPosX)
static int ImGui_SetCursorPosX(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    float local_x;
    local_x = static_cast<float>(luaL_checknumber(L, 1));
    ImGui::SetCursorPosX(local_x);
    return 0;
}


// function ImGui::SetCursorPosY

IMGUILUA_FUNCTION(ImGui::SetCursorPosY)
static int ImGui_SetCursorPosY(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    float local_y;
    local_y = static_cast<float>(luaL_checknumber(L, 1));
    ImGui::SetCursorPosY(local_y);
    return 0;
}


// function ImGui::SetCursorScreenPos

IMGUILUA_FUNCTION(ImGui::SetCursorScreenPos)
static int ImGui_SetCursorScreenPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImVec2 pos;
    pos = ImGuiLua_ToImVec2(L, 1);
    ImGui::SetCursorScreenPos(pos);
    return 0;
}


// function ImGui::SetDragDropPayload

// bool ImGui_SetDragDropPayload(const char*, const void*, size_t, ImGuiCond): Unknown argument type 'const void*' (const void*)


// function ImGui::SetItemAllowOverlap

IMGUILUA_FUNCTION(ImGui::SetItemAllowOverlap)
static int ImGui_SetItemAllowOverlap(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::SetItemAllowOverlap();
    return 0;
}


// function ImGui::SetItemDefaultFocus

IMGUILUA_FUNCTION(ImGui::SetItemDefaultFocus)
static int ImGui_SetItemDefaultFocus(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::SetItemDefaultFocus();
    return 0;
}


// function ImGui::SetKeyboardFocusHere

IMGUILUA_FUNCTION(ImGui::SetKeyboardFocusHere)
static int ImGui_SetKeyboardFocusHere(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int offset;
    if (ARGC < 1) {
        offset = 0;
    }
    else {
        offset = static_cast<int>(luaL_checkinteger(L, 1));
    }
    ImGui::SetKeyboardFocusHere(offset);
    return 0;
}


// function ImGui::SetMouseCursor

IMGUILUA_FUNCTION(ImGui::SetMouseCursor)
static int ImGui_SetMouseCursor(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int cursor_type;
    cursor_type = static_cast<int>(luaL_checkinteger(L, 1));
    ImGui::SetMouseCursor(cursor_type);
    return 0;
}


// function ImGui::SetNextItemOpen

IMGUILUA_FUNCTION(ImGui::SetNextItemOpen)
static int ImGui_SetNextItemOpen(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    bool is_open;
    is_open = lua_toboolean(L, 1);
    int cond;
    if (ARGC < 2) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::SetNextItemOpen(is_open,cond);
    return 0;
}


// function ImGui::SetNextItemWidth

IMGUILUA_FUNCTION(ImGui::SetNextItemWidth)
static int ImGui_SetNextItemWidth(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    float item_width;
    item_width = static_cast<float>(luaL_checknumber(L, 1));
    ImGui::SetNextItemWidth(item_width);
    return 0;
}


// function ImGui::SetNextWindowBgAlpha

IMGUILUA_FUNCTION(ImGui::SetNextWindowBgAlpha)
static int ImGui_SetNextWindowBgAlpha(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    float alpha;
    alpha = static_cast<float>(luaL_checknumber(L, 1));
    ImGui::SetNextWindowBgAlpha(alpha);
    return 0;
}


// function ImGui::SetNextWindowCollapsed

IMGUILUA_FUNCTION(ImGui::SetNextWindowCollapsed)
static int ImGui_SetNextWindowCollapsed(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    bool collapsed;
    collapsed = lua_toboolean(L, 1);
    int cond;
    if (ARGC < 2) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::SetNextWindowCollapsed(collapsed,cond);
    return 0;
}


// function ImGui::SetNextWindowContentSize

IMGUILUA_FUNCTION(ImGui::SetNextWindowContentSize)
static int ImGui_SetNextWindowContentSize(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 1);
    ImGui::SetNextWindowContentSize(size);
    return 0;
}


// function ImGui::SetNextWindowFocus

IMGUILUA_FUNCTION(ImGui::SetNextWindowFocus)
static int ImGui_SetNextWindowFocus(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::SetNextWindowFocus();
    return 0;
}


// function ImGui::SetNextWindowPos

IMGUILUA_FUNCTION(ImGui::SetNextWindowPos)
static int ImGui_SetNextWindowPos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 3);
    }
    ImVec2 pos;
    pos = ImGuiLua_ToImVec2(L, 1);
    int cond;
    if (ARGC < 2) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImVec2 pivot;
    if (ARGC < 3) {
        pivot = ImVec2(0,0);
    }
    else {
        pivot = ImGuiLua_ToImVec2(L, 3);
    }
    ImGui::SetNextWindowPos(pos,cond,pivot);
    return 0;
}


// function ImGui::SetNextWindowSize

IMGUILUA_FUNCTION(ImGui::SetNextWindowSize)
static int ImGui_SetNextWindowSize(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 1);
    int cond;
    if (ARGC < 2) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::SetNextWindowSize(size,cond);
    return 0;
}


// function ImGui::SetNextWindowSizeConstraints

// void ImGui_SetNextWindowSizeConstraints(const ImVec2, const ImVec2, ImGuiSizeCallback, void*): Unknown argument type 'ImGuiSizeCallback' (ImGuiSizeCallback)


// function ImGui::SetScrollFromPosX

IMGUILUA_FUNCTION(ImGui::SetScrollFromPosX)
static int ImGui_SetScrollFromPosX(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    float local_x;
    local_x = static_cast<float>(luaL_checknumber(L, 1));
    float center_x_ratio;
    if (ARGC < 2) {
        center_x_ratio = 0.5f;
    }
    else {
        center_x_ratio = static_cast<float>(luaL_checknumber(L, 2));
    }
    ImGui::SetScrollFromPosX(local_x,center_x_ratio);
    return 0;
}


// function ImGui::SetScrollFromPosY

IMGUILUA_FUNCTION(ImGui::SetScrollFromPosY)
static int ImGui_SetScrollFromPosY(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    float local_y;
    local_y = static_cast<float>(luaL_checknumber(L, 1));
    float center_y_ratio;
    if (ARGC < 2) {
        center_y_ratio = 0.5f;
    }
    else {
        center_y_ratio = static_cast<float>(luaL_checknumber(L, 2));
    }
    ImGui::SetScrollFromPosY(local_y,center_y_ratio);
    return 0;
}


// function ImGui::SetScrollHereX

IMGUILUA_FUNCTION(ImGui::SetScrollHereX)
static int ImGui_SetScrollHereX(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    float center_x_ratio;
    if (ARGC < 1) {
        center_x_ratio = 0.5f;
    }
    else {
        center_x_ratio = static_cast<float>(luaL_checknumber(L, 1));
    }
    ImGui::SetScrollHereX(center_x_ratio);
    return 0;
}


// function ImGui::SetScrollHereY

IMGUILUA_FUNCTION(ImGui::SetScrollHereY)
static int ImGui_SetScrollHereY(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    float center_y_ratio;
    if (ARGC < 1) {
        center_y_ratio = 0.5f;
    }
    else {
        center_y_ratio = static_cast<float>(luaL_checknumber(L, 1));
    }
    ImGui::SetScrollHereY(center_y_ratio);
    return 0;
}


// function ImGui::SetScrollX

IMGUILUA_FUNCTION(ImGui::SetScrollX)
static int ImGui_SetScrollX(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    float scroll_x;
    scroll_x = static_cast<float>(luaL_checknumber(L, 1));
    ImGui::SetScrollX(scroll_x);
    return 0;
}


// function ImGui::SetScrollY

IMGUILUA_FUNCTION(ImGui::SetScrollY)
static int ImGui_SetScrollY(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    float scroll_y;
    scroll_y = static_cast<float>(luaL_checknumber(L, 1));
    ImGui::SetScrollY(scroll_y);
    return 0;
}


// function ImGui::SetStateStorage

// void ImGui_SetStateStorage(ImGuiStorage*): Unknown argument type 'ImGuiStorage*' (ImGuiStorage*)


// function ImGui::SetTabItemClosed

IMGUILUA_FUNCTION(ImGui::SetTabItemClosed)
static int ImGui_SetTabItemClosed(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *tab_or_docked_window_label;
    tab_or_docked_window_label = lua_tostring(L, 1);
    ImGui::SetTabItemClosed(tab_or_docked_window_label);
    return 0;
}


// function ImGui::SetTooltip

// void ImGui_SetTooltip(const char*, ...): Unknown argument type '...' (...)


// function ImGui::SetTooltipV

// void ImGui_SetTooltipV(const char*, va_list): Unknown argument type 'va_list' (va_list)


// function ImGui::SetWindowCollapsed

static int ImGui_SetWindowCollapsed_bi(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    bool collapsed;
    collapsed = lua_toboolean(L, 1);
    int cond;
    if (ARGC < 2) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::SetWindowCollapsed(collapsed,cond);
    return 0;
}

static int ImGui_SetWindowCollapsed_sbi(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    const char *name;
    name = lua_tostring(L, 1);
    bool collapsed;
    collapsed = lua_toboolean(L, 2);
    int cond;
    if (ARGC < 3) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 3));
    }
    ImGui::SetWindowCollapsed(name,collapsed,cond);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::ImGui_SetWindowCollapsed)
static int ImGui_SetWindowCollapsed(lua_State *L)
{
    if (lua_isstring(L, 1)) {
        return ImGui_SetWindowCollapsed_sbi(L);
    }
    else {
        return ImGui_SetWindowCollapsed_bi(L);
    }
}


// function ImGui::SetWindowFocus

static int ImGui_SetWindowFocus_(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::SetWindowFocus();
    return 0;
}

static int ImGui_SetWindowFocus_s(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *name;
    name = lua_tostring(L, 1);
    ImGui::SetWindowFocus(name);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::ImGui_SetWindowFocus)
static int ImGui_SetWindowFocus(lua_State *L)
{
    int ARGC = lua_gettop(L);
    switch (ARGC) {
    case 0:
        return ImGui_SetWindowFocus_(L);
    case 1:
        return ImGui_SetWindowFocus_s(L);
    default:
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
}


// function ImGui::SetWindowFontScale

IMGUILUA_FUNCTION(ImGui::SetWindowFontScale)
static int ImGui_SetWindowFontScale(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    float scale;
    scale = static_cast<float>(luaL_checknumber(L, 1));
    ImGui::SetWindowFontScale(scale);
    return 0;
}


// function ImGui::SetWindowPos

static int ImGui_SetWindowPos_v2i(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    ImVec2 pos;
    pos = ImGuiLua_ToImVec2(L, 1);
    int cond;
    if (ARGC < 2) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::SetWindowPos(pos,cond);
    return 0;
}

static int ImGui_SetWindowPos_sv2i(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    const char *name;
    name = lua_tostring(L, 1);
    ImVec2 pos;
    pos = ImGuiLua_ToImVec2(L, 2);
    int cond;
    if (ARGC < 3) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 3));
    }
    ImGui::SetWindowPos(name,pos,cond);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::ImGui_SetWindowPos)
static int ImGui_SetWindowPos(lua_State *L)
{
    if (lua_isstring(L, 1)) {
        return ImGui_SetWindowPos_sv2i(L);
    }
    else {
        return ImGui_SetWindowPos_v2i(L);
    }
}


// function ImGui::SetWindowSize

static int ImGui_SetWindowSize_v2i(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 1);
    int cond;
    if (ARGC < 2) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImGui::SetWindowSize(size,cond);
    return 0;
}

static int ImGui_SetWindowSize_sv2i(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    const char *name;
    name = lua_tostring(L, 1);
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 2);
    int cond;
    if (ARGC < 3) {
        cond = 0;
    }
    else {
        cond = static_cast<int>(luaL_checkinteger(L, 3));
    }
    ImGui::SetWindowSize(name,size,cond);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::ImGui_SetWindowSize)
static int ImGui_SetWindowSize(lua_State *L)
{
    if (lua_isstring(L, 1)) {
        return ImGui_SetWindowSize_sv2i(L);
    }
    else {
        return ImGui_SetWindowSize_v2i(L);
    }
}


// function ImGui::ShowAboutWindow

IMGUILUA_FUNCTION(ImGui::ShowAboutWindow)
static int ImGui_ShowAboutWindow(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    bool *p_open;
    if (ARGC < 1) {
        p_open = NULL;
    }
    else {
        if (lua_type(L, 1) == LUA_TNIL) {
            p_open = NULL;
        }
        else {
            p_open = static_cast<bool *>(luaL_checkudata(L, 1, "ImGuiLua_BoolRef"));
        }
    }
    ImGui::ShowAboutWindow(p_open);
    return 0;
}


// function ImGui::ShowDemoWindow

IMGUILUA_FUNCTION(ImGui::ShowDemoWindow)
static int ImGui_ShowDemoWindow(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    bool *p_open;
    if (ARGC < 1) {
        p_open = NULL;
    }
    else {
        if (lua_type(L, 1) == LUA_TNIL) {
            p_open = NULL;
        }
        else {
            p_open = static_cast<bool *>(luaL_checkudata(L, 1, "ImGuiLua_BoolRef"));
        }
    }
    ImGui::ShowDemoWindow(p_open);
    return 0;
}


// function ImGui::ShowFontSelector

IMGUILUA_FUNCTION(ImGui::ShowFontSelector)
static int ImGui_ShowFontSelector(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *label;
    label = lua_tostring(L, 1);
    ImGui::ShowFontSelector(label);
    return 0;
}


// function ImGui::ShowMetricsWindow

IMGUILUA_FUNCTION(ImGui::ShowMetricsWindow)
static int ImGui_ShowMetricsWindow(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    bool *p_open;
    if (ARGC < 1) {
        p_open = NULL;
    }
    else {
        if (lua_type(L, 1) == LUA_TNIL) {
            p_open = NULL;
        }
        else {
            p_open = static_cast<bool *>(luaL_checkudata(L, 1, "ImGuiLua_BoolRef"));
        }
    }
    ImGui::ShowMetricsWindow(p_open);
    return 0;
}


// function ImGui::ShowStyleEditor

// void ImGui_ShowStyleEditor(ImGuiStyle*): Unknown argument type 'ImGuiStyle*' (ImGuiStyle*)


// function ImGui::ShowStyleSelector

IMGUILUA_FUNCTION(ImGui::ShowStyleSelector)
static int ImGui_ShowStyleSelector(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool RETVAL = ImGui::ShowStyleSelector(label);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::ShowUserGuide

IMGUILUA_FUNCTION(ImGui::ShowUserGuide)
static int ImGui_ShowUserGuide(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::ShowUserGuide();
    return 0;
}


// function ImGui::SliderAngle

IMGUILUA_FUNCTION(ImGui::SliderAngle)
static int ImGui_SliderAngle(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 6) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 6);
    }
    const char *label;
    label = lua_tostring(L, 1);
    float *v_rad;
    v_rad = static_cast<float *>(luaL_checkudata(L, 2, "ImGuiLua_FloatRef"));
    float v_degrees_min;
    if (ARGC < 3) {
        v_degrees_min = -360.0f;
    }
    else {
        v_degrees_min = static_cast<float>(luaL_checknumber(L, 3));
    }
    float v_degrees_max;
    if (ARGC < 4) {
        v_degrees_max = +360.0f;
    }
    else {
        v_degrees_max = static_cast<float>(luaL_checknumber(L, 4));
    }
    const char *format;
    if (ARGC < 5) {
        format = "%.0f deg";
    }
    else {
        format = lua_tostring(L, 5);
    }
    int flags;
    if (ARGC < 6) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 6));
    }
    bool RETVAL = ImGui::SliderAngle(label,v_rad,v_degrees_min,v_degrees_max,format,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::SliderFloat

IMGUILUA_FUNCTION(ImGui::SliderFloat)
static int ImGui_SliderFloat(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 4 || ARGC > 6) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 4, 6);
    }
    const char *label;
    label = lua_tostring(L, 1);
    float *v;
    v = static_cast<float *>(luaL_checkudata(L, 2, "ImGuiLua_FloatRef"));
    float v_min;
    v_min = static_cast<float>(luaL_checknumber(L, 3));
    float v_max;
    v_max = static_cast<float>(luaL_checknumber(L, 4));
    const char *format;
    if (ARGC < 5) {
        format = "%.3f";
    }
    else {
        format = lua_tostring(L, 5);
    }
    int flags;
    if (ARGC < 6) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 6));
    }
    bool RETVAL = ImGui::SliderFloat(label,v,v_min,v_max,format,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::SliderFloat2

// bool ImGui_SliderFloat2(const char*, float[2], float, float, const char*, ImGuiSliderFlags): Unknown argument type 'float[2]' (float[2])


// function ImGui::SliderFloat3

// bool ImGui_SliderFloat3(const char*, float[3], float, float, const char*, ImGuiSliderFlags): Unknown argument type 'float[3]' (float[3])


// function ImGui::SliderFloat4

// bool ImGui_SliderFloat4(const char*, float[4], float, float, const char*, ImGuiSliderFlags): Unknown argument type 'float[4]' (float[4])


// function ImGui::SliderInt

IMGUILUA_FUNCTION(ImGui::SliderInt)
static int ImGui_SliderInt(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 4 || ARGC > 6) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 4, 6);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int *v;
    v = static_cast<int *>(luaL_checkudata(L, 2, "ImGuiLua_IntRef"));
    int v_min;
    v_min = static_cast<int>(luaL_checkinteger(L, 3));
    int v_max;
    v_max = static_cast<int>(luaL_checkinteger(L, 4));
    const char *format;
    if (ARGC < 5) {
        format = "%d";
    }
    else {
        format = lua_tostring(L, 5);
    }
    int flags;
    if (ARGC < 6) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 6));
    }
    bool RETVAL = ImGui::SliderInt(label,v,v_min,v_max,format,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::SliderInt2

// bool ImGui_SliderInt2(const char*, int[2], int, int, const char*, ImGuiSliderFlags): Unknown argument type 'int[2]' (int[2])


// function ImGui::SliderInt3

// bool ImGui_SliderInt3(const char*, int[3], int, int, const char*, ImGuiSliderFlags): Unknown argument type 'int[3]' (int[3])


// function ImGui::SliderInt4

// bool ImGui_SliderInt4(const char*, int[4], int, int, const char*, ImGuiSliderFlags): Unknown argument type 'int[4]' (int[4])


// function ImGui::SliderScalar

// bool ImGui_SliderScalar(const char*, ImGuiDataType, void*, const void*, const void*, const char*, ImGuiSliderFlags): Unknown argument type 'void*' (void*)


// function ImGui::SliderScalarN

// bool ImGui_SliderScalarN(const char*, ImGuiDataType, void*, int, const void*, const void*, const char*, ImGuiSliderFlags): Unknown argument type 'void*' (void*)


// function ImGui::SmallButton

IMGUILUA_FUNCTION(ImGui::SmallButton)
static int ImGui_SmallButton(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool RETVAL = ImGui::SmallButton(label);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::Spacing

IMGUILUA_FUNCTION(ImGui::Spacing)
static int ImGui_Spacing(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::Spacing();
    return 0;
}


// function ImGui::StyleColorsClassic

// void ImGui_StyleColorsClassic(ImGuiStyle*): Unknown argument type 'ImGuiStyle*' (ImGuiStyle*)


// function ImGui::StyleColorsDark

// void ImGui_StyleColorsDark(ImGuiStyle*): Unknown argument type 'ImGuiStyle*' (ImGuiStyle*)


// function ImGui::StyleColorsLight

// void ImGui_StyleColorsLight(ImGuiStyle*): Unknown argument type 'ImGuiStyle*' (ImGuiStyle*)


// function ImGui::TabItemButton

IMGUILUA_FUNCTION(ImGui::TabItemButton)
static int ImGui_TabItemButton(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int flags;
    if (ARGC < 2) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::TabItemButton(label,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::TableGetColumnCount

IMGUILUA_FUNCTION(ImGui::TableGetColumnCount)
static int ImGui_TableGetColumnCount(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImGui::TableGetColumnCount();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::TableGetColumnFlags

IMGUILUA_FUNCTION(ImGui::TableGetColumnFlags)
static int ImGui_TableGetColumnFlags(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int column_n;
    if (ARGC < 1) {
        column_n = -1;
    }
    else {
        column_n = static_cast<int>(luaL_checkinteger(L, 1));
    }
    int RETVAL = ImGui::TableGetColumnFlags(column_n);
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::TableGetColumnIndex

IMGUILUA_FUNCTION(ImGui::TableGetColumnIndex)
static int ImGui_TableGetColumnIndex(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImGui::TableGetColumnIndex();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::TableGetColumnName

IMGUILUA_FUNCTION(ImGui::TableGetColumnName)
static int ImGui_TableGetColumnName(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    int column_n;
    if (ARGC < 1) {
        column_n = -1;
    }
    else {
        column_n = static_cast<int>(luaL_checkinteger(L, 1));
    }
    const char *RETVAL = ImGui::TableGetColumnName(column_n);
    lua_pushstring(L, RETVAL);
    return 1;
}


// function ImGui::TableGetRowIndex

IMGUILUA_FUNCTION(ImGui::TableGetRowIndex)
static int ImGui_TableGetRowIndex(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImGui::TableGetRowIndex();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImGui::TableGetSortSpecs

// ImGuiTableSortSpecs* ImGui_TableGetSortSpecs(): Unknown return type 'ImGuiTableSortSpecs*' (ImGuiTableSortSpecs*)


// function ImGui::TableHeader

IMGUILUA_FUNCTION(ImGui::TableHeader)
static int ImGui_TableHeader(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *label;
    label = lua_tostring(L, 1);
    ImGui::TableHeader(label);
    return 0;
}


// function ImGui::TableHeadersRow

IMGUILUA_FUNCTION(ImGui::TableHeadersRow)
static int ImGui_TableHeadersRow(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::TableHeadersRow();
    return 0;
}


// function ImGui::TableNextColumn

IMGUILUA_FUNCTION(ImGui::TableNextColumn)
static int ImGui_TableNextColumn(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImGui::TableNextColumn();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::TableNextRow

IMGUILUA_FUNCTION(ImGui::TableNextRow)
static int ImGui_TableNextRow(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 2);
    }
    int row_flags;
    if (ARGC < 1) {
        row_flags = 0;
    }
    else {
        row_flags = static_cast<int>(luaL_checkinteger(L, 1));
    }
    float min_row_height;
    if (ARGC < 2) {
        min_row_height = 0.0f;
    }
    else {
        min_row_height = static_cast<float>(luaL_checknumber(L, 2));
    }
    ImGui::TableNextRow(row_flags,min_row_height);
    return 0;
}


// function ImGui::TableSetBgColor

IMGUILUA_FUNCTION(ImGui::TableSetBgColor)
static int ImGui_TableSetBgColor(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 2 || ARGC > 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 2, 3);
    }
    int target;
    target = static_cast<int>(luaL_checkinteger(L, 1));
    ImU32 color;
    color = static_cast<ImU32>(luaL_checkinteger(L, 2));
    int column_n;
    if (ARGC < 3) {
        column_n = -1;
    }
    else {
        column_n = static_cast<int>(luaL_checkinteger(L, 3));
    }
    ImGui::TableSetBgColor(target,color,column_n);
    return 0;
}


// function ImGui::TableSetColumnEnabled

IMGUILUA_FUNCTION(ImGui::TableSetColumnEnabled)
static int ImGui_TableSetColumnEnabled(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int column_n;
    column_n = static_cast<int>(luaL_checkinteger(L, 1));
    bool v;
    v = lua_toboolean(L, 2);
    ImGui::TableSetColumnEnabled(column_n,v);
    return 0;
}


// function ImGui::TableSetColumnIndex

IMGUILUA_FUNCTION(ImGui::TableSetColumnIndex)
static int ImGui_TableSetColumnIndex(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int column_n;
    column_n = static_cast<int>(luaL_checkinteger(L, 1));
    bool RETVAL = ImGui::TableSetColumnIndex(column_n);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::TableSetupColumn

IMGUILUA_FUNCTION(ImGui::TableSetupColumn)
static int ImGui_TableSetupColumn(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 4) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 4);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int flags;
    if (ARGC < 2) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    float init_width_or_weight;
    if (ARGC < 3) {
        init_width_or_weight = 0.0f;
    }
    else {
        init_width_or_weight = static_cast<float>(luaL_checknumber(L, 3));
    }
    unsigned int user_id;
    if (ARGC < 4) {
        user_id = 0;
    }
    else {
        user_id = static_cast<unsigned int>(luaL_checkinteger(L, 4));
    }
    ImGui::TableSetupColumn(label,flags,init_width_or_weight,user_id);
    return 0;
}


// function ImGui::TableSetupScrollFreeze

IMGUILUA_FUNCTION(ImGui::TableSetupScrollFreeze)
static int ImGui_TableSetupScrollFreeze(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int cols;
    cols = static_cast<int>(luaL_checkinteger(L, 1));
    int rows;
    rows = static_cast<int>(luaL_checkinteger(L, 2));
    ImGui::TableSetupScrollFreeze(cols,rows);
    return 0;
}


// function ImGui::Text is excluded


// function ImGui::TextColored is excluded


// function ImGui::TextColoredV is excluded


// function ImGui::TextDisabled is excluded


// function ImGui::TextDisabledV is excluded


// function ImGui::TextUnformatted is excluded


// function ImGui::TextV is excluded


// function ImGui::TextWrapped is excluded


// function ImGui::TextWrappedV is excluded


// function ImGui::TreeNode

// bool ImGui_TreeNode(const char*, const char*, ...): Unknown argument type '...' (...)

// bool ImGui_TreeNode(const void*, const char*, ...): Unknown argument type 'const void*' (const void*)

IMGUILUA_FUNCTION(ImGui::TreeNode)
static int ImGui_TreeNode(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *label;
    label = lua_tostring(L, 1);
    bool RETVAL = ImGui::TreeNode(label);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::TreeNodeEx

// bool ImGui_TreeNodeEx(const char*, ImGuiTreeNodeFlags, const char*, ...): Unknown argument type '...' (...)

// bool ImGui_TreeNodeEx(const void*, ImGuiTreeNodeFlags, const char*, ...): Unknown argument type 'const void*' (const void*)

IMGUILUA_FUNCTION(ImGui::TreeNodeEx)
static int ImGui_TreeNodeEx(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    const char *label;
    label = lua_tostring(L, 1);
    int flags;
    if (ARGC < 2) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 2));
    }
    bool RETVAL = ImGui::TreeNodeEx(label,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::TreeNodeExV

// bool ImGui_TreeNodeExV(const char*, ImGuiTreeNodeFlags, const char*, va_list): Unknown argument type 'va_list' (va_list)

// bool ImGui_TreeNodeExV(const void*, ImGuiTreeNodeFlags, const char*, va_list): Unknown argument type 'const void*' (const void*)


// function ImGui::TreeNodeV

// bool ImGui_TreeNodeV(const char*, const char*, va_list): Unknown argument type 'va_list' (va_list)

// bool ImGui_TreeNodeV(const void*, const char*, va_list): Unknown argument type 'const void*' (const void*)


// function ImGui::TreePop

IMGUILUA_FUNCTION(ImGui::TreePop)
static int ImGui_TreePop(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImGui::TreePop();
    return 0;
}


// function ImGui::TreePush

// void ImGui_TreePush(const void*): Unknown argument type 'const void*' (const void*)

IMGUILUA_FUNCTION(ImGui::TreePush)
static int ImGui_TreePush(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *str_id;
    str_id = lua_tostring(L, 1);
    ImGui::TreePush(str_id);
    return 0;
}


// function ImGui::Unindent

IMGUILUA_FUNCTION(ImGui::Unindent)
static int ImGui_Unindent(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
    float indent_w;
    if (ARGC < 1) {
        indent_w = 0.0f;
    }
    else {
        indent_w = static_cast<float>(luaL_checknumber(L, 1));
    }
    ImGui::Unindent(indent_w);
    return 0;
}


// function ImGui::VSliderFloat

IMGUILUA_FUNCTION(ImGui::VSliderFloat)
static int ImGui_VSliderFloat(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 5 || ARGC > 7) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 5, 7);
    }
    const char *label;
    label = lua_tostring(L, 1);
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 2);
    float *v;
    v = static_cast<float *>(luaL_checkudata(L, 3, "ImGuiLua_FloatRef"));
    float v_min;
    v_min = static_cast<float>(luaL_checknumber(L, 4));
    float v_max;
    v_max = static_cast<float>(luaL_checknumber(L, 5));
    const char *format;
    if (ARGC < 6) {
        format = "%.3f";
    }
    else {
        format = lua_tostring(L, 6);
    }
    int flags;
    if (ARGC < 7) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 7));
    }
    bool RETVAL = ImGui::VSliderFloat(label,size,v,v_min,v_max,format,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::VSliderInt

IMGUILUA_FUNCTION(ImGui::VSliderInt)
static int ImGui_VSliderInt(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 5 || ARGC > 7) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 5, 7);
    }
    const char *label;
    label = lua_tostring(L, 1);
    ImVec2 size;
    size = ImGuiLua_ToImVec2(L, 2);
    int *v;
    v = static_cast<int *>(luaL_checkudata(L, 3, "ImGuiLua_IntRef"));
    int v_min;
    v_min = static_cast<int>(luaL_checkinteger(L, 4));
    int v_max;
    v_max = static_cast<int>(luaL_checkinteger(L, 5));
    const char *format;
    if (ARGC < 6) {
        format = "%d";
    }
    else {
        format = lua_tostring(L, 6);
    }
    int flags;
    if (ARGC < 7) {
        flags = 0;
    }
    else {
        flags = static_cast<int>(luaL_checkinteger(L, 7));
    }
    bool RETVAL = ImGui::VSliderInt(label,size,v,v_min,v_max,format,flags);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImGui::VSliderScalar

// bool ImGui_VSliderScalar(const char*, const ImVec2, ImGuiDataType, void*, const void*, const void*, const char*, ImGuiSliderFlags): Unknown argument type 'void*' (void*)


// function ImGui::Value is excluded


IMGUILUA_FUNCTION(ImGui::PushID)
static int ImGui_PushID(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    if (lua_type(L, 1) == LUA_TNUMBER) {
        int int_id = static_cast<int>(luaL_checkinteger(L, 1));
        ImGui::PushID(int_id);
    }
    else {
        size_t len;
        const char *str_id = luaL_checklstring(L, 1, &len);
        ImGui::PushID(str_id, str_id + len);
    }
    return 0;
}


IMGUILUA_FUNCTION(ImGui::GetID)
static int ImGui_GetID(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    size_t len;
    const char *str_id = luaL_checklstring(L, 1, &len);
    ImGuiID RETVAL = ImGui::GetID(str_id, str_id + len);
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


IMGUILUA_FUNCTION(ImGui::Text)
static int ImGui_Text(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    size_t len;
    const char *text = luaL_checklstring(L, 1, &len);
    ImGui::TextUnformatted(text, text + len);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::TextColored)
static int ImGui_TextColored(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    ImVec4 col = ImGuiLua_ToImVec4(L, 1);
    const char *text = luaL_checkstring(L, 2);
    ImGui::TextColored(col, "%s", text);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::TextDisabled)
static int ImGui_TextDisabled(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *text = luaL_checkstring(L, 1);
    ImGui::TextDisabled("%s", text);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::TextWrapped)
static int ImGui_TextWrapped(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *text = luaL_checkstring(L, 1);
    ImGui::TextWrapped("%s", text);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::LabelText)
static int ImGui_LabelText(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    const char *label = luaL_checkstring(L, 1);
    const char *text = luaL_checkstring(L, 2);
    ImGui::LabelText(label, "%s", text);
    return 0;
}

IMGUILUA_FUNCTION(ImGui::BulletText)
static int ImGui_BulletText(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *text = luaL_checkstring(L, 1);
    ImGui::BulletText("%s", text);
    return 0;
}


static luaL_Reg ImGuiLua_RegImGui[] = {
    {"bool", ImGuiLua_Bool},
    {"float", ImGuiLua_Float},
    {"int", ImGuiLua_Int},
    {"align_text_to_frame_padding", ImGui_AlignTextToFramePadding},
    {"arrow_button", ImGui_ArrowButton},
    {"begin", ImGui_Begin},
    {"im_gui_begin_child", ImGui_BeginChild},
    {"begin_child_frame", ImGui_BeginChildFrame},
    {"begin_combo", ImGui_BeginCombo},
    {"begin_drag_drop_source", ImGui_BeginDragDropSource},
    {"begin_drag_drop_target", ImGui_BeginDragDropTarget},
    {"begin_group", ImGui_BeginGroup},
    {"begin_list_box", ImGui_BeginListBox},
    {"begin_main_menu_bar", ImGui_BeginMainMenuBar},
    {"begin_menu", ImGui_BeginMenu},
    {"begin_menu_bar", ImGui_BeginMenuBar},
    {"begin_popup", ImGui_BeginPopup},
    {"begin_popup_context_item", ImGui_BeginPopupContextItem},
    {"begin_popup_context_void", ImGui_BeginPopupContextVoid},
    {"begin_popup_context_window", ImGui_BeginPopupContextWindow},
    {"begin_popup_modal", ImGui_BeginPopupModal},
    {"begin_tab_bar", ImGui_BeginTabBar},
    {"begin_tab_item", ImGui_BeginTabItem},
    {"begin_table", ImGui_BeginTable},
    {"begin_tooltip", ImGui_BeginTooltip},
    {"bullet", ImGui_Bullet},
    {"bullet_text", ImGui_BulletText},
    {"button", ImGui_Button},
    {"calc_item_width", ImGui_CalcItemWidth},
    {"calc_list_clipping", ImGui_CalcListClipping},
    {"calc_text_size", ImGui_CalcTextSize},
    {"capture_keyboard_from_app", ImGui_CaptureKeyboardFromApp},
    {"capture_mouse_from_app", ImGui_CaptureMouseFromApp},
    {"checkbox", ImGui_Checkbox},
    {"checkbox_flags", ImGui_CheckboxFlags},
    {"close_current_popup", ImGui_CloseCurrentPopup},
    {"im_gui_collapsing_header", ImGui_CollapsingHeader},
    {"color_button", ImGui_ColorButton},
    {"color_convert_float4to_u32", ImGui_ColorConvertFloat4ToU32},
    {"color_convert_hsvto_rgb", ImGui_ColorConvertHSVtoRGB},
    {"color_convert_rgbto_hsv", ImGui_ColorConvertRGBtoHSV},
    {"color_convert_u32to_float4", ImGui_ColorConvertU32ToFloat4},
    {"columns", ImGui_Columns},
    {"combo", ImGui_Combo},
    {"drag_float", ImGui_DragFloat},
    {"drag_float_range2", ImGui_DragFloatRange2},
    {"drag_int", ImGui_DragInt},
    {"drag_int_range2", ImGui_DragIntRange2},
    {"dummy", ImGui_Dummy},
    {"end", ImGui_End},
    {"end_child", ImGui_EndChild},
    {"end_child_frame", ImGui_EndChildFrame},
    {"end_combo", ImGui_EndCombo},
    {"end_drag_drop_source", ImGui_EndDragDropSource},
    {"end_drag_drop_target", ImGui_EndDragDropTarget},
    {"end_group", ImGui_EndGroup},
    {"end_list_box", ImGui_EndListBox},
    {"end_main_menu_bar", ImGui_EndMainMenuBar},
    {"end_menu", ImGui_EndMenu},
    {"end_menu_bar", ImGui_EndMenuBar},
    {"end_popup", ImGui_EndPopup},
    {"end_tab_bar", ImGui_EndTabBar},
    {"end_tab_item", ImGui_EndTabItem},
    {"end_table", ImGui_EndTable},
    {"end_tooltip", ImGui_EndTooltip},
    {"get_clipboard_text", ImGui_GetClipboardText},
    {"im_gui_get_color_u32", ImGui_GetColorU32},
    {"get_column_index", ImGui_GetColumnIndex},
    {"get_column_offset", ImGui_GetColumnOffset},
    {"get_column_width", ImGui_GetColumnWidth},
    {"get_columns_count", ImGui_GetColumnsCount},
    {"get_content_region_avail", ImGui_GetContentRegionAvail},
    {"get_content_region_max", ImGui_GetContentRegionMax},
    {"get_cursor_pos", ImGui_GetCursorPos},
    {"get_cursor_pos_x", ImGui_GetCursorPosX},
    {"get_cursor_pos_y", ImGui_GetCursorPosY},
    {"get_cursor_screen_pos", ImGui_GetCursorScreenPos},
    {"get_cursor_start_pos", ImGui_GetCursorStartPos},
    {"get_font_size", ImGui_GetFontSize},
    {"get_font_tex_uv_white_pixel", ImGui_GetFontTexUvWhitePixel},
    {"get_frame_count", ImGui_GetFrameCount},
    {"get_frame_height", ImGui_GetFrameHeight},
    {"get_frame_height_with_spacing", ImGui_GetFrameHeightWithSpacing},
    {"get_id", ImGui_GetID},
    {"get_item_rect_max", ImGui_GetItemRectMax},
    {"get_item_rect_min", ImGui_GetItemRectMin},
    {"get_item_rect_size", ImGui_GetItemRectSize},
    {"get_key_index", ImGui_GetKeyIndex},
    {"get_key_pressed_amount", ImGui_GetKeyPressedAmount},
    {"get_mouse_cursor", ImGui_GetMouseCursor},
    {"get_mouse_drag_delta", ImGui_GetMouseDragDelta},
    {"get_mouse_pos", ImGui_GetMousePos},
    {"get_mouse_pos_on_opening_current_popup", ImGui_GetMousePosOnOpeningCurrentPopup},
    {"get_scroll_max_x", ImGui_GetScrollMaxX},
    {"get_scroll_max_y", ImGui_GetScrollMaxY},
    {"get_scroll_x", ImGui_GetScrollX},
    {"get_scroll_y", ImGui_GetScrollY},
    {"get_style_color_name", ImGui_GetStyleColorName},
    {"get_text_line_height", ImGui_GetTextLineHeight},
    {"get_text_line_height_with_spacing", ImGui_GetTextLineHeightWithSpacing},
    {"get_time", ImGui_GetTime},
    {"get_tree_node_to_label_spacing", ImGui_GetTreeNodeToLabelSpacing},
    {"get_version", ImGui_GetVersion},
    {"get_window_content_region_max", ImGui_GetWindowContentRegionMax},
    {"get_window_content_region_min", ImGui_GetWindowContentRegionMin},
    {"get_window_content_region_width", ImGui_GetWindowContentRegionWidth},
    {"get_window_height", ImGui_GetWindowHeight},
    {"get_window_pos", ImGui_GetWindowPos},
    {"get_window_size", ImGui_GetWindowSize},
    {"get_window_width", ImGui_GetWindowWidth},
    {"indent", ImGui_Indent},
    {"input_float", ImGui_InputFloat},
    {"input_int", ImGui_InputInt},
    {"invisible_button", ImGui_InvisibleButton},
    {"is_any_item_active", ImGui_IsAnyItemActive},
    {"is_any_item_focused", ImGui_IsAnyItemFocused},
    {"is_any_item_hovered", ImGui_IsAnyItemHovered},
    {"is_any_mouse_down", ImGui_IsAnyMouseDown},
    {"is_item_activated", ImGui_IsItemActivated},
    {"is_item_active", ImGui_IsItemActive},
    {"is_item_clicked", ImGui_IsItemClicked},
    {"is_item_deactivated", ImGui_IsItemDeactivated},
    {"is_item_deactivated_after_edit", ImGui_IsItemDeactivatedAfterEdit},
    {"is_item_edited", ImGui_IsItemEdited},
    {"is_item_focused", ImGui_IsItemFocused},
    {"is_item_hovered", ImGui_IsItemHovered},
    {"is_item_toggled_open", ImGui_IsItemToggledOpen},
    {"is_item_visible", ImGui_IsItemVisible},
    {"is_key_down", ImGui_IsKeyDown},
    {"is_key_pressed", ImGui_IsKeyPressed},
    {"is_key_released", ImGui_IsKeyReleased},
    {"is_mouse_clicked", ImGui_IsMouseClicked},
    {"is_mouse_double_clicked", ImGui_IsMouseDoubleClicked},
    {"is_mouse_down", ImGui_IsMouseDown},
    {"is_mouse_dragging", ImGui_IsMouseDragging},
    {"is_mouse_hovering_rect", ImGui_IsMouseHoveringRect},
    {"is_mouse_released", ImGui_IsMouseReleased},
    {"is_popup_open", ImGui_IsPopupOpen},
    {"im_gui_is_rect_visible", ImGui_IsRectVisible},
    {"is_window_appearing", ImGui_IsWindowAppearing},
    {"is_window_collapsed", ImGui_IsWindowCollapsed},
    {"is_window_focused", ImGui_IsWindowFocused},
    {"is_window_hovered", ImGui_IsWindowHovered},
    {"label_text", ImGui_LabelText},
    {"load_ini_settings_from_disk", ImGui_LoadIniSettingsFromDisk},
    {"log_buttons", ImGui_LogButtons},
    {"log_finish", ImGui_LogFinish},
    {"log_to_clipboard", ImGui_LogToClipboard},
    {"log_to_file", ImGui_LogToFile},
    {"log_to_tty", ImGui_LogToTTY},
    {"menu_item", ImGui_MenuItem},
    {"new_line", ImGui_NewLine},
    {"next_column", ImGui_NextColumn},
    {"im_gui_open_popup", ImGui_OpenPopup},
    {"open_popup_on_item_click", ImGui_OpenPopupOnItemClick},
    {"pop_allow_keyboard_focus", ImGui_PopAllowKeyboardFocus},
    {"pop_button_repeat", ImGui_PopButtonRepeat},
    {"pop_clip_rect", ImGui_PopClipRect},
    {"pop_font", ImGui_PopFont},
    {"pop_id", ImGui_PopID},
    {"pop_item_width", ImGui_PopItemWidth},
    {"pop_style_color", ImGui_PopStyleColor},
    {"pop_style_var", ImGui_PopStyleVar},
    {"pop_text_wrap_pos", ImGui_PopTextWrapPos},
    {"progress_bar", ImGui_ProgressBar},
    {"push_allow_keyboard_focus", ImGui_PushAllowKeyboardFocus},
    {"push_button_repeat", ImGui_PushButtonRepeat},
    {"push_clip_rect", ImGui_PushClipRect},
    {"push_id", ImGui_PushID},
    {"push_item_width", ImGui_PushItemWidth},
    {"im_gui_push_style_color", ImGui_PushStyleColor},
    {"im_gui_push_style_var", ImGui_PushStyleVar},
    {"push_text_wrap_pos", ImGui_PushTextWrapPos},
    {"im_gui_radio_button", ImGui_RadioButton},
    {"reset_mouse_drag_delta", ImGui_ResetMouseDragDelta},
    {"same_line", ImGui_SameLine},
    {"save_ini_settings_to_disk", ImGui_SaveIniSettingsToDisk},
    {"selectable", ImGui_Selectable},
    {"separator", ImGui_Separator},
    {"set_clipboard_text", ImGui_SetClipboardText},
    {"set_color_edit_options", ImGui_SetColorEditOptions},
    {"set_column_offset", ImGui_SetColumnOffset},
    {"set_column_width", ImGui_SetColumnWidth},
    {"set_cursor_pos", ImGui_SetCursorPos},
    {"set_cursor_pos_x", ImGui_SetCursorPosX},
    {"set_cursor_pos_y", ImGui_SetCursorPosY},
    {"set_cursor_screen_pos", ImGui_SetCursorScreenPos},
    {"set_item_allow_overlap", ImGui_SetItemAllowOverlap},
    {"set_item_default_focus", ImGui_SetItemDefaultFocus},
    {"set_keyboard_focus_here", ImGui_SetKeyboardFocusHere},
    {"set_mouse_cursor", ImGui_SetMouseCursor},
    {"set_next_item_open", ImGui_SetNextItemOpen},
    {"set_next_item_width", ImGui_SetNextItemWidth},
    {"set_next_window_bg_alpha", ImGui_SetNextWindowBgAlpha},
    {"set_next_window_collapsed", ImGui_SetNextWindowCollapsed},
    {"set_next_window_content_size", ImGui_SetNextWindowContentSize},
    {"set_next_window_focus", ImGui_SetNextWindowFocus},
    {"set_next_window_pos", ImGui_SetNextWindowPos},
    {"set_next_window_size", ImGui_SetNextWindowSize},
    {"set_scroll_from_pos_x", ImGui_SetScrollFromPosX},
    {"set_scroll_from_pos_y", ImGui_SetScrollFromPosY},
    {"set_scroll_here_x", ImGui_SetScrollHereX},
    {"set_scroll_here_y", ImGui_SetScrollHereY},
    {"set_scroll_x", ImGui_SetScrollX},
    {"set_scroll_y", ImGui_SetScrollY},
    {"set_tab_item_closed", ImGui_SetTabItemClosed},
    {"im_gui_set_window_collapsed", ImGui_SetWindowCollapsed},
    {"im_gui_set_window_focus", ImGui_SetWindowFocus},
    {"set_window_font_scale", ImGui_SetWindowFontScale},
    {"im_gui_set_window_pos", ImGui_SetWindowPos},
    {"im_gui_set_window_size", ImGui_SetWindowSize},
    {"show_about_window", ImGui_ShowAboutWindow},
    {"show_demo_window", ImGui_ShowDemoWindow},
    {"show_font_selector", ImGui_ShowFontSelector},
    {"show_metrics_window", ImGui_ShowMetricsWindow},
    {"show_style_selector", ImGui_ShowStyleSelector},
    {"show_user_guide", ImGui_ShowUserGuide},
    {"slider_angle", ImGui_SliderAngle},
    {"slider_float", ImGui_SliderFloat},
    {"slider_int", ImGui_SliderInt},
    {"small_button", ImGui_SmallButton},
    {"spacing", ImGui_Spacing},
    {"tab_item_button", ImGui_TabItemButton},
    {"table_get_column_count", ImGui_TableGetColumnCount},
    {"table_get_column_flags", ImGui_TableGetColumnFlags},
    {"table_get_column_index", ImGui_TableGetColumnIndex},
    {"table_get_column_name", ImGui_TableGetColumnName},
    {"table_get_row_index", ImGui_TableGetRowIndex},
    {"table_header", ImGui_TableHeader},
    {"table_headers_row", ImGui_TableHeadersRow},
    {"table_next_column", ImGui_TableNextColumn},
    {"table_next_row", ImGui_TableNextRow},
    {"table_set_bg_color", ImGui_TableSetBgColor},
    {"table_set_column_enabled", ImGui_TableSetColumnEnabled},
    {"table_set_column_index", ImGui_TableSetColumnIndex},
    {"table_setup_column", ImGui_TableSetupColumn},
    {"table_setup_scroll_freeze", ImGui_TableSetupScrollFreeze},
    {"text", ImGui_Text},
    {"text_colored", ImGui_TextColored},
    {"text_disabled", ImGui_TextDisabled},
    {"text_wrapped", ImGui_TextWrapped},
    {"tree_node", ImGui_TreeNode},
    {"tree_node_ex", ImGui_TreeNodeEx},
    {"tree_pop", ImGui_TreePop},
    {"tree_push", ImGui_TreePush},
    {"unindent", ImGui_Unindent},
    {"vslider_float", ImGui_VSliderFloat},
    {"vslider_int", ImGui_VSliderInt},
    {NULL, NULL},
};

extern "C" int ImGuiLua_InitImGui(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    IMGUILUA_SETUP(L);
    ImGuiLua_InitTypes(L);
    lua_pushvalue(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImDrawFlags");
    ImGuiLua_RegisterEnumImDrawFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImDrawListFlags");
    ImGuiLua_RegisterEnumImDrawListFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImFontAtlasFlags");
    ImGuiLua_RegisterEnumImFontAtlasFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiBackendFlags");
    ImGuiLua_RegisterEnumImGuiBackendFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiButtonFlags");
    ImGuiLua_RegisterEnumImGuiButtonFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiCol");
    ImGuiLua_RegisterEnumImGuiCol(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiColorEditFlags");
    ImGuiLua_RegisterEnumImGuiColorEditFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiComboFlags");
    ImGuiLua_RegisterEnumImGuiComboFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiCond");
    ImGuiLua_RegisterEnumImGuiCond(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiConfigFlags");
    ImGuiLua_RegisterEnumImGuiConfigFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiDataType");
    ImGuiLua_RegisterEnumImGuiDataType(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiDir");
    ImGuiLua_RegisterEnumImGuiDir(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiDragDropFlags");
    ImGuiLua_RegisterEnumImGuiDragDropFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiFocusedFlags");
    ImGuiLua_RegisterEnumImGuiFocusedFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiHoveredFlags");
    ImGuiLua_RegisterEnumImGuiHoveredFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiInputTextFlags");
    ImGuiLua_RegisterEnumImGuiInputTextFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiKeyModFlags");
    ImGuiLua_RegisterEnumImGuiKeyModFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiKey");
    ImGuiLua_RegisterEnumImGuiKey(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiMouseButton");
    ImGuiLua_RegisterEnumImGuiMouseButton(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiMouseCursor");
    ImGuiLua_RegisterEnumImGuiMouseCursor(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiNavInput");
    ImGuiLua_RegisterEnumImGuiNavInput(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiPopupFlags");
    ImGuiLua_RegisterEnumImGuiPopupFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiSelectableFlags");
    ImGuiLua_RegisterEnumImGuiSelectableFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiSliderFlags");
    ImGuiLua_RegisterEnumImGuiSliderFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiSortDirection");
    ImGuiLua_RegisterEnumImGuiSortDirection(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiStyleVar");
    ImGuiLua_RegisterEnumImGuiStyleVar(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiTabBarFlags");
    ImGuiLua_RegisterEnumImGuiTabBarFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiTabItemFlags");
    ImGuiLua_RegisterEnumImGuiTabItemFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiTableBgTarget");
    ImGuiLua_RegisterEnumImGuiTableBgTarget(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiTableColumnFlags");
    ImGuiLua_RegisterEnumImGuiTableColumnFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiTableFlags");
    ImGuiLua_RegisterEnumImGuiTableFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiTableRowFlags");
    ImGuiLua_RegisterEnumImGuiTableRowFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiTreeNodeFlags");
    ImGuiLua_RegisterEnumImGuiTreeNodeFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiViewportFlags");
    ImGuiLua_RegisterEnumImGuiViewportFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGuiWindowFlags");
    ImGuiLua_RegisterEnumImGuiWindowFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImGui");
    luaL_setfuncs(L, ImGuiLua_RegImGui, 0);
    lua_pop(L, 1);
    return 0;
}
