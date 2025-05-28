// terminal.h - contains ANSI escape codes. Hacky as FUCK.
// project: weedmate

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <unistd.h>

// === Terminal control ===
#define ENTER_ALT_SCREEN()	\
	do { \
		ssize_t _ret = write(STDOUT_FILENO, "\x1b[?1049h", 8); \
		(void)_ret; \
	} while( 0 )

#define EXIT_ALT_SCREEN()	\
	do { \
		ssize_t _ret = write(STDOUT_FILENO, "\x1b[?1049l", 8); \
		(void)_ret; \
	} while( 0 )

#define CLEAR_SCREEN()	\
	do { \
		ssize_t _ret = write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7); \
		(void)_ret; \
	} while( 0 )

// === Terminal colors ===
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define UNDER   "\033[4m"
#define RESET   "\033[0m"

#define COLOR_KEY(char, desc) \
	if(useColor) printf(" Press [" YELLOW "%c" RESET "] to %s\n", char, desc); \
	else         printf(" Press [%c] to %s\n", char, desc);

#define QUIT_KEY(char, desc) \
	if(useColor) printf(" Press [" RED "%c" RESET "] to %s\n", char, desc); \
	else         printf(" Press [%c] to %s\n", char, desc);

#define ANY_KEY(key, desc) \
	if(useColor) printf("\nPress " YELLOW "%s" RESET " to %s\n", key, desc); \
	else         printf("\nPress [%s] to %s\n", key, desc);

#define MENU_KEY(char, desc) \
	if(useColor) printf("\nPress [" YELLOW "%c" RESET "] to %s\n", char, desc); \
	else         printf("\nPress [%c] to %s\n", char, desc);

#define ANY_KEY_MSG() \
	ANY_KEY("any key", "continue"); \
	printf("\n> ");

#define RETURN_TO_MENU_MSG() \
	MENU_KEY( 'm', "return to main menu." ); \
	printf( "\n> " );

#endif // TERMINAL_H_
