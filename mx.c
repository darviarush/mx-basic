#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>


#define BOLD			"\033[1m"
#define UNDERLINE		"\033[4m"
#define END				"\033[0m"

#define BLACK			"\033[30m"
#define ON_BLACK		"\033[40m"
#define RED				"\033[31m"
#define ON_RED			"\033[41m"
#define GREEN			"\033[32m"
#define ON_GREEN		"\033[42m"
#define YELLOW			"\033[33m"
#define ON_YELLOW		"\033[43m"
#define BLUE			"\033[34m"
#define ON_BLUE			"\033[44m"
#define MAGENTA			"\033[35m"
#define ON_MAGENTA		"\033[45m"
#define CYAN			"\033[36m"
#define ON_CYAN			"\033[46m"
#define WHITE			"\033[37m"
#define ON_WHITE		"\033[47m"

#define LBLACK			"\033[90m"
#define ON_LBLACK		"\033[100m"
#define LRED			"\033[91m"
#define ON_LRED			"\033[101m"
#define LGREEN			"\033[92m"
#define ON_LGREEN		"\033[102m"
#define LYELLOW			"\033[93m"
#define ON_LYELLOW		"\033[103m"
#define LBLUE			"\033[94m"
#define ON_LBLUE		"\033[104m"
#define LMAGENTA		"\033[95m"
#define ON_LMAGENTA		"\033[105m"
#define LCYAN			"\033[96m"
#define ON_LCYAN		"\033[106m"
#define LWHITE			"\033[97m"
#define ON_LWHITE		"\033[107m"


static char *_read_line = NULL;
static char *line = NULL;
static char *file = NULL;

static char *prompt = CYAN "🐈" END;



void mx_free() {
	if(_read_line) free(_read_line);
	_read_line = NULL;
	line = NULL;
}

char* mx_trim(char* s) {
	while(isspace(*s)) s++;
	char* m = s+strlen(s)-1;
	if(m>s) { while(isspace(*m)) m--; m[1] = '\0'; }
	return s;
}

char* mx_gets() {
	mx_free();
  	
  	_read_line = readline(prompt);
  	if(!_read_line) return NULL;
  	
  	line = mx_trim(_read_line);

  	if(*line) add_history(line);

  	return line;
}

void mx_save_line() {
	FILE* f = fopen(file, "rb");
	if(!f) { fprintf(stderr, "Нет такого.\n"); return; }
	char buf[1024*1024];
	while(fgets(f, buf, sizeof(buf))) fprintf(o, "%s", buf);
}

void mx_command() {
	if(isnum(*line)) {
		mx_save_line();
	}
	else {
		printf(RED "?" END "\n");
	}
}

int main() {
	while(mx_gets()) {
		mx_command();
	}
	mx_free();
	return 0;
}
