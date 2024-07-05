# Lines starting with '#' are comments.

# Explicitely specify which files to compile
SOURCES = $(wildcard *.c)

# Options that get passed to the C compiler
CFLAGS = -I./

#CFLAGS += -DDEBUG
