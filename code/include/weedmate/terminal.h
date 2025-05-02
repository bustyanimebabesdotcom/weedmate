// macros.h - contains ANSI escape codes. Hacky as FUCK.
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

#endif // TERMINAL_H
