// signals.c
#include <signal.h>
#include "common.h"

static void on_stop( int signo ) {

	exitWeedMate();
	struct sigaction sa = { .sa_handler = SIG_DFL };
	sigaction( signo, &sa, NULL );
	raise( signo );

}

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