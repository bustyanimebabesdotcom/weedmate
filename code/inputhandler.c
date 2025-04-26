// inputhandler.c - safe(er than scanf) input handling
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
int getIntInput( const char * prompt ) {

	char buffer[BUFFER_SIZE];
	int value;

	while ( 1 ) {
		printf( "%s", prompt );

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		if ( sscanf( buffer, "%d", &value ) == 1 ) return value;
	
		fprintf( stderr, "Invalid number. Try again\n");

		
	}

}

/**
 * getFloatInput - a safer alternative to scanf for floating point numbers
 *
 * usage - float = getFloatInput( "String\n" );
 */
float getFloatInput( const char * prompt ) {

	char buffer[BUFFER_SIZE];
	float value;

	while ( 1 ) {
		printf( "%s", prompt );

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		if ( sscanf( buffer, "%f", &value ) == 1 ) return value;
	
		fprintf( stderr, "Invalid number. Try again\n");

		
	}

}

/**
 * getUsIntInput - a safer alternative to scanf for unsigned integers
 *
 * usage - unsigned int = getUsIntInput( "String\n" );
 */
unsigned int getUsIntInput( const char * prompt ) {

	char buffer[BUFFER_SIZE];
	unsigned int value;

	while ( 1 ) {
		printf( "%s", prompt );

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		if ( sscanf( buffer, "%u", &value ) == 1 ) return value;
	
		fprintf( stderr, "Invalid number. Try again\n");
		
	}

}


/**
 * getCharInput - a safer alternative to scanf for chars
 *
 * usage - char = getCharInput( "String\n" );
 */
char getCharInput( const char * prompt ) {

	char buffer[BUFFER_SIZE];
	char value;

	while ( 1 ) {
		printf( "%s", prompt );

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}
		
		if ( sscanf( buffer, " %c", &value ) == 1 ) return value;
	
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