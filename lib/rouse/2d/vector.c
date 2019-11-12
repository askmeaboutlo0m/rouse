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
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <cglm/struct.h>
#include "../3rdparty/nanovg_inc.h"
#include "../common.h"
#include "../parse.h"
#include "nvg.h"
#include "refcount.h"
#include "vector.h"

/*
 * ASCII characters for parsing these commands. These could also be the values
 * of the `R_VectorCommandType` enum, but since so much `switch`ing happens
 * over them I'd rather not spread the values all over a sparse range.
 */
#define R_VECTOR_CHAR_BEGIN     0x61 /* a */
#define R_VECTOR_CHAR_TRANSFORM 0x74 /* t */
#define R_VECTOR_CHAR_COLOR     0x63 /* c */
#define R_VECTOR_CHAR_MOVE      0x4d /* M */
#define R_VECTOR_CHAR_LINE      0x4c /* L */
#define R_VECTOR_CHAR_BEZIER    0x43 /* C */
#define R_VECTOR_CHAR_WINDING   0x77 /* w */
#define R_VECTOR_CHAR_FILL      0x66 /* f */

typedef enum R_VectorCommandType {
    R_VECTOR_COMMAND_BEGIN,
    R_VECTOR_COMMAND_TRANSFORM,
    R_VECTOR_COMMAND_COLOR,
    R_VECTOR_COMMAND_MOVE,
    R_VECTOR_COMMAND_LINE,
    R_VECTOR_COMMAND_BEZIER,
    R_VECTOR_COMMAND_WINDING,
    R_VECTOR_COMMAND_FILL,
} R_VectorCommandType;

#define R_VECTOR_COMMAND_HEADER \
    R_MAGIC_FIELD \
    R_VectorCommandType type

typedef struct R_VectorCommandAny {
    R_VECTOR_COMMAND_HEADER;
} R_VectorCommandAny;

typedef struct R_VectorCommandBegin {
    R_VECTOR_COMMAND_HEADER;
} R_VectorCommandBegin;

typedef struct R_VectorCommandTransform {
    R_VECTOR_COMMAND_HEADER;
    float matrix[6];
} R_VectorCommandTransform;

typedef struct R_VectorCommandColor {
    R_VECTOR_COMMAND_HEADER;
    NVGcolor color;
} R_VectorCommandColor;

typedef struct R_VectorCommandMove {
    R_VECTOR_COMMAND_HEADER;
    float x, y;
} R_VectorCommandMove;

typedef struct R_VectorCommandLine {
    R_VECTOR_COMMAND_HEADER;
    float x, y;
} R_VectorCommandLine;

typedef struct R_VectorCommandBezier {
    R_VECTOR_COMMAND_HEADER;
    float c1x, c1y, c2x, c2y, x, y;
} R_VectorCommandBezier;

typedef struct R_VectorCommandWinding {
    R_VECTOR_COMMAND_HEADER;
    int winding;
} R_VectorCommandWinding;

typedef struct R_VectorCommandFill {
    R_VECTOR_COMMAND_HEADER;
} R_VectorCommandFill;

typedef union R_VectorCommand {
    R_VectorCommandAny       any;
    R_VectorCommandBegin     begin;
    R_VectorCommandTransform transform;
    R_VectorCommandColor     color;
    R_VectorCommandMove      move;
    R_VectorCommandLine      line;
    R_VectorCommandBezier    bezier;
    R_VectorCommandWinding   winding;
    R_VectorCommandFill      fill;
} R_VectorCommand;

struct R_VectorImage {
    R_MAGIC_FIELD
    int             refs;
    int             width;
    int             height;
    int             count;
    R_VectorCommand commands[];
};


static void check_file_magic(R_Parse *parse)
{
    static const char FILE_MAGIC[] = "rvector1";
    R_parse_read_bytes(parse, sizeof(FILE_MAGIC));
    if (memcmp(parse->buffer, FILE_MAGIC, sizeof(FILE_MAGIC)) != 0) {
        R_PARSE_DIE(parse, "bad file magic, want \"%s\"", FILE_MAGIC);
    }
}


