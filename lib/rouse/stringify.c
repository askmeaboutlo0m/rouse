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
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <cglm/struct.h>
#include <SDL2/SDL.h>
#include <rouse_config.h>
#include "common.h"
#include "geom.h"
#include "render/frame_buffer.h"
#include "stringify.h"


#define R_CASE_STR(X) case X: return #X;


const char *R_stringify_sdl_event_type(uint32_t event_type)
{
    switch (event_type) {
        R_CASE_STR(SDL_FIRSTEVENT)
        R_CASE_STR(SDL_QUIT)
        R_CASE_STR(SDL_APP_TERMINATING)
        R_CASE_STR(SDL_APP_LOWMEMORY)
        R_CASE_STR(SDL_APP_WILLENTERBACKGROUND)
        R_CASE_STR(SDL_APP_DIDENTERBACKGROUND)
        R_CASE_STR(SDL_APP_WILLENTERFOREGROUND)
        R_CASE_STR(SDL_APP_DIDENTERFOREGROUND)
        R_CASE_STR(SDL_WINDOWEVENT)
        R_CASE_STR(SDL_SYSWMEVENT)
        R_CASE_STR(SDL_KEYDOWN)
        R_CASE_STR(SDL_KEYUP)
        R_CASE_STR(SDL_TEXTEDITING)
        R_CASE_STR(SDL_TEXTINPUT)
        R_CASE_STR(SDL_KEYMAPCHANGED)
        R_CASE_STR(SDL_MOUSEMOTION)
        R_CASE_STR(SDL_MOUSEBUTTONDOWN)
        R_CASE_STR(SDL_MOUSEBUTTONUP)
        R_CASE_STR(SDL_MOUSEWHEEL)
        R_CASE_STR(SDL_JOYAXISMOTION)
        R_CASE_STR(SDL_JOYBALLMOTION)
        R_CASE_STR(SDL_JOYHATMOTION)
        R_CASE_STR(SDL_JOYBUTTONDOWN)
        R_CASE_STR(SDL_JOYBUTTONUP)
        R_CASE_STR(SDL_JOYDEVICEADDED)
        R_CASE_STR(SDL_JOYDEVICEREMOVED)
        R_CASE_STR(SDL_CONTROLLERAXISMOTION)
        R_CASE_STR(SDL_CONTROLLERBUTTONDOWN)
        R_CASE_STR(SDL_CONTROLLERBUTTONUP)
        R_CASE_STR(SDL_CONTROLLERDEVICEADDED)
        R_CASE_STR(SDL_CONTROLLERDEVICEREMOVED)
        R_CASE_STR(SDL_CONTROLLERDEVICEREMAPPED)
        R_CASE_STR(SDL_FINGERDOWN)
        R_CASE_STR(SDL_FINGERUP)
        R_CASE_STR(SDL_FINGERMOTION)
        R_CASE_STR(SDL_DOLLARGESTURE)
        R_CASE_STR(SDL_DOLLARRECORD)
        R_CASE_STR(SDL_MULTIGESTURE)
        R_CASE_STR(SDL_CLIPBOARDUPDATE)
        R_CASE_STR(SDL_DROPFILE)
        R_CASE_STR(SDL_DROPTEXT)
        R_CASE_STR(SDL_DROPBEGIN)
        R_CASE_STR(SDL_DROPCOMPLETE)
        R_CASE_STR(SDL_AUDIODEVICEADDED)
        R_CASE_STR(SDL_AUDIODEVICEREMOVED)
        R_CASE_STR(SDL_RENDER_TARGETS_RESET)
        R_CASE_STR(SDL_RENDER_DEVICE_RESET)
        R_CASE_STR(SDL_USEREVENT)
        R_CASE_STR(SDL_LASTEVENT)
        default:
            return "(unknown event)";
    }
}


const char *R_stringify_sdl_window_event_type(uint32_t window_event_type)
{
    switch (window_event_type) {
        R_CASE_STR(SDL_WINDOWEVENT_NONE)
        R_CASE_STR(SDL_WINDOWEVENT_SHOWN)
        R_CASE_STR(SDL_WINDOWEVENT_HIDDEN)
        R_CASE_STR(SDL_WINDOWEVENT_EXPOSED)
        R_CASE_STR(SDL_WINDOWEVENT_MOVED)
        R_CASE_STR(SDL_WINDOWEVENT_RESIZED)
        R_CASE_STR(SDL_WINDOWEVENT_SIZE_CHANGED)
        R_CASE_STR(SDL_WINDOWEVENT_MINIMIZED)
        R_CASE_STR(SDL_WINDOWEVENT_MAXIMIZED)
        R_CASE_STR(SDL_WINDOWEVENT_RESTORED)
        R_CASE_STR(SDL_WINDOWEVENT_ENTER)
        R_CASE_STR(SDL_WINDOWEVENT_LEAVE)
        R_CASE_STR(SDL_WINDOWEVENT_FOCUS_GAINED)
        R_CASE_STR(SDL_WINDOWEVENT_FOCUS_LOST)
        R_CASE_STR(SDL_WINDOWEVENT_CLOSE)
        R_CASE_STR(SDL_WINDOWEVENT_TAKE_FOCUS)
        default:
            return "(unknown window event)";
    }
}
