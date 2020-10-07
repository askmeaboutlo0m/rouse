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
#include <string.h>
#include <errno.h>
#include <rouse_config.h>
#include "../common.h"
#include "../3rdparty/openal_inc.h"
#include "../3rdparty/stb_vorbis.h"
#include "sample.h"



struct R_SampleStream {
    R_MAGIC_FIELD
    stb_vorbis *vorbis;
    int        channels, rate;
    size_t     size;
    short      output[];
};


static R_Sample *new_sample(int len, int channels, int rate, short *data)
{
    R_Sample *sample = R_NEW_INIT_STRUCT(sample, R_Sample,
            R_MAGIC_INIT(R_Sample) len, channels, rate, data);
    R_MAGIC_CHECK(R_Sample, sample);
    return sample;
}

R_Sample *R_sample_from_file(const char *path)
{
    int channels, rate;
    short *data;
    int len = stb_vorbis_decode_filename(path, &channels, &rate, &data);
    if (data && len > 0) {
        return new_sample(len, channels, rate, data);
    }
    else {
        R_die("Can't decode '%s' as vorbis", path);
    }
}

void R_sample_free(R_Sample *sample)
{
    if (sample) {
        R_MAGIC_CHECK(R_Sample, sample);
        free(sample->data);
        R_MAGIC_POISON(R_Sample, sample);
        free(sample);
    }
}

/* We want to return 0 for invalid formats, so make sure that's available. */
static_assert(AL_FORMAT_MONO16   != 0, "AL formats must not conflict with 0");
static_assert(AL_FORMAT_STEREO16 != 0, "AL formats must not conflict with 0");

int R_sample_al_format(R_Sample *sample)
{
    R_MAGIC_CHECK(R_Sample, sample);
    switch (sample->channels) {
        case 1:
            return AL_FORMAT_MONO16;
        case 2:
            return AL_FORMAT_STEREO16;
        default:
            return 0;
    }
}

int R_sample_al_buffer_size(R_Sample *sample)
{
    R_MAGIC_CHECK(R_Sample, sample);
    return sample->len * (int) sizeof(*sample->data) * sample->channels;
}
