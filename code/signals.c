// signals.c
// project: weedmate

/**
 * Handles CTRL-Z and CTRL-C to make sure the user isn't stuck in the alt screen.
 * SIGINT, SIGTERM, and SIGTSTP exit alt screen and re-raise default signal.
 * SIGCONT resumes alt screen after a pause.
 *
 * Adds SA_RESTART so syscalls like fgets() don't break when signals hit.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <weedmate/common.h>

/**
 * on_stop - Handles termination or suspend signals (like CTRL-C / CTRL-Z).
 * Clears screen, exits alt screen, restores default handler and re-raises signal.
 */
static void on_stop( int signo ) {

	CLEAR_SCREEN();
	EXIT_ALT_SCREEN();

	// Reset signal action to default
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);

	// Restore default handler for the signal
	if ( sigaction(signo, &sa, NULL) != 0 ) {
		fprintf(stderr, "sigaction restore failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Raise the signal again with default behavior
	raise(signo);
}

/**
 * on_cont - Handles resume signal (SIGCONT).
 * Re-enters alternate screen mode.
 */
static void on_cont( int signo ) {
	(void)signo; // Unused
	ENTER_ALT_SCREEN();
}

/**
 * installSignalHandlers - Installs all signal handlers needed for alt screen support.
 * Catches CTRL-C, CTRL-Z, kill, and resume signals to keep UI consistent.
 */
void installSignalHandlers( void ) {

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);

	// Handle exit/suspend signals
	sa.sa_handler = on_stop;
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGINT, &sa, NULL) != 0 ||
		sigaction(SIGTERM, &sa, NULL) != 0 ||
		sigaction(SIGTSTP, &sa, NULL) != 0) {
		fprintf(stderr, "Failed to install stop signal handlers: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Handle resume signal
	sa.sa_handler = on_cont;
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGCONT, &sa, NULL) != 0) {
		fprintf(stderr, "Failed to install continue signal handler: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}
