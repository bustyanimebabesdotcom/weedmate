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

#include <stdio.h>
#include <stdlib.h>
#include <weedmate/args.h>
#include <weedmate/common.h>
#include <weedmate/version.h>
#include <input.h>

int main ( int argc, char *argv[] ) {

	if ( handleArgs( argc, argv ) != ARGS_NOT_HANDLED ) return EXIT_CODE;
	
	loadSaveFile();
	atexit(exitWeedMate);
	installSignalHandlers();
	
	int useColor = isatty(STDOUT_FILENO);

	ENTER_ALT_SCREEN();
	CLEAR_SCREEN();
	motd();

	int c;

	while ( ( c = getCharInput () ) != EOF ) {
		switch ( c ) {

			case 'm':
			case 'M':
				CLEAR_SCREEN();
				motd();
				break;

			case 'q':
			case 'Q':
				return EXIT_CODE;

			case 'l':
			case 'L':
				CLEAR_SCREEN();
				printStrainList();
				RETURN_TO_MENU_MSG();
				break;

			case 'p':
			case 'P':
				CLEAR_SCREEN();
				handleStrainPriceLookup();
				RETURN_TO_MENU_MSG();
				break;

			case 'b':
			case 'B':
				CLEAR_SCREEN();
				budTenderMenu();
				RETURN_TO_MENU_MSG();
				break;

			case '+':
				CLEAR_SCREEN();
				strainPriceAdjust( strains, ADJUST_DOUBLE );
				puts( "Strain prices have been" GREEN" doubled" RESET "." );
				RETURN_TO_MENU_MSG();
				break;

			case '-':
				CLEAR_SCREEN();
				strainPriceAdjust( strains, ADJUST_HALVE );
				puts( "Strain prices have been" RED " halved" RESET "." );
				RETURN_TO_MENU_MSG();
				break;

			case 'c':
			case 'C':
				CLEAR_SCREEN();
				weedCalc();
				RETURN_TO_MENU_MSG();
				break;

			case 'r':
			case 'R':
				CLEAR_SCREEN();
				renameStrain();
				RETURN_TO_MENU_MSG();
				break;

			case 's':
			case 'S':
				CLEAR_SCREEN();
				selectCity();
				RETURN_TO_MENU_MSG();
				break;

			default:
				CLEAR_SCREEN();
				puts( "Unrecognized input." );
				RETURN_TO_MENU_MSG();
		}
	}

	return EXIT_CODE;
}
