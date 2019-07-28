# Guess if we were run as `emmake make`.
ifdef EMMAKEN_COMPILER
    MODE ?= em
else
    MODE ?= debug
endif

R_CC    ?= gcc
R_CXX   ?= g++
R_AR    ?= ar
R_EMCC  ?= emcc
R_EMCXX ?= em++
R_EMAR  ?= emar
R_PROVE ?= prove
R_NODE  ?= node

DEBUG_CFLAGS := -DROUSE_DEBUG -DROUSE_MAGIC -g -fsanitize=address

ifeq ($(MODE),debug)
    MODE_CFLAGS := $(DEBUG_CFLAGS)
else ifeq ($(MODE),cover)
    MODE_CFLAGS := $(DEBUG_CFLAGS) -DROUSE_GCOV -fprofile-arcs -ftest-coverage
else ifeq ($(MODE),em)
    MODE_CFLAGS := -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'
else
    $(error 'Unknown mode "$(MODE)"')
endif

ifeq ($(MODE),em)
    CC                := $(R_EMCC)
    CXX               := $(R_EMCXX)
    AR                := $(R_EMAR)
    MODE_LIBS         := -lcglm -lGLEW -lGLU -lGL -lm
    TEST_EXECUTOR     := $(R_NODE)
    R_CONFIGURE       := emconfigure
    R_MAKE            := emmake
    EXE_SUFFIX        := .html
    TEST_EXE_SUFFIX   := .js
    MODE_TEST_LDFLAGS := --embed-file t/data -s EXIT_RUNTIME=1
    # This needs to be = instead of := because $< doesn't exist here yet.
    MODE_EXAMPLE_LDFLAGS = `./gather-preloads $<`
else
    CC            := $(R_CC)
    CXX           := $(R_CXX)
    AR            := $(R_AR)
    MODE_LIBS     := -l:libcglm.a -lSDL2 -lSDL2_image -lGLEW -lGLU -lGL -lm
    TEST_EXECUTOR := ''
    R_CONFIGURE   :=
    R_MAKE        :=
endif

PROVE := $(R_PROVE)

DEFINES  ?= -std=c11 $(MORE_DEFINES)
INCLUDES ?= $(MORE_INCLUDES)
FEATURES ?= -fPIC -fno-exceptions $(MORE_FEATURES)
WARNINGS ?= -Wall -Wextra -Wshadow -Wstrict-prototypes -Wmissing-include-dirs \
            -pedantic -Werror -Wno-error=unused-parameter $(MORE_WARNINGS)
CFLAGS   := $(DEFINES) $(INCLUDES) $(FEATURES) $(WARNINGS) $(MODE_CFLAGS)
ARFLAGS  := rcs

SOURCES := rouse/anim/seq.c \
           rouse/anim/util.c \
           rouse/camera.c \
           rouse/common.c \
           rouse/interact/input.c \
           rouse/main.c \
           rouse/model.c \
           rouse/model_parser.c \
           rouse/render/binder.c \
           rouse/render/frame_buffer.c \
           rouse/render/frame_renderer.c \
           rouse/render/gl.c \
           rouse/render/viewport.c \
           rouse/resource.c \
           rouse/stringify.c

HEADERS := rouse/anim/seq.h \
           rouse/anim/util.h \
           rouse/camera.h \
           rouse/common.h \
           rouse/geom.h \
           rouse/interact/input.h \
           rouse/main.h \
           rouse/model.h \
           rouse/render/binder.h \
           rouse/render/frame_buffer.h \
           rouse/render/frame_renderer.h \
           rouse/render/gl.h \
           rouse/render/viewport.h \
           rouse/resource.h \
           rouse/rouse.h \
           rouse/sanity.h \
           rouse/stringify.h

TEST_SOURCES := t/common.c \
                t/model.c \
                t/resource.c \
                t/viewport.c

EXAMPLE_SOURCES := examples/cube/cube.c \
                   examples/skel/skel.c

