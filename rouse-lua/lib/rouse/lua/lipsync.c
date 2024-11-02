/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
/*
 * Copyright (c) 2022 askmeaboutloom
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
#include <pocketsphinx.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"


typedef struct R_LuaLipSync {
    ps_decoder_t      *ps;
    FILE              *fp;
    SDL_AudioDeviceID dev;
    int               silence_delay;
    int               silences;
    SDL_atomic_t      phone;
} R_LuaLipSync;

typedef enum R_LuaPhone {
    R_LUA_PHONE_NONE,
    R_LUA_PHONE_SIL,
    R_LUA_PHONE_AO,
    R_LUA_PHONE_AA,
    R_LUA_PHONE_IY,
    R_LUA_PHONE_UW,
    R_LUA_PHONE_EH,
    R_LUA_PHONE_IH,
    R_LUA_PHONE_UH,
    R_LUA_PHONE_AH,
    R_LUA_PHONE_AE,
    R_LUA_PHONE_EY,
    R_LUA_PHONE_AY,
    R_LUA_PHONE_OW,
    R_LUA_PHONE_AW,
    R_LUA_PHONE_OY,
    R_LUA_PHONE_ER,
    R_LUA_PHONE_P,
    R_LUA_PHONE_B,
    R_LUA_PHONE_T,
    R_LUA_PHONE_D,
    R_LUA_PHONE_K,
    R_LUA_PHONE_G,
    R_LUA_PHONE_CH,
    R_LUA_PHONE_JH,
    R_LUA_PHONE_F,
    R_LUA_PHONE_V,
    R_LUA_PHONE_TH,
    R_LUA_PHONE_DH,
    R_LUA_PHONE_S,
    R_LUA_PHONE_Z,
    R_LUA_PHONE_SH,
    R_LUA_PHONE_ZH,
    R_LUA_PHONE_HH,
    R_LUA_PHONE_M,
    R_LUA_PHONE_N,
    R_LUA_PHONE_NG,
    R_LUA_PHONE_L,
    R_LUA_PHONE_R,
    R_LUA_PHONE_Y,
    R_LUA_PHONE_W,
    R_LUA_PHONE_NOISE_NSN,
    R_LUA_PHONE_NOISE_SPN,
} R_LuaPhone;


static int new_ps(lua_State *L)
{
    lua_getfield(L, 1, "args");
    const char *args = lua_tostring(L, -1);
    if (!args) {
        R_LUA_DIE(L, "ps_args.args is not a string");
    }

    ps_config_t *config = ps_config_parse_json(NULL, args);
    lua_pop(L, 1);
    if (!config) {
        R_LUA_DIE(L, "Error parsing arguments");
    }

    const char *json = ps_config_serialize_json(config);
    if (json) {
        R_info("Config: %s", json);
    }

    ps_decoder_t *ps = ps_init(config);
    ps_config_free(config);
    if (!ps) {
        R_LUA_DIE(L, "Error creating Pocketsphinx decoder");
    }

    lua_pushlightuserdata(L, ps);
    return 1;
}

static void free_lua_lip_sync(R_LuaLipSync *lls)
{
    if (lls->dev != 0) {
        SDL_CloseAudioDevice(lls->dev);
    }
    if (lls->fp) {
        fclose(lls->fp);
    }
    if (lls->ps) {
        ps_free(lls->ps);
    }
    free(lls);
}


static int set_up_ps(lua_State *L)
{
    ps_decoder_t *ps = lua_touserdata(L, 1);

    lua_getfield(L, 2, "dump_search");
    if (lua_toboolean(L, -1)) {
        int i = 0;
        ps_search_iter_t *iter = ps_search_iter(ps);
        while (iter) {
            ++i;
            R_info("Pocketsphinx search %d: '%s'", i, ps_search_iter_val(iter));
            iter = ps_search_iter_next(iter);
        }
        R_info("Dumped %d Pocketsphinx search(es)", i);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "search");
    const char *search = lua_tostring(L, -1);
    if (!search) {
        R_LUA_DIE(L, "ps_args.search is not a string");
    }
    if (ps_activate_search(ps, search) != 0) {
        R_LUA_DIE(L, "Error setting Pocketsphinx search to '%s'", search);
    }
    lua_pop(L, 1);

    if (ps_start_stream(ps) != 0) {
        R_LUA_DIE(L, "Error starting Pocketsphinx stream");
    }

    if (ps_start_utt(ps) != 0) {
        R_LUA_DIE(L, "Error starting Pocketsphinx utterance");
    }

    return 0;
}


static int open_fp(lua_State *L)
{
    FILE *fp;
    if (lua_getfield(L, 1, "dump_audio_path")) {
        const char *dump_audio_path = luaL_checkstring(L, -1);
        fp = fopen(dump_audio_path, "wb");
        if (!fp) {
            R_LUA_DIE(L, "Can't open '%s': %s", dump_audio_path,
                      strerror(errno));
        }
    }
    else {
        fp = NULL;
    }
    lua_pushlightuserdata(L, fp);
    return 1;
}


#define STRING_TO_LUA_PHONE(INPUT, EXPECTED, PHONE) do { \
        if (R_str_equal(INPUT, EXPECTED)) { \
            return PHONE; \
        } \
    } while (0)

static int to_lua_phone(const char *phone)
{
    STRING_TO_LUA_PHONE(phone, "SIL",   R_LUA_PHONE_SIL      );
    STRING_TO_LUA_PHONE(phone, "AO",    R_LUA_PHONE_AO       );
    STRING_TO_LUA_PHONE(phone, "AA",    R_LUA_PHONE_AA       );
    STRING_TO_LUA_PHONE(phone, "IY",    R_LUA_PHONE_IY       );
    STRING_TO_LUA_PHONE(phone, "UW",    R_LUA_PHONE_UW       );
    STRING_TO_LUA_PHONE(phone, "EH",    R_LUA_PHONE_EH       );
    STRING_TO_LUA_PHONE(phone, "IH",    R_LUA_PHONE_IH       );
    STRING_TO_LUA_PHONE(phone, "UH",    R_LUA_PHONE_UH       );
    STRING_TO_LUA_PHONE(phone, "AH",    R_LUA_PHONE_AH       );
    STRING_TO_LUA_PHONE(phone, "AE",    R_LUA_PHONE_AE       );
    STRING_TO_LUA_PHONE(phone, "EY",    R_LUA_PHONE_EY       );
    STRING_TO_LUA_PHONE(phone, "AY",    R_LUA_PHONE_AY       );
    STRING_TO_LUA_PHONE(phone, "OW",    R_LUA_PHONE_OW       );
    STRING_TO_LUA_PHONE(phone, "AW",    R_LUA_PHONE_AW       );
    STRING_TO_LUA_PHONE(phone, "OY",    R_LUA_PHONE_OY       );
    STRING_TO_LUA_PHONE(phone, "ER",    R_LUA_PHONE_ER       );
    STRING_TO_LUA_PHONE(phone, "P",     R_LUA_PHONE_P        );
    STRING_TO_LUA_PHONE(phone, "B",     R_LUA_PHONE_B        );
    STRING_TO_LUA_PHONE(phone, "T",     R_LUA_PHONE_T        );
    STRING_TO_LUA_PHONE(phone, "D",     R_LUA_PHONE_D        );
    STRING_TO_LUA_PHONE(phone, "K",     R_LUA_PHONE_K        );
    STRING_TO_LUA_PHONE(phone, "G",     R_LUA_PHONE_G        );
    STRING_TO_LUA_PHONE(phone, "CH",    R_LUA_PHONE_CH       );
    STRING_TO_LUA_PHONE(phone, "JH",    R_LUA_PHONE_JH       );
    STRING_TO_LUA_PHONE(phone, "F",     R_LUA_PHONE_F        );
    STRING_TO_LUA_PHONE(phone, "V",     R_LUA_PHONE_V        );
    STRING_TO_LUA_PHONE(phone, "TH",    R_LUA_PHONE_TH       );
    STRING_TO_LUA_PHONE(phone, "DH",    R_LUA_PHONE_DH       );
    STRING_TO_LUA_PHONE(phone, "S",     R_LUA_PHONE_S        );
    STRING_TO_LUA_PHONE(phone, "Z",     R_LUA_PHONE_Z        );
    STRING_TO_LUA_PHONE(phone, "SH",    R_LUA_PHONE_SH       );
    STRING_TO_LUA_PHONE(phone, "ZH",    R_LUA_PHONE_ZH       );
    STRING_TO_LUA_PHONE(phone, "HH",    R_LUA_PHONE_HH       );
    STRING_TO_LUA_PHONE(phone, "M",     R_LUA_PHONE_M        );
    STRING_TO_LUA_PHONE(phone, "N",     R_LUA_PHONE_N        );
    STRING_TO_LUA_PHONE(phone, "NG",    R_LUA_PHONE_NG       );
    STRING_TO_LUA_PHONE(phone, "L",     R_LUA_PHONE_L        );
    STRING_TO_LUA_PHONE(phone, "R",     R_LUA_PHONE_R        );
    STRING_TO_LUA_PHONE(phone, "Y",     R_LUA_PHONE_Y        );
    STRING_TO_LUA_PHONE(phone, "W",     R_LUA_PHONE_W        );
    STRING_TO_LUA_PHONE(phone, "+NSN+", R_LUA_PHONE_NOISE_NSN);
    STRING_TO_LUA_PHONE(phone, "+SPN+", R_LUA_PHONE_NOISE_SPN);
    R_warn("Unrecognized phoneme '%s'", phone);
    return R_LUA_PHONE_NONE;
}

#undef STRING_TO_LUA_PHONE

static void on_capture(void *user, Uint8 *stream, int len)
{
    if (len > 0) {
        R_LuaLipSync *lls = user;
        ps_decoder_t *ps  = lls->ps;

        int processed =
                ps_process_raw(ps, (int16 *)stream, (size_t)(len / 2), 0, 0);
        (void)processed;

        const char *guess = NULL;
        for (ps_seg_t *seg = ps_seg_iter(ps); seg; seg = ps_seg_next(seg)) {
            guess = ps_seg_word(seg);
        }

        if (guess) {
            SDL_AtomicSet(&lls->phone, to_lua_phone(guess));
            lls->silences = 0;
            if (ps_end_utt(ps) != 0) {
                R_warn("Error ending utterance");
            }
            if (ps_start_utt(ps) != 0) {
                R_warn("Error starting utterance");
            }
        }
        else if (lls->silences < lls->silence_delay) {
            ++lls->silences;
        }
        else {
            SDL_AtomicSet(&lls->phone, R_LUA_PHONE_NONE);
        }

        FILE *fp = lls->fp;
        if (fp) {
            fwrite(stream, 1, R_int2size(len), fp);
        }
    }
}

static int open_dev(lua_State *L)
{
    if (!SDL_WasInit(SDL_INIT_AUDIO) && SDL_Init(SDL_INIT_AUDIO) != 0) {
        R_LUA_DIE(L, "Error initializing SDL audio: %s", SDL_GetError());
    }

    const char *device_name = NULL;
    if (lua_getfield(L, 2, "device_name")) {
        device_name = lua_tostring(L, -1);
        if (!device_name) {
            R_LUA_DIE(L, "dev_args.device_name is not a string");
        }
    }

    lua_Integer freq;
    {
        lua_getfield(L, 2, "freq");
        int freq_isnum;
        freq = lua_tointegerx(L, -1, &freq_isnum);
        if (!freq_isnum) {
            R_LUA_DIE(L, "dev_args.freq is not an integer");
        }
        lua_pop(L, 1);
    }

    lua_Integer samples;
    {
        lua_getfield(L, 2, "samples");
        int samples_isnum;
        samples = lua_tointegerx(L, -1, &samples_isnum);
        if (!samples_isnum || samples < 0) {
            R_LUA_DIE(L, "dev_args.samples is not a positive integer");
        }
        lua_pop(L, 1);
    }

    lua_Integer silence_delay;
    {
        lua_getfield(L, 2, "silence_delay");
        int silence_delay_isnum;
        silence_delay = lua_tointegerx(L, -1, &silence_delay_isnum);
        if (!silence_delay_isnum) {
            R_LUA_DIE(L, "dev_args.silence_delay is not an integer");
        }
        lua_pop(L, 1);
    }

    SDL_AudioSpec want;
    SDL_zero(want);
    want.freq     = (int)freq;
    want.format   = AUDIO_S16SYS;
    want.channels = 1;
    want.samples  = (Uint16)samples;
    want.callback = on_capture;
    want.userdata = lua_touserdata(L, 1);

    SDL_AudioSpec got;
    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(device_name, 1, &want, &got, 0);
    if (dev <= 0) {
        if (device_name) {
            R_LUA_DIE(L, "Error opening audio device '%s': %s\n",
                      device_name, SDL_GetError());
        }
        else {
            R_LUA_DIE(L, "Error opening default audio device: %s\n",
                      SDL_GetError());
        }
        return 0;
    }

    lua_pushinteger(L, (lua_Integer)dev);
    lua_pushinteger(L, silence_delay);
    return 2;
}

static int make_lua_lip_sync(lua_State *L)
{
    R_LuaLipSync *lls = lua_touserdata(L, 1);

    lua_pushcfunction(L, new_ps);
    lua_pushvalue(L, 2);
    lua_call(L, 1, 1);
    lls->ps = lua_touserdata(L, -1);
    lua_pop(L, 1);

    lua_pushcfunction(L, set_up_ps);
    lua_pushlightuserdata(L, lls->ps);
    lua_pushvalue(L, 2);
    lua_call(L, 2, 0);

    lua_pushcfunction(L, open_fp);
    lua_pushvalue(L, 3);
    lua_call(L, 1, 1);
    lls->fp = lua_touserdata(L, -1);
    lua_pop(L, 1);

    lua_pushcfunction(L, open_dev);
    lua_pushvalue(L, 1);
    lua_pushvalue(L, 3);
    lua_call(L, 2, 2);
    lls->dev           = (SDL_AudioDeviceID)lua_tointeger(L, -2);
    lls->silence_delay = (int)lua_tointeger(L, -1);
    lua_pop(L, 2);

    return 0;
}



static int r_lipsync_new_xl(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    int ps_args = 1;
    luaL_checktype(L, 2, LUA_TTABLE);
    int dev_args = 2;
    R_LuaLipSync *RETVAL;
    RETVAL = R_NEW_INIT_STRUCT(RETVAL, R_LuaLipSync,
                               NULL, NULL, 0, 0, 0, {R_LUA_PHONE_NONE});
    lua_pushcfunction(L, make_lua_lip_sync);
    lua_pushlightuserdata(L, RETVAL);
    lua_pushvalue(L, ps_args);
    lua_pushvalue(L, dev_args);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
        free_lua_lip_sync(RETVAL);
        return lua_error(L);
    }
    XL_pushnewpptypeuv(L, RETVAL, "R_LuaLipSync", 0);
    return 1;
}

static int r_lualipsync_method_gc_xl(lua_State *L)
{
    R_LuaLipSync *self = R_CPPCAST(R_LuaLipSync *, XL_checkpptype_nullable(L, 1, "R_LuaLipSync"));
    free_lua_lip_sync(self);
    return 0;
}

static int r_lualipsync_phone_index_xl(lua_State *L)
{
    R_LuaLipSync *self = R_CPPCAST(R_LuaLipSync *, XL_checkpptype(L, 1, "R_LuaLipSync"));
    int phone = SDL_AtomicGet(&self->phone);
    switch (phone) {
        case R_LUA_PHONE_NONE:
            lua_pushnil(L);
            break;
        case R_LUA_PHONE_SIL:
            lua_pushliteral(L, "SIL");
            break;
        case R_LUA_PHONE_AO:
            lua_pushliteral(L, "AO");
            break;
        case R_LUA_PHONE_AA:
            lua_pushliteral(L, "AA");
            break;
        case R_LUA_PHONE_IY:
            lua_pushliteral(L, "IY");
            break;
        case R_LUA_PHONE_UW:
            lua_pushliteral(L, "UW");
            break;
        case R_LUA_PHONE_EH:
            lua_pushliteral(L, "EH");
            break;
        case R_LUA_PHONE_IH:
            lua_pushliteral(L, "IH");
            break;
        case R_LUA_PHONE_UH:
            lua_pushliteral(L, "UH");
            break;
        case R_LUA_PHONE_AH:
            lua_pushliteral(L, "AH");
            break;
        case R_LUA_PHONE_AE:
            lua_pushliteral(L, "AE");
            break;
        case R_LUA_PHONE_EY:
            lua_pushliteral(L, "EY");
            break;
        case R_LUA_PHONE_AY:
            lua_pushliteral(L, "AY");
            break;
        case R_LUA_PHONE_OW:
            lua_pushliteral(L, "OW");
            break;
        case R_LUA_PHONE_AW:
            lua_pushliteral(L, "AW");
            break;
        case R_LUA_PHONE_OY:
            lua_pushliteral(L, "OY");
            break;
        case R_LUA_PHONE_ER:
            lua_pushliteral(L, "ER");
            break;
        case R_LUA_PHONE_P:
            lua_pushliteral(L, "P");
            break;
        case R_LUA_PHONE_B:
            lua_pushliteral(L, "B");
            break;
        case R_LUA_PHONE_T:
            lua_pushliteral(L, "T");
            break;
        case R_LUA_PHONE_D:
            lua_pushliteral(L, "D");
            break;
        case R_LUA_PHONE_K:
            lua_pushliteral(L, "K");
            break;
        case R_LUA_PHONE_G:
            lua_pushliteral(L, "G");
            break;
        case R_LUA_PHONE_CH:
            lua_pushliteral(L, "CH");
            break;
        case R_LUA_PHONE_JH:
            lua_pushliteral(L, "JH");
            break;
        case R_LUA_PHONE_F:
            lua_pushliteral(L, "F");
            break;
        case R_LUA_PHONE_V:
            lua_pushliteral(L, "V");
            break;
        case R_LUA_PHONE_TH:
            lua_pushliteral(L, "TH");
            break;
        case R_LUA_PHONE_DH:
            lua_pushliteral(L, "DH");
            break;
        case R_LUA_PHONE_S:
            lua_pushliteral(L, "S");
            break;
        case R_LUA_PHONE_Z:
            lua_pushliteral(L, "Z");
            break;
        case R_LUA_PHONE_SH:
            lua_pushliteral(L, "SH");
            break;
        case R_LUA_PHONE_ZH:
            lua_pushliteral(L, "ZH");
            break;
        case R_LUA_PHONE_HH:
            lua_pushliteral(L, "HH");
            break;
        case R_LUA_PHONE_M:
            lua_pushliteral(L, "M");
            break;
        case R_LUA_PHONE_N:
            lua_pushliteral(L, "N");
            break;
        case R_LUA_PHONE_NG:
            lua_pushliteral(L, "NG");
            break;
        case R_LUA_PHONE_L:
            lua_pushliteral(L, "L");
            break;
        case R_LUA_PHONE_R:
            lua_pushliteral(L, "R");
            break;
        case R_LUA_PHONE_Y:
            lua_pushliteral(L, "Y");
            break;
        case R_LUA_PHONE_W:
            lua_pushliteral(L, "W");
            break;
        case R_LUA_PHONE_NOISE_NSN:
            lua_pushliteral(L, "+NSN+");
            break;
        case R_LUA_PHONE_NOISE_SPN:
            lua_pushliteral(L, "+SPN+");
            break;
        default:
            R_warn("Unknown phone %d", phone);
            lua_pushnil(L);
            break;
    }
    return 1;
}

static int r_lualipsync_method_start_xl(lua_State *L)
{
    R_LuaLipSync *self = R_CPPCAST(R_LuaLipSync *, XL_checkpptype(L, 1, "R_LuaLipSync"));
    SDL_PauseAudioDevice(self->dev, 0);
    return 0;
}

static int r_lualipsync_method_pause_xl(lua_State *L)
{
    R_LuaLipSync *self = R_CPPCAST(R_LuaLipSync *, XL_checkpptype(L, 1, "R_LuaLipSync"));
    int i = lua_gettop(L) == 1 ? 1 : luaL_checkinteger(L, 2);
    SDL_PauseAudioDevice(self->dev, i);
    return 0;
}

static int r_lualipsync_method_process_xl(lua_State *L)
{
    R_LuaLipSync *self = R_CPPCAST(R_LuaLipSync *, XL_checkpptype(L, 1, "R_LuaLipSync"));
    static unsigned char *buffer = NULL;
    static size_t capacity = 0;
    if(!buffer) {
        capacity = (size_t)1049000ULL;
        buffer = calloc(capacity, 1);
        fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    }

    ssize_t got = read(0, buffer, capacity);
    if (got == -1) {
        int e = errno;
        if (e != EAGAIN && e != EWOULDBLOCK) {
            R_warn("Lipsync process error: %s", strerror(e));
        }
    } else {
        on_capture(self, buffer, (int)got);
    }
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

static int r_lualipsync_index_anchor_xl;
static int r_lualipsync_index_xl(lua_State *L)
{
    return XL_index(L, "R_LuaLipSync", &r_lualipsync_index_anchor_xl, 1, 2);
}

static luaL_Reg r_lipsync_function_registry_xl[] = {
    {"new", r_lipsync_new_xl},
    {NULL, NULL},
};

static luaL_Reg r_lualipsync_index_registry_xl[] = {
    {"phone", r_lualipsync_phone_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_lualipsync_method_registry_xl[] = {
    {"__gc", r_lualipsync_method_gc_xl},
    {"__index", r_lualipsync_index_xl},
    {"pause", r_lualipsync_method_pause_xl},
    {"process", r_lualipsync_method_process_xl},
    {"start", r_lualipsync_method_start_xl},
    {NULL, NULL},
};

int R_lua_lipsync_init(lua_State *L)
{
    XL_initmetatable(L, "R_LuaLipSync", r_lualipsync_method_registry_xl);
    XL_initindextable(L, &r_lualipsync_index_anchor_xl, r_lualipsync_index_registry_xl);
    XL_initfunctions(L, r_lipsync_function_registry_xl, "R", "LipSync", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
