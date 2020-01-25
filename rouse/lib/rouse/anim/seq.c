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
#include <assert.h>
#include "../3rdparty/parson.h"
#include <rouse_config.h>
#include "../common.h"
#include "../json.h"
#include "seq.h"

struct R_Step {
    R_MAGIC_FIELD
    R_Sequence   *seq;
    void         *state;
    R_StepTickFn on_tick;
    R_StepFreeFn on_free;
    R_StepJsonFn to_json;
    R_Step       *next;
};

struct R_Sequence {
    R_MAGIC_FIELD
    R_Animator       *an;
    int              id, lap, max_laps;
    bool             killed;
    R_SequenceDoneFn on_done;
    R_SequenceFreeFn on_free;
    R_UserData       user;
    R_Sequence       *next;
    R_Step           *first, *current;
};

struct R_Animator {
    R_MAGIC_FIELD
    int        last_id;
    R_Sequence *first, *last;
};


R_Step *R_step_new(void *state, R_StepTickFn on_tick, R_StepFreeFn on_free,
                   R_StepJsonFn to_json)
{
    R_assert_not_null(on_tick);
    R_Step *step = R_NEW_INIT_STRUCT(step, R_Step,
            R_MAGIC_INIT(R_Step) NULL, state, on_tick, on_free, to_json, NULL);
    R_MAGIC_CHECK(R_Step, step);
    return step;
}

static void check_parent(const char *child_type, const char *parent_type,
                         void *child, void *actual_parent,
                         void *purported_parent)
{
    if (actual_parent != purported_parent) {
        if (purported_parent) {
            R_die("Catastrophic failure: attempt to free %s %p from %s %p, but"
                  "it's actually owned by %p", child_type, child, parent_type,
                  purported_parent, actual_parent);
        }
        else {
            R_die("Attempting to free %s %p after adding it to %s %p - don't "
                  "do that, it's owned by the %s now and will be freed later",
                  child_type, child, parent_type, actual_parent, parent_type);
        }
    }
}

static void free_step_from(R_Step *step, R_Sequence *seq)
{
    if (step) {
        R_MAGIC_CHECK(R_Step, step);
        check_parent("step", "sequence", step, step->seq, seq);
        if (step->on_free) {
            step->on_free(step->state);
        }
        R_MAGIC_POISON(R_Step, step);
        free(step);
    }
}

void R_step_free(R_Step *step)
{
    free_step_from(step, NULL);
}

void *R_step_state(R_Step *step)
{
    R_MAGIC_CHECK(R_Step, step);
    return step->state;
}


JSON_Value *R_step_to_json(R_Step *step)
{
    if (!step) {
        return json_value_init_null();
    }

    R_MAGIC_CHECK(R_Step, step);
    JSON_Value  *val = json_value_init_object();
    JSON_Object *obj = json_value_get_object(val);

    if (step->to_json) {
        step->to_json(obj, step->state);
    }
    else {
        json_object_set_string(obj, "type", "R_Step");
    }

    return val;
}


R_Sequence *R_sequence_new(void)
{
    R_Sequence *seq = R_NEW_INIT_STRUCT(seq, R_Sequence,
            R_MAGIC_INIT(R_Sequence) NULL, -1, -1, 0, false,
            NULL, NULL, R_user_null(), NULL, NULL, NULL);
    R_MAGIC_CHECK(R_Sequence, seq);
    return seq;
}

static void free_steps(R_Sequence *seq)
{
    R_Step *step = seq->first;
    while (step) {
        R_Step *next = step->next;
        free_step_from(step, seq);
        step = next;
    }
}

static void free_sequence_from(R_Sequence *seq, R_Animator *an)
{
    if (seq) {
        R_MAGIC_CHECK(R_Sequence, seq);
        check_parent("sequence", "animator", seq, seq->an, an);
        free_steps(seq);
        if (seq->on_free) {
            seq->on_free(seq->user);
        }
        R_MAGIC_POISON(R_Sequence, seq);
        free(seq);
    }
}

void R_sequence_free(R_Sequence *seq)
{
    free_sequence_from(seq, NULL);
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
    R_MAGIC_CHECK(R_Sequence, seq);
    R_MAGIC_CHECK(R_Step, step);
    check_step_not_added(step, __func__);
    add_step_to_sequence(seq, step);
    set_up_step_after_adding(seq, step);
}


static bool sequence_has_laps_remaining(R_Sequence *seq)
{
    int max_laps = seq->max_laps;
    return max_laps < 0 || seq->lap < max_laps;
}

static R_StepStatus tick_step(R_Sequence *seq, bool rendered, float seconds)
{
    R_Step *step = seq->current;
    R_MAGIC_CHECK(R_Step, step);
    return step->on_tick((R_StepTickArgs){
            step->state, rendered, seconds, seq->lap, seq->user});
}

static bool keep_stepping_sequence(R_Sequence *seq, bool rendered,
                                   float seconds)
{
    return !seq->killed
        && sequence_has_laps_remaining(seq)
        && tick_step(seq, rendered, seconds) == R_STEP_STATUS_COMPLETE;
}

