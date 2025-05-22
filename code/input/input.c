/* input.c - version 1.0.7
 *
 * safe(r than scanf) input handling
 * This can be ported to any project, to be used as a standalone input library.
 * There are obviously better input libraries, but the purpose of this is to be
 * a safe(r) alternative to scanf for morons like me.
 * 
 * Main safety features are simple prevention of buffer overflow exploits,
 * and validating input.
 *
 * TODO: 	Wrap more copy pasted shit into helpers.
 * TODO: 	Optimize codebase where possible.
 * PHASE 2 PLAN
 * TODO:	Implement string handling without null terminator. Byte for byte getchar() loop in a function that returns a string_t
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include "input.h"

static void printError( const char *msg ) {

	fputs( msg, stderr );

}

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
 * 		0 - on success,
 *		1 - on error,
 *	   -1 - on EOF ( caller should check and handle appropriately ),
 */
static int readInputLine( char *buf, size_t size ) {

	// Read input line into buffer, check for errors or EOF
	if ( fgets( buf, size, stdin ) == NULL ) {
		if ( feof( stdin ) ) {
			clearerr(stdin);
			return -1;
		}

		else {
			printError( "Error in input stream.\n" );
			return 1;	// caller should retry
		}
	}
	
	// check for newline
	char *newline = strchr( buf, '\n' );

	// Check for overflow: no newline and not EOF means input too long
	if ( !newline && !feof( stdin ) ) {
		// flush excess input to prevent overflow
		drainStdin();
		printError( "Input exceeding buffer size. Try again.\n" );
		return 1;
	}

	// strip trailing newline
	if ( newline ) *newline = '\0';
	return 0;

}

/**
 * getIntInput - a safer alternative to scanf for integers
 *
 * usage - int x = getIntInput();
 * 
 * returns INT_MIN on error or EOF
 */
int getIntInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		if ( readInputLine( buffer, sizeof(buffer) )) return INT_MIN;

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			printError( "Invalid number. Try again.\n" );
			continue;
		}

		if ( errno == ERANGE || value < INT_MIN || value > INT_MAX ) {
			printError( "Value out of range. Try again.\n" );
			continue;
		}

		return (int)value;
	}

}

/**
 * getUIntInput - a safer alternative to scanf for unsigned integers
 *
 * usage - unsigned int x = getUIntInput();
 * 
 * returns UINT_MAX on error or EOF
 */
unsigned int getUIntInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		if ( readInputLine( buffer, sizeof(buffer) )) return UINT_MAX;

		if ( buffer[0] == '-' ) {
			printError( "Invalid number. Try again.\n" );
			continue;
		}

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );

		if ( endptr == buffer || *endptr != '\0') {
			printError( "Invalid number. Try again.\n" );
			continue;
		}
		
		if ( errno == ERANGE || value > UINT_MAX ) {
			printError( "Value out of range. Try again.\n" );
			continue;
		}

		return (unsigned int)value;
	}

}

/**
 * getFloatInput - a safer alternative to scanf for floating point numbers
 *
 * usage - float x = getFloatInput();
 * 
 * returns NAN on error or EOF
 */
float getFloatInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	float value;

	while ( 1 ) {

		if ( readInputLine( buffer, sizeof(buffer) )) return NAN;

		errno = 0;
		// Convert string to float. 'endptr' is set to the first invalid character after the number.
		value = strtof( buffer, &endptr );

		if ( endptr == buffer || *endptr != '\0') {
			printError( "Invalid number. Try again." );
			continue;
		}

		if ( errno == ERANGE || !isfinite( value ) ) {
			printError( "Number out of range. Try again.\n" );
			continue;
		}
	
		return value;
	}

}

/**
 * getDoubleInput - a safer alternative to scanf for doubles
 *
 * usage - double x = getDoubleInput();
 * 
 * returns NAN on error or EOF
 */
double getDoubleInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	double value;

	while ( 1 ) {

		if ( readInputLine( buffer, sizeof(buffer) )) return NAN;

		errno = 0;
		value = strtod( buffer, &endptr );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			printError( "Invalid number. Try again.\n" );
			continue;
		}

		if ( errno == ERANGE || !isfinite( value ) ) {
			printError( "Value out of range. Try again.\n" );
			continue;
		}

		return value;
	}

}

/**
 * getLongInput - a safer alternative to scanf for longs
 *
 * usage - long x = getLongInput();
 * 
 * returns LONG_MIN on error or EOF
 */
long getLongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		if ( readInputLine( buffer, sizeof(buffer) )) return LONG_MIN;

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			printError( "Invalid number. Try again.\n" );
			continue;
		}

		if ( errno == ERANGE ) {
			printError( "Value out of range. Try again.\n" );
			continue;
		}

		return value;
	}

}

/**
 * getULongInput - a safer alternative to scanf for unsigned longs
 *
 * usage - unsigned long x = getULongInput();
 * 
 * returns ULONG_MAX on error or EOF
 */
unsigned long getULongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		if ( readInputLine( buffer, sizeof(buffer) )) return ULONG_MAX;

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			printError( "Invalid number. Try again.\n" );
			continue;
		}

		if ( errno == ERANGE ) {
			printError( "Value out of range. Try again.\n" );
			continue;
		}

		return value;
	}

}

