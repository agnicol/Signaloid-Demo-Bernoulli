# Lines starting with '#' are comments.

# Explicitely specify which files to compile
SOURCES = \
	main.c

# Options that get passed to the C compiler
CFLAGS = -DAIRSPEED=30.0
CFLAGS += -DWINGAREA=1.0
#CFLAGS += -DDEBUG
