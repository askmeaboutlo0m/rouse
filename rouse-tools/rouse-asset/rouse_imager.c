/*
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

static int check_x(Imager img, i_img_dim y, i_img_dim w)
{
    i_img_dim x;
    for (x = 0; x < w; ++x) {
        i_color color;
        i_gpix(img, x, y, &color);
        if (color.rgba.a != 0) {
            return 1;
        }
    }
    return 0;
}

static int check_y(Imager img, i_img_dim x, i_img_dim h)
{
    i_img_dim y;
    for (y = 0; y < h; ++y) {
        i_color color;
        i_gpix(img, x, y, &color);
        if (color.rgba.a != 0) {
            return 1;
        }
    }
    return 0;
}


SV *rouse_autocrop(Imager img)
{
    AV        *arr;
    i_img_dim x, y, w, h, minx, miny, maxx = 0, maxy = 0;

    minx = w = i_img_get_width (img);
    miny = h = i_img_get_height(img);

    for (y = 0; y < h; ++y) {
        if (check_x(img, y, w)) {
            miny = y;
            break;
        }
    }

    for (y = h - 1; y >= 0; --y) {
        if (check_x(img, y, w)) {
            maxy = y;
            break;
        }
    }

    for (x = 0; x < w; ++x) {
        if (check_y(img, x, h)) {
            minx = x;
            break;
        }
    }

    for (x = w - 1; x >= 0; --x) {
        if (check_y(img, x, h)) {
            maxx = x;
            break;
        }
    }

    if (minx < maxx && miny < maxy) {
        arr = newAV();
        av_push(arr, newSVuv(minx));
        av_push(arr, newSVuv(miny));
        av_push(arr, newSVuv(maxx));
        av_push(arr, newSVuv(maxy));
        return newRV_noinc((SV *)arr);
    }
    else {
        return newSV(0);
    }
}


SV *rouse_quantify(Imager img)
{
    HV        *hv = newHV();
    i_img_dim x, y, w, h;

    w = i_img_get_width (img);
    h = i_img_get_height(img);

    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x) {
            i_color c;
            i_gpix(img, x, y, &c);

            if (c.rgba.a != 0) {
                SV *key;
                HE *he;

                key = newSVpvf("%02x%02x%02x", c.rgba.r, c.rgba.g, c.rgba.b);
                if (!hv_exists_ent(hv, key, 0)) {
                    hv_store_ent(hv, key, newSVuv(0), 0);
                }

                he = hv_fetch_ent(hv, key, 1, 0);
                sv_inc(HeVAL(he));
            }
        }
    }

    return newRV_noinc((SV *)hv);
}


void rouse_posterize(Imager img, double threshold)
{
    i_img_dim  x, y, w, h;
    i_sample_t t;
    i_color    black, white;

    w = i_img_get_width (img);
    h = i_img_get_height(img);
    t = 255.0 * threshold;

    black.rgba.r = black.rgba.g = black.rgba.b = 0;
    white.rgba.r = white.rgba.g = white.rgba.b = 255;
    black.rgba.a = white.rgba.a = 255;

    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x) {
            i_color c;
            i_gpix(img, x, y, &c);
            i_ppix(img, x, y, c.rgba.a < t ? &white : &black);
        }
    }
}
