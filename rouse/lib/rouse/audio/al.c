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
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <assert.h>
#include <rouse_config.h>
#include <cglm/struct.h>
#include <SDL2/SDL.h>
#include "../common.h"
#include "../geom.h"
#include "sample.h"
#include "../refcount.h"
#include "../main.h"
#include "al.h"

#define R_AL_DEFINE_FALLBACKS
#include "../3rdparty/openal_inc.h"

struct R_AlBuffer {
    R_MAGIC_FIELD
    int          refs;
    unsigned int id;
};

struct R_AlSource {
    R_MAGIC_FIELD
    unsigned int id;
    R_AlBuffer   *buffer;
};

/*
 * Emscripten links OpenAL functions at compile time, all other
 * platforms link it at runtime using dlsym or similar mechanisms.
 */
#ifdef __EMSCRIPTEN__

static bool load_openal(void)
{
    return true;
}

#else

#include "../dl.h"

#define R_AL_PROC_X(TYPE, NAME) TYPE R_ ## NAME = R_ ## NAME ## _fallback;
R_AL_PROC_LIST
#undef R_AL_PROC_X

#define GET_AL_PROC(HANDLE, TYPE, NAME) do { \
        void *_proc = R_DL_SYM(HANDLE, #NAME); \
        if (_proc) { \
            R_debug("got proc address: " #TYPE " " #NAME " at %p", _proc); \
            R_DL_ASSIGN(R_ ## NAME, _proc); \
        } \
        else { \
            R_warn("did not find proc address for " #TYPE " " #NAME); \
        } \
    } while (0)

static void load_openal_functions(void *handle)
{
    R_debug("populating al proc function pointers");
#   define R_AL_PROC_X(TYPE, NAME) GET_AL_PROC(handle, TYPE, NAME);
    R_AL_PROC_LIST
#   undef R_AL_PROC_X
}

static bool load_openal(void)
{
    void *handle = R_DL_OPEN("openal");
    if (handle) {
        load_openal_functions(handle);
        return true;
    }
    else {
        R_warn("Can't link openal: %s", R_DL_ERROR());
        return false;
    }
}

#endif


static ALCdevice *init_al_device(R_AlGetDeviceNameHook get_device_name,
                                 void *user)
{
    const char *name = get_device_name ? get_device_name(user) : NULL;
    ALCdevice *device;
    R_AL_ASSIGN(device, alcOpenDevice, name);
    return device;
}

static ALCcontext *init_al_context(
    ALCdevice *device, R_AlGetContextAttributesHook get_context_attributes,
    void *user)
{
    const int *attributes = get_context_attributes
                          ? get_context_attributes(user)
                          : NULL;
    ALCcontext *context;
    R_AL_ASSIGN(context, alcCreateContext, device, attributes);
    R_AL_CHECK(alcMakeContextCurrent, context);
    return context;
}


static const char *get_al_string(int name)
{
    const char *value = R_AL(alGetString)(name);
    R_AL_CLEAR_ERROR();
    return value ? value : "(NULL)";
}

static void dump_al_info(void)
{
#define R_AL_DUMP_STRING(LOG, NAME) LOG(#NAME ": %s", get_al_string(NAME))
    R_AL_DUMP_STRING(R_info,  AL_VENDOR);
    R_AL_DUMP_STRING(R_info,  AL_RENDERER);
    R_AL_DUMP_STRING(R_info,  AL_VERSION);
    R_AL_DUMP_STRING(R_debug, AL_EXTENSIONS);
#undef R_AL_DUMP_STRING
}


static bool       initialized = false;
static ALCdevice  *device     = NULL;
static ALCcontext *context    = NULL;

bool R_al_init(R_AlGetDeviceNameHook        get_device_name,
               R_AlGetContextAttributesHook get_context_attributes,
               void                         *user)
{
    if (initialized) {
        R_die("Attempt to call R_al_init twice");
    }
    initialized = true;

    if (load_openal()) {
        device  = init_al_device(get_device_name, user);
        context = init_al_context(device, get_context_attributes, user);
        dump_al_info();
        return true;
    }
    else {
        R_al_deinit();
        return false;
    }
}


static void deinit_context(void)
{
    R_AL_CHECK_WARN(alcMakeContextCurrent, NULL);
    R_AL(alcDestroyContext)(context); /* returns void, so I guess it can't fail */
    context = NULL;
}

static void deinit_device(void)
{
    R_AL_CHECK_WARN(alcCloseDevice, device);
    device = NULL;
}

void R_al_deinit(void)
{
    if (context) {
        deinit_context();
    }
    if (device) {
        deinit_device();
    }
}


void R_al_die(int err, const char *where)
{
    R_die("OpenAL error %d (%s) in %s", err,
          R_al_strerror(err), where ? where : "");
}

void R_al_warn(int err, const char *where)
{
    R_warn("OpenAL error %d (%s) in %s", err,
           R_al_strerror(err), where ? where : "");
}


const char *R_al_strerror(int err)
{
    switch (err) {
        case AL_NO_ERROR:
            return "AL_NO_ERROR";
        case AL_INVALID_NAME:
            return "AL_INVALID_NAME";
        case AL_INVALID_ENUM:
            return "AL_INVALID_ENUM";
        case AL_INVALID_VALUE:
            return "AL_INVALID_VALUE";
        case AL_INVALID_OPERATION:
            return "AL_INVALID_OPERATION";
        case AL_OUT_OF_MEMORY:
            return "AL_OUT_OF_MEMORY";
        default:
            return "unknown AL error";
    }
}


R_V3 R_al_listener_position(void)
{
    R_V3 position;
    R_AL_CHECK_VOID(alGetListener3f, AL_POSITION,
                    &position.x, &position.y, &position.z);
    return position;
}

R_V3 R_al_listener_velocity(void)
{
    R_V3 velocity;
    R_AL_CHECK_VOID(alGetListener3f, AL_VELOCITY,
                    &velocity.x, &velocity.y, &velocity.z);
    return velocity;
}

R_AlOrientation R_al_listener_orientation(void)
{
    float orientation[6];
    R_AL_CHECK_VOID(alListenerfv, AL_ORIENTATION, orientation);
    return (R_AlOrientation){
        R_v3(orientation[0], orientation[1], orientation[2]),
        R_v3(orientation[3], orientation[4], orientation[5])};
}

float R_al_listener_gain(void)
{
    float gain;
    R_AL_CHECK_VOID(alGetListenerf, AL_GAIN, &gain);
    return gain;
}


void R_al_listener_position_set(R_V3 position)
{
    R_AL_CHECK_VOID(alListener3f, AL_POSITION,
                    position.x, position.y, position.z);
}

void R_al_listener_velocity_set(R_V3 velocity)
{
    R_AL_CHECK_VOID(alListener3f, AL_VELOCITY,
                    velocity.x, velocity.y, velocity.z);
}

void R_al_listener_orientation_set(R_AlOrientation orientation)
{
    R_AL_CHECK_VOID(alListenerfv, AL_ORIENTATION, (float[6]){
            orientation.front.x, orientation.front.y, orientation.front.z,
            orientation.up   .x, orientation.up   .y, orientation.up   .z});
}

void R_al_listener_gain_set(float gain)
{
    R_AL_CHECK_VOID(alListenerf, AL_GAIN, gain);
}


static inline void check_al_buffer(
    R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_AlBuffer *buffer)
{
    R_MAGIC_CHECK(R_AlBuffer, buffer);
    R_assert(buffer->refs > 0, "refcount must always be positive");
}

static R_AlBuffer *make_al_buffer(unsigned int id)
{
    R_AlBuffer *buffer = R_NEW_INIT_STRUCT(buffer, R_AlBuffer,
            R_MAGIC_INIT(R_AlBuffer) 1, id);
    check_al_buffer(buffer);
    return buffer;
}

R_AlBuffer *R_al_buffer_from_sample(R_Sample *sample)
{
    R_MAGIC_CHECK(R_Sample, sample);

    int format = R_sample_al_format(sample);
    if (format == 0) {
        R_die("Unknown format for sample with %d channels", sample->channels);
    }

    R_AL_CLEAR_ERROR();

    unsigned int buffer_id;
    R_AL_CHECK_VOID(alGenBuffers, 1, &buffer_id);

    int size = R_sample_al_buffer_size(sample);
    R_AL_CHECK_VOID(alBufferData, buffer_id, format,
                    sample->data, size, sample->rate);

    return make_al_buffer(buffer_id);
}

R_AlBuffer *R_al_buffer_from_file(const char *path)
{
    R_Sample   *sample = R_sample_from_file(path);
    R_AlBuffer *buffer = R_al_buffer_from_sample(sample);
    R_sample_free(sample);
    return buffer;
}

static void free_al_buffer(R_AlBuffer *buffer)
{
    R_AL_CLEAR_ERROR();
    R_AL_CHECK_VOID(alDeleteBuffers, 1, &buffer->id);
    free(buffer);
}

R_DEFINE_REFCOUNT_FUNCS(R_AlBuffer, al_buffer, refs)

unsigned int R_al_buffer_id(R_AlBuffer *buffer)
{
    check_al_buffer(buffer);
    return buffer->id;
}


R_AlSource *R_al_source_new(void)
{
    R_AL_CLEAR_ERROR();

    unsigned int source_id;
    R_AL_CHECK_VOID(alGenSources, 1, &source_id);

    R_AlSource *source = R_NEW_INIT_STRUCT(source, R_AlSource,
            R_MAGIC_INIT(R_AlSource) source_id, NULL);
    R_MAGIC_CHECK(R_AlSource, source);
    return source;
}

R_AlSource *R_al_source_new_from_buffer(R_AlBuffer *buffer)
{
    R_AlSource *source = R_al_source_new();
    R_al_source_buffer_set(source, buffer);
    return source;
}

R_AlSource *R_al_source_new_from_buffer_noinc(R_AlBuffer *buffer)
{
    R_AlSource *source = R_al_source_new();
    R_al_source_buffer_set_noinc(source, buffer);
    return source;
}

R_AlSource *R_al_source_new_from_file(const char *path)
{
    R_AlBuffer *buffer = R_al_buffer_from_file(path);
    return R_al_source_new_from_buffer_noinc(buffer);
}

void R_al_source_free(R_AlSource *source)
{
    if (source) {
        R_MAGIC_CHECK(R_AlSource, source);
        R_AL_CLEAR_ERROR();
        R_AL_CHECK_VOID(alDeleteSources, 1, &source->id);
        R_al_buffer_decref(source->buffer);
        free(source);
    }
}

unsigned int R_al_source_id(R_AlSource *source)
{
    R_MAGIC_CHECK(R_AlSource, source);
    return source->id;
}

R_AlBuffer *R_al_source_buffer(R_AlSource *source)
{
    R_MAGIC_CHECK(R_AlSource, source);
    return source->buffer;
}

void R_al_source_buffer_set(R_AlSource *source, R_AlBuffer *buffer)
{
    R_MAGIC_CHECK(R_AlSource, source);

    R_AlBuffer *prev_buffer = source->buffer;
    if (prev_buffer == buffer) {
        return;
    }
    else if (prev_buffer) {
        R_al_source_stop(source);
    }

    int buffer_id;
    if (buffer) {
        buffer_id = R_uint2int(R_al_buffer_id(buffer));
        R_al_buffer_incref(buffer);
    }
    else {
        buffer_id = 0;
    }

    R_AL_CLEAR_ERROR();
    R_AL_CHECK_VOID(alSourcei, source->id, AL_BUFFER, buffer_id);
    source->buffer = buffer;

    if (prev_buffer) {
        R_al_buffer_decref(prev_buffer);
    }
}

void R_al_source_buffer_set_noinc(R_AlSource *source, R_AlBuffer *buffer)
{
    R_al_source_buffer_set(source, buffer);
    R_al_buffer_decref(buffer);
}


R_V3 R_al_source_position(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    R_V3         position;
    R_AL_CHECK_VOID(alGetSource3f, source_id, AL_POSITION,
                    &position.x, &position.y, &position.z);
    return position;
}

R_V3 R_al_source_velocity(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    R_V3         velocity;
    R_AL_CHECK_VOID(alGetSource3f, source_id, AL_VELOCITY,
                    &velocity.x, &velocity.y, &velocity.z);
    return velocity;
}

float R_al_source_gain(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    float        gain;
    R_AL_CHECK_VOID(alGetSourcef, source_id, AL_GAIN, &gain);
    return gain;
}

float R_al_source_pitch(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    float        pitch;
    R_AL_CHECK_VOID(alGetSourcef, source_id, AL_PITCH, &pitch);
    return pitch;
}

float R_al_source_reference_distance(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    float        distance;
    R_AL_CHECK_VOID(alGetSourcef, source_id, AL_REFERENCE_DISTANCE, &distance);
    return distance;
}

float R_al_source_offset_in_seconds(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    float        seconds;
    R_AL_CHECK_VOID(alGetSourcef, source_id, AL_SEC_OFFSET, &seconds);
    return seconds;
}

bool R_al_source_looping(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    int          looping;
    R_AL_CHECK_VOID(alGetSourcei, source_id, AL_LOOPING, &looping);
    return looping != 0;
}


void R_al_source_position_set(R_AlSource *source, R_V3 position)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSource3f, source_id, AL_POSITION,
                    position.x, position.y, position.z);
}

void R_al_source_velocity_set(R_AlSource *source, R_V3 velocity)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSource3f, source_id, AL_VELOCITY,
                    velocity.x, velocity.y, velocity.z);
}

