// strain.c
// project: weedmate

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <weedmate/common.h>
#include <input.h>

static void printStrainPriceMessage ( int choice, const char* tense ) {

	CLEAR_SCREEN();
	if ( choice >= 0 && choice < STRAIN_COUNT )
		printf( "The price of Strain #%d - " YELLOW "%s" RESET " %s $%s%u%s/g\n", 
			choice + USER_INPUT_OFFSET,
			strains[choice].name, tense,
			GREEN,
			strains[choice].price,
			RESET );
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
void printNewStrainPrice ( int choice ) {

	printStrainPriceMessage( choice, "has been changed to" );
}



/**
 * printStrainList - Displays a formatted list of all strains and their prices.
 *
 * Called by both user lookup and budtender menu screens.
 */
void printStrainList ( void ) {

	puts( "=========== Strain List ===========\n" );
	
	float mod = cities[currentCityIndex].modifier;	
	printf( BOLD "%4s  %-20s %s\n" RESET, "#", "Strain Name", "Price" );
	puts("  --------------------------------");

	for ( int i = 0; i < STRAIN_COUNT; i++ ) {
		float adjusted = strains[i].price*mod;

		printf( "%4d. %-20s $%s%.2f%s\n",
			i+USER_INPUT_OFFSET,
			strains[i].name,
			GREEN,
			adjusted,
			RESET );
	}

	printf( "\nYour current city is: " YELLOW "%s" RESET ".\n", cities[currentCityIndex].name );
}



/**
 * strainPriceAdjust - Lets you quickly double or halve all strain prices
 * 
 * strainPriceAdjust( strains, 1 ); - doubles prices
 * 
 * strainPriceAdjust( strains, -1 ); - halves prices
 */
void strainPriceAdjust ( strain_s* strains, int mode ) {

	for ( int i = 0; i < STRAIN_COUNT; i++) {
		if ( mode == ADJUST_DOUBLE ) strains[i].price <<= 1;
		else if ( mode == ADJUST_HALVE ) strains[i].price >>= 1;
		else fprintf( stderr, "strainPriceAdjust: invalid mode ( %d )\n", mode );
	}

	saveToFile();
}



/**
 * getStrainChoice - Prompts user to select a strain by number.
 *
 * Prints the strain list and validates user input.
 */
int getStrainChoice( void ) {

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
 * handleStrainPriceLookup - Lets user select a strain and view its price.
 *
 * Provides a numbered list and handles bad input or out-of-range entries.
 */
void handleStrainPriceLookup ( void ) {
	
	while (1) {

		printf( "Enter " YELLOW "strain" RESET " number ( 1–%d ).\n\n> ", STRAIN_COUNT );

		int choice = getIntInput();
		if ( choice == INT_MIN ) {
			CLEAR_SCREEN();
			fputs( "EOF detected. Function aborted.\n", stderr );
			break;
		}

		if ( choice < 1 || choice > STRAIN_COUNT ) {
			printf( "That number is not on the list. Please pick a valid number.\n" );
			continue;
		}

		choice -= USER_INPUT_OFFSET;

		printStrainPrice( choice );
		break;
	}
}



/** 
 * renameStrain - prompts user to select and rename a strain entry
 * 
 * This function displays the strain list, reads a slot number with
 * getIntInput(), then prompts for a new name with getStringInput().
 * 
 * It performs comprehensive validation
 * ( EOF, out-of-range slot, empty input, and max-length checks),
 * updates the fixed-size name buffer in the strains array,
 * frees the temporary heap allocation, and saves the updated list to file.
 */
void renameStrain( void ) {

	// Get user selected strain slot
	printStrainList();
	fprintf( stdout, "\nEnter strain to rename ( 1-%d ).\n\n> ", STRAIN_COUNT );
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

	strain_s *s = &strains[slot-USER_INPUT_OFFSET];
	
	// Get strain name from user
	CLEAR_SCREEN();

	printf( "Enter new name for strain #%d, '" YELLOW "%s" RESET "'.\n\n> ", 
		slot, 
		s->name );

	// Read user input into a struct with .data (heap pointer) and .len (byte count)
	string_s str = getStringInput();

	// Get length of string, or 0 if str is NULL
	size_t len = str.data ? str.len : 0;

	// Check for NULL or empty string
	if ( !str.data || !len ) {
		free(str.data);
		str.data = NULL;
		CLEAR_SCREEN();
		fputs( "Empty string or error, slot unchanged.\n", stderr );
		return;
	}
	
	// Check if string exceeds max length
	if ( len >= MAX_STRAIN_LENGTH ) {
		CLEAR_SCREEN();
		fprintf( stderr, "Strain name exceeds max length of %d characters.\n", MAX_STRAIN_LENGTH - 1 );
		free(str.data);
		str.data = NULL;
		return;
	}

	// Prevent buffer overflow, ensure input fits (including '\0')
	if( len+1 > sizeof( s->name ) ) {
		fputs( "Input too long. Max 20 characters.\n", stderr );
		free(str.data);
		str.data = NULL;
		return;
	}

	// Copy string from heap to struct, manually add null terminator
	memcpy( s->name, str.data, len );
	s->name[len] = '\0';
	
	// Display success message
	CLEAR_SCREEN();
	fprintf( stdout, "Strain name '" YELLOW "%s" RESET"' successfully applied to slot #%d\n", s->name, slot );

	// Free the pointer
	free(str.data);
	str.data = NULL;

	saveToFile();
}
