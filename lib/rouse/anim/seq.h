/*
 * anim/seq.h - animations and sequencing thereof.
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

#define R_SEQUENCE_RUN_FOREVER (-1)

typedef struct R_Step     R_Step;
typedef struct R_Sequence R_Sequence;
typedef struct R_Animator R_Animator;

typedef enum R_StepStatus {
    R_STEP_STATUS_RUNNING,
    R_STEP_STATUS_COMPLETE,
} R_StepStatus;

typedef R_StepStatus (*R_StepTickFn)(R_Step *, bool, float);
typedef void         (*R_StepFreeFn)(R_Step *);

typedef void (*R_SequenceDoneFn)(R_Sequence *);
typedef void (*R_SequenceFreeFn)(R_Sequence *);


R_Step *R_step_new(void *state, R_StepTickFn on_tick, R_StepFreeFn on_free);
void R_step_free(R_Step *step);


R_Sequence *R_sequence_new(int max_laps, R_SequenceFreeFn on_free,
                           R_UserData user);
void R_sequence_free(R_Sequence *seq);

void R_sequence_add(R_Sequence *seq, R_Step *step);

void R_sequence_tick(R_Sequence *seq, bool rendered, float ratio);


R_Animator *R_animator_new(void);
void R_animator_free(R_Animator *an);

void R_animator_add(R_Animator *an, R_Sequence *seq);

void R_animator_tick(R_Animator *an, bool rendered, float ratio);
