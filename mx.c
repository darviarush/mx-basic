#define _GNU_SOURCE

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


#define asf(FMT, ARGS...)		({ char* s; asprintf(&s, FMT, ARGS); s })


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


#define BUFSIZE		1024*1024
void mx_save_line() {
	if(!file) { fprintf(stderr, "Откройте метод командой "WHITE"o"MAGENTA"pen "CYAN"<signature>"END".\n"); return; }
	
	int lineno = atoi(line);
	int size = strlen(line);
	if(BUFSIZE-1 < size) { fprintf(stderr, "Записываемая строка слишком велика.\n"); return; }

	FILE* f = fopen(file, "rb");
	if(!f) { fprintf(stderr, "Нет файла %s.\n", file); return; }
	char buf[BUFSIZE]; 	// буффер ввода

	/***
		1. дойти до нужной строки
		2. открыть на запись и перейти к началу строки
		3. скачать блок больше записываемой строки
		4. записать строку и пол блока
		5. заменить пол блока на незаписанный и загрузить ещё пол блока
		6. менять их местами до конца файла

		Такой алгоритм позволит редактировать очень большие файлы без расхода прамяти... но ждать всё равно придётся.
		С другой стороны, это позволит быстрее, без обнаружения конца строки, перекачать конец файла.
	***/

	int pos = 0;
	while( fgets(buf, BUFSIZE, f) ) {
		int n = atoi(buf);
		
		if(lineno == n) { // заменяем строку: отбрасываем
			line[0] = '\0';
			break;
		}
		else if(n > lineno) {	// pos - в конец
			pos += strlen(buf);
			break;
		}
		pos = ftell(f);
	}

	// 2:
	FILE* o = fopen(file, "ab");
	fseek(o, pos, 0);
	
	int i = fread(buf, BUFSIZE, 1, f);
	fprintf(o, "%s\n", line);
	
	while( i ) {
		fwrite(buf, BUFSIZE, 1, o);
		i = fread(buf, BUFSIZE, 1, f);
	}
	
	fclose(f);
	fclose(o);
}

void mx_set_file(char* s) {
	if(file) free(file);
	file = strdup(s);
}

char* is_command(char** commands) {
	char* s = line;
	while(*commands) {
		char* p = *commands++;
		while(*s && *s == *p) { s++; p++; }
		if(*p == '\0' && isspace(*s)) {
			while( *s && isspace(*s) ) s++;
			return s;
		} 
	}
	return NULL;
}

char* _new_command[] = {"n", "new", NULL};
char* _open_command[] = {"o", "open", NULL};

void mx_command() {
	char* s;
	
	
	if( isdigit(*line) ) {
		mx_save_line();
	}
	else if( s = is_command(_new_command) ) {
		if(!*s) { fprintf(stderr, "Нет сигнатуры.\n"); return; }

		FILE* f = fopen(s, "a");
		if(!f) { perror(RED "?" END); return; }
		
		mx_set_file(s);
		
		printf(RED "Ok" END "\n");
	}
	else if( s = is_command(_open_command) ) {
		if(!*s) { fprintf(stderr, "Нет сигнатуры.\n"); return; }

		FILE* f = fopen(s, "r");
		if(!f) { fprintf(stderr, "Нет файла.\n"); return; }
		
		mx_set_file(s);
		
		// жест "ок": 👌
		// ок в квадрате 🆗
		printf(RED "Ok" END "\n");
	}
	else {
		printf(RED "?" END "\n");
	}
}

int main() {
	while( mx_gets() ) mx_command();

	mx_free();
	return 0;
}