static void parse_begin(R_VectorCommandBegin *command)
{
    R_MAGIC_SET(command);
    command->type = R_VECTOR_COMMAND_BEGIN;
}

static void parse_transform(R_Parse *parse, R_VectorCommandTransform *command)
{
    R_MAGIC_SET(command);
    command->type      = R_VECTOR_COMMAND_TRANSFORM;
    command->matrix[0] = R_parse_read_float(parse);
    command->matrix[1] = R_parse_read_float(parse);
    command->matrix[2] = R_parse_read_float(parse);
    command->matrix[3] = R_parse_read_float(parse);
    command->matrix[4] = R_parse_read_float(parse);
    command->matrix[5] = R_parse_read_float(parse);
}

static void parse_color(R_Parse *parse, R_VectorCommandColor *command)
{
    unsigned char r = R_parse_read_uchar(parse);
    unsigned char g = R_parse_read_uchar(parse);
    unsigned char b = R_parse_read_uchar(parse);
    unsigned char a = R_parse_read_uchar(parse);
    R_MAGIC_SET(command);
    command->type  = R_VECTOR_COMMAND_COLOR;
    command->color = nvgRGBA(r, g, b, a);
}

static void parse_move(R_Parse *parse, R_VectorCommandMove *command)
{
    R_MAGIC_SET(command);
    command->type = R_VECTOR_COMMAND_MOVE;
    command->x    = R_parse_read_float(parse);
    command->y    = R_parse_read_float(parse);
}

static void parse_line(R_Parse *parse, R_VectorCommandLine *command)
{
    R_MAGIC_SET(command);
    command->type = R_VECTOR_COMMAND_LINE;
    command->x    = R_parse_read_float(parse);
    command->y    = R_parse_read_float(parse);
}

static void parse_bezier(R_Parse *parse, R_VectorCommandBezier *command)
{
    R_MAGIC_SET(command);
    command->type = R_VECTOR_COMMAND_BEZIER;
    command->c1x  = R_parse_read_float(parse);
    command->c1y  = R_parse_read_float(parse);
    command->c2x  = R_parse_read_float(parse);
    command->c2y  = R_parse_read_float(parse);
    command->x    = R_parse_read_float(parse);
    command->y    = R_parse_read_float(parse);
}

static void parse_winding(R_Parse *parse, R_VectorCommandWinding *command)
{
    int parsed = R_parse_read_uchar(parse);
    int winding;
    if (parsed == 0) {
        winding = NVG_CCW;
    }
    else if (parsed == 1) {
        winding = NVG_CW;
    }
    else {
        R_PARSE_DIE(parse, "unknown winding: %d", parsed);
    }
    R_MAGIC_SET(command);
    command->type    = R_VECTOR_COMMAND_WINDING;
    command->winding = winding;
}

static void parse_fill(R_VectorCommandFill *command)
{
    R_MAGIC_SET(command);
    command->type = R_VECTOR_COMMAND_FILL;
}

static void parse_command(R_Parse *parse, R_VectorCommand *command)
{
    int c = R_parse_read_uchar(parse);
    switch (c) {
        case R_VECTOR_CHAR_BEGIN:
            R_PARSE_DEBUG(parse, "%d: begin", c);
            parse_begin(&command->begin);
            break;
        case R_VECTOR_CHAR_TRANSFORM:
            R_PARSE_DEBUG(parse, "%d: transform", c);
            parse_transform(parse, &command->transform);
            break;
        case R_VECTOR_CHAR_COLOR:
            R_PARSE_DEBUG(parse, "%d: color", c);
            parse_color(parse, &command->color);
            break;
        case R_VECTOR_CHAR_MOVE:
            R_PARSE_DEBUG(parse, "%d: move", c);
            parse_move(parse, &command->move);
            break;
        case R_VECTOR_CHAR_LINE:
            R_PARSE_DEBUG(parse, "%d: line", c);
            parse_line(parse, &command->line);
            break;
        case R_VECTOR_CHAR_BEZIER:
            R_PARSE_DEBUG(parse, "%d: bezier", c);
            parse_bezier(parse, &command->bezier);
            break;
        case R_VECTOR_CHAR_WINDING:
            R_PARSE_DEBUG(parse, "%d: winding", c);
            parse_winding(parse, &command->winding);
            break;
        case R_VECTOR_CHAR_FILL:
            R_PARSE_DEBUG(parse, "%d: fill", c);
            parse_fill(&command->fill);
            break;
        default:
            R_PARSE_DIE(parse, "unknown command type: %d (%c)", c, (char) c);
    }
}

