// input.c - safe(r than scanf) input handling
// This can be ported to any project, to be used as a standalone input library.
// There are obviously better input libraries, but the purpose of this is to be
// A safe(r) alternative to scanf for noobs like me.

// TODO: Add EOF handling to stop infinite looping. Obvious bug with easy fix.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include "input.h"

/**
 * getIntInput - a safer alternative to scanf for integers
 *
 * usage - int = getIntInput();
 */
int getIntInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fprintf( stderr, "Invalid number. Try again.\n");
			continue;
		}

		if ( errno == ERANGE || value < INT_MIN || value > INT_MAX ) {
			fprintf( stderr, "Value out of range. Try again\n");
			continue;
		}

		return (int)value;
	}

}

/**
 * getUIntInput - a safer alternative to scanf for unsigned integers
 *
 * usage - unsigned int = getUIntInput();
 */
unsigned int getUIntInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );

		if ( endptr == buffer || *endptr != '\0') {
			fprintf( stderr, "Invalid number. Try again.\n");
			continue;
		}
		
		if ( errno == ERANGE || value > UINT_MAX ) {
			fprintf( stderr, "Value out of range. Try again.\n");
			continue;
		}

		return (unsigned int)value;
	}

}

/**
 * getFloatInput - a safer alternative to scanf for floating point numbers
 *
 * usage - float = getFloatInput();
 */
float getFloatInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	float value;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		errno = 0;
		// Convert string to float. 'endptr' is set to the first invalid character after the number.
		value = strtof( buffer, &endptr );

		if ( endptr == buffer || *endptr != '\0') {
			fprintf( stderr, "Invalid number. Try again.\n");
			continue;
		}

		if ( errno == ERANGE || !isfinite( value ) ) {
			fprintf( stderr, "Number out of range. Try again.\n" );
			continue;
		}
	
		return value;
	}

}

/**
 * getLongInput - a safer alternative to scanf for integers
 *
 * usage - long = getLongInput();
 */
long getLongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fprintf( stderr, "Invalid number. Try again.\n");
			continue;
		}

		if ( errno == ERANGE ) {
			fprintf( stderr, "Value out of range. Try again\n");
			continue;
		}

		return value;
	}

}

/**
 * getULongInput - a safer alternative to scanf for unsigned longs
 *
 * usage - unsigned long = getULongInput();
 */
unsigned long getULongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fprintf( stderr, "Invalid number. Try again.\n");
			continue;
		}

		if ( errno == ERANGE || value > ULONG_MAX ) {
			fprintf( stderr, "Value out of range. Try again\n");
			continue;
		}

		return value;
	}

}

/**
 * getDoubleInput - a safer alternative to scanf for doubles
 *
 * usage - double = getDoubleInput();
 */
double getDoubleInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	double value;

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';

		errno = 0;
		value = strtod( buffer, &endptr );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fprintf( stderr, "Invalid number. Try again.\n");
			continue;
		}

		if ( errno == ERANGE || !isfinite( value ) ) {
			fprintf( stderr, "Value out of range. Try again\n");
			continue;
		}

		return value;
	}

}

/**
 * getCharInput - a safer alternative to scanf for chars
 *
 * usage - char = getCharInput();
 */
char getCharInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';
		
		if ( strlen(buffer) == 1 ) {
			return buffer[0];
		}
	
		fprintf( stderr, "Invalid input. Please enter a single character.\n");
	}

}

/**
 * getCharInputFiltered - a safer alternative to scanf for chars
 *
 * usage - char = getCharInputFiltered( "abc" );
 */
char getCharInputFiltered( const char *allowed ) {

	if ( allowed == NULL ){
		fprintf( stderr, "getCharInputFiltered(): NULL passed to 'allowed'. Exiting.\n" );
		exit(EXIT_FAILURE);
	}

	char buffer[INPUT_BUFFER_SIZE];

	while ( 1 ) {

		if ( fgets( buffer, sizeof(buffer), stdin ) == NULL ) {
			fprintf( stderr, "Error in input stream.\n" );
			continue;
		}

		buffer[strcspn( buffer, "\n" )] = '\0';
		
		if ( strlen(buffer) != 1 ) {
			fprintf( stderr, "Invalid input. Please enter a single character.\n");
			continue;
		}

		char c = buffer[0];

		if ( strchr( allowed, c ) != NULL ) {
			return c;
		}
	
		fprintf( stderr, "Invalid input. Allowed %s\n", allowed );
		
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