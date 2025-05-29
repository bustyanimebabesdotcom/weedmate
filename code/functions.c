// functions.c
// project: weedmate

#include <stdio.h>
#include <limits.h>
#include <weedmate/common.h>
#include <input.h>

/** 
 * selectCity - lets the user select a city from the cities array  
 * which applies a price modifier to the strain prices.
 * When new city is selected, it is written to the savefile and rememberd.
 */
void selectCity ( void ) {
	
	puts( "========== City List ==========\n" );

	for ( int i = 0; i < CITY_COUNT; i++ ) {
		printf( "%4d. %s%-20s%s\n", 
			i+USER_INPUT_OFFSET,
			GREEN,
			cities[i].name,
			RESET );
	}
	
	printf( "\nYour current city is: %s%s%s.\n", 
		YELLOW,
		cities[currentCityIndex].name,
		RESET );
	printf( "\nPlease select your location ( 1-%d )\n\n> ", CITY_COUNT );
	int choice = getIntInput();

	if ( choice >= 1 && choice <= CITY_COUNT ) currentCityIndex = choice-1;

	CLEAR_SCREEN();
	printf( "Your location has been set to: %s%s%s.\n", 
		GREEN, 
		cities[currentCityIndex].name,
		RESET );

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
}



/**
 * budTenderSanityCheck - Validates logical price constraints for a strain.
 * 
 * yes, budtenderStatus is a real return type. enum in macros.h
 *
 * @param x The price entered by the user.
 */
static budtender_e budTenderSanityCheck ( int x ) {

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

	printf( "===== " GREEN "Budtender Access Granted" RESET " ====\n" );

	while ( 1 ) {

		putchar( '\n' );
		printStrainList();
		putchar( '\n' );

		printf( "Enter strain number to update ( 1–%d ).\n", STRAIN_COUNT );
		printf( "\n> " );
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
		printf( "Enter your new price for %s%s%s.\n",
			YELLOW,
			strains[choice].name,
			RESET );
		printf( "\n> " );
		unsigned int newPrice = getUIntInput();

		if ( newPrice == UINT_MAX ) {
			CLEAR_SCREEN();
			fputs( "EOF detected. Function aborted.\n", stderr );
			break;
		}

		budtender_e sanityStatus = budTenderSanityCheck( newPrice );
		if (sanityStatus == BUDTENDER_CONTINUE) continue;

		strains[choice].price = newPrice;
		printNewStrainPrice( choice );
		break;

	}

	saveToFile();
}



/**
 * CalcInput - handles input for weedcalc
 *
 * Lets the user input first number, second number, and a modifier
 */
static bool calcInput ( int *a, char *op, int *b ) {

	CLEAR_SCREEN();
	printf( "Enter your first " GREEN "number" RESET ".\n\n> " );
	*a = getIntInput();
	if ( *a == INT_MIN ) return false;

	CLEAR_SCREEN();
	printf( "Enter your operator ( " YELLOW "+ - * /" RESET " ).\n\n> " );
	int inputChar = getCharInputFiltered("+-*/");
	if ( inputChar == EOF ) return false;
	*op = (char)inputChar;

	CLEAR_SCREEN();
	printf( "Enter your second " GREEN "number" RESET ".\n\n> " );
	*b = getIntInput();
	if ( *b == INT_MIN ) return false;

	return true;
}



/*
 * doCalculation - performs basic arithmetic operations for weedCalc
 *
 * No input, automates the process.
 */
static float doCalculation ( int a, char op, int b, bool *success ) {
	
	*success = true;

	switch ( op ) {
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;

		case '/':
			if ( b == 0 ) {
				CLEAR_SCREEN();
				puts( "You cannot divide by 0!" );
				*success = false;
				return DECIMAL_EXIT;
			}
			return (float)a / b;

		default:
			CLEAR_SCREEN();
			fputs( "Invalid input or EOF detected. Function aborted.\n", stderr );
			*success = false;
			return DECIMAL_EXIT;
	}
}



/** 
 * weedCalc - simple calculator
 */
void weedCalc ( void ) {

	int a = 0, b = 0;
	char op = 0;
	float result = 0.0;
	bool success = false;

	calcInput( &a, &op, &b );
	result = doCalculation( a, op, b, &success );
	if ( !success ) return;

	CLEAR_SCREEN();
	printf( "Your number is: " );
	if ( op == '/' ) printf( "%.6f\n", result );
	else printf( "%d\n", (int)result );
}
