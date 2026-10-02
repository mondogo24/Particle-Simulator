# Implicits rules and variables are not allowed
MAKEFLAGS += -rR

PROGRAM_NAME := Particle_Simulator

# Rutes
## Includes
INCLUDEDIR := include
TESTINCLUDEDIR := test_include

## Builds
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
TESTBUILDDIR := $(BUILDDIR)/tests
DEBUGDIR := $(BUILDDIR)/debug

## Source code
SRCDIR := src
SRCFILES = $(shell find $(SRCDIR) -name "*.c")
TESTSRCDIR := test
TESTFILES = $(shell find $(TESTSRCDIR) -name "*.test.c")

#Utils
SRCTOOBJ = $(SRCFILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

#Compiler related variables
CC := gcc
CFLAGS_TESTS = -I$(INCLUDEDIR) -I$(TESTINCLUDEDIR)
CFLAGS = -I$(INCLUDEDIR) -Wpedantic
CFLAGS_VECTORIZE = -I$(INCLUDEDIR) -Wpedantic -O3 -fopt-info-vec-all
LDFLAGS = -lm

#Project recipies
$(OBJDIR)/physic/particle.o: $(SRCDIR)/physic/particle.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS_VECTORIZE) -o $@ -c $^

$(OBJDIR)/main.o: main.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $^

$(DEBUGDIR)/$(PROGRAM_NAME): $(SRCTOOBJ) $(OBJDIR)/main.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

#Tests recipies
$(OBJDIR)/%.test.o: $(TESTSRCDIR)/%.test.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS_TESTS) -o $@ -c $^

$(TESTBUILDDIR)/%.test: $(OBJDIR)/%.test.o $(SRCTOOBJ)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS_TESTS) -o $@ $^ $(LDFLAGS)

PHONY := build
build: $(DEBUGDIR)/$(PROGRAM_NAME)

PHONY += run
run: $(DEBUGDIR)/$(PROGRAM_NAME)
	./$(DEBUGDIR)/$(PROGRAM_NAME)

PHONY += test
test: $(TESTFILES:$(TESTSRCDIR)/%.test.c=$(TESTBUILDDIR)/%.test)
	for test in $^; do \
		./$$test; \
	done

PHONY += clean
clean:
	rm -r $(BUILDDIR)/*

.PHONY: PHONY