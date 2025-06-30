// main.c
// project: weedmate

/** 
 * NOTE: glibc lazily allocates 1KB buffer for stdout.
 * This triggers a false leak warning in Heaptrack for 2KB.
 * Harmless - OS reclaims it at exit.
 * DO NOT workaround this on non-glibc platforms (portability risk).
 */

#include <stdlib.h>
#include <weedmate/args.h>
#include <weedmate/common.h>
#include <weedmate/menu.h>
#include <input.h>

int main ( int argc, char *argv[] ) {

	if ( handleArgs( argc, argv ) != ARGS_NOT_HANDLED ) return EXIT_CODE;
	
	loadSaveFile();
	atexit(exitWeedMate);
	installSignalHandlers();
	
	initMenu();

	return EXIT_CODE;
}
