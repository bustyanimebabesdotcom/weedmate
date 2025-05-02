// signals.c
/**
 * Handles CTRL-Z and CTRL-C to make sure the user isn't stuck in the alt screen.
 * I sort of understand what this code does, but not line by line.
 * This is hacky and not at all robust. will fix if I manage to break it.
 * TODO: add SA_RESTART, error-check sigactions, guard-async-unsafe calls.
 */

#include <signal.h>
#include <weedmate/common.h>		// for ENTER_ALT_SCREEN(), exitWeedMate()

// on_stop (SIGINT/SIGTERM/SIGTSTP): exit alt buffer then re-raise so the shell suspends/exits.
static void on_stop( int signo ) {

	CLEAR_SCREEN();
	EXIT_ALT_SCREEN();
	struct sigaction sa = { .sa_handler = SIG_DFL };
	sigaction( signo, &sa, NULL );
	raise( signo );

}

// on_cont (SIGCONT): re-enter alt buffer on resume.
static void on_cont( int signo ) {

	(void)signo;
	ENTER_ALT_SCREEN();
}

void installSignalHandlers( void ) {
	
	struct sigaction sa = { .sa_flags = 0 };
	sigemptyset(&sa.sa_mask);
	
	sa.sa_handler = on_stop;
	sigaction( SIGINT,	&sa, NULL );	// Ctrl-c
	sigaction( SIGTERM,	&sa, NULL );	// kill
	sigaction( SIGTSTP, &sa, NULL );	// Ctrl-Z

	sa.sa_handler = on_cont;
	sigaction( SIGCONT, &sa, NULL );

}
