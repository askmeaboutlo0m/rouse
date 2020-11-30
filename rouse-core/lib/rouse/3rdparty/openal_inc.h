#include <al.h>
#include <alc.h>

#ifdef __EMSCRIPTEN__
#   define R_AL_PROC_LIST
#else
#   define R_AL_PROC_LIST \
        R_AL_PROC_X(LPALDOPPLERFACTOR, alDopplerFactor) \
        R_AL_PROC_X(LPALDOPPLERVELOCITY, alDopplerVelocity) \
        R_AL_PROC_X(LPALSPEEDOFSOUND, alSpeedOfSound) \
        R_AL_PROC_X(LPALDISTANCEMODEL, alDistanceModel) \
        R_AL_PROC_X(LPALENABLE, alEnable) \
        R_AL_PROC_X(LPALDISABLE, alDisable) \
        R_AL_PROC_X(LPALISENABLED, alIsEnabled) \
        R_AL_PROC_X(LPALGETSTRING, alGetString) \
        R_AL_PROC_X(LPALGETBOOLEANV, alGetBooleanv) \
        R_AL_PROC_X(LPALGETINTEGERV, alGetIntegerv) \
        R_AL_PROC_X(LPALGETFLOATV, alGetFloatv) \
        R_AL_PROC_X(LPALGETDOUBLEV, alGetDoublev) \
        R_AL_PROC_X(LPALGETBOOLEAN, alGetBoolean) \
        R_AL_PROC_X(LPALGETINTEGER, alGetInteger) \
        R_AL_PROC_X(LPALGETFLOAT, alGetFloat) \
        R_AL_PROC_X(LPALGETDOUBLE, alGetDouble) \
        R_AL_PROC_X(LPALGETERROR, alGetError) \
        R_AL_PROC_X(LPALISEXTENSIONPRESENT, alIsExtensionPresent) \
        R_AL_PROC_X(LPALGETPROCADDRESS, alGetProcAddress) \
        R_AL_PROC_X(LPALGETENUMVALUE, alGetEnumValue) \
        R_AL_PROC_X(LPALLISTENERF, alListenerf) \
        R_AL_PROC_X(LPALLISTENER3F, alListener3f) \
        R_AL_PROC_X(LPALLISTENERFV, alListenerfv) \
        R_AL_PROC_X(LPALLISTENERI, alListeneri) \
        R_AL_PROC_X(LPALLISTENER3I, alListener3i) \
        R_AL_PROC_X(LPALLISTENERIV, alListeneriv) \
        R_AL_PROC_X(LPALGETLISTENERF, alGetListenerf) \
        R_AL_PROC_X(LPALGETLISTENER3F, alGetListener3f) \
        R_AL_PROC_X(LPALGETLISTENERFV, alGetListenerfv) \
        R_AL_PROC_X(LPALGETLISTENERI, alGetListeneri) \
        R_AL_PROC_X(LPALGETLISTENER3I, alGetListener3i) \
        R_AL_PROC_X(LPALGETLISTENERIV, alGetListeneriv) \
        R_AL_PROC_X(LPALGENSOURCES, alGenSources) \
        R_AL_PROC_X(LPALDELETESOURCES, alDeleteSources) \
        R_AL_PROC_X(LPALISSOURCE, alIsSource) \
        R_AL_PROC_X(LPALSOURCEF, alSourcef) \
        R_AL_PROC_X(LPALSOURCE3F, alSource3f) \
        R_AL_PROC_X(LPALSOURCEFV, alSourcefv) \
        R_AL_PROC_X(LPALSOURCEI, alSourcei) \
        R_AL_PROC_X(LPALSOURCE3I, alSource3i) \
        R_AL_PROC_X(LPALSOURCEIV, alSourceiv) \
        R_AL_PROC_X(LPALGETSOURCEF, alGetSourcef) \
        R_AL_PROC_X(LPALGETSOURCE3F, alGetSource3f) \
        R_AL_PROC_X(LPALGETSOURCEFV, alGetSourcefv) \
        R_AL_PROC_X(LPALGETSOURCEI, alGetSourcei) \
        R_AL_PROC_X(LPALGETSOURCE3I, alGetSource3i) \
        R_AL_PROC_X(LPALGETSOURCEIV, alGetSourceiv) \
        R_AL_PROC_X(LPALSOURCEPLAYV, alSourcePlayv) \
        R_AL_PROC_X(LPALSOURCESTOPV, alSourceStopv) \
        R_AL_PROC_X(LPALSOURCEREWINDV, alSourceRewindv) \
        R_AL_PROC_X(LPALSOURCEPAUSEV, alSourcePausev) \
        R_AL_PROC_X(LPALSOURCEPLAY, alSourcePlay) \
        R_AL_PROC_X(LPALSOURCESTOP, alSourceStop) \
        R_AL_PROC_X(LPALSOURCEREWIND, alSourceRewind) \
        R_AL_PROC_X(LPALSOURCEPAUSE, alSourcePause) \
        R_AL_PROC_X(LPALSOURCEQUEUEBUFFERS, alSourceQueueBuffers) \
        R_AL_PROC_X(LPALSOURCEUNQUEUEBUFFERS, alSourceUnqueueBuffers) \
        R_AL_PROC_X(LPALGENBUFFERS, alGenBuffers) \
        R_AL_PROC_X(LPALDELETEBUFFERS, alDeleteBuffers) \
        R_AL_PROC_X(LPALISBUFFER, alIsBuffer) \
        R_AL_PROC_X(LPALBUFFERDATA, alBufferData) \
        R_AL_PROC_X(LPALBUFFERF, alBufferf) \
        R_AL_PROC_X(LPALBUFFER3F, alBuffer3f) \
        R_AL_PROC_X(LPALBUFFERFV, alBufferfv) \
        R_AL_PROC_X(LPALBUFFERI, alBufferi) \
        R_AL_PROC_X(LPALBUFFER3I, alBuffer3i) \
        R_AL_PROC_X(LPALBUFFERIV, alBufferiv) \
        R_AL_PROC_X(LPALGETBUFFERF, alGetBufferf) \
        R_AL_PROC_X(LPALGETBUFFER3F, alGetBuffer3f) \
        R_AL_PROC_X(LPALGETBUFFERFV, alGetBufferfv) \
        R_AL_PROC_X(LPALGETBUFFERI, alGetBufferi) \
        R_AL_PROC_X(LPALGETBUFFER3I, alGetBuffer3i) \
        R_AL_PROC_X(LPALGETBUFFERIV, alGetBufferiv) \
        R_AL_PROC_X(LPALCCREATECONTEXT, alcCreateContext) \
        R_AL_PROC_X(LPALCMAKECONTEXTCURRENT, alcMakeContextCurrent) \
        R_AL_PROC_X(LPALCPROCESSCONTEXT, alcProcessContext) \
        R_AL_PROC_X(LPALCSUSPENDCONTEXT, alcSuspendContext) \
        R_AL_PROC_X(LPALCDESTROYCONTEXT, alcDestroyContext) \
        R_AL_PROC_X(LPALCGETCURRENTCONTEXT, alcGetCurrentContext) \
        R_AL_PROC_X(LPALCGETCONTEXTSDEVICE, alcGetContextsDevice) \
        R_AL_PROC_X(LPALCOPENDEVICE, alcOpenDevice) \
        R_AL_PROC_X(LPALCCLOSEDEVICE, alcCloseDevice) \
        R_AL_PROC_X(LPALCGETERROR, alcGetError) \
        R_AL_PROC_X(LPALCISEXTENSIONPRESENT, alcIsExtensionPresent) \
        R_AL_PROC_X(LPALCGETPROCADDRESS, alcGetProcAddress) \
        R_AL_PROC_X(LPALCGETENUMVALUE, alcGetEnumValue) \
        R_AL_PROC_X(LPALCGETSTRING, alcGetString) \
        R_AL_PROC_X(LPALCGETINTEGERV, alcGetIntegerv) \
        R_AL_PROC_X(LPALCCAPTUREOPENDEVICE, alcCaptureOpenDevice) \
        R_AL_PROC_X(LPALCCAPTURECLOSEDEVICE, alcCaptureCloseDevice) \
        R_AL_PROC_X(LPALCCAPTURESTART, alcCaptureStart) \
        R_AL_PROC_X(LPALCCAPTURESTOP, alcCaptureStop) \
        R_AL_PROC_X(LPALCCAPTURESAMPLES, alcCaptureSamples)
