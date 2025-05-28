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
	atexit(exitWeedMate);		// Make sure program runs exit function to leave alternate screen buffer
	installSignalHandlers();	// Make sure we can catch CTRL-Z, CTRL-C, CTRL-D, etc...
	int useColor = isatty(STDOUT_FILENO);

	ENTER_ALT_SCREEN();
	CLEAR_SCREEN();
	motd();

	int c;

	while ( ( c = getCharInput () ) != EOF ) {
		switch ( c ) {

			case 'm':
			case 'M':
				// Prints menu
				CLEAR_SCREEN();
				motd();
				break;

			case 'q':
			case 'Q':
				// clean exit
				CLEAR_SCREEN();
				return EXIT_CODE;

			case 'l':
			case 'L':
				// prints strain list
				CLEAR_SCREEN();
				printStrainList();
				RETURN_TO_MENU_MSG();
				break;

			case 'p':
			case 'P':
				CLEAR_SCREEN();
				// redundant function, might be useful one day
				handleStrainPriceLookup();
				RETURN_TO_MENU_MSG();
				break;

			case 'b':
			case 'B':
				// secret menu
				CLEAR_SCREEN();
				budTenderMenu();
				RETURN_TO_MENU_MSG();
				break;

			case '+':
				// doubles all strain prices
				CLEAR_SCREEN();
				strainPriceAdjust( strains, 1 );
				puts( "Strain prices have been" GREEN" doubled" RESET "." );
				RETURN_TO_MENU_MSG();
				break;

			case '-':
				// halve all strain prices
				CLEAR_SCREEN();
				strainPriceAdjust( strains, -1 );
				puts( "Strain prices have been" RED " halved" RESET "." );
				RETURN_TO_MENU_MSG();
				break;

			case 'c':
			case 'C':
				// opens the calculator
				CLEAR_SCREEN();
				weedCalc();
				RETURN_TO_MENU_MSG();
				break;

			case 'r':
			case 'R':
				// lets user rename strains
				CLEAR_SCREEN();
				renameStrain();
				RETURN_TO_MENU_MSG();
				break;

			case 's':
			case 'S':
				// lets the user select city
				CLEAR_SCREEN();
				selectCity();
				RETURN_TO_MENU_MSG();
				break;

			default:
				// in case of oh shit moment
				CLEAR_SCREEN();
				puts( "Unrecognized input." );
				RETURN_TO_MENU_MSG();

		}
	}

	return EXIT_CODE;
}
