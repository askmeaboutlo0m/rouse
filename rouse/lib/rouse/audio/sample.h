#ifndef ROUSE_CONFIG_H_INCLUDED
#   error "Missing #include of rouse_config.h, must come before sample.h"
#endif
#ifndef ROUSE_AL_ENABLED
#   error "ROUSE_AL_ENABLED not defined, sample.h is not available"
#endif

typedef struct R_Sample {
    R_MAGIC_FIELD
    int   len, channels, rate;
    short *data;
} R_Sample;


R_Sample *R_sample_from_file(const char *path);

void R_sample_free(R_Sample *sample);

int R_sample_al_format(R_Sample *sample);

int R_sample_al_buffer_size(R_Sample *sample);
