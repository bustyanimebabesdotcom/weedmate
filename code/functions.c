// functions.c - arrays, functions, and the big meat of WeedMate
// Main routing:            main.c
// Input handling			input.c
// Declarations:   			common.h
//							input.h
// Signals:					signals.c
// Macros:					macros.h
// project:					weedmate

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <weedmate/common.h>
#include "input.h"

strain_t strains[STRAIN_COUNT] = {
	{ "White Death",            13 },
	{ "Purple Plutonium OG",    12 },
	{ "Dankmestic Abuse",       24 },
	{ "Fentanyl Kush",          48 },
	{ "Blunt Trauma",           10 },
	{ "Petroleum #42",          35 },
	{ "Ganjaffi",               23 },
	{ "Dental Prescription",     9 },
	{ "Hebrew Kush",             6 },
	{ "Rohypno-Chronic",        30 },
	{ "Holy Shit OG",            8 },
	{ "George W Kush",           8 },
	{ "Bad Dragon",             17 },
	{ "Carpet Muncher",         11 },
	{ "Freedom Highve",         22 }
};

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
 * motd - Displays the Message of the Day (program instructions).
 *
 * Called at startup or when the user presses 'm'.
 */
void motd ( void ) {

	puts( " ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" );
	puts( " ██ ███ ██ ▄▄▄██ ▄▄▄██ ▄▄▀██ ▄▀▄ █ ▄▄▀█▄▄ ▄▄██ ▄▄▄██" );
	puts( " ██ █ █ ██ ▄▄▄██ ▄▄▄██ ██ ██ █ █ █ ▀▀ ███ ████ ▄▄▄██" );
	puts( " ██▄▀▄▀▄██ ▀▀▀██ ▀▀▀██ ▀▀ ██ ███ █ ██ ███ ████ ▀▀▀██" );
	puts( " ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" );

	putchar( '\n' );
	puts( "               Welcome to weedmate!                      " );
	putchar( '\n' );
	puts( "press 'l' to browse the strain list." );
	puts( "press 'p' to check the price of individual strains." );
	puts( "press 'b' to open the budtender menu." );
	putchar('\n');
	puts( "press '+' to double prices." );
	puts( "press '-' to cut prices in half." );
	puts( "press 'c' to open the calculator." );
	puts( "press 'r' to rename strains." );
	putchar('\n');
	puts( "press 'm' to reprint this message." );
	puts( "press 'q' to quit." );
	putchar( '\n' );
	printf( "Enter your input: " );

}

static void printStrainPriceMessage ( int choice, const char* tense ) {

	CLEAR_SCREEN();
	if ( choice >= 0 && choice < STRAIN_COUNT ) {
	printf( "The price of Strain #%d - %s %s $%u/g\n", choice + USER_INPUT_OFFSET, strains[choice].name, tense, strains[choice].price );
	}

}

/**
 * printStrainPrice - Prints the price of a specific strain.
 *
 * @param choice Index of the strain in the array (0-based).
 */
static void printStrainPrice ( int choice ) {

	printStrainPriceMessage( choice, "is" );

}

/**
 * printNewStrainPrice - Displays updated price info for a strain.
 *
 * @param choice Index of the strain in the array (0-based).
 */
static void printNewStrainPrice ( int choice ) {

	printStrainPriceMessage( choice, "has been changed to" );

}

/**
 * printStrainList - Displays a formatted list of all strains and their prices.
 *
 * Called by both user lookup and budtender menu screens.
 */
void printStrainList ( void ) {

	puts( "========= Strain List =========" );
	putchar( '\n' );

	for ( int i = 0; i < STRAIN_COUNT; i++ ) {
	printf( "%4d. %-20s $%2u\n", i + USER_INPUT_OFFSET, strains[i].name, strains[i].price );
	}

}

/**
 * strainPriceAdjust - Lets you quickly double or halve all strain prices
 * 
 * strainPriceAdjust( strains, 1 ); - doubles prices
 * 
 * strainPriceAdjust( strains, -1 ); - halves prices
 */
