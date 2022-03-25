.PHONY: all clean

CLIBS = -lreadline
CFLAGS = -Wall -Wextra -Werror -fmax-errors=1 -Wno-parentheses -O4

all: mx
	./mx

mx: mx.c
	gcc $(CFLAGS) $(CLIBS) mx.c -o mx

clean:
	rm -f mx
