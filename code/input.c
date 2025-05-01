// input.c - safe(r than scanf) input handling
// Main routing:            main.c
// Functions live in:       functions.c
// Declarations:   			common.h
// Macros:					macros.h
// project:					weedmate

#include <stdio.h>
#include <string.h>
#include "common.h"

/**
 * getIntInput - a safer alternative to scanf for integers
 *
 * usage - int = getIntInput( "String\n" );
 */
int getIntInput( void ) {

	char buffer[BUFFER_SIZE];
	int value;
	char extra;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		if ( sscanf( buffer, "%d %c", &value, &extra ) == 1 ) return value;
	
		fprintf( stderr, "Invalid number. Try again\n");

		
	}

}

/**
 * getFloatInput - a safer alternative to scanf for floating point numbers
 *
 * usage - float = getFloatInput( "String\n" );
 */
float getFloatInput( void ) {

	char buffer[BUFFER_SIZE];
	float value;
	char *endptr;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		// Convert string to float. 'endptr' is set to the first invalid character after the number.
		value = strtof( buffer, &endptr );

		if ( endptr == buffer || *endptr != '\0') {
			fprintf( stderr, "Invalid number. Try again\n");
			continue;
		}
	
		return value;
	}

}

/**
 * getUIntInput - a safer alternative to scanf for unsigned integers
 *
 * usage - unsigned int = getUIntInput( "String\n" );
 */
unsigned int getUIntInput( void ) {

	char buffer[BUFFER_SIZE];
	unsigned int value;
	char extra;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		if ( sscanf( buffer, "%u %c", &value, &extra ) == 1 ) return value;
	
		fprintf( stderr, "Invalid number. Try again\n");
		
	}
}


/**
 * getCharInput - a safer alternative to scanf for chars
 *
 * usage - char = getCharInput( "String\n" );
 */
char getCharInput( void ) {

	char buffer[BUFFER_SIZE];
	char value;
	char extra;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';
		
		if ( sscanf( buffer, " %c %c", &value, &extra ) == 1 ) return value;
	
		fprintf( stderr, "Invalid modifier. Try again\n");
	}

}

/**
 * flushInputBuffer - Clears any remaining characters from stdin.
 *
 * Used after scanf fails or reads partial input, to prevent leftover
 * characters from screwing up future input operations.
 */
void flushInputBuffer ( void ) {

	int ch;
	while ( ( ch = getchar() ) != '\n' && ch != EOF );

}