// main.c — Terminal ritual grounds for WeedMate
// Functions live in:       functions.c
// Input handling:			input.c
// Declarations:   			common.h
// Macros:					macros.h
// project:					weedmate

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "input/input.h"

int main ( void ) {

	// Enter alternate screen buffer — user won’t see their terminal history after quitting.
	ENTER_ALT_SCREEN();
	atexit(exitWeedMate);
	CLEAR_SCREEN();
	motd();

	int c;

	// Main loop, runs until EOF
	while ( ( c = getchar () ) != EOF ) {

	// Ignore stray newline inputs
	if ( c == '\n' ) {
			continue;
		}

		// Input processing
		switch ( c ) {

			case 'm':
			case 'M':
				// Basically back to menu key. reprints the motd
				CLEAR_SCREEN();
				motd();
				break;

			case 'q':
			case 'Q':
				// clean exit
				CLEAR_SCREEN();
				EXIT_ALT_SCREEN();
				puts( "Thank you for using weedmate!" );
				putchar( '\n' );
				printf( "Terminating with exit code %d!\n", EXIT_CODE );
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
				// redundant function, might be useful one day
				CLEAR_SCREEN();
				flushInputBuffer();
				handleStrainPriceLookup();
				RETURN_TO_MENU_MSG();
				break;

			case 'b':
			case 'B':
				// secret menu
				CLEAR_SCREEN();
				flushInputBuffer();
				budTenderMenu();
				RETURN_TO_MENU_MSG();
				break;

			case '+':
				// doubles all strain prices
				CLEAR_SCREEN();
				strainPriceAdjust( strains, 1 );
				puts( "Strain prices have been doubled." );
				RETURN_TO_MENU_MSG();
				break;

			case '-':
				// halve all strain prices
				CLEAR_SCREEN();
				strainPriceAdjust( strains, -1 );
				puts( "Strain prices have been cut in half." );
				RETURN_TO_MENU_MSG();
				break;

			case 'c':
			case 'C':
				CLEAR_SCREEN();
				flushInputBuffer();
				weedCalc();
				RETURN_TO_MENU_MSG();
				break;
			default:
				// in case of oh shit moment
				CLEAR_SCREEN();
				puts( "Unrecognized input." );
				RETURN_TO_MENU_MSG();

		}
	}

	// exitWeedMate function helps us leave the alt screen clean
	exitWeedMate();
	return EXIT_CODE;
}