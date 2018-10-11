/*
 * stringify.h - making constants and data structures human-readable.
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


#ifdef SDL_events_h_
/*
 * Gives a string for an SDL event type. For unknown types, you get
 * "(unknown event)" back.
 */
const char *R_stringify_sdl_event_type(Uint32 event_type);

/*
 * Gives a string for an SDL window event type (those constants starting with
 * `SDL_WINDOWEVENT_`). Unknown types get you "(unknown window event)".
 */
const char *R_stringify_sdl_window_event_type(Uint32 window_event_type);
#endif


#define R_FORMAT_STRING_FRAME_BUFFER \
    "flags=%#x, width=%d, height=%d, real_width=%d, " \
    "real_height=%d, handle=%u, color=%u, depth=%u, pixels=%p)"

#define R_FORMAT_ARGS_FRAME_BUFFER(FB) \
    (FB)->flags, (FB)->width, (FB)->height, (FB)->real_width, \
    (FB)->real_height, (FB)->handle, (FB)->color, (FB)->depth, (FB)->pixels
