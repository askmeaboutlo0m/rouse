#ifndef ROUSE_CONFIG_H_INCLUDED
#   error "Missing #include of rouse_config.h, must come before al.h"
#endif
#ifndef ROUSE_AL_ENABLED
#   error "ROUSE_AL_ENABLED not defined, al.h is not available"
#endif

#ifdef ROUSE_AL_CHECKS
#   define R_AL_CLEAR_ERROR() do { \
            int _alerror; \
            while ((_alerror = R_alGetError()) != AL_NO_ERROR) { \
                R_al_warn(_alerror, "R_AL_CLEAR_ERROR"); \
            } \
        } while (0)

#   define R_AL_CHECK_RESULT_WARN(WHERE, RESULT) do { \
            if (!(RESULT)) { \
                R_al_warn(R_alGetError(), WHERE); \
            } \
        } while (0)

#   define R_AL_CHECK_RESULT(WHERE, RESULT) do { \
            if (!(RESULT)) { \
                R_al_die(R_alGetError(), WHERE); \
            } \
        } while (0)

#   define R_AL_CHECK(FUNC, ...) do { \
            R_AL_CHECK_RESULT(#FUNC, R_ ## FUNC(__VA_ARGS__)); \
        } while (0)

#   define R_AL_CHECK_VOID(FUNC, ...) do { \
            R_ ## FUNC(__VA_ARGS__); \
            int _alerror = R_alGetError(); \
            if (_alerror != AL_NO_ERROR) { \
                R_al_die(_alerror, #FUNC); \
            } \
        } while (0)

#   define R_AL_CHECK_WARN(FUNC, ...) do { \
            R_AL_CHECK_RESULT_WARN(#FUNC, R_ ## FUNC(__VA_ARGS__)); \
        } while (0)

#   define R_AL_ASSIGN(TARGET, FUNC, ...) do { \
            R_AL_CHECK_RESULT(#FUNC, (TARGET) = R_ ## FUNC(__VA_ARGS__)); \
        } while (0)
#else
#   define R_AL_CLEAR_ERROR()             ((void) 0)
#   define R_AL_CHECK(FUNC, ...)          R_ ## FUNC(__VA_ARGS__)
#   define R_AL_CHECK_VOID(FUNC, ...)     R_ ## FUNC(__VA_ARGS__)
#   define R_AL_CHECK_WARN(FUNC, ...)     R_ ## FUNC(__VA_ARGS__)
#   define R_AL_ASSIGN(TARGET, FUNC, ...) (TARGET) = R_ ## FUNC(__VA_ARGS__)
#endif


typedef struct R_AlBuffer R_AlBuffer;
typedef struct R_AlSource R_AlSource;

typedef struct R_AlOrientation {
    R_V3 front, up;
} R_AlOrientation;

bool R_al_init(R_AlGetDeviceNameHook        get_device_name,
               R_AlGetContextAttributesHook get_context_attributes,
               void                         *user);

void R_al_deinit(void);

noreturn void R_al_die(int err, const char *where);

void R_al_warn(int err, const char *where);

const char *R_al_strerror(int err);


R_V3            R_al_listener_position   (void);
R_V3            R_al_listener_velocity   (void);
R_AlOrientation R_al_listener_orientation(void);
float           R_al_listener_gain       (void);

void R_al_listener_position_set   (R_V3            position);
void R_al_listener_velocity_set   (R_V3            velocity);
void R_al_listener_orientation_set(R_AlOrientation orientation);
void R_al_listener_gain_set       (float           gain);

R_AlBuffer *R_al_buffer_from_sample(R_Sample *sample);
R_AlBuffer *R_al_buffer_from_file(const char *path);

R_AlBuffer *R_al_buffer_incref(R_AlBuffer *buffer);
R_AlBuffer *R_al_buffer_decref(R_AlBuffer *buffer);

unsigned int R_al_buffer_id(R_AlBuffer *buffer);


R_AlSource *R_al_source_new(void);
R_AlSource *R_al_source_new_from_buffer(R_AlBuffer *buffer);
R_AlSource *R_al_source_new_from_buffer_noinc(R_AlBuffer *buffer);
R_AlSource *R_al_source_new_from_file(const char *path);

void R_al_source_free(R_AlSource *source);

unsigned int R_al_source_id(R_AlSource *source);

R_AlBuffer *R_al_source_buffer(R_AlSource *source);
void R_al_source_buffer_set(R_AlSource *source, R_AlBuffer *buffer);
void R_al_source_buffer_set_noinc(R_AlSource *source, R_AlBuffer *buffer);

R_V3  R_al_source_position          (R_AlSource *source);
R_V3  R_al_source_velocity          (R_AlSource *source);
float R_al_source_gain              (R_AlSource *source);
float R_al_source_pitch             (R_AlSource *source);
float R_al_source_reference_distance(R_AlSource *source);
float R_al_source_offset_in_seconds (R_AlSource *source);
bool  R_al_source_looping           (R_AlSource *source);

void R_al_source_position_set          (R_AlSource *source, R_V3  position);
void R_al_source_velocity_set          (R_AlSource *source, R_V3  velocity);
void R_al_source_gain_set              (R_AlSource *source, float gain);
void R_al_source_pitch_set             (R_AlSource *source, float pitch);
void R_al_source_reference_distance_set(R_AlSource *source, float distance);
void R_al_source_offset_in_seconds_set (R_AlSource *source, float seconds);
void R_al_source_looping_set           (R_AlSource *source, bool  looping);

void R_al_source_play  (R_AlSource *source);
void R_al_source_stop  (R_AlSource *source);
void R_al_source_rewind(R_AlSource *source);
void R_al_source_pause (R_AlSource *source);

int  R_al_source_state  (R_AlSource *source);
bool R_al_source_playing(R_AlSource *source);
