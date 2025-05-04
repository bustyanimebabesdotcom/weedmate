// input.c - version 1.0.1
/* safe(r than scanf) input handling
 * This can be ported to any project, to be used as a standalone input library.
 * There are obviously better input libraries, but the purpose of this is to be
 * a safe(r) alternative to scanf for noobs like me.
 * 
 * Main safety features are simple prevention of buffer overflow exploits,
 * and validating input.
 */


// TODO: Wrap more copy pasted shit into helpers.
// TODO: Find better solution than exit on EOF.
// TODO: Optimize codebase where possible.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include "input.h"

/**
 * drainStdin - Drains leftover input from stdin to prevent buffer overflows.
 * Reads until newline or EOF is encountered
 * 
 * called by readInputLine()
 */
static void drainStdin( void ) {

	int ch;
	// rRead and discard characters until newline or EOF
	while ( ( ch = getchar() ) != '\n' && ch != EOF )
		; // Empty loop body, smaller than {}

}

/**
 * readInputLine - read one line from stdin into buffer, strip newline,
 * 					drain the rest if line is too long, and handle EOF/errors
 * 
 * @buf: buffer to fill
 * @size: size of buf (e.g. INPUT_BUFFER_SIZE)
 * 
 * Returns:
 * 		0 on success,
 *		1 on recoverable error ( caller should retry ),
 *		on EOF, clears error and returns 1 (caller should retry)
 */
static int readInputLine( char *buf, size_t size ) {

	// Read input line into buffer, check for errors or EOF
	if ( fgets( buf, size, stdin ) == NULL ) {
		if ( feof( stdin ) ) {
			clearerr(stdin);
			return 1;
		}

		else {
			fputs( "Error in input stream.\n", stderr );
			return 1;	// caller should retry
		}
	}

	// Check for overflow: no newline and not EOF means input too long
	if ( !strchr( buf, '\n' ) && !feof( stdin ) ) {
		// flush excess input to prevent overflow
		drainStdin();
		fputs( "Input exceeding buffer size. Try again.\n", stderr );
		return 1;
	}

	// strip trailing newline
	buf[strcspn( buf, "\n" )] = '\0';
	return 0;

}

/**
 * getIntInput - a safer alternative to scanf for integers
 *
 * usage - int x = getIntInput();
 */
int getIntInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fputs( "Invalid number. Try again.\n", stderr);
			continue;
		}

		if ( errno == ERANGE || value < INT_MIN || value > INT_MAX ) {
			fputs( "Value out of range. Try again.\n", stderr );
			continue;
		}

		return (int)value;
	}

}

/**
 * getUIntInput - a safer alternative to scanf for unsigned integers
 *
 * usage - unsigned int x = getUIntInput();
 */
unsigned int getUIntInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;

		if ( buffer[0] == '-' ) {
			fputs( "Invalid number. Try again.\n", stderr );
			continue;
		}

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );

		if ( endptr == buffer || *endptr != '\0') {
			fputs( "Invalid number. Try again.\n", stderr );
			continue;
		}
		
		if ( errno == ERANGE || value > UINT_MAX ) {
			fputs( "Value out of range. Try again.\n", stderr);
			continue;
		}

		return (unsigned int)value;
	}

}

/**
 * getFloatInput - a safer alternative to scanf for floating point numbers
 *
 * usage - float x = getFloatInput();
 */
float getFloatInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	float value;

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;

		errno = 0;
		// Convert string to float. 'endptr' is set to the first invalid character after the number.
		value = strtof( buffer, &endptr );

		if ( endptr == buffer || *endptr != '\0') {
			fputs( "Invalid number. Try again.\n", stderr );
			continue;
		}

		if ( errno == ERANGE || !isfinite( value ) ) {
			fputs( "Number out of range. Try again.\n", stderr );
			continue;
		}
	
		return value;
	}

}

/**
 * getDoubleInput - a safer alternative to scanf for doubles
 *
 * usage - double x = getDoubleInput();
 */
double getDoubleInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	double value;

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;

		errno = 0;
		value = strtod( buffer, &endptr );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fputs( "Invalid number. Try again.\n", stderr );
			continue;
		}

		if ( errno == ERANGE || !isfinite( value ) ) {
			fputs( "Value out of range. Try again.\n", stderr );
			continue;
		}

		return value;
	}

}

/**
 * getLongInput - a safer alternative to scanf for longs
 *
 * usage - long x = getLongInput();
 */
long getLongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fputs( "Invalid number. Try again.\n", stderr );
			continue;
		}

		if ( errno == ERANGE ) {
			fputs( "Value out of range. Try again.\n", stderr );
			continue;
		}

		return value;
	}

}

/**
 * getULongInput - a safer alternative to scanf for unsigned longs
 *
 * usage - unsigned long x = getULongInput();
 */
unsigned long getULongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fputs( "Invalid number. Try again.\n", stderr );
			continue;
		}

		if ( errno == ERANGE ) {
			fputs( "Value out of range. Try again.\n", stderr );
			continue;
		}

		return value;
	}

}

/**
 * getLongLongInput - a safer alternative to scanf for long longs
 *
 * usage - long long x = getLongLongInput();
 */
long long getLongLongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long long value;

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;

		errno = 0;
		value = strtoll( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fputs( "Invalid number. Try again.\n", stderr );
			continue;
		}

		if ( errno == ERANGE ) {
			fputs( "Value out of range. Try again.\n", stderr );
			continue;
		}

		return value;
	}

}

/**
 * getULongLongInput - a safer alternative to scanf for unsigned long longs
 *
 * usage - unsigned long long x = getULongLongInput();
 */
unsigned long long getULongLongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long long value;

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;

		errno = 0;
		value = strtoull( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			fputs( "Invalid number. Try again.\n", stderr );
			continue;
		}

		if ( errno == ERANGE ) {
			fputs( "Value out of range. Try again.\n", stderr );
			continue;
		}

		return value;
	}

}

/**
 * getCharInput - a safer alternative to scanf for chars
 *
 * usage - char x = getCharInput();
 *
 * IMPORTANT: Only accepts one character. " a" and "a " are considered invalid.
 */
char getCharInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;
		
		if ( strlen(buffer) == 1 ) {
			return buffer[0];
		}
	
		fputs( "Invalid input. Please enter a single character.\n", stderr);
	}

}

/**
 * getCharInputFiltered - a safer alternative to scanf for chars
 *
 * usage - char x = getCharInputFiltered( "abc" );
 */
char getCharInputFiltered( const char *allowed ) {

	if ( allowed == NULL ){
		fputs( "getCharInputFiltered(): NULL passed to 'allowed'. Exiting.\n", stderr );
		exit(EXIT_FAILURE);
	}

	if ( allowed[0] == '\0' ) {
		fputs( "No allowed characters specified. Exiting.\n", stderr );
		return 1;
	}

	char buffer[INPUT_BUFFER_SIZE];

	while ( 1 ) {

		if (readInputLine(buffer, sizeof(buffer))) continue;
		
		// Ensure input is exactly one character
		if ( strlen(buffer) != 1 ) {
			fputs( "Invalid input. Please enter a single character.\n", stderr );
			continue;
		}

		char c = buffer[0];

		// Return if character is in allowed set
		if ( strchr( allowed, c ) != NULL ) {
			return c;
		}
	
		fprintf( stderr, "Invalid input. Allowed: %s\n", allowed );
		
	}

}

/**
 * getStringInput - allocates and returns a line of user input from stdin
 *
 * NOTE: Caller must free the returned string!!!
 * 
 * safe usage example
 * 

	char *input = getStringInput();

	if (!input) fputs( "Failed to read input. Try again.\n", stderr );
	else {
		printf("You entered: %s\n", input);
		free(input);
		input = NULL;
	}

 */
char *getStringInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];

	// read input (return NULL on error/EOF ), get length of input string
	if ( readInputLine( buffer, sizeof(buffer) ) ) return NULL;
	size_t len = strlen( buffer );

	// Allocate memory for string( length + 1 for null terminator )
	char *str = malloc( len + 1 );
	// Check for allocation failure
	if ( !str ) {
		fputs( "Memory allocation failed.\n", stderr );
		return NULL;
	}

	// copy buffer to allocated string
	memcpy( str, buffer, len + 1 );
	return str;

}

/**
 * getBoolInput - a safer alternative to scanf for boolean values
 *
 * Accepts "y" or "n" (case-insensitive).
 *
 * usage - bool x = getBoolInput();
 */
bool getBoolInput( void ) {

	while ( 1 ) {
		// Read single character, convert to lowercase
		char c = tolower( getCharInput() );

		if ( c == 'y' ) return true;
		if ( c == 'n' ) return false;
		fputs( "Invalid input. Enter 'y' or 'n'.\n", stderr );
	}

}
