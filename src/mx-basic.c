#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>



static char *_read_line = NULL;
static char *line = NULL;
static char *file = NULL;

//static char *prompt = CYAN "🐈" END;
static char *prompt = CYAN "\xf0\x9f\x90\x88 " END;



void mx_free() {
	if(_read_line) free(_read_line);
	_read_line = NULL;
	line = NULL;
}

char* mx_trim(char* s) {
	while(isspace(*s)) s++;
	char* m = s+strlen(s)-1;
	if(m > s) { while(isspace(*m)) m--; m[1] = '\0'; }
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

	FILE* f = fopen(file, "rb+");
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
		
		10 x
		20 y
		
		1. insert line - pos -= length line
		
		10 x
		15 к
		20 y
		
		2. replace line - 
		
		10 y
		20 y
	***/

	FILE* o = fopen(file, "rb+");
	if(!o) { perror(RED"open file"END); return; }
	fseek(o, 0, SEEK_END);
	
	while( fgets(buf, BUFSIZE, f) ) {
		int n = atoi(buf);
		
		if(lineno == n) {		// заменить строку
			long int x = strlen(buf);
			fseek(o, ftell(f) - x, SEEK_SET);	// вставляем на строку назад
			// читаем c текущего места
			break;
		}
		else if(n > lineno) {					// вставить строку
			long int x = strlen(buf);
			fseek(o, ftell(f) - x, SEEK_SET);	// вставляем на строку назад
			fseek(f, x, SEEK_CUR); 				// читаем на строку назад
			break;
		}
	}

	// 2:
	
	int i = fread(buf, BUFSIZE, 1, f);
	fprintf(o, "%s\n", line);
	
	while( i ) {
		fwrite(buf, i, 1, o);
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
	while(*commands) {
		char* s = line;
		char* p = *commands++;
		
		while(*s && *s == *p) { s++; p++; }
		
		if( *p != '\0' ) continue;
		if( isalpha(*s) ) continue;
		
		while( *s && isspace(*s) ) s++;

		return s;
	}
	return NULL;
}

int maybe_uint(char** f) {
	char *s = *f;
	while( isdigit(*s) ) s++;
	int x = -1;
	if(s != *f) x = atoi(*f);
	*f = s;
	return x;
}

void mx_list(char* s) {
	// формат l10-20, l -30, l 40
	int from = maybe_uint(&s), to = 2147483647;
	if(*s == '-') { s++; to = maybe_uint(&s); if(to == -1) to = 2147483647; }
	
	if(*s != '\0') { fprintf(stderr, RED"??"END"\n"); return; }

	FILE* f = fopen(file, "rb");
	if(!f) { perror(RED"use: open file"END); return; }

	char buf[BUFSIZE];
	
	while( fgets(buf, BUFSIZE, f) ) {
		int i = atoi(buf);
		if(i > to) break;
		if(i >= from) printf("%s", buf);
	}
}

char* _new_command[] = {"n", "new", NULL};
char* _open_command[] = {"o", "open", NULL};
char* _list_command[] = {"l", "list", NULL};

void mx_command() {
	char* s;
	
	
	if( isdigit(*line) ) {
		mx_save_line();
	}
	else if( s = is_command(_list_command) ) {
		mx_list(s);
	}
	else if( s = is_command(_new_command) ) {
		if(!*s) { fprintf(stderr, "Нет сигнатуры.\n"); return; }

		FILE* f = fopen(s, "a");
		if(!f) { perror(RED"?"END); return; }
		
		mx_set_file(s);
		
		printf(RED"Ok"END"\n");
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