void R_sequence_tick(R_Sequence *seq, bool rendered, float seconds)
{
    R_MAGIC_CHECK(R_Sequence, seq);
    while (keep_stepping_sequence(seq, rendered, seconds)) {
        if (!(seq->current = seq->current->next)) {
            seq->current = seq->first;
            ++seq->lap;
        }
    }
}


static JSON_Value *steps_to_json(R_Sequence *seq)
{
    JSON_Value *val = json_value_init_array();
    JSON_Array *arr = json_value_get_array(val);

    for (R_Step *step = seq->first; step; step = step->next) {
        json_array_append_value(arr, R_step_to_json(step));
    }

    return val;
}

static int find_current_step_index(R_Step *current, R_Step *steps)
{
    if (current) {
        int i = 0;
        for (R_Step *step = steps; step; step = step->next, ++i) {
            if (step == current) {
                return i;
            }
        }
    }
    return -1;
}

JSON_Value *R_sequence_to_json(R_Sequence *seq)
{
    if (!seq) {
        return json_value_init_null();
    }

    R_MAGIC_CHECK(R_Sequence, seq);
    JSON_Value  *val = json_value_init_object();
    JSON_Object *obj = json_value_get_object(val);

    json_object_set_string(   obj, "type",     "R_Sequence");
    json_object_set_number(   obj, "lap",      R_int2double(seq->lap));
    json_object_set_number(   obj, "max_laps", R_int2double(seq->max_laps));
    R_json_object_set_hexdump(obj, "user",     &seq->user, sizeof(seq->user));

    int step_index = find_current_step_index(seq->current, seq->first);
    if (step_index < 0) {
        json_object_set_null(obj, "current_step_index");
    }
    else {
        json_object_set_number(obj, "current_step_index", step_index);
    }

    json_object_set_value(obj, "steps", steps_to_json(seq));
    return val;
}


R_Animator *R_animator_new(void)
{
    R_Animator *an = R_NEW_INIT_STRUCT(an, R_Animator,
            R_MAGIC_INIT(R_Animator) 0, NULL, NULL);
    R_MAGIC_CHECK(R_Animator, an);
    return an;
}

void R_animator_free(R_Animator *an)
{
    if (an) {
        R_MAGIC_CHECK(R_Animator, an);
        R_Sequence *seq = an->first;

        while (seq) {
            R_Sequence *next = seq->next;
            free_sequence_from(seq, an);
            seq = next;
        }

        R_MAGIC_POISON(R_Animator, an);
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

static int set_up_sequence_after_adding(R_Animator *an, R_Sequence *seq,
                                        int max_laps, R_SequenceDoneFn on_done,
                                        R_SequenceFreeFn on_free, R_UserData user)
{
    seq->an       = an;
    seq->lap      = 0;
    seq->current  = seq->first;
    seq->max_laps = max_laps;
    seq->on_done  = on_done;
    seq->on_free  = on_free;
    seq->user     = user;
    return seq->id = ++an->last_id;
}

int R_animator_add(R_Animator *an, R_Sequence *seq, int max_laps,
                   R_SequenceDoneFn on_done, R_SequenceFreeFn on_free,
                   R_UserData user)
{
    R_assert(max_laps >= 0 || !on_done, "can't use on_done with infinite laps");
    R_MAGIC_CHECK(R_Animator, an);
    R_MAGIC_CHECK(R_Sequence, seq);
    check_sequence_not_added(seq, __func__);
    if (!seq->first) {
        R_die("%s: added sequence is empty", __func__);
    }
    add_sequence_to_animator(an, seq);
    return set_up_sequence_after_adding(an, seq, max_laps,
                                        on_done, on_free, user);
}


static void tick_sequences(R_Animator *an, bool rendered, float seconds)
{
    for (R_Sequence *seq = an->first; seq; seq = seq->next) {
        R_sequence_tick(seq, rendered, seconds);
    }
}

static R_Sequence *filter_dead_sequences(R_Animator *an)
{
    R_Sequence **cur = &an->first;
    R_Sequence *dead = NULL;
    an->last         = NULL;

    while (*cur) {
        R_Sequence *seq = *cur;
        R_MAGIC_CHECK(R_Sequence, seq);
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

static void free_sequences(R_Animator *an, R_Sequence *dead)
{
    while (dead) {
        R_Sequence *seq = dead;
        R_MAGIC_CHECK(R_Sequence, seq);
        dead = dead->next;
        if (!seq->killed && seq->on_done) {
            seq->on_done(seq->user);
        }
        free_sequence_from(seq, an);
    }
}

void R_animator_tick(R_Animator *an, bool rendered, float seconds)
{
    R_MAGIC_CHECK(R_Animator, an);
    tick_sequences(an, rendered, seconds);
    R_Sequence *dead = filter_dead_sequences(an);
    free_sequences(an, dead);
}


static JSON_Value *sequences_to_json(R_Animator *an)
{
    JSON_Value *val = json_value_init_array();
    JSON_Array *arr = json_value_get_array(val);

    for (R_Sequence *seq = an->first; seq; seq = seq->next) {
        json_array_append_value(arr, R_sequence_to_json(seq));
    }

    return val;
}

JSON_Value *R_animator_to_json(R_Animator *an)
{
    if (!an) {
        return json_value_init_null();
    }
    R_MAGIC_CHECK(R_Animator, an);
    return sequences_to_json(an);
}
