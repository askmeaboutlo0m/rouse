/*
 * config.h - configuration decided at build type, automatically generated
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
#ifndef ROUSE_CONFIG_H_INCLUDED
#define ROUSE_CONFIG_H_INCLUDED

/* Version of this library. */
#define ROUSE_VERSION "@CMAKE_PROJECT_VERSION@"

/* What kind of build this is. Usually one of "Debug" or "Release". */
#define ROUSE_BUILD_TYPE "@CMAKE_BUILD_TYPE@"

/*
 * Debug mode, enables debug logging, assertions, and other extraneous things.
 * Should be off in release mode. This is unaffected by `NDEBUG`, you can
 * enable or disable them independently.
 */
#cmakedefine ROUSE_DEBUG

/*
 * Magic numbers in structs to allow runtime type checking. Causes a
 * performance hit, so in a release it's probably disabled.
 */
#cmakedefine ROUSE_MAGIC

/*
 * Checking of every OpenGL call for errors. This is immensely slow, so
 * definitely turn it off in release mode.
 */
#cmakedefine ROUSE_GL_CHECKS

/*
 * The platform we're running on, roughly. Emscripten and Windows in particular
 * have some interesting properties that sometimes need special casing.
 */
#if defined(__EMSCRIPTEN__)
#   define ROUSE_PLATFORM "emscripten"
#elif defined(_WIN32)
#   define ROUSE_PLATFORM "windows"
#elif defined(__APPLE__)
#   define ROUSE_PLATFORM "darwin"
#elif defined(__linux__)
#   define ROUSE_PLATFORM "linux"
#else
#   error "unknown ROUSE_PLATFORM, please add it here"
#endif

/*
 * Maximum steps that are allowed per tick before we consider a sequence stuck
 * and abort the program. Set to 1000 by default in debug mode, which is far
 * more steps than any animation sequence should perform in one tick. Set to 0
 * in release mode by default, which means no checking at all.
 */
#define ROUSE_MAX_STEPS_PER_TICK @ROUSE_MAX_STEPS_PER_TICK@

#endif
