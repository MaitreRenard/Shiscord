#!/usr/bin/env make

PROJECT_NAME = Shiscord
BINARY = shiscord

.SILENT:

# Default values
.DEFAULT_GOAL := all
SHELL := bash

# Color definition for print purpose
END=\x1b[0m
BOLD=\x1b[1m
UNDER=\x1b[4m
REV=\x1b[7m

GREY=\x1b[30m
RED=\x1b[31m
GREEN=\x1b[32m
YELLOW=\x1b[33m
BLUE=\x1b[34m
PURPLE=\x1b[35m
CYAN=\x1b[36m
WHITE=\x1b[37m
BROWN=\x1b[0;33m
BLUE=\x1b[1;34m
END_COLOR=\x1b[0m

# Source code directory structure
BINDIR := bin
SRCDIR := src
LOGDIR := log
DOCDIR := doc
LIBDIR := lib
TESTDIR := test

# Defines the C Compiler
CC := gcc

# Defines the language standards for GCC
STD := -std=c18

# Protection for stack-smashing attack
SEC_OPTS := -fstack-protector-all -Wstack-protector -z,relro,-z,now -D_FORTIFY_SOURCE=1 

# Specifies to GCC the required warnings
WARNS := -Wall -Wextra -pedantic \
  -Wformat=2 -Wno-unused-parameter -Wshadow \
  -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
  -Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
  -Wjump-misses-init -Wlogical-op

# Flags for compiling
CFLAGS := -O3 $(STD) $(STACK) $(WARNS)

# Debug options
DEBUG := -g3 -DDEBUG=1

# Dependency libraries -lm  -I some/path/to/library
LIBS := 

# Test libraries
TEST_LIBS := -l cmocka -L /usr/lib

# Tests binary file
TEST_BINARY := $(BINARY)_test_runner

# %.o file names
NAMES := $(notdir $(basename $(wildcard $(SRCDIR)/*.c)))
OBJECTS :=$(patsubst %,$(LIBDIR)/%.o,$(NAMES))

# Help message
help:
	echo -e "$(GREEN)all$(END_COLOR)      Compiles and generates binary file"
	echo -e "$(GREEN)tests$(END_COLOR)    Compiles with cmocka and run tests binary file"
	echo -e "$(GREEN)valgrind$(END_COLOR) Runs binary file using valgrind tool"
	echo -e "$(GREEN)clean$(END_COLOR)    Clean the project by removing binaries"
	echo -e "$(GREEN)help$(END_COLOR)     Prints a help message with target rules"
	echo -e "$(GREEN)doxygen$(END_COLOR)  Make the Doxygen doc"

# Rule for link and generate the binary file
all: $(OBJECTS)
	$(CC) -o $(BINDIR)/$(BINARY) $+ $(CFLAGS) $(LIBS)

# Rule for object binaries compilation
$(LIBDIR)/%.o: $(SRCDIR)/%.c
	echo -e "$(BROWN)Compilation en cours $(END_COLOR)";
	$(CC) -c $^ -o $@ $(DEBUG) $(CFLAGS) $(LIBS)


# Rule for run valgrind tool
valgrind:
	echo -e "$(BLUE)Beginning check for memory leak$(END_COLOR)"
	valgrind \
		--track-origins=yes \
		--leak-check=full \
		--leak-resolution=high \
		$(BINDIR)/$(BINARY)
	echo -e "$(BLUE)End of check$(END_COLOR)"


doxygen:
	echo -e "$(BLUE)Start to generate the documentation$(END_COLOR)"
	doxygen
	echo -e "$(BLUE)End of the generation$(END_COLOR)"
	echo -e "$(BROWN)Documentation available in $(DOCDIR)$(END_COLOR)"

# Compile tests and run the test binary
tests:
	echo -e "$(BLUE)Compiling tests$(END_COLOR)"
	$(CC) $(TESTDIR)/main.c -o $(BINDIR)/$(TEST_BINARY) $(DEBUG) $(CFLAGS) $(LIBS) $(TEST_LIBS)
	which ldconfig && ldconfig -C /tmp/ld.so.cache || true # caching the library linking
	echo -en "$(BLUE) Running tests: $(END_COLOR)";
	./$(BINDIR)/$(TEST_BINARY)


# Rule for cleaning the project
clean:
	echo -e "$(BLUE)Start to clean$(END_COLOR)"
	mv $(DOCDIR)/assets/ .;
	rm -rvf $(BINDIR)/* $(LIBDIR)/*.o $(LOGDIR)/* $(DOCDIR)/*;
	mv ./assets/ $(DOCDIR)/
	echo -e "$(BLUE)End cleaning$(END_COLOR)"

