// terminal.h - contains ANSI escape codes. Hacky as FUCK.
// project: weedmate

#ifndef TERMINAL_H
#define TERMINAL_H

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
#define RESET   "\033[0m"

#define COLOR_KEY(char, desc) \
	if(useColor) printf(" press '" YELLOW "%c" RESET "' to %s\n", char, desc); \
	else         printf(" press '%c' to %s\n", char, desc);

#define QUIT_KEY(char, desc) \
	if(useColor) printf(" press '" RED "%c" RESET "' to %s\n", char, desc); \
	else         printf(" press '%c' to %s\n", char, desc);


#endif // TERMINAL_H
