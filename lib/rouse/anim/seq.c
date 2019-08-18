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
#include <stddef.h>
#include <stdnoreturn.h>
#include "../common.h"
#include "seq.h"


struct R_Step {
    R_MAGIC_FIELD
    R_Sequence   *seq;
    void         *state;
    R_StepTickFn on_tick;
    R_StepFreeFn on_free;
    R_Step       *next;
};

struct R_Sequence {
    R_MAGIC_FIELD
    R_Animator       *an;
    int              lap, max_laps;
    bool             killed;
    R_Sequence       *next;
    R_Step           *first, *current;
    R_SequenceDoneFn on_done;
    R_SequenceFreeFn on_free;
    R_UserData       user;
};

struct R_Animator {
    R_MAGIC_FIELD
    R_Sequence *first, *last;
};


R_Step *R_step_new(void *state, R_StepTickFn on_tick, R_StepFreeFn on_free)
{
    R_Step *step = R_NEW_INIT_STRUCT(step, R_Step,
            R_MAGIC_INIT(step) NULL, state, on_tick, on_free, NULL);
    R_MAGIC_CHECK(step);
    return step;
}

void R_step_free(R_Step *step)
{
    if (step) {
        R_MAGIC_CHECK(step);
        if (step->on_free) {
            step->on_free(step);
        }
        R_MAGIC_POISON(step);
        free(step);
    }
}


R_Sequence *R_sequence_new(int max_laps, R_SequenceFreeFn on_free,
                           R_UserData user)
{
    R_Sequence *seq = R_NEW_INIT_STRUCT(seq, R_Sequence,
            R_MAGIC_INIT(seq) NULL, -1, max_laps, false,
            NULL, NULL, NULL, NULL, on_free, user);
    R_MAGIC_CHECK(seq);
    return seq;
}

void R_sequence_free(R_Sequence *seq)
{
    if (seq) {
        R_MAGIC_CHECK(seq);
        R_Step *step = seq->first;

        while (step) {
            R_Step *next = step->next;
            R_step_free(step);
            step = next;
        }

        if (seq->on_free) {
            seq->on_free(seq);
        }
        R_MAGIC_POISON(seq);
        free(seq);
    }
}


static void check_step_not_added(R_Step *step, const char *prefix)
{
    if (step->seq) {
        R_die("%s: step has already been added to a sequence", prefix);
    }
}

static void add_step_to_sequence(R_Sequence *seq, R_Step *step)
{
    if (seq->current) {
        seq->current->next = step;
    } else {
        seq->first = step;
    }
    seq->current = step;
}

static void set_up_step_after_adding(R_Sequence *seq, R_Step *step)
{
    step->seq = seq;
}

void R_sequence_add(R_Sequence *seq, R_Step *step)
{
    R_MAGIC_CHECK_2(seq, step);
    check_step_not_added(step, __func__);
    add_step_to_sequence(seq, step);
    set_up_step_after_adding(seq, step);
}


static bool sequence_has_laps_remaining(R_Sequence *seq)
{
    int max_laps = seq->max_laps;
    return max_laps < 0 || seq->lap < max_laps;
}

static R_StepStatus tick_step(R_Sequence *seq, bool rendered, float ratio)
{
    R_Step *step = seq->current;
    R_MAGIC_CHECK(step);
    return step->on_tick(step, rendered, ratio);
}

static bool keep_stepping_sequence(R_Sequence *seq, bool rendered, float ratio)
{
    return !seq->killed
        && sequence_has_laps_remaining(seq)
        && tick_step(seq, rendered, ratio) == R_STEP_STATUS_COMPLETE;
}

void R_sequence_tick(R_Sequence *seq, bool rendered, float ratio)
{
    R_MAGIC_CHECK(seq);
    while (keep_stepping_sequence(seq, rendered, ratio)) {
        if (!(seq->current = seq->current->next)) {
            seq->current = seq->first;
            ++seq->lap;
        }
    }
}


R_Animator *R_animator_new(void)
{
    R_Animator *an = R_NEW_INIT_STRUCT(an, R_Animator,
            R_MAGIC_INIT(an) NULL, NULL);
    R_MAGIC_CHECK(an);
    return an;
}

void R_animator_free(R_Animator *an)
{
    if (an) {
        R_MAGIC_CHECK(an);
        R_Sequence *seq = an->first;

        while (seq) {
            R_Sequence *next = seq->next;
            R_sequence_free(seq);
            seq = next;
        }

        R_MAGIC_POISON(an);
        free(an);
    }
}


static void check_sequence_not_added(R_Sequence *seq, const char *prefix)
{
    if (seq->an) {
        R_die("%s: sequence has already been started", prefix);
    }
}

static void add_sequence_to_animator(R_Animator *an, R_Sequence *seq)
{
    if (an->last) {
        an->last->next = seq;
    } else {
        an->first = seq;
    }
    an->last = seq;
}

static void set_up_sequence_after_adding(R_Animator *an, R_Sequence *seq)
{
    seq->an      = an;
    seq->lap     = 0;
    seq->current = seq->first;
}

void R_animator_add(R_Animator *an, R_Sequence *seq)
{
    R_MAGIC_CHECK_2(an, seq);
    check_sequence_not_added(seq, __func__);
    if (!seq->first) {
        R_die("%s: added sequence is empty", __func__);
    }
    add_sequence_to_animator(an, seq);
    set_up_sequence_after_adding(an, seq);
}


static void tick_sequences(R_Animator *an, bool rendered, float ratio)
{
    for (R_Sequence *seq = an->first; seq; seq = seq->next) {
        R_sequence_tick(seq, rendered, ratio);
    }
}

static R_Sequence *filter_dead_sequences(R_Animator *an)
{
    R_Sequence **cur = &an->first;
    R_Sequence *dead = NULL;
    an->last         = NULL;

    while (*cur) {
        R_Sequence *seq = *cur;
        R_MAGIC_CHECK(seq);
        if (seq->killed || !sequence_has_laps_remaining(seq)) {
            *cur      = seq->next;
            seq->next = dead;
            dead      = seq;
        }
        else {
            cur      = &(seq->next);
            an->last = seq;
        }
    }

    return dead;
}

static void free_sequences(R_Sequence *dead)
{
    while (dead) {
        R_Sequence *seq = dead;
        R_MAGIC_CHECK(seq);
        dead = dead->next;
        if (!seq->killed && seq->on_done) {
            seq->on_done(seq);
        }
        R_sequence_free(seq);
    }
}

void R_animator_tick(R_Animator *an, bool rendered, float ratio)
{
    R_MAGIC_CHECK(an);
    tick_sequences(an, rendered, ratio);
    R_Sequence *dead = filter_dead_sequences(an);
    free_sequences(dead);
}
