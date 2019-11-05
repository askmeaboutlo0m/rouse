/*
 * input.h - key binding and more
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

typedef struct R_Input R_Input;

typedef void (*R_InputKeyFn)(SDL_KeyboardEvent *, R_UserData, R_UserData);
typedef void (*R_InputFreeFn)(R_UserData);

typedef enum R_InputHandled {
    R_INPUT_UNHANDLED     = 0x0,
    R_INPUT_BOUND         = 0x1,
    R_INPUT_UNBOUND_KEY   = 0x2,
    R_INPUT_BOUND_KEY     = 0x3,
    R_INPUT_UNBOUND_MOUSE = 0x4,
    R_INPUT_BOUND_MOUSE   = 0x5,
} R_InputHandled;


R_Input *R_input_new(void);

void R_input_clear(R_Input *in);
void R_input_free(R_Input *in);


void R_input_key_bind(R_Input *in, unsigned int key, R_InputKeyFn fn,
                      R_UserData user, R_InputFreeFn on_free);
bool R_input_key_unbind(R_Input *in, unsigned int key);
void R_input_key_clear(R_Input *in);


R_InputHandled R_input_handle_event(R_Input *in, SDL_Event *event,
                                    R_UserData context);
