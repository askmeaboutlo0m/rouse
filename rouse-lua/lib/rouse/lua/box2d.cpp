/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
/*
 * Copyright (c) 2020 askmeaboutloom
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

/* There's some signedness issues when compiling for Windows. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#include <box2d/box2d.h>
#pragma GCC diagnostic pop

extern "C" {

#include "lua_inc.h"
#include "intern.h"
#include "util.h"

static b2Vec2 from_v2(R_V2 v)
{
    return b2Vec2(v.x, v.y);
}

static R_V2 to_v2(b2Vec2 v)
{
    return R_v2(v.x, v.y);
}


static int b2_shape_new_box_xl(lua_State *L)
{
    float hw = XL_checkfloat(L, 1);
    float hh = XL_checkfloat(L, 2);
    b2Shape *RETVAL;
    b2PolygonShape *shape;
    if (lua_gettop(L) <= 2) {
        shape = new b2PolygonShape;
        shape->SetAsBox(hw, hh);
    }
    else {
        R_V2  *center = R_CPPCAST(R_V2 *, luaL_checkudata(L, 3, "R_V2"));
        float angle   = XL_checkfloat(L, 4);
        shape         = new b2PolygonShape;
        shape->SetAsBox(hw, hh, from_v2(*center), angle);
    }
    RETVAL = shape;
    XL_pushnewpptypeuv(L, RETVAL, "b2Shape", 0);
    return 1;
}


static int b2vec2_array_from_table(lua_State *L)
{
    lua_len(L, 2);
    int len = XL_checkint(L, -1);
    if (len < 3) {
        R_LUA_DIE(L, "Can't make a polygon from %d vertices", len);
    }

    b2Vec2 **pp    = R_CPPCAST(b2Vec2 **, lua_touserdata(L, 1));
    b2Vec2 *points = new b2Vec2[len];
    *pp            = points;

    for (int i = 0; i < len; ++i) {
        lua_geti(L, 2, R_lua_int2i(i + 1));
        R_V2 *point = R_CPPCAST(R_V2 *, XL_checkutype(L, -1, "R_V2"));
        points[i]   = from_v2(*point);
        lua_pop(L, 1);
    }

    return 1;
}


static int b2_shape_new_polygon_xl(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int points = 1;
    b2Shape *RETVAL;
    b2Vec2 *b2vec2s = NULL;
    lua_pushcfunction(L, b2vec2_array_from_table);
    lua_pushlightuserdata(L, &b2vec2s);
    lua_pushvalue(L, points);
    if (lua_pcall(L, 2, 1, 0)) {
        delete[] b2vec2s;
        lua_error(L);
    }

    b2PolygonShape *shape = new b2PolygonShape;
    shape->Set(b2vec2s, XL_checkint(L, -1));
    delete[] b2vec2s;

    if (lua_toboolean(L, points + 1) && !shape->Validate()) {
        delete shape;
        R_LUA_DIE(L, "Polygon is not convex");
    }

    RETVAL = shape;
    XL_pushnewpptypeuv(L, RETVAL, "b2Shape", 0);
    return 1;
}

static int b2_shape_new_circle_xl(lua_State *L)
{
    R_V2 pos = *((R_V2 *)luaL_checkudata(L, 1, "R_V2"));
    float radius = XL_checkfloat(L, 2);
    b2Shape *RETVAL;
    b2CircleShape *shape = new b2CircleShape;
    shape->m_p.Set(pos.x, pos.y);
    shape->m_radius = radius;
    RETVAL = shape;
    XL_pushnewpptypeuv(L, RETVAL, "b2Shape", 0);
    return 1;
}

static int b2shape_method_gc_xl(lua_State *L)
{
    b2Shape *self = R_CPPCAST(b2Shape *, XL_checkpptype_nullable(L, 1, "b2Shape"));
    delete self;
    return 0;
}

static int b2fixture_destroyed_index_xl(lua_State *L)
{
    b2Fixture *self = R_CPPCAST(b2Fixture *, XL_checkpptype_nullable(L, 1, "b2Fixture"));
    bool RETVAL;
    RETVAL = self;
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int b2fixturedef_shape_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    b2Shape *VALUE = R_CPPCAST(b2Shape *, XL_checkpptype(L, 2, "b2Shape"));
    self->shape = VALUE;
    return 0;
}

static int b2fixturedef_friction_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->friction = VALUE;
    return 0;
}

static int b2fixturedef_restitution_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->restitution = VALUE;
    return 0;
}

static int b2fixturedef_restitution_threshold_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->restitutionThreshold = VALUE;
    return 0;
}

static int b2fixturedef_density_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->density = VALUE;
    return 0;
}

static int b2fixturedef_is_sensor_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->isSensor = VALUE;
    return 0;
}

static int b2fixturedef_category_bits_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    unsigned short VALUE = XL_checkushort(L, 2);
    self->filter.categoryBits = VALUE;
    return 0;
}

static int b2fixturedef_mask_bits_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    unsigned short VALUE = XL_checkushort(L, 2);
    self->filter.maskBits = VALUE;
    return 0;
}

static int b2fixturedef_group_index_newindex_xl(lua_State *L)
{
    b2FixtureDef *self = R_CPPCAST(b2FixtureDef *, XL_checkpptype(L, 1, "b2FixtureDef"));
    short VALUE = XL_checkshort(L, 2);
    self->filter.groupIndex = VALUE;
    return 0;
}

static int b2body_destroyed_index_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype_nullable(L, 1, "b2Body"));
    bool RETVAL;
    RETVAL = self;
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int b2body_position_index_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype(L, 1, "b2Body"));
    R_V2 RETVAL;
    RETVAL = to_v2(self->GetPosition());
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
    return 1;
}

static int b2body_pos_index_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype(L, 1, "b2Body"));
    R_V2 RETVAL;
    RETVAL = to_v2(self->GetPosition());
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
    return 1;
}

static int b2body_angle_index_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype(L, 1, "b2Body"));
    float RETVAL;
    RETVAL = self->GetAngle();
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int b2body_rotation_index_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype(L, 1, "b2Body"));
    float RETVAL;
    RETVAL = R_to_deg(self->GetAngle());
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int b2body_linear_velocity_newindex_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype(L, 1, "b2Body"));
    R_V2 VALUE = *((R_V2 *)luaL_checkudata(L, 2, "R_V2"));
    self->SetLinearVelocity(from_v2(VALUE));
    return 0;
}

static int b2body_angular_velocity_newindex_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype(L, 1, "b2Body"));
    float VALUE = XL_checkfloat(L, 2);
    self->SetAngularVelocity(VALUE);
    return 0;
}

static int b2body_method_set_transform_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype(L, 1, "b2Body"));
    R_V2 position = *((R_V2 *)luaL_checkudata(L, 2, "R_V2"));
    float angle = XL_checkfloat(L, 3);
    self->SetTransform(from_v2(position), angle);
    return 0;
}

static int b2body_method_create_fixture_xl(lua_State *L)
{
    b2Body *self = R_CPPCAST(b2Body *, XL_checkpptype(L, 1, "b2Body"));
    luaL_checktype(L, 2, LUA_TTABLE);
    int args = 2;
    b2Fixture *RETVAL;
    b2FixtureDef def;
    XL_pushnewpptypeuv(L, &def, "b2FixtureDef", 0);
    XL_setfromtable(L, "b2FixtureDef", args + 1, args);

    lua_pop(L, 1);
    RETVAL = self->CreateFixture(&def);
    XL_pushnewpptypeuv(L, RETVAL, "b2Fixture", 1);
    lua_pushvalue(L, 1);
    XL_setiuservalue(L, -2, 1);
    return 1;
}

static int b2bodydef_angle_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->angle = VALUE;
    return 0;
}

static int b2bodydef_angular_velocity_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->angularVelocity = VALUE;
    return 0;
}

static int b2bodydef_linear_damping_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->linearDamping = VALUE;
    return 0;
}

static int b2bodydef_angular_damping_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->angularDamping = VALUE;
    return 0;
}

static int b2bodydef_allow_sleep_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->allowSleep = VALUE;
    return 0;
}

static int b2bodydef_awake_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->awake = VALUE;
    return 0;
}

static int b2bodydef_fixed_rotation_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->fixedRotation = VALUE;
    return 0;
}

static int b2bodydef_bullet_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->bullet = VALUE;
    return 0;
}

static int b2bodydef_enabled_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->enabled = VALUE;
    return 0;
}

static int b2bodydef_gravity_scale_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->gravityScale = VALUE;
    return 0;
}

static int b2bodydef_type_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    const char *VALUE = luaL_checkstring(L, 2);
    if (R_str_equal(VALUE, "static")) {
        self->type = b2_staticBody;
    }
    else if (R_str_equal(VALUE, "kinematic")) {
        self->type = b2_kinematicBody;
    }
    else if (R_str_equal(VALUE, "dynamic")) {
        self->type = b2_dynamicBody;
    }
    else {
        R_LUA_DIE(L, "Unknown body type '%s'", VALUE);
    }
    return 0;
}

static int b2bodydef_position_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    R_V2 VALUE = *((R_V2 *)luaL_checkudata(L, 2, "R_V2"));
    self->position = from_v2(VALUE);
    return 0;
}

static int b2bodydef_linear_velocity_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    R_V2 VALUE = *((R_V2 *)luaL_checkudata(L, 2, "R_V2"));
    self->linearVelocity = from_v2(VALUE);
    return 0;
}

static int b2bodydef_rotation_newindex_xl(lua_State *L)
{
    b2BodyDef *self = R_CPPCAST(b2BodyDef *, XL_checkpptype(L, 1, "b2BodyDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->angle = R_to_rad(VALUE);
    return 0;
}

static int b2joint_destroyed_index_xl(lua_State *L)
{
    b2Joint *self = R_CPPCAST(b2Joint *, XL_checkpptype_nullable(L, 1, "b2Joint"));
    bool RETVAL;
    RETVAL = self;
    lua_pushboolean(L, RETVAL);
    return 1;
}


struct R_LuaB2MotorJointDef : public b2MotorJointDef {
    bool initialize_from_bodies;

    void after_properties_set(lua_State *L)
    {
        if (initialize_from_bodies) {
            if (bodyA && bodyB) {
                Initialize(bodyA, bodyB);
            }
            else {
                R_LUA_DIE(L, "Trying to initialize from bodies without "
                             "defining both body_a and body_b");
            }
        }
    }
};


static int r_luab2motorjointdef_body_a_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    b2Body *VALUE = R_CPPCAST(b2Body *, XL_checkpptype(L, 2, "b2Body"));
    self->bodyA = VALUE;
    return 0;
}

static int r_luab2motorjointdef_body_b_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    b2Body *VALUE = R_CPPCAST(b2Body *, XL_checkpptype(L, 2, "b2Body"));
    self->bodyB = VALUE;
    return 0;
}

static int r_luab2motorjointdef_collide_connected_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->collideConnected = VALUE;
    return 0;
}

static int r_luab2motorjointdef_angular_offset_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->angularOffset = VALUE;
    return 0;
}

static int r_luab2motorjointdef_max_force_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->maxForce = VALUE;
    return 0;
}

static int r_luab2motorjointdef_max_torque_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->maxTorque = VALUE;
    return 0;
}

static int r_luab2motorjointdef_correction_factor_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->correctionFactor = VALUE;
    return 0;
}

static int r_luab2motorjointdef_initialize_from_bodies_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->initialize_from_bodies = VALUE;
    return 0;
}

static int r_luab2motorjointdef_linear_offset_newindex_xl(lua_State *L)
{
    R_LuaB2MotorJointDef *self = R_CPPCAST(R_LuaB2MotorJointDef *, XL_checkpptype(L, 1, "R_LuaB2MotorJointDef"));
    R_V2 VALUE = *((R_V2 *)luaL_checkudata(L, 2, "R_V2"));
    self->linearOffset = from_v2(VALUE);
    return 0;
}


struct R_LuaB2RevoluteJointDef : public b2RevoluteJointDef {
    bool has_world_anchor;
    R_V2 world_anchor;

    void set_world_anchor(R_V2 value)
    {
        has_world_anchor = true;
        world_anchor     = value;
    }

    void after_properties_set(lua_State *L)
    {
        if (has_world_anchor) {
            if (bodyA && bodyB) {
                Initialize(bodyA, bodyB, from_v2(world_anchor));
            }
            else {
                R_LUA_DIE(L, "Trying to set world_anchor without "
                             "defining both body_a and body_b");
            }
        }
    }
};


static int r_luab2revolutejointdef_body_a_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    b2Body *VALUE = R_CPPCAST(b2Body *, XL_checkpptype(L, 2, "b2Body"));
    self->bodyA = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_body_b_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    b2Body *VALUE = R_CPPCAST(b2Body *, XL_checkpptype(L, 2, "b2Body"));
    self->bodyB = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_collide_connected_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->collideConnected = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_reference_angle_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->referenceAngle = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_enable_limit_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->enableLimit = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_lower_angle_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->lowerAngle = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_upper_angle_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->upperAngle = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_enable_motor_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    bool VALUE = XL_checkbool(L, 2);
    self->enableMotor = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_motor_speed_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->motorSpeed = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_max_motor_torque_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->maxMotorTorque = VALUE;
    return 0;
}

static int r_luab2revolutejointdef_reference_rotation_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->referenceAngle = R_to_rad(VALUE);
    return 0;
}

static int r_luab2revolutejointdef_lower_rotation_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->lowerAngle = R_to_rad(VALUE);
    return 0;
}

static int r_luab2revolutejointdef_upper_rotation_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    float VALUE = XL_checkfloat(L, 2);
    self->upperAngle = R_to_rad(VALUE);
    return 0;
}

static int r_luab2revolutejointdef_world_anchor_newindex_xl(lua_State *L)
{
    R_LuaB2RevoluteJointDef *self = R_CPPCAST(R_LuaB2RevoluteJointDef *, XL_checkpptype(L, 1, "R_LuaB2RevoluteJointDef"));
    R_V2 VALUE = *((R_V2 *)luaL_checkudata(L, 2, "R_V2"));
    self->set_world_anchor(VALUE);
    return 0;
}

static int b2_world_new_xl(lua_State *L)
{
    R_V2 gravity = *((R_V2 *)luaL_checkudata(L, 1, "R_V2"));
    b2World *RETVAL;
    RETVAL = new b2World(from_v2(gravity));
    XL_pushnewpptypeuv(L, RETVAL, "b2World", 0);
    return 1;
}

static int b2world_method_gc_xl(lua_State *L)
{
    b2World *self = R_CPPCAST(b2World *, XL_checkpptype_nullable(L, 1, "b2World"));
    delete self;
    return 0;
}

static int b2world_method_create_body_xl(lua_State *L)
{
    b2World *self = R_CPPCAST(b2World *, XL_checkpptype(L, 1, "b2World"));
    luaL_checktype(L, 2, LUA_TTABLE);
    int args = 2;
    b2Body *RETVAL;
    b2BodyDef def;
    XL_pushnewpptypeuv(L, &def, "b2BodyDef", 0);
    XL_setfromtable(L, "b2BodyDef", args + 1, args);

    lua_pop(L, 1);
    RETVAL = self->CreateBody(&def);
    XL_pushnewpptypeuv(L, RETVAL, "b2Body", 1);
    lua_pushvalue(L, 1);
    XL_setiuservalue(L, -2, 1);
    return 1;
}

static int b2world_method_destroy_body_xl(lua_State *L)
{
    b2World *self = R_CPPCAST(b2World *, XL_checkpptype(L, 1, "b2World"));
    b2Body *body = R_CPPCAST(b2Body *, XL_checkpptype(L, 2, "b2Body"));
    self->DestroyBody(body);
    XL_clearpptypeuv(L, 2, 1);
    return 0;
}

static int b2world_method_create_motor_joint_xl(lua_State *L)
{
    b2World *self = R_CPPCAST(b2World *, XL_checkpptype(L, 1, "b2World"));
    luaL_checktype(L, 2, LUA_TTABLE);
    int args = 2;
    b2Joint *RETVAL;
    R_LuaB2MotorJointDef def;
    XL_pushnewpptypeuv(L, &def, "R_LuaB2MotorJointDef", 0);
    XL_setfromtable(L, "R_LuaB2MotorJointDef", args + 1, args);

    def.after_properties_set(L);

    lua_pop(L, 1);
    RETVAL = self->CreateJoint(&def);
    XL_pushnewpptypeuv(L, RETVAL, "b2Joint", 1);
    lua_pushvalue(L, 1);
    XL_setiuservalue(L, -2, 1);
    return 1;
}

static int b2world_method_create_revolute_joint_xl(lua_State *L)
{
    b2World *self = R_CPPCAST(b2World *, XL_checkpptype(L, 1, "b2World"));
    luaL_checktype(L, 2, LUA_TTABLE);
    int args = 2;
    b2Joint *RETVAL;
    R_LuaB2RevoluteJointDef def;
    XL_pushnewpptypeuv(L, &def, "R_LuaB2RevoluteJointDef", 0);
    XL_setfromtable(L, "R_LuaB2RevoluteJointDef", args + 1, args);

    def.after_properties_set(L);

    lua_pop(L, 1);
    RETVAL = self->CreateJoint(&def);
    XL_pushnewpptypeuv(L, RETVAL, "b2Joint", 1);
    lua_pushvalue(L, 1);
    XL_setiuservalue(L, -2, 1);
    return 1;
}

static int b2world_method_destroy_joint_xl(lua_State *L)
{
    b2World *self = R_CPPCAST(b2World *, XL_checkpptype(L, 1, "b2World"));
    b2Joint *joint = R_CPPCAST(b2Joint *, XL_checkpptype(L, 2, "b2Joint"));
    self->DestroyJoint(joint);
    XL_clearpptypeuv(L, 2, 1);
    return 0;
}

static int b2world_method_step_xl(lua_State *L)
{
    b2World *self = R_CPPCAST(b2World *, XL_checkpptype(L, 1, "b2World"));
    float time_step = XL_checkfloat(L, 2);
    int velocity_iterations = XL_checkint(L, 3);
    int position_iterations = XL_checkint(L, 4);
    self->Step(time_step, velocity_iterations, position_iterations);
    return 0;
}


static NVGcolor to_nvg_color(const b2Color &color)
{
    return nvgRGBAf(color.r, color.g, color.b, color.a);
}

static NVGcolor to_halved_alpha_nvg_color(const b2Color &color)
{
    return nvgRGBAf(color.r, color.g, color.b, color.a / 2.0f);
}

static void draw_polygon_with(R_Nvg *nvg, float stroke_width,
                              const b2Vec2 *vertices, int vertex_count,
                              const b2Color &color, bool fill)
{
    if (vertex_count >= 3) {
        NVGcontext *ctx = R_nvg_context(nvg);

        nvgBeginPath(ctx);
        nvgMoveTo(ctx, vertices[0].x, vertices[0].y);
        for (int32 i = 1; i < vertex_count; ++i) {
            nvgLineTo(ctx, vertices[i].x, vertices[i].y);
        }
        nvgClosePath(ctx);

        if (fill) {
            nvgFillColor(ctx, to_halved_alpha_nvg_color(color));
            nvgFill(ctx);
        }

        nvgStrokeColor(ctx, to_nvg_color(color));
        nvgStrokeWidth(ctx, stroke_width);
        nvgStroke(ctx);
    }
}

struct R_B2DrawNvg : public b2Draw {
    R_Nvg *m_nvg;
    float m_stroke_width;

    R_B2DrawNvg(R_Nvg *nvg, float stroke_width = 1.0f) :
        m_nvg{nvg}, m_stroke_width{stroke_width} {}

    R_B2DrawNvg           (const R_B2DrawNvg &) = delete;
    R_B2DrawNvg &operator=(const R_B2DrawNvg &) = delete;

    virtual void DrawPolygon(const b2Vec2 *vertices, int32 vertex_count,
                             const b2Color &color) override
    {
        draw_polygon_with(m_nvg, m_stroke_width, vertices,
                          vertex_count, color, false);
    }

    virtual void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertex_count,
                                  const b2Color &color)
    {
        draw_polygon_with(m_nvg, m_stroke_width, vertices,
                          vertex_count, color, true);
    }

    virtual void DrawCircle(const b2Vec2 &center, float radius,
                            const b2Color &color)
    {
        NVGcontext *ctx = R_nvg_context(m_nvg);
        nvgBeginPath(ctx);
        nvgCircle(ctx, center.x, center.y, radius);
        nvgStrokeColor(ctx, to_nvg_color(color));
        nvgStrokeWidth(ctx, m_stroke_width);
        nvgStroke(ctx);
    }

    virtual void DrawSolidCircle(const b2Vec2& center, float radius,
                                 const b2Vec2& axis, const b2Color& color)
    {
        NVGcontext *ctx = R_nvg_context(m_nvg);

        nvgBeginPath(ctx);
        nvgCircle(ctx, center.x, center.y, radius);
        nvgStrokeColor(ctx, to_nvg_color(color));
        nvgStrokeWidth(ctx, m_stroke_width);
        nvgStroke(ctx);
        nvgFillColor(ctx, to_halved_alpha_nvg_color(color));
        nvgFill(ctx);

        nvgBeginPath(ctx);
        nvgMoveTo(ctx, center.x, center.y);
        b2Vec2 outer = center + radius * axis;
        nvgLineTo(ctx, outer.x, outer.y);
        nvgStrokeWidth(ctx, m_stroke_width);
        nvgStroke(ctx);
    }

    virtual void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2,
                             const b2Color& color)
    {
        NVGcontext *ctx = R_nvg_context(m_nvg);
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, p1.x, p1.y);
        nvgLineTo(ctx, p2.x, p2.y);
        nvgStrokeColor(ctx, to_nvg_color(color));
        nvgStrokeWidth(ctx, m_stroke_width);
        nvgStroke(ctx);
    }

    virtual void DrawTransform(const b2Transform &xf) override
    {
        NVGcontext *ctx = R_nvg_context(m_nvg);
        float matrix[6] = {xf.q.c, xf.q.s, -xf.q.s, xf.q.c, xf.p.x,  xf.p.y};
        R_nvg_transform(ctx, matrix);
    }

    virtual void DrawPoint(const b2Vec2 &p, float size, const b2Color& color)
    {
        NVGcontext *ctx = R_nvg_context(m_nvg);
        nvgBeginPath(ctx);
        nvgCircle(ctx, p.x, p.y, size * 0.5f);
        nvgFillColor(ctx, to_nvg_color(color));
    }
};


static int b2world_method_debug_draw_xl(lua_State *L)
{
    b2World *self = R_CPPCAST(b2World *, XL_checkpptype(L, 1, "b2World"));
    R_Nvg *nvg = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 2, "R_Nvg"));
    R_LuaNvgTransform transform = *((R_LuaNvgTransform *)luaL_checkudata(L, 3, "R_LuaNvgTransform"));
    float stroke_width = XL_checkfloat(L, 4);
    int flags = XL_checkint(L, 5);
    R_nvg_transform_set(R_nvg_context(nvg), transform.matrix);
    R_B2DrawNvg draw{nvg, stroke_width};
    draw.SetFlags(flags);
    self->SetDebugDraw(&draw);
    self->DebugDraw();
    self->SetDebugDraw(nullptr);
    return 0;
}


}

#ifdef __cplusplus
extern "C" {
#endif

static int b2body_index_dummy_xl;
static int b2body_index_xl(lua_State *L)
{
    return XL_index(L, "b2Body", &b2body_index_dummy_xl, 1, 2);
}

static int b2fixture_index_dummy_xl;
static int b2fixture_index_xl(lua_State *L)
{
    return XL_index(L, "b2Fixture", &b2fixture_index_dummy_xl, 1, 2);
}

static int b2joint_index_dummy_xl;
static int b2joint_index_xl(lua_State *L)
{
    return XL_index(L, "b2Joint", &b2joint_index_dummy_xl, 1, 2);
}

static int b2shape_index_xl(lua_State *L)
{
    return XL_index_fallback(L, "b2Shape", 1, 2);
}

static int b2world_index_xl(lua_State *L)
{
    return XL_index_fallback(L, "b2World", 1, 2);
}

static int r_luab2motorjointdef_newindex_dummy_xl;
static int r_luab2motorjointdef_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_LuaB2MotorJointDef", &r_luab2motorjointdef_newindex_dummy_xl, 1, 2, 3);
}

static int r_luab2revolutejointdef_newindex_dummy_xl;
static int r_luab2revolutejointdef_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_LuaB2RevoluteJointDef", &r_luab2revolutejointdef_newindex_dummy_xl, 1, 2, 3);
}

static int b2body_newindex_dummy_xl;
static int b2body_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "b2Body", &b2body_newindex_dummy_xl, 1, 2, 3);
}

static int b2bodydef_newindex_dummy_xl;
static int b2bodydef_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "b2BodyDef", &b2bodydef_newindex_dummy_xl, 1, 2, 3);
}

static int b2fixturedef_newindex_dummy_xl;
static int b2fixturedef_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "b2FixtureDef", &b2fixturedef_newindex_dummy_xl, 1, 2, 3);
}

static luaL_Reg b2_shape_function_registry_xl[] = {
    {"new_box", b2_shape_new_box_xl},
    {"new_circle", b2_shape_new_circle_xl},
    {"new_polygon", b2_shape_new_polygon_xl},
    {NULL, NULL},
};

static luaL_Reg b2_world_function_registry_xl[] = {
    {"new", b2_world_new_xl},
    {NULL, NULL},
};

static luaL_Reg b2body_index_registry_xl[] = {
    {"angle", b2body_angle_index_xl},
    {"destroyed", b2body_destroyed_index_xl},
    {"pos", b2body_pos_index_xl},
    {"position", b2body_position_index_xl},
    {"rotation", b2body_rotation_index_xl},
    {NULL, NULL},
};

static luaL_Reg b2fixture_index_registry_xl[] = {
    {"destroyed", b2fixture_destroyed_index_xl},
    {NULL, NULL},
};

static luaL_Reg b2joint_index_registry_xl[] = {
    {"destroyed", b2joint_destroyed_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_luab2motorjointdef_method_registry_xl[] = {
    {"__newindex", r_luab2motorjointdef_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg r_luab2revolutejointdef_method_registry_xl[] = {
    {"__newindex", r_luab2revolutejointdef_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg b2body_method_registry_xl[] = {
    {"__index", b2body_index_xl},
    {"__newindex", b2body_newindex_xl},
    {"create_fixture", b2body_method_create_fixture_xl},
    {"set_transform", b2body_method_set_transform_xl},
    {NULL, NULL},
};

static luaL_Reg b2bodydef_method_registry_xl[] = {
    {"__newindex", b2bodydef_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg b2fixture_method_registry_xl[] = {
    {"__index", b2fixture_index_xl},
    {NULL, NULL},
};

static luaL_Reg b2fixturedef_method_registry_xl[] = {
    {"__newindex", b2fixturedef_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg b2joint_method_registry_xl[] = {
    {"__index", b2joint_index_xl},
    {NULL, NULL},
};

static luaL_Reg b2shape_method_registry_xl[] = {
    {"__gc", b2shape_method_gc_xl},
    {"__index", b2shape_index_xl},
    {NULL, NULL},
};

static luaL_Reg b2world_method_registry_xl[] = {
    {"__gc", b2world_method_gc_xl},
    {"__index", b2world_index_xl},
    {"create_body", b2world_method_create_body_xl},
    {"create_motor_joint", b2world_method_create_motor_joint_xl},
    {"create_revolute_joint", b2world_method_create_revolute_joint_xl},
    {"debug_draw", b2world_method_debug_draw_xl},
    {"destroy_body", b2world_method_destroy_body_xl},
    {"destroy_joint", b2world_method_destroy_joint_xl},
    {"step", b2world_method_step_xl},
    {NULL, NULL},
};

static luaL_Reg r_luab2motorjointdef_newindex_registry_xl[] = {
    {"angular_offset", r_luab2motorjointdef_angular_offset_newindex_xl},
    {"body_a", r_luab2motorjointdef_body_a_newindex_xl},
    {"body_b", r_luab2motorjointdef_body_b_newindex_xl},
    {"collide_connected", r_luab2motorjointdef_collide_connected_newindex_xl},
    {"correction_factor", r_luab2motorjointdef_correction_factor_newindex_xl},
    {"initialize_from_bodies", r_luab2motorjointdef_initialize_from_bodies_newindex_xl},
    {"linear_offset", r_luab2motorjointdef_linear_offset_newindex_xl},
    {"max_force", r_luab2motorjointdef_max_force_newindex_xl},
    {"max_torque", r_luab2motorjointdef_max_torque_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg r_luab2revolutejointdef_newindex_registry_xl[] = {
    {"body_a", r_luab2revolutejointdef_body_a_newindex_xl},
    {"body_b", r_luab2revolutejointdef_body_b_newindex_xl},
    {"collide_connected", r_luab2revolutejointdef_collide_connected_newindex_xl},
    {"enable_limit", r_luab2revolutejointdef_enable_limit_newindex_xl},
    {"enable_motor", r_luab2revolutejointdef_enable_motor_newindex_xl},
    {"lower_angle", r_luab2revolutejointdef_lower_angle_newindex_xl},
    {"lower_rotation", r_luab2revolutejointdef_lower_rotation_newindex_xl},
    {"max_motor_torque", r_luab2revolutejointdef_max_motor_torque_newindex_xl},
    {"motor_speed", r_luab2revolutejointdef_motor_speed_newindex_xl},
    {"reference_angle", r_luab2revolutejointdef_reference_angle_newindex_xl},
    {"reference_rotation", r_luab2revolutejointdef_reference_rotation_newindex_xl},
    {"upper_angle", r_luab2revolutejointdef_upper_angle_newindex_xl},
    {"upper_rotation", r_luab2revolutejointdef_upper_rotation_newindex_xl},
    {"world_anchor", r_luab2revolutejointdef_world_anchor_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg b2body_newindex_registry_xl[] = {
    {"angular_velocity", b2body_angular_velocity_newindex_xl},
    {"linear_velocity", b2body_linear_velocity_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg b2bodydef_newindex_registry_xl[] = {
    {"allow_sleep", b2bodydef_allow_sleep_newindex_xl},
    {"angle", b2bodydef_angle_newindex_xl},
    {"angular_damping", b2bodydef_angular_damping_newindex_xl},
    {"angular_velocity", b2bodydef_angular_velocity_newindex_xl},
    {"awake", b2bodydef_awake_newindex_xl},
    {"bullet", b2bodydef_bullet_newindex_xl},
    {"enabled", b2bodydef_enabled_newindex_xl},
    {"fixed_rotation", b2bodydef_fixed_rotation_newindex_xl},
    {"gravity_scale", b2bodydef_gravity_scale_newindex_xl},
    {"linear_damping", b2bodydef_linear_damping_newindex_xl},
    {"linear_velocity", b2bodydef_linear_velocity_newindex_xl},
    {"position", b2bodydef_position_newindex_xl},
    {"rotation", b2bodydef_rotation_newindex_xl},
    {"type", b2bodydef_type_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg b2fixturedef_newindex_registry_xl[] = {
    {"category_bits", b2fixturedef_category_bits_newindex_xl},
    {"density", b2fixturedef_density_newindex_xl},
    {"friction", b2fixturedef_friction_newindex_xl},
    {"group_index", b2fixturedef_group_index_newindex_xl},
    {"is_sensor", b2fixturedef_is_sensor_newindex_xl},
    {"mask_bits", b2fixturedef_mask_bits_newindex_xl},
    {"restitution", b2fixturedef_restitution_newindex_xl},
    {"restitution_threshold", b2fixturedef_restitution_threshold_newindex_xl},
    {"shape", b2fixturedef_shape_newindex_xl},
    {NULL, NULL},
};

static XL_EnumEntry b2_draw_enum_xl[] = {
    {"SHAPE", (lua_Integer) b2Draw::e_shapeBit},
    {"JOINT", (lua_Integer) b2Draw::e_jointBit},
    {"AABB", (lua_Integer) b2Draw::e_aabbBit},
    {"PAIR", (lua_Integer) b2Draw::e_pairBit},
    {"CENTEROFMASS", (lua_Integer) b2Draw::e_centerOfMassBit},
    {NULL, (lua_Integer) 0},
};

int R_lua_box2d_init(lua_State *L)
{
    XL_initmetatable(L, "R_LuaB2MotorJointDef", r_luab2motorjointdef_method_registry_xl);
    XL_initmetatable(L, "R_LuaB2RevoluteJointDef", r_luab2revolutejointdef_method_registry_xl);
    XL_initmetatable(L, "b2Body", b2body_method_registry_xl);
    XL_initmetatable(L, "b2BodyDef", b2bodydef_method_registry_xl);
    XL_initmetatable(L, "b2Fixture", b2fixture_method_registry_xl);
    XL_initmetatable(L, "b2FixtureDef", b2fixturedef_method_registry_xl);
    XL_initmetatable(L, "b2Joint", b2joint_method_registry_xl);
    XL_initmetatable(L, "b2Shape", b2shape_method_registry_xl);
    XL_initmetatable(L, "b2World", b2world_method_registry_xl);
    XL_initindextable(L, &b2body_index_dummy_xl, b2body_index_registry_xl);
    XL_initindextable(L, &b2fixture_index_dummy_xl, b2fixture_index_registry_xl);
    XL_initindextable(L, &b2joint_index_dummy_xl, b2joint_index_registry_xl);
    XL_initnewindextable(L, &r_luab2motorjointdef_newindex_dummy_xl, r_luab2motorjointdef_newindex_registry_xl);
    XL_initnewindextable(L, &r_luab2revolutejointdef_newindex_dummy_xl, r_luab2revolutejointdef_newindex_registry_xl);
    XL_initnewindextable(L, &b2body_newindex_dummy_xl, b2body_newindex_registry_xl);
    XL_initnewindextable(L, &b2bodydef_newindex_dummy_xl, b2bodydef_newindex_registry_xl);
    XL_initnewindextable(L, &b2fixturedef_newindex_dummy_xl, b2fixturedef_newindex_registry_xl);
    XL_initfunctions(L, b2_shape_function_registry_xl, "B2", "Shape", (const char *)NULL);
    XL_initfunctions(L, b2_world_function_registry_xl, "B2", "World", (const char *)NULL);
    XL_initenum(L, b2_draw_enum_xl, "B2", "Draw", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