static R_VectorImage *make_vector_image(int width, int height, int count)
{
    R_VectorImage *vi = R_malloc(
            sizeof(*vi) + R_int2size(count) * sizeof(vi->commands[0]));
    R_MAGIC_SET(vi);
    vi->refs   = 1;
    vi->width  = width;
    vi->height = height;
    vi->count  = count;
    return vi;
}

static R_VectorImage *parse_vector_image(R_Parse *parse)
{
    int width  = R_parse_read_ushort(parse);
    int height = R_parse_read_ushort(parse);
    int count  = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "width: %d, height: %d, command count: %d",
                  width, height, count);

    R_VectorImage *vi = make_vector_image(width, height, count);
    for (int i = 0; i < count; ++i) {
        R_PARSE_DEBUG(parse, "command %d/%d", i + 1, count);
        parse_command(parse, &vi->commands[i]);
    }
    return vi;
}


static inline void check_vector_image(R_VectorImage *vi)
{
    R_MAGIC_CHECK(vi);
    R_assert(vi->refs > 0, "refcount must always be positive");
}


#ifdef ROUSE_MAGIC
static void check_command_magic(R_VectorCommand *c)
{
    R_VectorCommandType type = c->any.type;
    switch (type) {
        case R_VECTOR_COMMAND_BEGIN:
            R_MAGIC_CHECK(&c->begin);
            break;
        case R_VECTOR_COMMAND_TRANSFORM:
            R_MAGIC_CHECK(&c->transform);
            break;
        case R_VECTOR_COMMAND_COLOR:
            R_MAGIC_CHECK(&c->color);
            break;
        case R_VECTOR_COMMAND_MOVE:
            R_MAGIC_CHECK(&c->move);
            break;
        case R_VECTOR_COMMAND_LINE:
            R_MAGIC_CHECK(&c->line);
            break;
        case R_VECTOR_COMMAND_BEZIER:
            R_MAGIC_CHECK(&c->bezier);
            break;
        case R_VECTOR_COMMAND_WINDING:
            R_MAGIC_CHECK(&c->winding);
            break;
        case R_VECTOR_COMMAND_FILL:
            R_MAGIC_CHECK(&c->fill);
            break;
        default:
            R_die("Unknown vector command type %d", type);
    }
}

static void check_magics(R_VectorImage *vi)
{
    check_vector_image(vi);
    for (int i = 0; i < vi->count; ++i) {
        check_command_magic(&vi->commands[i]);
    }
}
#endif

R_VectorImage *R_vector_image_new(const char *title, R_ParseReadFn read,
                                  R_UserData user, int bufsize,
                                  unsigned char buffer[static bufsize])
{
    R_Parse parse = R_parse(title, read, user, bufsize, buffer);
    check_file_magic(&parse);

    R_PARSE_DEBUG(&parse, "parsing vector image with bufsize %d", bufsize);

    R_VectorImage *vi = parse_vector_image(&parse);
    check_vector_image(vi);

    R_parse_die_unless_eof(&parse);
#ifdef ROUSE_MAGIC
    check_magics(vi);
#endif
    return vi;
}

