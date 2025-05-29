/*
 * main.c — Terminal ritual grounds for WeedMate
 * Functions live in:       functions.c
 * Input handling:			input.c
 *							input.h
 * Structs:					data.c
 * Strain logic:			strains.c
 * Signals:					signals.c
 * Declarations:   			common.h
 * Macros:					macros.h
 * project:					weedmate
 * 
 * NOTE: glibc lazily allocates 1KB buffer for stdout.
 * This triggers a false leak warning in Heaptrack.
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
