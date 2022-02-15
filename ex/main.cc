#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

#include "ed.cc"
#include "sua.cc"


int main(int ac, char** av) {
	printf("%i\n", ac);

	if (ac > 1 && string(av[1]) == "-d") {
		// By default readline does filename completion. With -d, we disable this
		// by asking readline to just insert the TAB character itself.
		rl_bind_key('\t', rl_insert);
	}

	char* buf;
	while ((buf = readline(">> ")) != nullptr) {
		if (strlen(buf) > 0) {
			add_history(buf);
		}

		printf("[%s]\n", buf);

		// readline malloc's a new buffer every time.
		free(buf);
	}

	return 0;
}
