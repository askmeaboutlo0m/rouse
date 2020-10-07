#ifndef ROUSE_CONFIG_H_INCLUDED
#   error "Missing #include of rouse_config.h, must come before stb_vorbis.h"
#endif
#ifndef ROUSE_AL_ENABLED
#   error "ROUSE_AL_ENABLED not defined, stb_vorbis.h is not available"
#endif

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"
