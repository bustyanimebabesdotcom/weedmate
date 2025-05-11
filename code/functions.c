// functions.c
// project: weedmate

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <weedmate/common.h>
#include "input.h"


/**
 * motd - Displays the Message of the Day (program instructions).
 *
 * Called at startup or when the user presses 'm'.
 */
void motd ( void ) {

	int useColor = isatty(STDOUT_FILENO);

	puts( "  ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" );
	puts( "  ██ ███ ██ ▄▄▄██ ▄▄▄██ ▄▄▀██ ▄▀▄ █ ▄▄▀█▄▄ ▄▄██ ▄▄▄██" );
	puts( "  ██ █ █ ██ ▄▄▄██ ▄▄▄██ ██ ██ █ █ █ ▀▀ ███ ████ ▄▄▄██" );
	puts( "  ██▄▀▄▀▄██ ▀▀▀██ ▀▀▀██ ▀▀ ██ ███ █ ██ ███ ████ ▀▀▀██" );
	puts( "  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" );
	puts( "=======================================================\n" );

	printf( "               Welcome to " GREEN "weedmate" RESET "!\n\n");

	COLOR_KEY( 'l', "browse the strain list" );
	COLOR_KEY( 'p', "check the price of individual strains." );
	COLOR_KEY( 'r', "rename strains." );
	COLOR_KEY( 'b', "open the budtender menu.\n" );

	COLOR_KEY( '+', "double prices." );
	COLOR_KEY( '-', "cut prices in half." );
	COLOR_KEY( 'c', "open the calculator." );
	COLOR_KEY( 's', "select your location.\n" );

	COLOR_KEY( 'm', "reprint this message" );
	QUIT_KEY( 'q', "quit.\n" );

	printf( "Enter your input: " );

}

/** 
 * selectCity - lets the user select a city from the cities array  
 * which applies a price modifier to the strain prices.
 * When new city is selected, it is written to the savefile and rememberd.
 */
void selectCity ( void ) {
	
	puts( "========== City List ==========\n" );

	for ( int i = 0; i < CITY_COUNT; i++ ) {
		printf( "%4d. %-20s\n", i+USER_INPUT_OFFSET, cities[i].name );
	}
	printf( "\nYour current city is: %s.\n", cities[currentCityIndex].name );
	printf( "\nPlease select your location ( 1-%d ):\n", CITY_COUNT );
	int choice = getIntInput();

	if (choice >= 1 && choice <= CITY_COUNT) currentCityIndex = choice - 1;

	CLEAR_SCREEN();
	printf( "Your location has been set to: %s.\n", cities[currentCityIndex].name );

	saveToFile();

}

/**
 * exitWeedMate - Displays goodbye message and exits alt screen mode.
 *
 * Called when the user quits the program. Handles visual cleanup.
 */
void exitWeedMate ( void ) {

	CLEAR_SCREEN();
	EXIT_ALT_SCREEN();
	puts( "Thank you for using weedmate!" );
	putchar( '\n' );
	printf( "Terminating with exit code %d!\n", EXIT_CODE );

}

/**
 * budTenderSanityCheck - Validates logical price constraints for a strain.
 *
 * @param x The price entered by the user.
 */
static int budTenderSanityCheck ( int x ) {

	if ( x == MIN_STRAIN_PRICE ) {
		CLEAR_SCREEN();
		puts( "Nothing is free, not even death." );
		return BUDTENDER_CONTINUE;
	}

	if ( x > MAX_STRAIN_PRICE ) {
		CLEAR_SCREEN();
		puts( "Bro... no weed is worth that much." );
		return BUDTENDER_CONTINUE;
	}

	if ( x < 0 ) {
		CLEAR_SCREEN();
		printf( "Illegal input value interpreted as '%u'\n", (unsigned int)x );
		return BUDTENDER_CONTINUE;
	}

	return BUDTENDER_OK;
}

/**
 * budTenderMenu - Allows the user to update strain prices.
 */
void budTenderMenu ( void ) {

	puts( "=== Budtender Access Granted ===" );

	while ( 1 ) {

		putchar( '\n' );
		printStrainList();
		putchar( '\n' );

		printf( "Enter strain number to update ( 1–%d ):\n", STRAIN_COUNT );
		int choice = getStrainChoice();

		if ( choice == BUDTENDER_BREAK ) break;
		if ( choice == BUDTENDER_CONTINUE ) continue;

		choice -= USER_INPUT_OFFSET;

		if ( choice == FENT_KUSH ) {
			CLEAR_SCREEN();
			printf( "You are not worthy to alter the price of %s!\n", strains[FENT_KUSH].name );
			continue;
		}

		CLEAR_SCREEN();
		printf( "Enter your new price for %s.\n", strains[choice].name );
		unsigned int newPrice = getUIntInput();

		if ( newPrice == UINT_MAX ) {
			CLEAR_SCREEN();
			fputs( "EOF detected. Function aborted.\n", stderr );
			break;
		}

		int sanityStatus = budTenderSanityCheck( newPrice );
		if (sanityStatus == BUDTENDER_CONTINUE) continue;

		strains[choice].price = newPrice;
		printNewStrainPrice( choice );
		break;

	}

}

/**
 * weedCalcInput - handles input for weedcalc
 *
 * Lets the user input first number, second number, and a modifier
 */
static bool weedCalcInput ( int *a, char *mod, int *b ) {

	int x, y;
	int m;

	puts( "Enter your first number:" );
	x = getIntInput();
	if ( x == INT_MIN ) return false;

	puts( "Enter your modifier ( + - * / ):" );
	m = getCharInputFiltered("+-*/");
	if ( m == EOF ) return false;

	puts( "Enter your second number:" );
	y = getIntInput();
	if ( y == INT_MIN ) return false;

	*a = x;
	*mod = (char)m;
	*b = y;
	return true;

}

/*
 * doCalculation - performs basic arithmetic operations for weedCalc
 *
 * No input, automates the process.
 */
static double doCalculation ( int a, char mod, int b, bool *success ) {
	
	*success = true;

	switch ( mod ) {

		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': {
			if ( b == 0) {
				puts( "You cannot divide by 0!" );
				*success = false;
				return DECIMAL_EXIT;
			}

			return (double)a / b;
		}

		default: {
			CLEAR_SCREEN();
			fputs( "Invalid input or EOF detected. Function aborted.\n", stderr );
			*success = false;
			return DECIMAL_EXIT;
		}

	}

}

/** 
 * weedCalc - simple calculator, self explanatory.  
 */
void weedCalc ( void ) {

	// declare variables
	int num1 = 0;
	int num2 = 0;
	char mod = 0;
	double result = 0.0;
	bool success = false;

	// take user input, and perform arithmetic operation
	weedCalcInput( &num1, &mod, &num2 );
	result = doCalculation( num1, mod, num2, &success );

	if ( !success ) {
		// Error handled by doCalculation
		return;
	}

	// print result
	printf( "Your number is: " );
	if ( mod == '/' ) {
		printf( "%.8f\n", result );
	}

	else {
		printf( "%d\n", (int)result );
	}

}