void R_al_source_gain_set(R_AlSource *source, float gain)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourcef, source_id, AL_GAIN, gain);
}

void R_al_source_pitch_set(R_AlSource *source, float pitch)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourcef, source_id, AL_PITCH, pitch);
}

void R_al_source_reference_distance_set(R_AlSource *source, float distance)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourcef, source_id, AL_REFERENCE_DISTANCE, distance);
}

void R_al_source_offset_in_seconds_set(R_AlSource *source, float seconds)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourcef, source_id, AL_SEC_OFFSET, seconds);
}

void R_al_source_looping_set(R_AlSource *source, bool looping)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourcei, source_id, AL_LOOPING, looping);
}


void R_al_source_play(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourcePlay, source_id);
}

void R_al_source_stop(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourceStop, source_id);
}

void R_al_source_pause(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourcePause, source_id);
}

void R_al_source_rewind(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    R_AL_CHECK_VOID(alSourceRewind, source_id);
}


int R_al_source_state(R_AlSource *source)
{
    unsigned int source_id = R_al_source_id(source);
    int state;
    R_AL_CHECK_VOID(alGetSourcei, source_id, AL_SOURCE_STATE, &state);
    return state;
}

bool R_al_source_playing(R_AlSource *source)
{
    return R_al_source_state(source) == AL_PLAYING;
}
