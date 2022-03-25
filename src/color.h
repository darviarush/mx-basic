#ifndef __COLOR__H__
#define __COLOR__H__

/**
 * Добавляет макросы для колоризации и некоторые макросы-утилиты
 */

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

#endif
