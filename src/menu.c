// menu.c
// project: weedmate

#include <stdio.h>
#include <ctype.h>
#include <weedmate/common.h>
#include <weedmate/menu.h>
#include <input.h>

void initMenu ( void ) {

	int useColor = isatty(STDOUT_FILENO);

	ENTER_ALT_SCREEN();
	CLEAR_SCREEN();
	motd();

	int c;

	while ( ( c = getCharInput () ) != EOF ) {
		switch ( tolower( c ) ) {

			case 'm':
				CLEAR_SCREEN();
				motd();
				break;

			case 'q':
				return;

			case 'l':
				CLEAR_SCREEN();
				printStrainList();
				RETURN_TO_MENU_MSG();
				break;

			case 'p':
				CLEAR_SCREEN();
				handleStrainPriceLookup();
				RETURN_TO_MENU_MSG();
				break;

			case 'b':
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
				CLEAR_SCREEN();
				weedCalc();
				RETURN_TO_MENU_MSG();
				break;

			case 'r':
				CLEAR_SCREEN();
				renameStrain();
				RETURN_TO_MENU_MSG();
				break;

			case 's':
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
}