/**
 * getLongLongInput - a safer alternative to scanf for long longs
 *
 * usage - long long x = getLongLongInput();
 * 
 * returns LLONG_MIN on error or EOF
 */
long long getLongLongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long long value;

	while ( 1 ) {

		if ( readInputLine( buffer, sizeof(buffer) )) return LLONG_MIN;

		errno = 0;
		value = strtoll( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			printError( "Invalid number. Try again.\n" );
			continue;
		}

		if ( errno == ERANGE ) {
			printError( "Value out of range. Try again.\n" );
			continue;
		}

		return value;
	}

}

/**
 * getULongLongInput - a safer alternative to scanf for unsigned long longs
 *
 * usage - unsigned long long x = getULongLongInput();
 * 
 * returns ULLONG_MAX on error or EOF
 */
unsigned long long getULongLongInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long long value;

	while ( 1 ) {

		if ( readInputLine( buffer, sizeof(buffer) )) return ULLONG_MAX;

		errno = 0;
		value = strtoull( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' ) {
			printError( "Invalid number. Try again.\n" );
			continue;
		}

		if ( errno == ERANGE ) {
			printError( "Value out of range. Try again.\n" );
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
 * 
 * returns EOF on error or EOF
 */
char getCharInput( void ) {

	char buffer[CHAR_INPUT_BUFFER_SIZE];

	while ( 1 ) {

		int result = readInputLine( buffer, sizeof(buffer) );
		
		if ( result == EOF ) return EOF;
		if ( result == 1 ) continue;
		if ( strlen(buffer) == 0 ) return '\n';
		if ( strlen(buffer) == 1 ) return buffer[0];
	
		printError( "Invalid input. Please enter a single character.\n" );
	}

}

/**
 * getCharInputFiltered - a safer alternative to scanf for chars
 *
 * usage - char x = getCharInputFiltered( "abc" );
 * 
 * returns EOF on error or EOF
 */
char getCharInputFiltered( const char *allowed ) {

	if ( allowed == NULL ){
		printError( "getCharInputFiltered(): NULL passed to 'allowed'. Exiting.\n" );
		exit(EXIT_FAILURE);
	}

	if ( allowed[0] == '\0' ) {
		printError( "No allowed characters specified. Exiting.\n" );
		return 1;
	}

	char buffer[CHAR_INPUT_BUFFER_SIZE];

	while ( 1 ) {

		int result = readInputLine( buffer, sizeof(buffer) );
		
		if ( result == EOF ) return EOF;
		if ( result == 1 ) continue;
		if ( strlen(buffer) == 1 ) return buffer[0];
		
		// Ensure input is exactly one character
		if ( strlen(buffer) != 1 ) {
			printError( "Invalid input. Please enter a single character.\n" );
			continue;
		}

		char c = buffer[0];

		// Return if character is in allowed set
		if ( strchr( allowed, c ) != NULL ) return c;
	
		fprintf( stderr, "Invalid input. Allowed: %s\n", allowed );
		
	}

}

/**
 * getStringInput - allocates and returns a line of user input from stdin
 *
 * NOTE: Caller must free the returned string!!!
 * 
 *
 * safe usage example
 * 
 *
 *	char *input = getStringInput();
 *
 *	if (!input) fputs( "Failed to read input. Try again.\n", stderr );
 *	else {
 *		printf("You entered: %s\n", input);
 *		free(input);
 *		input = NULL;
 *	}
 *
 * NOTE: we use memcpy() because it copies exactly len + 1 bytes from buffer, avoiding overflow and termination issues.
 * 			the overhead is worth it in this case for the safety is provides over strpy() or strncpy()
 * 
 * returns NULL on error or EOF
 */
char *getStringInput( void ) {

	char buffer[INPUT_BUFFER_SIZE];

	// read input (return NULL on error/EOF ), get length of input string
	if ( readInputLine( buffer, sizeof(buffer) )) return NULL;
	size_t len = strlen( buffer );

	// Allocate memory for string( length + 1 for null terminator )
	char *str = malloc( len+1 );
	// Check for allocation failure
	if ( !str ) {
		printError( "Memory allocation failed.\n" );
		return NULL;
	}

	// copy buffer to allocated string
	memcpy( str, buffer, len+1 );
	return str;

}

/**
 * getBoolInput - a safer alternative to scanf for boolean values
 *
 * Accepts "y" or "n" (case-insensitive).
 *
 * usage - bool x = getBoolInput();
 * 
 * returns false on EOF. Terrible solution but it's the best i could figure out.
 */
bool getBoolInput( void ) {

	while ( 1 ) {
		// Read single character, convert to lowercase
		char c = getCharInput();

		if ( c == EOF  ) {
			printError( "EOF detected. Returning false by default.\n" );
			return false;
		}

		// we avoid using tolower() due to EOF potentially triggering UB
		if ( c == 'Y' || c == 'y' ) return true;
		if ( c == 'N' || c == 'n' ) return false;
		printError( "Invalid input. Enter 'y' or 'n'.\n" );
	}

}
