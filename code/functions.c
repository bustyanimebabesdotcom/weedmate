// functions.c - arrays, functions, and the big meat of WeedMate
// Main routing:            main.c
// Input handling			inputhandler.c
// Declarations:   			common.h
// Macros:					macros.h
// project:					weedmate

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"

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

	puts( "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" );
	puts( "██ ███ ██ ▄▄▄██ ▄▄▄██ ▄▄▀██ ▄▀▄ █ ▄▄▀█▄▄ ▄▄██ ▄▄▄██" );
	puts( "██ █ █ ██ ▄▄▄██ ▄▄▄██ ██ ██ █ █ █ ▀▀ ███ ████ ▄▄▄██" );
	puts( "██▄▀▄▀▄██ ▀▀▀██ ▀▀▀██ ▀▀ ██ ███ █ ██ ███ ████ ▀▀▀██" );
	puts( "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" );

	putchar( '\n' );
	puts( "               Welcome to weedmate!                      " );
	putchar( '\n' );
	puts( "press 'l' to browse the strain list." );
	puts( "press 'p' to check the price of individual strains." );
	puts( "press 'c' to open the calculator." );
	puts( "press '+' to double prices." );
	puts( "press '-' to cut prices in half." );
	puts( "press 'm' to reprint this message." );
	puts( "press 'q' to quit." );
	putchar( '\n' );
	puts( "Enter your input." );

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
	puts( "Thank you for using weedmate!" );
	putchar( '\n' );
	printf( "Terminating with exit code %d!\n", EXIT_CODE );
	EXIT_ALT_SCREEN();

}

/**
 * getStrainChoice - Prompts user to select a strain by number.
 *
 * Prints the strain list and validates user input.
 */
static int getStrainChoice ( const char *prompt ) {

	printf( prompt, STRAIN_COUNT );

	int choice = getIntInput("");

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

	CLEAR_SCREEN();
	flushInputBuffer();
	puts( "=== Budtender Access Granted ===" );

	while ( 1 ) {

		putchar( '\n' );
		printStrainList();
		putchar( '\n' );

		printf( "Enter strain number to update ( 1–%d ):\n", STRAIN_COUNT );
		int choice = getStrainChoice("");

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
		unsigned int newPrice = getUsIntInput("");

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

	CLEAR_SCREEN();
	flushInputBuffer();
	while (1) {

		putchar( '\n' );
		printf( "Enter strain number ( 1–%d ):\n", STRAIN_COUNT );

		int choice = getIntInput("");

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
static void weedCalcInput ( int *a, char *mod, int *b ) {

	flushInputBuffer();
	*a = getIntInput( "Enter your first number:\n" );
	*mod = getCharInput( "Enter your modifier ( + - * / ):\n" );
	*b = getIntInput( "Enter your second number:\n" );

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
			puts( "Invalid operator. Please try again." );
			*success = false;
			return DECIMAL_EXIT;
		}

	}

}

/** 
 * weedCalc - simple calculator, self explanatory.  
 */
void weedCalc ( void ) {

	int num1 = 0;
	int num2 = 0;
	char mod = 0;
	double result = 0.0;
	bool success = false;

	weedCalcInput( &num1, &mod, &num2 );
	result = doCalculation( num1, mod, num2, &success );

	if ( !success ) {
		// Error handled by doCalculation
		return;
	}

	printf( "Your number is: " );
	if ( mod == '/' ) {
		printf( "%.8f\n", result );
	}

	else {
		printf( "%d\n", (int)result );
	}

}