/*
 * nvg.h - reference-counted NanoVG wrapper and utility functions
 *
 * Copyright (c) 2019 askmeaboutloom
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

typedef struct R_Nvg R_Nvg;

R_Nvg *R_nvg_new(int flags);

R_Nvg *R_nvg_incref(R_Nvg *nvg);
R_Nvg *R_nvg_decref(R_Nvg *nvg);
int R_nvg_refs(R_Nvg *nvg);

NVGcontext *R_nvg_context(R_Nvg *nvg);


static inline void R_nvg_transform(NVGcontext *vg, const float m[R_STATIC(6)])
{
    nvgTransform(vg, m[0], m[1], m[2], m[3], m[4], m[5]);
}

static inline void R_nvg_transform_set(NVGcontext *vg,
                                       const float m[R_STATIC(6)])
{
    nvgResetTransform(vg);
    R_nvg_transform(vg, m);
}

static inline void R_nvg_transform_set_2(NVGcontext *vg,
                                         const float m[R_STATIC(6)],
                                         const float n[R_STATIC(6)])
{
    nvgResetTransform(vg);
    R_nvg_transform(vg, m);
    R_nvg_transform(vg, n);
}
