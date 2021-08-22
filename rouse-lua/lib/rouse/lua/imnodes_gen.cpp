/*
 * This file is auto-generated.
 *
 * The generator is Copyright (c) 2021 askmeaboutloom.
 *
 * That generator is based on cimnodes <https://github.com/cimgui/cimgui>,
 * which is Copyright (c) 2022 Victor Bambi.
 *
 * That is based on imnodes <https://github.com/Nelarius/imnodes>, which is
 * Copyright (c) 2019 Johann Muszynski.
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
#include <imnodes.h>
#include <rouse.h>

extern "C" {
#include "lua_inc.h"
#include "intern.h"
#include "util.h"
}

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
        luaL_error(L, "Enum target '%s' should be a table, but is %s",
                   name, lua_typename(L, type));
    }
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


static void ImGuiLua_InitTypes(lua_State *L)
{
    (void) L; // nothing to do here
}


IMGUILUA_ENUM(ImNodesAttributeFlags)
static void ImGuiLua_RegisterEnumImNodesAttributeFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "EnableLinkDetachWithDragClick", 1 << 0);
    ImGuiLua_SetEnumValue(L, "EnableLinkCreationOnSnap", 1 << 1);
}

IMGUILUA_ENUM(ImNodesCol)
static void ImGuiLua_RegisterEnumImNodesCol(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "NodeBackground", 0);
    ImGuiLua_SetEnumValue(L, "NodeBackgroundHovered", 1);
    ImGuiLua_SetEnumValue(L, "NodeBackgroundSelected", 2);
    ImGuiLua_SetEnumValue(L, "NodeOutline", 3);
    ImGuiLua_SetEnumValue(L, "TitleBar", 4);
    ImGuiLua_SetEnumValue(L, "TitleBarHovered", 5);
    ImGuiLua_SetEnumValue(L, "TitleBarSelected", 6);
    ImGuiLua_SetEnumValue(L, "Link", 7);
    ImGuiLua_SetEnumValue(L, "LinkHovered", 8);
    ImGuiLua_SetEnumValue(L, "LinkSelected", 9);
    ImGuiLua_SetEnumValue(L, "Pin", 10);
    ImGuiLua_SetEnumValue(L, "PinHovered", 11);
    ImGuiLua_SetEnumValue(L, "BoxSelector", 12);
    ImGuiLua_SetEnumValue(L, "BoxSelectorOutline", 13);
    ImGuiLua_SetEnumValue(L, "GridBackground", 14);
    ImGuiLua_SetEnumValue(L, "GridLine", 15);
    ImGuiLua_SetEnumValue(L, "MiniMapBackground", 16);
    ImGuiLua_SetEnumValue(L, "MiniMapBackgroundHovered", 17);
    ImGuiLua_SetEnumValue(L, "MiniMapOutline", 18);
    ImGuiLua_SetEnumValue(L, "MiniMapOutlineHovered", 19);
    ImGuiLua_SetEnumValue(L, "MiniMapNodeBackground", 20);
    ImGuiLua_SetEnumValue(L, "MiniMapNodeBackgroundHovered", 21);
    ImGuiLua_SetEnumValue(L, "MiniMapNodeBackgroundSelected", 22);
    ImGuiLua_SetEnumValue(L, "MiniMapNodeOutline", 23);
    ImGuiLua_SetEnumValue(L, "MiniMapLink", 24);
    ImGuiLua_SetEnumValue(L, "MiniMapLinkSelected", 25);
    ImGuiLua_SetEnumValue(L, "COUNT", 26);
}

IMGUILUA_ENUM(ImNodesMiniMapLocation)
static void ImGuiLua_RegisterEnumImNodesMiniMapLocation(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "BottomLeft", 0);
    ImGuiLua_SetEnumValue(L, "BottomRight", 1);
    ImGuiLua_SetEnumValue(L, "TopLeft", 2);
    ImGuiLua_SetEnumValue(L, "TopRight", 3);
}

IMGUILUA_ENUM(ImNodesPinShape)
static void ImGuiLua_RegisterEnumImNodesPinShape(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "Circle", 0);
    ImGuiLua_SetEnumValue(L, "CircleFilled", 1);
    ImGuiLua_SetEnumValue(L, "Triangle", 2);
    ImGuiLua_SetEnumValue(L, "TriangleFilled", 3);
    ImGuiLua_SetEnumValue(L, "Quad", 4);
    ImGuiLua_SetEnumValue(L, "QuadFilled", 5);
}

IMGUILUA_ENUM(ImNodesStyleFlags)
static void ImGuiLua_RegisterEnumImNodesStyleFlags(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "None", 0);
    ImGuiLua_SetEnumValue(L, "NodeOutline", 1 << 0);
    ImGuiLua_SetEnumValue(L, "GridLines", 1 << 2);
}

IMGUILUA_ENUM(ImNodesStyleVar)
static void ImGuiLua_RegisterEnumImNodesStyleVar(lua_State *L)
{
    ImGuiLua_SetEnumValue(L, "GridSpacing", 0);
    ImGuiLua_SetEnumValue(L, "NodeCornerRounding", 1);
    ImGuiLua_SetEnumValue(L, "NodePaddingHorizontal", 2);
    ImGuiLua_SetEnumValue(L, "NodePaddingVertical", 3);
    ImGuiLua_SetEnumValue(L, "NodeBorderThickness", 4);
    ImGuiLua_SetEnumValue(L, "LinkThickness", 5);
    ImGuiLua_SetEnumValue(L, "LinkLineSegmentsPerLength", 6);
    ImGuiLua_SetEnumValue(L, "LinkHoverDistance", 7);
    ImGuiLua_SetEnumValue(L, "PinCircleRadius", 8);
    ImGuiLua_SetEnumValue(L, "PinQuadSideLength", 9);
    ImGuiLua_SetEnumValue(L, "PinTriangleSideLength", 10);
    ImGuiLua_SetEnumValue(L, "PinLineThickness", 11);
    ImGuiLua_SetEnumValue(L, "PinHoverRadius", 12);
    ImGuiLua_SetEnumValue(L, "PinOffset", 13);
}


// function ImNodes::BeginInputAttribute

IMGUILUA_FUNCTION(ImNodes::BeginInputAttribute)
static int ImNodes_BeginInputAttribute(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    int id;
    id = static_cast<int>(luaL_checkinteger(L, 1));
    int shape;
    if (ARGC < 2) {
        shape = ImNodesPinShape_CircleFilled;
    }
    else {
        shape = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImNodes::BeginInputAttribute(id,shape);
    IMGUILUA_BEGIN(L, ImNodes::BeginInputAttribute, ImNodes::EndInputAttribute, 17);
    return 0;
}


// function ImNodes::BeginNode

IMGUILUA_FUNCTION(ImNodes::BeginNode)
static int ImNodes_BeginNode(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int id;
    id = static_cast<int>(luaL_checkinteger(L, 1));
    ImNodes::BeginNode(id);
    IMGUILUA_BEGIN(L, ImNodes::BeginNode, ImNodes::EndNode, 18);
    return 0;
}


// function ImNodes::BeginNodeEditor

IMGUILUA_FUNCTION(ImNodes::BeginNodeEditor)
static int ImNodes_BeginNodeEditor(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::BeginNodeEditor();
    IMGUILUA_BEGIN(L, ImNodes::BeginNodeEditor, ImNodes::EndNodeEditor, 19);
    return 0;
}


// function ImNodes::BeginNodeTitleBar

IMGUILUA_FUNCTION(ImNodes::BeginNodeTitleBar)
static int ImNodes_BeginNodeTitleBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::BeginNodeTitleBar();
    IMGUILUA_BEGIN(L, ImNodes::BeginNodeTitleBar, ImNodes::EndNodeTitleBar, 20);
    return 0;
}


// function ImNodes::BeginOutputAttribute

IMGUILUA_FUNCTION(ImNodes::BeginOutputAttribute)
static int ImNodes_BeginOutputAttribute(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC < 1 || ARGC > 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 1, 2);
    }
    int id;
    id = static_cast<int>(luaL_checkinteger(L, 1));
    int shape;
    if (ARGC < 2) {
        shape = ImNodesPinShape_CircleFilled;
    }
    else {
        shape = static_cast<int>(luaL_checkinteger(L, 2));
    }
    ImNodes::BeginOutputAttribute(id,shape);
    IMGUILUA_BEGIN(L, ImNodes::BeginOutputAttribute, ImNodes::EndOutputAttribute, 21);
    return 0;
}


// function ImNodes::BeginStaticAttribute

IMGUILUA_FUNCTION(ImNodes::BeginStaticAttribute)
static int ImNodes_BeginStaticAttribute(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int id;
    id = static_cast<int>(luaL_checkinteger(L, 1));
    ImNodes::BeginStaticAttribute(id);
    IMGUILUA_BEGIN(L, ImNodes::BeginStaticAttribute, ImNodes::EndStaticAttribute, 22);
    return 0;
}


// function ImNodes::ClearLinkSelection

static int ImNodes_ClearLinkSelection_(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::ClearLinkSelection();
    return 0;
}

static int ImNodes_ClearLinkSelection_i(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int link_id;
    link_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImNodes::ClearLinkSelection(link_id);
    return 0;
}

IMGUILUA_FUNCTION(ImNodes::ImNodes_ClearLinkSelection)
static int ImNodes_ClearLinkSelection(lua_State *L)
{
    int ARGC = lua_gettop(L);
    switch (ARGC) {
    case 0:
        return ImNodes_ClearLinkSelection_(L);
    case 1:
        return ImNodes_ClearLinkSelection_i(L);
    default:
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
}


// function ImNodes::ClearNodeSelection

static int ImNodes_ClearNodeSelection_(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::ClearNodeSelection();
    return 0;
}

static int ImNodes_ClearNodeSelection_i(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImNodes::ClearNodeSelection(node_id);
    return 0;
}

IMGUILUA_FUNCTION(ImNodes::ImNodes_ClearNodeSelection)
static int ImNodes_ClearNodeSelection(lua_State *L)
{
    int ARGC = lua_gettop(L);
    switch (ARGC) {
    case 0:
        return ImNodes_ClearNodeSelection_(L);
    case 1:
        return ImNodes_ClearNodeSelection_i(L);
    default:
        return luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }
}


// function ImNodes::CreateContext is excluded


// function ImNodes::DestroyContext is excluded


// function ImNodes::EditorContextCreate

// ImNodesEditorContext* ImNodes_EditorContextCreate(): Unknown return type 'ImNodesEditorContext*' (ImNodesEditorContext*)


// function ImNodes::EditorContextFree

// void ImNodes_EditorContextFree(ImNodesEditorContext*): Unknown argument type 'ImNodesEditorContext*' (ImNodesEditorContext*)


// function ImNodes::EditorContextGetPanning

IMGUILUA_FUNCTION(ImNodes::EditorContextGetPanning)
static int ImNodes_EditorContextGetPanning(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImVec2 RETVAL = ImNodes::EditorContextGetPanning();
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImNodes::EditorContextMoveToNode

IMGUILUA_FUNCTION(ImNodes::EditorContextMoveToNode)
static int ImNodes_EditorContextMoveToNode(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImNodes::EditorContextMoveToNode(node_id);
    return 0;
}


// function ImNodes::EditorContextResetPanning

IMGUILUA_FUNCTION(ImNodes::EditorContextResetPanning)
static int ImNodes_EditorContextResetPanning(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    ImVec2 pos;
    pos = ImGuiLua_ToImVec2(L, 1);
    ImNodes::EditorContextResetPanning(pos);
    return 0;
}


// function ImNodes::EditorContextSet

// void ImNodes_EditorContextSet(ImNodesEditorContext*): Unknown argument type 'ImNodesEditorContext*' (ImNodesEditorContext*)


// function ImNodes::EndInputAttribute

IMGUILUA_FUNCTION(ImNodes::EndInputAttribute)
static int ImNodes_EndInputAttribute(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImNodes::EndInputAttribute, 17, ImNodes::BeginInputAttribute);
    ImNodes::EndInputAttribute();
    return 0;
}


// function ImNodes::EndNode

IMGUILUA_FUNCTION(ImNodes::EndNode)
static int ImNodes_EndNode(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImNodes::EndNode, 18, ImNodes::BeginNode);
    ImNodes::EndNode();
    return 0;
}


// function ImNodes::EndNodeEditor

IMGUILUA_FUNCTION(ImNodes::EndNodeEditor)
static int ImNodes_EndNodeEditor(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImNodes::EndNodeEditor, 19, ImNodes::BeginNodeEditor);
    ImNodes::EndNodeEditor();
    return 0;
}


// function ImNodes::EndNodeTitleBar

IMGUILUA_FUNCTION(ImNodes::EndNodeTitleBar)
static int ImNodes_EndNodeTitleBar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImNodes::EndNodeTitleBar, 20, ImNodes::BeginNodeTitleBar);
    ImNodes::EndNodeTitleBar();
    return 0;
}


// function ImNodes::EndOutputAttribute

IMGUILUA_FUNCTION(ImNodes::EndOutputAttribute)
static int ImNodes_EndOutputAttribute(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImNodes::EndOutputAttribute, 21, ImNodes::BeginOutputAttribute);
    ImNodes::EndOutputAttribute();
    return 0;
}


// function ImNodes::EndStaticAttribute

IMGUILUA_FUNCTION(ImNodes::EndStaticAttribute)
static int ImNodes_EndStaticAttribute(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    IMGUILUA_END(L, ImNodes::EndStaticAttribute, 22, ImNodes::BeginStaticAttribute);
    ImNodes::EndStaticAttribute();
    return 0;
}


// function ImNodes::GetCurrentContext

// ImNodesContext* ImNodes_GetCurrentContext(): Unknown return type 'ImNodesContext*' (ImNodesContext*)


// function ImNodes::GetIO

// ImNodesIO* ImNodes_GetIO(): Unknown return type 'ImNodesIO*' (ImNodesIO*)


// function ImNodes::GetNodeDimensions

IMGUILUA_FUNCTION(ImNodes::GetNodeDimensions)
static int ImNodes_GetNodeDimensions(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int id;
    id = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec2 RETVAL = ImNodes::GetNodeDimensions(id);
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImNodes::GetNodeEditorSpacePos

IMGUILUA_FUNCTION(ImNodes::GetNodeEditorSpacePos)
static int ImNodes_GetNodeEditorSpacePos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec2 RETVAL = ImNodes::GetNodeEditorSpacePos(node_id);
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImNodes::GetNodeGridSpacePos

IMGUILUA_FUNCTION(ImNodes::GetNodeGridSpacePos)
static int ImNodes_GetNodeGridSpacePos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec2 RETVAL = ImNodes::GetNodeGridSpacePos(node_id);
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImNodes::GetNodeScreenSpacePos

IMGUILUA_FUNCTION(ImNodes::GetNodeScreenSpacePos)
static int ImNodes_GetNodeScreenSpacePos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec2 RETVAL = ImNodes::GetNodeScreenSpacePos(node_id);
    ImGuiLua_PushImVec2(L, RETVAL);
    return 1;
}


// function ImNodes::GetSelectedLinks is excluded


// function ImNodes::GetSelectedNodes is excluded


// function ImNodes::GetStyle

// ImNodesStyle* ImNodes_GetStyle(): Unknown return type 'ImNodesStyle*' (ImNodesStyle*)


// function ImNodes::IsAnyAttributeActive is excluded


// function ImNodes::IsAttributeActive

IMGUILUA_FUNCTION(ImNodes::IsAttributeActive)
static int ImNodes_IsAttributeActive(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImNodes::IsAttributeActive();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImNodes::IsEditorHovered

IMGUILUA_FUNCTION(ImNodes::IsEditorHovered)
static int ImNodes_IsEditorHovered(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    bool RETVAL = ImNodes::IsEditorHovered();
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImNodes::IsLinkCreated is excluded


// function ImNodes::IsLinkDestroyed is excluded


// function ImNodes::IsLinkDropped is excluded


// function ImNodes::IsLinkHovered is excluded


// function ImNodes::IsLinkSelected

IMGUILUA_FUNCTION(ImNodes::IsLinkSelected)
static int ImNodes_IsLinkSelected(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int link_id;
    link_id = static_cast<int>(luaL_checkinteger(L, 1));
    bool RETVAL = ImNodes::IsLinkSelected(link_id);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImNodes::IsLinkStarted is excluded


// function ImNodes::IsNodeHovered is excluded


// function ImNodes::IsNodeSelected

IMGUILUA_FUNCTION(ImNodes::IsNodeSelected)
static int ImNodes_IsNodeSelected(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    bool RETVAL = ImNodes::IsNodeSelected(node_id);
    lua_pushboolean(L, RETVAL);
    return 1;
}


// function ImNodes::IsPinHovered is excluded


// function ImNodes::Link

IMGUILUA_FUNCTION(ImNodes::Link)
static int ImNodes_Link(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 3) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 3);
    }
    int id;
    id = static_cast<int>(luaL_checkinteger(L, 1));
    int start_attribute_id;
    start_attribute_id = static_cast<int>(luaL_checkinteger(L, 2));
    int end_attribute_id;
    end_attribute_id = static_cast<int>(luaL_checkinteger(L, 3));
    ImNodes::Link(id,start_attribute_id,end_attribute_id);
    return 0;
}


// function ImNodes::LoadCurrentEditorStateFromIniFile

IMGUILUA_FUNCTION(ImNodes::LoadCurrentEditorStateFromIniFile)
static int ImNodes_LoadCurrentEditorStateFromIniFile(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *file_name;
    file_name = lua_tostring(L, 1);
    ImNodes::LoadCurrentEditorStateFromIniFile(file_name);
    return 0;
}


// function ImNodes::LoadCurrentEditorStateFromIniString

// void ImNodes_LoadCurrentEditorStateFromIniString(const char*, size_t): Unknown argument type 'size_t' (size_t)


// function ImNodes::LoadEditorStateFromIniFile

// void ImNodes_LoadEditorStateFromIniFile(ImNodesEditorContext*, const char*): Unknown argument type 'ImNodesEditorContext*' (ImNodesEditorContext*)


// function ImNodes::LoadEditorStateFromIniString

// void ImNodes_LoadEditorStateFromIniString(ImNodesEditorContext*, const char*, size_t): Unknown argument type 'ImNodesEditorContext*' (ImNodesEditorContext*)


// function ImNodes::MiniMap

// void ImNodes_MiniMap(const float, const ImNodesMiniMapLocation, const ImNodesMiniMapNodeHoveringCallback, void*): Unknown argument type 'const ImNodesMiniMapNodeHoveringCallback' (const ImNodesMiniMapNodeHoveringCallback)


// function ImNodes::NumSelectedLinks

IMGUILUA_FUNCTION(ImNodes::NumSelectedLinks)
static int ImNodes_NumSelectedLinks(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImNodes::NumSelectedLinks();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImNodes::NumSelectedNodes

IMGUILUA_FUNCTION(ImNodes::NumSelectedNodes)
static int ImNodes_NumSelectedNodes(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    int RETVAL = ImNodes::NumSelectedNodes();
    lua_pushinteger(L, static_cast<lua_Integer>(RETVAL));
    return 1;
}


// function ImNodes::PopAttributeFlag

IMGUILUA_FUNCTION(ImNodes::PopAttributeFlag)
static int ImNodes_PopAttributeFlag(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::PopAttributeFlag();
    return 0;
}


// function ImNodes::PopColorStyle

IMGUILUA_FUNCTION(ImNodes::PopColorStyle)
static int ImNodes_PopColorStyle(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::PopColorStyle();
    return 0;
}


// function ImNodes::PopStyleVar

IMGUILUA_FUNCTION(ImNodes::PopStyleVar)
static int ImNodes_PopStyleVar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::PopStyleVar();
    return 0;
}


// function ImNodes::PushAttributeFlag

IMGUILUA_FUNCTION(ImNodes::PushAttributeFlag)
static int ImNodes_PushAttributeFlag(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int flag;
    flag = static_cast<int>(luaL_checkinteger(L, 1));
    ImNodes::PushAttributeFlag(flag);
    return 0;
}


// function ImNodes::PushColorStyle

IMGUILUA_FUNCTION(ImNodes::PushColorStyle)
static int ImNodes_PushColorStyle(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int item;
    item = static_cast<int>(luaL_checkinteger(L, 1));
    unsigned int color;
    color = static_cast<unsigned int>(luaL_checkinteger(L, 2));
    ImNodes::PushColorStyle(item,color);
    return 0;
}


// function ImNodes::PushStyleVar

IMGUILUA_FUNCTION(ImNodes::PushStyleVar)
static int ImNodes_PushStyleVar(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int style_item;
    style_item = static_cast<int>(luaL_checkinteger(L, 1));
    float value;
    value = static_cast<float>(luaL_checknumber(L, 2));
    ImNodes::PushStyleVar(style_item,value);
    return 0;
}


// function ImNodes::SaveCurrentEditorStateToIniFile

IMGUILUA_FUNCTION(ImNodes::SaveCurrentEditorStateToIniFile)
static int ImNodes_SaveCurrentEditorStateToIniFile(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    const char *file_name;
    file_name = lua_tostring(L, 1);
    ImNodes::SaveCurrentEditorStateToIniFile(file_name);
    return 0;
}


// function ImNodes::SaveCurrentEditorStateToIniString

// const char* ImNodes_SaveCurrentEditorStateToIniString(size_t*): Unknown argument type 'size_t*' (size_t*)


// function ImNodes::SaveEditorStateToIniFile

// void ImNodes_SaveEditorStateToIniFile(const ImNodesEditorContext*, const char*): Unknown argument type 'const ImNodesEditorContext*' (const ImNodesEditorContext*)


// function ImNodes::SaveEditorStateToIniString

// const char* ImNodes_SaveEditorStateToIniString(const ImNodesEditorContext*, size_t*): Unknown argument type 'const ImNodesEditorContext*' (const ImNodesEditorContext*)


// function ImNodes::SelectLink

IMGUILUA_FUNCTION(ImNodes::SelectLink)
static int ImNodes_SelectLink(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int link_id;
    link_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImNodes::SelectLink(link_id);
    return 0;
}


// function ImNodes::SelectNode

IMGUILUA_FUNCTION(ImNodes::SelectNode)
static int ImNodes_SelectNode(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 1) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 1);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImNodes::SelectNode(node_id);
    return 0;
}


// function ImNodes::SetCurrentContext

// void ImNodes_SetCurrentContext(ImNodesContext*): Unknown argument type 'ImNodesContext*' (ImNodesContext*)


// function ImNodes::SetImGuiContext

// void ImNodes_SetImGuiContext(ImGuiContext*): Unknown argument type 'ImGuiContext*' (ImGuiContext*)


// function ImNodes::SetNodeDraggable

IMGUILUA_FUNCTION(ImNodes::SetNodeDraggable)
static int ImNodes_SetNodeDraggable(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    bool draggable;
    draggable = lua_toboolean(L, 2);
    ImNodes::SetNodeDraggable(node_id,draggable);
    return 0;
}


// function ImNodes::SetNodeEditorSpacePos

IMGUILUA_FUNCTION(ImNodes::SetNodeEditorSpacePos)
static int ImNodes_SetNodeEditorSpacePos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec2 editor_space_pos;
    editor_space_pos = ImGuiLua_ToImVec2(L, 2);
    ImNodes::SetNodeEditorSpacePos(node_id,editor_space_pos);
    return 0;
}


// function ImNodes::SetNodeGridSpacePos

IMGUILUA_FUNCTION(ImNodes::SetNodeGridSpacePos)
static int ImNodes_SetNodeGridSpacePos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec2 grid_pos;
    grid_pos = ImGuiLua_ToImVec2(L, 2);
    ImNodes::SetNodeGridSpacePos(node_id,grid_pos);
    return 0;
}


// function ImNodes::SetNodeScreenSpacePos

IMGUILUA_FUNCTION(ImNodes::SetNodeScreenSpacePos)
static int ImNodes_SetNodeScreenSpacePos(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 2) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 2);
    }
    int node_id;
    node_id = static_cast<int>(luaL_checkinteger(L, 1));
    ImVec2 screen_space_pos;
    screen_space_pos = ImGuiLua_ToImVec2(L, 2);
    ImNodes::SetNodeScreenSpacePos(node_id,screen_space_pos);
    return 0;
}


// function ImNodes::StyleColorsClassic

IMGUILUA_FUNCTION(ImNodes::StyleColorsClassic)
static int ImNodes_StyleColorsClassic(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::StyleColorsClassic();
    return 0;
}


// function ImNodes::StyleColorsDark

IMGUILUA_FUNCTION(ImNodes::StyleColorsDark)
static int ImNodes_StyleColorsDark(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::StyleColorsDark();
    return 0;
}


// function ImNodes::StyleColorsLight

IMGUILUA_FUNCTION(ImNodes::StyleColorsLight)
static int ImNodes_StyleColorsLight(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        return luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }
    ImNodes::StyleColorsLight();
    return 0;
}


template<int (*GETNUM)(), void (*GETSELECTED)(int *)>
static int ImGuiLua_GetSelected(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }

    int n = GETNUM();
    lua_createtable(L, n, 0);

    if (n > 0) {
        void *ud;
        lua_Alloc alloc = lua_getallocf(L, &ud);

        size_t size = sizeof(int) * static_cast<size_t>(n);
        int *ids = static_cast<int *>(alloc(ud, NULL, LUA_TNIL, size));
        if (!ids) {
            return luaL_error(L, "Could not allocate buffer for %d ids", n);
        }

        GETSELECTED(ids);
        for (int i = 0; i < n; ++i) {
            lua_pushinteger(L, ids[i]);
            lua_rawseti(L, -2, static_cast<lua_Integer>(i + 1));
        }

        alloc(ud, ids, size, 0);
    }

    return 1;
}

IMGUILUA_FUNCTION(ImNodes::GetSelectedLinks)
static int ImNodes_GetSelectedLinks(lua_State *L)
{
    return ImGuiLua_GetSelected<ImNodes::NumSelectedLinks, ImNodes::GetSelectedLinks>(L);
}

IMGUILUA_FUNCTION(ImNodes::GetSelectedNodes)
static int ImNodes_GetSelectedNodes(lua_State *L)
{
    return ImGuiLua_GetSelected<ImNodes::NumSelectedNodes, ImNodes::GetSelectedNodes>(L);
}


template<bool (*ISID)(int *)>
static int ImGuiLua_IsIdOrNil(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC != 0) {
        luaL_error(L, "Wrong number of arguments: got %d, wanted %d", ARGC, 0);
    }

    int id;
    if (ISID(&id)) {
        lua_pushinteger(L, static_cast<lua_Integer>(id));
    }
    else {
        lua_pushnil(L);
    }

    return 1;
}

IMGUILUA_FUNCTION(ImNodes::IsAnyAttributeActive)
static int ImNodes_IsAnyAttributeActive(lua_State *L)
{
    return ImGuiLua_IsIdOrNil<ImNodes::IsAnyAttributeActive>(L);
}

IMGUILUA_FUNCTION(ImNodes::IsLinkHovered)
static int ImNodes_IsLinkHovered(lua_State *L)
{
    return ImGuiLua_IsIdOrNil<ImNodes::IsLinkHovered>(L);
}

IMGUILUA_FUNCTION(ImNodes::IsNodeHovered)
static int ImNodes_IsNodeHovered(lua_State *L)
{
    return ImGuiLua_IsIdOrNil<ImNodes::IsNodeHovered>(L);
}

IMGUILUA_FUNCTION(ImNodes::IsPinHovered)
static int ImNodes_IsPinHovered(lua_State *L)
{
    return ImGuiLua_IsIdOrNil<ImNodes::IsPinHovered>(L);
}

IMGUILUA_FUNCTION(ImNodes::IsLinkStarted)
static int ImNodes_IsLinkStarted(lua_State *L)
{
    return ImGuiLua_IsIdOrNil<ImNodes::IsLinkStarted>(L);
}

IMGUILUA_FUNCTION(ImNodes::IsLinkDestroyed)
static int ImNodes_IsLinkDestroyed(lua_State *L)
{
    return ImGuiLua_IsIdOrNil<ImNodes::IsLinkDestroyed>(L);
}

IMGUILUA_FUNCTION(ImNodes::IsLinkDropped)
static int ImNodes_IsLinkDropped(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }

    bool including_detached_links;
    if (ARGC < 1) {
        including_detached_links = true;
    }
    else {
        including_detached_links = lua_toboolean(L, 1);
    }

    int started_at_attribute_id;
    if (ImNodes::IsLinkDropped(&started_at_attribute_id, including_detached_links)) {
        lua_pushinteger(L, static_cast<lua_Integer>(started_at_attribute_id));
    }
    else {
        lua_pushnil(L);
    }

    return 1;
}

IMGUILUA_FUNCTION(ImNodes::IsLinkCreated)
static int ImNodes_IsLinkCreated(lua_State *L)
{
    int ARGC = lua_gettop(L);
    if (ARGC > 1) {
        luaL_error(L, "Wrong number of arguments: got %d, wanted between %d and %d", ARGC, 0, 1);
    }

    bool including_node_ids;
    if (ARGC < 1) {
        including_node_ids = false;
    }
    else {
        including_node_ids = lua_toboolean(L, 1);
    }

    if (including_node_ids) {
        int started_at_node_id, started_at_attribute_id;
        int ended_at_node_id, ended_at_attribute_id;
        bool created_from_snap;
        if (ImNodes::IsLinkCreated(&started_at_node_id, &started_at_attribute_id,
                                   &ended_at_node_id, &ended_at_attribute_id,
                                   &created_from_snap)) {
            lua_pushinteger(L, static_cast<lua_Integer>(started_at_node_id));
            lua_pushinteger(L, static_cast<lua_Integer>(started_at_attribute_id));
            lua_pushinteger(L, static_cast<lua_Integer>(ended_at_node_id));
            lua_pushinteger(L, static_cast<lua_Integer>(ended_at_attribute_id));
            lua_pushboolean(L, created_from_snap);
        }
        else {
            lua_pushnil(L);
            lua_pushnil(L);
            lua_pushnil(L);
            lua_pushnil(L);
            lua_pushnil(L);
        }
        return 5;
    }
    else {
        int started_at_attribute_id, ended_at_attribute_id;
        bool created_from_snap;
        if (ImNodes::IsLinkCreated(&started_at_attribute_id, &ended_at_attribute_id,
                                   &created_from_snap)) {
            lua_pushinteger(L, static_cast<lua_Integer>(started_at_attribute_id));
            lua_pushinteger(L, static_cast<lua_Integer>(ended_at_attribute_id));
            lua_pushboolean(L, created_from_snap);
        }
        else {
            lua_pushnil(L);
            lua_pushnil(L);
            lua_pushnil(L);
        }
        return 3;
    }
}


static luaL_Reg ImGuiLua_RegImNodes[] = {
    {"begin_input_attribute", ImNodes_BeginInputAttribute},
    {"begin_node", ImNodes_BeginNode},
    {"begin_node_editor", ImNodes_BeginNodeEditor},
    {"begin_node_title_bar", ImNodes_BeginNodeTitleBar},
    {"begin_output_attribute", ImNodes_BeginOutputAttribute},
    {"begin_static_attribute", ImNodes_BeginStaticAttribute},
    {"im_nodes_clear_link_selection", ImNodes_ClearLinkSelection},
    {"im_nodes_clear_node_selection", ImNodes_ClearNodeSelection},
    {"editor_context_get_panning", ImNodes_EditorContextGetPanning},
    {"editor_context_move_to_node", ImNodes_EditorContextMoveToNode},
    {"editor_context_reset_panning", ImNodes_EditorContextResetPanning},
    {"end_input_attribute", ImNodes_EndInputAttribute},
    {"end_node", ImNodes_EndNode},
    {"end_node_editor", ImNodes_EndNodeEditor},
    {"end_node_title_bar", ImNodes_EndNodeTitleBar},
    {"end_output_attribute", ImNodes_EndOutputAttribute},
    {"end_static_attribute", ImNodes_EndStaticAttribute},
    {"get_node_dimensions", ImNodes_GetNodeDimensions},
    {"get_node_editor_space_pos", ImNodes_GetNodeEditorSpacePos},
    {"get_node_grid_space_pos", ImNodes_GetNodeGridSpacePos},
    {"get_node_screen_space_pos", ImNodes_GetNodeScreenSpacePos},
    {"get_selected_links", ImNodes_GetSelectedLinks},
    {"get_selected_nodes", ImNodes_GetSelectedNodes},
    {"is_any_attribute_active", ImNodes_IsAnyAttributeActive},
    {"is_attribute_active", ImNodes_IsAttributeActive},
    {"is_editor_hovered", ImNodes_IsEditorHovered},
    {"is_link_created", ImNodes_IsLinkCreated},
    {"is_link_destroyed", ImNodes_IsLinkDestroyed},
    {"is_link_dropped", ImNodes_IsLinkDropped},
    {"is_link_hovered", ImNodes_IsLinkHovered},
    {"is_link_selected", ImNodes_IsLinkSelected},
    {"is_link_started", ImNodes_IsLinkStarted},
    {"is_node_hovered", ImNodes_IsNodeHovered},
    {"is_node_selected", ImNodes_IsNodeSelected},
    {"is_pin_hovered", ImNodes_IsPinHovered},
    {"link", ImNodes_Link},
    {"load_current_editor_state_from_ini_file", ImNodes_LoadCurrentEditorStateFromIniFile},
    {"num_selected_links", ImNodes_NumSelectedLinks},
    {"num_selected_nodes", ImNodes_NumSelectedNodes},
    {"pop_attribute_flag", ImNodes_PopAttributeFlag},
    {"pop_color_style", ImNodes_PopColorStyle},
    {"pop_style_var", ImNodes_PopStyleVar},
    {"push_attribute_flag", ImNodes_PushAttributeFlag},
    {"push_color_style", ImNodes_PushColorStyle},
    {"push_style_var", ImNodes_PushStyleVar},
    {"save_current_editor_state_to_ini_file", ImNodes_SaveCurrentEditorStateToIniFile},
    {"select_link", ImNodes_SelectLink},
    {"select_node", ImNodes_SelectNode},
    {"set_node_draggable", ImNodes_SetNodeDraggable},
    {"set_node_editor_space_pos", ImNodes_SetNodeEditorSpacePos},
    {"set_node_grid_space_pos", ImNodes_SetNodeGridSpacePos},
    {"set_node_screen_space_pos", ImNodes_SetNodeScreenSpacePos},
    {"style_colors_classic", ImNodes_StyleColorsClassic},
    {"style_colors_dark", ImNodes_StyleColorsDark},
    {"style_colors_light", ImNodes_StyleColorsLight},
    {NULL, NULL},
};

extern "C" int ImGuiLua_InitImNodes(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    IMGUILUA_SETUP(L);
    ImGuiLua_InitTypes(L);
    lua_pushvalue(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImNodesAttributeFlags");
    ImGuiLua_RegisterEnumImNodesAttributeFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImNodesCol");
    ImGuiLua_RegisterEnumImNodesCol(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImNodesMiniMapLocation");
    ImGuiLua_RegisterEnumImNodesMiniMapLocation(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImNodesPinShape");
    ImGuiLua_RegisterEnumImNodesPinShape(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImNodesStyleFlags");
    ImGuiLua_RegisterEnumImNodesStyleFlags(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImNodesStyleVar");
    ImGuiLua_RegisterEnumImNodesStyleVar(L);
    lua_pop(L, 1);
    ImGuiLua_GetOrCreateTable(L, "ImNodes");
    luaL_setfuncs(L, ImGuiLua_RegImNodes, 0);
    lua_pop(L, 1);
    return 0;
}