BUILDDIR   := build/$(MODE)
OBJECTS    := $(patsubst %.c,$(BUILDDIR)/%.o,$(SOURCES))
OUTPUT     := $(BUILDDIR)/librouse.a
EXE_SUFFIX ?=

TEST_EXE_SUFFIX ?= $(EXE_SUFFIX)
TEST_OUTPUTS    := $(patsubst %.c,$(BUILDDIR)/%$(TEST_EXE_SUFFIX),$(TEST_SOURCES))
TEST_CFLAGS     ?= $(CFLAGS) -I. -DTAP_ON_FAIL=1
TEST_LIBS       ?= $(MODE_LIBS)
TEST_LDFLAGS    ?= -L$(BUILDDIR) -lrouse $(TEST_LIBS) $(MODE_TEST_LDFLAGS)

EXAMPLE_OUTPUTS := $(patsubst %.c,$(BUILDDIR)/%$(EXE_SUFFIX),$(EXAMPLE_SOURCES))
EXAMPLE_CFLAGS  ?= $(CFLAGS) -I.
EXAMPLE_LIBS    ?= $(TEST_LIBS)
EXAMPLE_LDFLAGS ?= -L$(BUILDDIR) -lrouse $(EXAMPLE_LIBS) $(MODE_EXAMPLE_LDFLAGS)


all: $(OUTPUT)

test: $(TEST_OUTPUTS)
	$(PROVE) -e $(TEST_EXECUTOR) $(TEST_OUTPUTS)

examples: $(EXAMPLE_OUTPUTS)


$(OUTPUT): $(OBJECTS) | Makefile
	$(AR) $(ARFLAGS) $@ $^

$(BUILDDIR)/%.o: %.c Makefile
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

rouse/geom.h: generate-geom-h
	./$< $@


$(BUILDDIR)/t/%$(TEST_EXE_SUFFIX): t/%.c $(OUTPUT)
	mkdir -p $(@D)
	$(CC) $(TEST_CFLAGS) -o $@ $< $(TEST_LDFLAGS)


$(BUILDDIR)/examples/common.o: examples/common.c $(OUTPUT)
	mkdir -p $(@D)
	$(CC) $(EXAMPLE_CFLAGS) -c -o $@ $<

$(BUILDDIR)/examples/%$(EXE_SUFFIX): examples/%.c $(OUTPUT) $(BUILDDIR)/examples/common.o
	mkdir -p $(@D)
	SOURCE_FILE='$<'; \
		$(CC) $(EXAMPLE_CFLAGS) -o $@ $< $(BUILDDIR)/examples/common.o $(EXAMPLE_LDFLAGS)


$(BUILDDIR)/depend.mk: $(HEADERS) $(SOURCES) Makefile
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MM $(SOURCES) | perl -pe 's{^(\w+\.o):\s*((\S+)/\w+.c)}{$(BUILDDIR)/$$3/$$1: $$2}' > $@

include $(BUILDDIR)/depend.mk


ifeq ($(MODE),cover)
coverage:
	mkdir -p cover
	lcov --capture --directory . --output-file cover/cover.info
	lcov --remove cover/cover.info '/usr/include/*' 't/tap.h' --output-file cover/cover.info
	genhtml cover/cover.info --output-directory cover
endif


clean:
	rm -f $(OBJECTS) $(TEST_OUTPUTS) $(shell find -name '*.gcda' -or -name '*.gcno' -or -name '*.gcov')

realclean: clean
	rm -rf build rouse/geom.h


sources:
	@echo
	@find rouse -name '*.c' | sort | perl -ne 'BEGIN { print "SOURCES := " } chomp; print " \\\n           " if $$x; $$x = 1; print; END { print "\n" }'
	@echo
	@find rouse -name '*.h' | sort | perl -ne 'BEGIN { print "HEADERS := " } chomp; print " \\\n           " if $$x; $$x = 1; print; END { print "\n" }'
	@echo
	@find t -name '*.c' | sort | perl -ne 'BEGIN { print "TEST_SOURCES := " } chomp; print " \\\n                " if $$x; $$x = 1; print; END { print "\n" }'
	@echo


.PHONY: debug all test examples clean realclean sources coverage
