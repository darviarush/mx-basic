.PHONY: all clean

CLIBS = -lreadline
CFLAGS = -Wall -Wextra -Werror -fmax-errors=1 -O4

all: sua
	./sua

sua: main.cc ed.cc sua.cc
	g++ $(CFLAGS) $(CLIBS) main.cc -o 1

clean:
	rm -f 1
