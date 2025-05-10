// main.c — Terminal ritual grounds for WeedMate
// Functions live in:       functions.c
// Input handling:			input.c
// Declarations:   			common.h
//							input.h
// Signals:					signals.c
// Macros:					macros.h
// project:					weedmate

#include <stdio.h>
#include <stdlib.h>
#include <weedmate/common.h>
#include "input.h"

int main ( void ) {

	ENTER_ALT_SCREEN();
	atexit(exitWeedMate);		// Make sure program runs exit function to leave alternate screen buffer
	installSignalHandlers();	// Make sure we can catch CTRL-Z, CTRL-C, CTRL-D, etc...
	CLEAR_SCREEN();
	loadSaveFile();
	motd();

	int c;

	// Main loop, runs until EOF
	while ( ( c = getCharInput () ) != EOF ) {

		// Input processing
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

			default:
				// in case of oh shit moment
				CLEAR_SCREEN();
				puts( "Unrecognized input." );
				RETURN_TO_MENU_MSG();

		}
	}

	return EXIT_CODE;
}
