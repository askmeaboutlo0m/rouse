MODE ?= debug

ifeq ($(MODE),debug)
    MORE_CFLAGS := -DROUSE_DEBUG -DROUSE_MAGIC -g
else ifeq ($(MODE),cover)
    MORE_CFLAGS := -DROUSE_DEBUG -DROUSE_MAGIC -DROUSE_GCOV -g -fprofile-arcs -ftest-coverage
else
    $(error 'Unknown mode "$(MODE)"')
endif


CC    := gcc
CXX   := g++
AR    := ar
LEX   := lex
YACC  := yacc
RM    := rm -f
PROVE := prove

DEFINES  := -std=c11
INCLUDES :=
FEATURES := -fPIC -fno-exceptions -fsanitize=address
WARNINGS := -Wall -Wextra -Wshadow -Wstrict-prototypes -Wmissing-include-dirs \
            -pedantic -Werror -Wno-error=unused-parameter
CFLAGS   := $(DEFINES) $(INCLUDES) $(FEATURES) $(WARNINGS) $(MORE_CFLAGS)
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

BUILDDIR = build/$(MODE)
OBJECTS  = $(patsubst %.c,$(BUILDDIR)/%.o,$(SOURCES))
OUTPUT   = $(BUILDDIR)/librouse.a

TEST_OUTPUTS := $(patsubst %.c,$(BUILDDIR)/%,$(TEST_SOURCES))
TEST_CFLAGS  := $(CFLAGS) -I. -DTAP_ON_FAIL=1
TEST_LIBS    := -l:libcglm.a -lSDL2 -lSDL2_image -lGLEW -lGLU -lGL -lz -lm
TEST_LDFLAGS := -L. -l:$(OUTPUT) $(TEST_LIBS)

EXAMPLE_OUTPUTS := $(patsubst %.c,$(BUILDDIR)/%,$(EXAMPLE_SOURCES))
EXAMPLE_CFLAGS  := $(CFLAGS) -I.
EXAMPLE_LDFLAGS := $(TEST_LDFLAGS)


all: $(OUTPUT)

test: $(TEST_OUTPUTS)
	$(PROVE) -e '' $(TEST_OUTPUTS)

examples: $(EXAMPLE_OUTPUTS)


$(OUTPUT): $(OBJECTS) | Makefile
	$(AR) $(ARFLAGS) $@ $^

$(BUILDDIR)/%.o: %.c Makefile
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

rouse/geom.h: rouse/geom.pl
	./$< >$@


$(BUILDDIR)/t/%: t/%.c $(OUTPUT)
	mkdir -p $(@D)
	$(CC) $(TEST_CFLAGS) -o $@ $< $(TEST_LDFLAGS)


$(BUILDDIR)/examples/common.o: examples/common.c $(OUTPUT)
	mkdir -p $(@D)
	$(CC) $(EXAMPLE_CFLAGS) -c -o $@ $<

$(BUILDDIR)/examples/%: examples/%.c $(OUTPUT) $(BUILDDIR)/examples/common.o
	mkdir -p $(@D)
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
	$(RM) $(OBJECTS) $(TEST_OUTPUTS) $(shell find -name '*.gcda' -or -name '*.gcno' -or -name '*.gcov')

realclean: clean
	rm -rf build rouse/geom.h


sources:
	@echo
	@find rouse -name '*.c' | sort | perl -ne 'BEGIN { print "SOURCES := " } next if /3rdparty/; chomp; print " \\\n           " if $$x; $$x = 1; print; END { print "\n" }'
	@echo
	@find rouse -name '*.h' | sort | perl -ne 'BEGIN { print "HEADERS := " } next if /3rdparty/; chomp; print " \\\n           " if $$x; $$x = 1; print; END { print "\n" }'
	@echo
	@find t -name '*.c' | sort | perl -ne 'BEGIN { print "TEST_SOURCES := " } chomp; print " \\\n                " if $$x; $$x = 1; print; END { print "\n" }'
	@echo


.PHONY: debug all test examples clean realclean sources coverage