R_VectorImage *R_vector_image_from_file(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        R_die("Can't open '%s': %s", path, strerror(errno));
    }

    unsigned char buffer[256];
    R_VectorImage *vi = R_vector_image_new(
            path, R_parse_from_file, R_user_data(fp), sizeof(buffer), buffer);

    if (fclose(fp) != 0) {
        R_warn("Error closing '%s': %s", path, strerror(errno));
    }

    return vi;
}

static void free_vector_image(R_VectorImage *vi)
{
    R_MAGIC_POISON(vi);
    free(vi);
}

R_DEFINE_REFCOUNT_FUNCS(R_VectorImage, vector_image, refs)


int R_vector_image_width(R_VectorImage *vi)
{
    check_vector_image(vi);
    return vi->width;
}

int R_vector_image_height(R_VectorImage *vi)
{
    check_vector_image(vi);
    return vi->height;
}


static void run_begin(R_VectorCommandBegin *command, NVGcontext *ctx)
{
    R_MAGIC_CHECK(command);
    nvgBeginPath(ctx);
}

static void run_transform(R_VectorCommandTransform *command, NVGcontext *ctx,
                          const float parent_matrix[static 6])
{
    R_MAGIC_CHECK(command);
    R_nvg_transform_set_2(ctx, parent_matrix, command->matrix);
}

static void run_color(R_VectorCommandColor *command, NVGcontext *ctx)
{
    R_MAGIC_CHECK(command);
    nvgFillColor(ctx, command->color);
}

static void run_move(R_VectorCommandMove *command, NVGcontext *ctx)
{
    R_MAGIC_CHECK(command);
    nvgMoveTo(ctx, command->x, command->y);
}

static void run_line(R_VectorCommandLine *command, NVGcontext *ctx)
{
    R_MAGIC_CHECK(command);
    nvgLineTo(ctx, command->x, command->y);
}

static void run_bezier(R_VectorCommandBezier *command, NVGcontext *ctx)
{
    R_MAGIC_CHECK(command);
    nvgBezierTo(ctx, command->c1x, command->c1y,
                     command->c2x, command->c2y,
                     command->x,   command->y);
}

static void run_winding(R_VectorCommandWinding *command, NVGcontext *ctx)
{
    R_MAGIC_CHECK(command);
    nvgPathWinding(ctx, command->winding);
}

static void run_fill(R_VectorCommandFill *command, NVGcontext *ctx)
{
    R_MAGIC_CHECK(command);
    nvgFill(ctx);
}

static void run_vector_command(R_VectorCommand *command, NVGcontext *ctx,
                               const float parent_matrix[static 6])
{
    R_VectorCommandType type = command->any.type;
    switch (type) {
        case R_VECTOR_COMMAND_BEGIN:
            run_begin(&command->begin, ctx);
            break;
        case R_VECTOR_COMMAND_TRANSFORM:
            run_transform(&command->transform, ctx, parent_matrix);
            break;
        case R_VECTOR_COMMAND_COLOR:
            run_color(&command->color, ctx);
            break;
        case R_VECTOR_COMMAND_MOVE:
            run_move(&command->move, ctx);
            break;
        case R_VECTOR_COMMAND_LINE:
            run_line(&command->line, ctx);
            break;
        case R_VECTOR_COMMAND_BEZIER:
            run_bezier(&command->bezier, ctx);
            break;
        case R_VECTOR_COMMAND_WINDING:
            run_winding(&command->winding, ctx);
            break;
        case R_VECTOR_COMMAND_FILL:
            run_fill(&command->fill, ctx);
            break;
        default:
            R_die("Unknown vector command type %d", type);
    }
}

void R_vector_image_draw(R_VectorImage *vi, NVGcontext *ctx,
                         const float parent_matrix[static 6])
{
    check_vector_image(vi);
    int count = vi->count;
    for (int i = 0; i < count; ++i) {
        run_vector_command(&vi->commands[i], ctx, parent_matrix);
    }
}