#endif

#define R_AL_PROC_X(TYPE, NAME) extern TYPE R_ ## NAME;
R_AL_PROC_LIST
#undef R_AL_PROC_X

#ifdef R_AL_DEFINE_FALLBACKS
#   define R_AL_DEFINE_FALLBACK(APIENTRY, TYPE, NAME, ARGS, RETVAL) \
        static TYPE APIENTRY R_ ## NAME ## _fallback ARGS \
        { \
            R_warn("OpenAL procedure " #NAME " was not loaded"); \
            return RETVAL; \
        }

#   ifdef __EMSCRIPTEN__
#       define R_AL_FALLBACK(...)  /* nothing, these are linked statically */
#       define R_ALC_FALLBACK(...) /* nothing, these are linked statically */
#   else
#       define R_AL_FALLBACK(...) R_AL_DEFINE_FALLBACK(AL_APIENTRY, __VA_ARGS__)
#       define R_ALC_FALLBACK(...) R_AL_DEFINE_FALLBACK(ALC_APIENTRY, __VA_ARGS__)
#   endif

R_AL_FALLBACK(void, alDopplerFactor, (R_UNUSED ALfloat value),)
R_AL_FALLBACK(void, alDopplerVelocity, (R_UNUSED ALfloat value),)
R_AL_FALLBACK(void, alSpeedOfSound, (R_UNUSED ALfloat value),)
R_AL_FALLBACK(void, alDistanceModel, (R_UNUSED ALenum distanceModel),)
R_AL_FALLBACK(void, alEnable, (R_UNUSED ALenum capability),)
R_AL_FALLBACK(void, alDisable, (R_UNUSED ALenum capability),)
R_AL_FALLBACK(ALboolean, alIsEnabled, (R_UNUSED ALenum capability), AL_FALSE)
R_AL_FALLBACK(const ALchar *, alGetString, (R_UNUSED ALenum param), NULL)
R_AL_FALLBACK(void, alGetBooleanv, (R_UNUSED ALenum param, R_UNUSED ALboolean *values),)
R_AL_FALLBACK(void, alGetIntegerv, (R_UNUSED ALenum param, R_UNUSED ALint *values),)
R_AL_FALLBACK(void, alGetFloatv, (R_UNUSED ALenum param, R_UNUSED ALfloat *values),)
R_AL_FALLBACK(void, alGetDoublev, (R_UNUSED ALenum param, R_UNUSED ALdouble *values),)
R_AL_FALLBACK(ALboolean, alGetBoolean, (R_UNUSED ALenum param), AL_FALSE)
R_AL_FALLBACK(ALint, alGetInteger, (R_UNUSED ALenum param), 0)
R_AL_FALLBACK(ALfloat, alGetFloat, (R_UNUSED ALenum param), 0.0f)
R_AL_FALLBACK(ALdouble, alGetDouble, (R_UNUSED ALenum param), 0.0)
R_AL_FALLBACK(ALenum, alGetError, (void), 0)
R_AL_FALLBACK(ALboolean, alIsExtensionPresent, (R_UNUSED const ALchar *extname), AL_FALSE)
R_AL_FALLBACK(void *, alGetProcAddress, (R_UNUSED const ALchar *fname), NULL)
R_AL_FALLBACK(ALenum, alGetEnumValue, (R_UNUSED const ALchar *ename), 0)
R_AL_FALLBACK(void, alListenerf, (R_UNUSED ALenum param, R_UNUSED ALfloat value),)
R_AL_FALLBACK(void, alListener3f, (R_UNUSED ALenum param, R_UNUSED ALfloat value1, R_UNUSED ALfloat value2, R_UNUSED ALfloat value3),)
R_AL_FALLBACK(void, alListenerfv, (R_UNUSED ALenum param, R_UNUSED const ALfloat *values),)
R_AL_FALLBACK(void, alListeneri, (R_UNUSED ALenum param, R_UNUSED ALint value),)
R_AL_FALLBACK(void, alListener3i, (R_UNUSED ALenum param, R_UNUSED ALint value1, R_UNUSED ALint value2, R_UNUSED ALint value3),)
R_AL_FALLBACK(void, alListeneriv, (R_UNUSED ALenum param, R_UNUSED const ALint *values),)
R_AL_FALLBACK(void, alGetListenerf, (R_UNUSED ALenum param, R_UNUSED ALfloat *value),)
R_AL_FALLBACK(void, alGetListener3f, (R_UNUSED ALenum param, R_UNUSED ALfloat *value1, R_UNUSED ALfloat *value2, R_UNUSED ALfloat *value3),)
R_AL_FALLBACK(void, alGetListenerfv, (R_UNUSED ALenum param, R_UNUSED ALfloat *values),)
R_AL_FALLBACK(void, alGetListeneri, (R_UNUSED ALenum param, R_UNUSED ALint *value),)
R_AL_FALLBACK(void, alGetListener3i, (R_UNUSED ALenum param, R_UNUSED ALint *value1, R_UNUSED ALint *value2, R_UNUSED ALint *value3),)
R_AL_FALLBACK(void, alGetListeneriv, (R_UNUSED ALenum param, R_UNUSED ALint *values),)
R_AL_FALLBACK(void, alGenSources, (R_UNUSED ALsizei n, R_UNUSED ALuint *sources),)
R_AL_FALLBACK(void, alDeleteSources, (R_UNUSED ALsizei n, R_UNUSED const ALuint *sources),)
R_AL_FALLBACK(ALboolean, alIsSource, (R_UNUSED ALuint source), AL_FALSE)
R_AL_FALLBACK(void, alSourcef, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALfloat value),)
R_AL_FALLBACK(void, alSource3f, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALfloat value1, R_UNUSED ALfloat value2, R_UNUSED ALfloat value3),)
R_AL_FALLBACK(void, alSourcefv, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED const ALfloat *values),)
R_AL_FALLBACK(void, alSourcei, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALint value),)
R_AL_FALLBACK(void, alSource3i, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALint value1, R_UNUSED ALint value2, R_UNUSED ALint value3),)
R_AL_FALLBACK(void, alSourceiv, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED const ALint *values),)
R_AL_FALLBACK(void, alGetSourcef, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALfloat *value),)
R_AL_FALLBACK(void, alGetSource3f, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALfloat *value1, R_UNUSED ALfloat *value2, R_UNUSED ALfloat *value3),)
R_AL_FALLBACK(void, alGetSourcefv, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALfloat *values),)
R_AL_FALLBACK(void, alGetSourcei, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALint *value),)
R_AL_FALLBACK(void, alGetSource3i, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALint *value1, R_UNUSED ALint *value2, R_UNUSED ALint *value3),)
R_AL_FALLBACK(void, alGetSourceiv, (R_UNUSED ALuint source, R_UNUSED ALenum param, R_UNUSED ALint *values),)
R_AL_FALLBACK(void, alSourcePlayv, (R_UNUSED ALsizei n, R_UNUSED const ALuint *sources),)
R_AL_FALLBACK(void, alSourceStopv, (R_UNUSED ALsizei n, R_UNUSED const ALuint *sources),)
R_AL_FALLBACK(void, alSourceRewindv, (R_UNUSED ALsizei n, R_UNUSED const ALuint *sources),)
R_AL_FALLBACK(void, alSourcePausev, (R_UNUSED ALsizei n, R_UNUSED const ALuint *sources),)
R_AL_FALLBACK(void, alSourcePlay, (R_UNUSED ALuint source),)
R_AL_FALLBACK(void, alSourceStop, (R_UNUSED ALuint source),)
R_AL_FALLBACK(void, alSourceRewind, (R_UNUSED ALuint source),)
R_AL_FALLBACK(void, alSourcePause, (R_UNUSED ALuint source),)
R_AL_FALLBACK(void, alSourceQueueBuffers, (R_UNUSED ALuint source, R_UNUSED ALsizei nb, R_UNUSED const ALuint *buffers),)
R_AL_FALLBACK(void, alSourceUnqueueBuffers, (R_UNUSED ALuint source, R_UNUSED ALsizei nb, R_UNUSED ALuint *buffers),)
R_AL_FALLBACK(void, alGenBuffers, (R_UNUSED ALsizei n, R_UNUSED ALuint *buffers),)
R_AL_FALLBACK(void, alDeleteBuffers, (R_UNUSED ALsizei n, R_UNUSED const ALuint *buffers),)
R_AL_FALLBACK(ALboolean, alIsBuffer, (R_UNUSED ALuint buffer), AL_FALSE)
R_AL_FALLBACK(void, alBufferData, (R_UNUSED ALuint buffer, R_UNUSED ALenum format, R_UNUSED const ALvoid *data, R_UNUSED ALsizei size, R_UNUSED ALsizei freq),)
R_AL_FALLBACK(void, alBufferf, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALfloat value),)
R_AL_FALLBACK(void, alBuffer3f, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALfloat value1, R_UNUSED ALfloat value2, R_UNUSED ALfloat value3),)
R_AL_FALLBACK(void, alBufferfv, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED const ALfloat *values),)
R_AL_FALLBACK(void, alBufferi, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALint value),)
R_AL_FALLBACK(void, alBuffer3i, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALint value1, R_UNUSED ALint value2, R_UNUSED ALint value3),)
R_AL_FALLBACK(void, alBufferiv, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED const ALint *values),)
R_AL_FALLBACK(void, alGetBufferf, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALfloat *value),)
R_AL_FALLBACK(void, alGetBuffer3f, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALfloat *value1, R_UNUSED ALfloat *value2, R_UNUSED ALfloat *value3),)
R_AL_FALLBACK(void, alGetBufferfv, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALfloat *values),)
R_AL_FALLBACK(void, alGetBufferi, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALint *value),)
R_AL_FALLBACK(void, alGetBuffer3i, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALint *value1, R_UNUSED ALint *value2, R_UNUSED ALint *value3),)
R_AL_FALLBACK(void, alGetBufferiv, (R_UNUSED ALuint buffer, R_UNUSED ALenum param, R_UNUSED ALint *values),)
R_ALC_FALLBACK(ALCcontext *, alcCreateContext, (R_UNUSED ALCdevice *device, R_UNUSED const ALCint* attrlist), NULL)
R_ALC_FALLBACK(ALCboolean, alcMakeContextCurrent, (R_UNUSED ALCcontext *context), AL_FALSE)
R_ALC_FALLBACK(void, alcProcessContext, (R_UNUSED ALCcontext *context),)
R_ALC_FALLBACK(void, alcSuspendContext, (R_UNUSED ALCcontext *context),)
R_ALC_FALLBACK(void, alcDestroyContext, (R_UNUSED ALCcontext *context),)
R_ALC_FALLBACK(ALCcontext *, alcGetCurrentContext, (void), NULL)
R_ALC_FALLBACK(ALCdevice *, alcGetContextsDevice, (R_UNUSED ALCcontext *context), NULL)
R_ALC_FALLBACK(ALCdevice *, alcOpenDevice, (R_UNUSED const ALCchar *devicename), NULL)
R_ALC_FALLBACK(ALCboolean, alcCloseDevice, (R_UNUSED ALCdevice *device), AL_FALSE)
R_ALC_FALLBACK(ALCenum, alcGetError, (R_UNUSED ALCdevice *device), 0)
R_ALC_FALLBACK(ALCboolean, alcIsExtensionPresent, (R_UNUSED ALCdevice *device, R_UNUSED const ALCchar *extname), AL_FALSE)
R_ALC_FALLBACK(void *, alcGetProcAddress, (R_UNUSED ALCdevice *device, R_UNUSED const ALCchar *funcname), NULL)
R_ALC_FALLBACK(ALCenum, alcGetEnumValue, (R_UNUSED ALCdevice *device, R_UNUSED const ALCchar *enumname), 0)
R_ALC_FALLBACK(const ALCchar *, alcGetString, (R_UNUSED ALCdevice *device, R_UNUSED ALCenum param), NULL)
R_ALC_FALLBACK(void, alcGetIntegerv, (R_UNUSED ALCdevice *device, R_UNUSED ALCenum param, R_UNUSED ALCsizei size, R_UNUSED ALCint *values),)
R_ALC_FALLBACK(ALCdevice *, alcCaptureOpenDevice, (R_UNUSED const ALCchar *devicename, R_UNUSED ALCuint frequency, R_UNUSED ALCenum format, R_UNUSED ALCsizei buffersize), NULL)
R_ALC_FALLBACK(ALCboolean, alcCaptureCloseDevice, (R_UNUSED ALCdevice *device), AL_FALSE)
R_ALC_FALLBACK(void, alcCaptureStart, (R_UNUSED ALCdevice *device),)
R_ALC_FALLBACK(void, alcCaptureStop, (R_UNUSED ALCdevice *device),)
R_ALC_FALLBACK(void, alcCaptureSamples, (R_UNUSED ALCdevice *device, R_UNUSED ALCvoid *buffer, R_UNUSED ALCsizei samples),)

#endif