void strainPriceAdjust ( strain_t* strains, int mode ) {

	for ( int i = 0; i < STRAIN_COUNT; i++) {
		if ( mode == 1 ) {
			strains[i].price <<= 1;
		}
		else if ( mode == -1 ) {
			strains[i].price >>= 1;
		}
		else {
			// error handling
			fprintf( stderr, "strainPriceAdjust: invalid mode ( %d )\n", mode );
		}
	}

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
 * getStrainChoice - Prompts user to select a strain by number.
 *
 * Prints the strain list and validates user input.
 */
static int getStrainChoice( void ) {

	int choice = getIntInput();
	if ( choice == INT_MIN ) {
		CLEAR_SCREEN();
		fputs( "EOF detected. Function aborted.\n", stderr );
		return BUDTENDER_BREAK;
	}

	if ( choice < 1 || choice > STRAIN_COUNT ) {
		CLEAR_SCREEN();
		printf( "That number is not on the list. Please pick a valid number.\n" );

		return BUDTENDER_CONTINUE;
	}

	return choice;
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
 * handleStrainPriceLookup - Lets user select a strain and view its price.
 *
 * Provides a numbered list and handles bad input or out-of-range entries.
 */
void handleStrainPriceLookup ( void ) {
	
	while (1) {

		printf( "Enter strain number ( 1–%d ):\n", STRAIN_COUNT );

		int choice = getIntInput();
		if ( choice == INT_MIN ) {
			CLEAR_SCREEN();
			fputs( "EOF detected. Function aborted.\n", stderr );
			break;
		}

		if ( choice == 0 ) {
			CLEAR_SCREEN();
			puts("Canceled strain search...");
			break;
		}

		if ( choice < 1 || choice > STRAIN_COUNT ) {
			printf( "Try Again.\n" );
			continue;
		}

		choice -= USER_INPUT_OFFSET;

		printStrainPrice( choice );
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

/** 
 * renameStrain - uses getStringInput() from the input library to  
 * store a string in memory, write it to selected array slot
 * and freeing the memory at the end. clean shit.
 */
void renameStrain( void ) {

	// Get user selected strain slot
	fprintf( stdout, "Enter strain to rename ( 1-%d ):\n", STRAIN_COUNT );
	int slot = getIntInput();

	// check for EOF
	if ( slot == INT_MIN ) {
		CLEAR_SCREEN();
		fputs( "EOF detected. Rename aborted.\n", stderr );
		return;
	}
	// Check that selected slot is a valid number
	if ( slot < 1 || slot > STRAIN_COUNT ) {
		CLEAR_SCREEN();
		fprintf( stderr, "Invalid strain number. Must be between 1-%d!\n", STRAIN_COUNT );
		return;
	}
	
	// Get strain name from user
	CLEAR_SCREEN();
	printf( "Enter new name for strain #%d,  '%s':\n", slot, strains[slot].name );
	char *str = getStringInput();

	// Get length of string, or 0 if str is NULL
	size_t len = str ? strlen(str) : 0;

	// Check for NULL or empty string
	if ( len == 0 ) {
		free(str);
		str = NULL;
		CLEAR_SCREEN();
		fputs( "Empty string or error, slot unchanged.\n", stderr );
		return;
	}
	
	// Check if string exceeds max length
	if ( len >= MAX_STRAIN_LENGTH ) {
		CLEAR_SCREEN();
		fprintf( stderr, "Strain name exceeds max length of %d characters.\n", MAX_STRAIN_LENGTH - 1 );
		free(str);
		str = NULL;
		return;
	}

	// Copy  len+1 bytes from heap to struct, which is stored in stack
	memcpy(
		strains[slot - USER_INPUT_OFFSET].name, // destination
		str,                                    // source
		len + 1                                 // include null terminator
	);
	
	// Display success message
	CLEAR_SCREEN();
	fprintf( stdout, "Strain name '%s' successfully applied to slot #%d\n", str, slot );

	// Free the pointer
	free(str);
	str = NULL;

}
