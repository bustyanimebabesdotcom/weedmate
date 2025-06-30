/**
 * input.c - version 1.0.9
 *
 * Author: 		https://github.com/bustyanimebabesdotcom
 * License:		The Unlicense
 *
 * safe(r than scanf) input handling
 * This can be ported to any project, to be used as a standalone input library.
 * There are obviously better input libraries, but the purpose of this is to be
 * a safe(r) alternative to scanf for morons like me.
 * 
 * Main safety features are simple prevention of buffer overflow exploits,
 * and validating input.
 *
 * TODO: 		Wrap more copy pasted shit into helpers.
 * TODO: 		Optimize codebase where possible.
 * PHASE 2 PLAN
 * COMPLETE:	Implement string handling without null terminator. 
 *				Byte for byte getchar() loop in a function that returns a string_s
 *
 * You are free to use this in any project, commercial or personal.
 * Attribution is appreciated but not required.
 *
 * For more information, see LICENSE or visit <https://unlicense.org/>
 */

#define _GNU_SOURCE

#define alwaysInline	__attribute__((always_inline)) inline
#define cold			__attribute__((cold))
#define unlikely(x)		__builtin_expect(!!(x), 0)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include "input.h"

/**
 * printError - calls fputs into stderr, simple wrapper
 */
static cold void printError ( const char *msg ) {

	fputs( msg, stderr );
}



/**
 * drainStdin - Drains leftover input from stdin to prevent buffer overflows.
 * Reads until newline or EOF is encountered
 * 
 * called by readByteInput()
 */
static cold void drainStdin ( void ) {

	int c;

	while ( ( c = getchar_unlocked() ) != '\n' && c != EOF )
		;
}



/**
 * readByteInput - read up to maxLen bytes from stdin into buffer, 
 * 					stop at newline or EOF, drain excess input, and report length.
 * 
 * @buf:		buffer to fill with input characters
 * @maxLen:		maximum number of bytes to read into buf
 * @outLen:		pointer to size_t to receive the number of bytes read
 * 
 * Returns:
 * 		0 - on success
 * 		1 - on error ( NULL buf or outLen, or maxLen < 1 )
 *	   -1 - on EOF ( caller should check and handle appropriately )
 */
static alwaysInline int readByteInput ( char *buf, size_t maxLen, size_t *outLen ) {

	if ( unlikely( !buf || !outLen || maxLen < 1 )) return 1;

	size_t i = 0;
	int c;

	for ( ; i < maxLen; i++ ) {
		c = getchar_unlocked();
		if ( unlikely( c == '\n' || c == EOF )) break;
		buf[i] = (char)c;
	}

	*outLen = i;

	if ( unlikely( i == maxLen && c != '\n' && c != EOF )) {
		drainStdin();
		printError( "Input exceeding buffer size. Try again.\n" );
		return 1;
	}

	if ( unlikely( c == EOF && i == 0 )) return EOF;

	return 0;
}



/**
 * getIntInput - a safer alternative to scanf for integers
 *
 * usage - int x = getIntInput();
 * 
 * returns INT_MIN on error or EOF
 */
int getIntInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		size_t len;
		if ( readByteInput( buffer, sizeof(buffer) - 1, &len )) return INT_MIN;
		buffer[len] = '\0';

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE || value != (int)value ) {
			printError( "Invalid input. Try again.\n" );
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
unsigned int getUIntInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		size_t len;
		if ( readByteInput( buffer, sizeof(buffer) - 1, &len )) return UINT_MAX;
		buffer[len] = '\0';

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );

		if ( buffer[0] == '-' ) {
			printError( "Value can not be negative.\n" );
			continue;
		}

		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE || value > UINT_MAX ) {
			printError( "Invalid input. Try again.\n" );
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
float getFloatInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	float value;

	while ( 1 ) {

		size_t len;
		if ( readByteInput( buffer, sizeof(buffer) - 1, &len )) return NAN;
		buffer[len] = '\0';

		errno = 0;
		value = strtof( buffer, &endptr );

		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE || !isfinite( value )) {
			printError( "Invalid input. Try again.\n" );
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
double getDoubleInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	double value;

	while ( 1 ) {

		size_t len;
		if ( readByteInput( buffer, sizeof(buffer) - 1, &len )) return NAN;
		buffer[len] = '\0';

		errno = 0;
		value = strtod( buffer, &endptr );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE || !isfinite( value )) {
			printError( "Invalid input. Try again.\n" );
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
long getLongInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long value;

	while ( 1 ) {

		size_t len;
		if ( readByteInput( buffer, sizeof(buffer) - 1, &len )) return LONG_MIN;
		buffer[len] = '\0';

		errno = 0;
		value = strtol( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE) {
			printError( "Invalid input. Try again.\n" );
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
unsigned long getULongInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long value;

	while ( 1 ) {

		size_t len;
		if ( readByteInput( buffer, sizeof(buffer) - 1, &len )) return ULONG_MAX;
		buffer[len] = '\0';

		errno = 0;
		value = strtoul( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE ) {
			printError( "Invalid input. Try again.\n" );
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
long long getLongLongInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	long long value;

	while ( 1 ) {

		size_t len;
		if ( readByteInput( buffer, sizeof(buffer) - 1, &len )) return LLONG_MIN;
		buffer[len] = '\0';

		errno = 0;
		value = strtoll( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE ) {
			printError( "Invalid input. Try again.\n" );
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
unsigned long long getULongLongInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	char *endptr;
	unsigned long long value;

	while ( 1 ) {

		size_t len;
		if ( readByteInput( buffer, sizeof(buffer) - 1, &len )) return ULLONG_MAX;
		buffer[len] = '\0';

		errno = 0;
		value = strtoull( buffer, &endptr, 10 );
		
		if ( endptr == buffer || *endptr != '\0' || errno == ERANGE ) {
			printError( "Invalid input. Try again.\n" );
			continue;
		}

		return value;
	}
}



/**
 * getCharInput - a safer alternative to scanf for chars
 *
 * usage - int x = getCharInput();
 *
 * IMPORTANT: Only accepts one character. " a" and "a " are considered invalid.
 * 
 * returns EOF on error or EOF
 */
int getCharInput ( void ) {

	char buffer[CHAR_INPUT_BUFFER_SIZE];
	size_t len;

	while ( 1 ) {

		int result = readByteInput( buffer, sizeof(buffer) - 1, &len );
		buffer[len] = '\0';

		if ( result == EOF ) return EOF;
		if ( result == 1 ) continue;
		if ( len == 0 ) return '\n';
		if ( len == 1 ) return (unsigned char)buffer[0];
		printError( "Invalid input. Please enter a single character.\n" );

	}
}



/**
 * getCharInputFiltered - a safer alternative to scanf for chars
 *
 * usage - int x = getCharInputFiltered( "abc" );
 * 
 * returns EOF on error or EOF
 */
int getCharInputFiltered ( const char *allowed ) {

	if ( !allowed ){
		printError( "ERROR: NULL passed to 'allowed'.\n" );
		exit(EXIT_FAILURE);
	}

	if ( allowed[0] == '\0' ) {
		printError( "No allowed characters specified. Exiting.\n" );
		return 1;
	}

	char buffer[CHAR_INPUT_BUFFER_SIZE];
	size_t len;

	while ( 1 ) {

		int result = readByteInput( buffer, sizeof(buffer) - 1, &len );
		buffer[len] = '\0';
		
		if ( result == EOF ) return EOF;
		if ( result == 1 ) continue;
		
		// Ensure input is exactly one character
		if ( len != 1 ) {
			printError( "Invalid input. Please enter a single character.\n" );
			continue;
		}

		char c = buffer[0];

		if ( strchr( allowed, c ) != NULL ) return (unsigned char)c;
	
		fprintf( stderr, "Invalid input. Allowed: %s\n", allowed );
		
	}
}



/**
 * getCStringInput - allocates and returns a line of user input from stdin
 *
 * NOTE: 	Caller must free the returned string!!!
 * 
 *
 * safe usage example
 * 
 *
 *	char *input = getCStringInput();
 *
 *	if (!input) fputs( "Failed to read input. Try again.\n", stderr );
 *	else {
 *		printf("You entered: %s\n", input);
 *		free(input);
 *		input = NULL;
 *	}
 *
 * NOTE: 	we use memcpy() because it copies exactly len + 1 bytes from buffer, 
 *			avoiding overflow and termination issues.
 * 			the overhead is minimal, memcpy() is well optimized.
 * 
 * returns NULL on error or EOF
 */
char *getCStringInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	size_t len;

	// read input (return NULL on error/EOF )
	if ( readByteInput( buffer, sizeof(buffer)-1, &len )) return NULL;

	// add null-terminator for C string
	buffer[len] = '\0';

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
 * getStringInput - reads a line from stdin into a heap-allocated string_s
 * 
 * NOTE: 	Caller is responsible for freeing the returned buffer:
 * 			free(str.data);
 * 
 * Safe usage example:
 * 
 * 		string_s str = getStringInput();
 *
 *		if (!str.data) return;
 *		printf("%.*s", (int)str.len, str.data)
 *		free(str.data);
 *		str.data = NULL;
 *
 * NOTE: 	Uses memcpy() to copy exactly len bytes from internal buffer
 * 			No null terminator is appended-use the length field for safe access.
 * 
 * Returns:
 * 		A string_s with .data == NULL and .len == 0 on error or EOF,
 * 		otherwise .data points to malloc( len ? len : 1 ) and .len is the byte count.
 */
string_s getStringInput ( void ) {

	char buffer[INPUT_BUFFER_SIZE];
	size_t len;

	// read input ( return NULL on error/EOF )
	if ( readByteInput( buffer, sizeof(buffer), &len )) return (string_s){ NULL, 0 };

	// Allocate memory for string data, length is stored in len parameter
	string_s str;
	str.data = malloc( len ? len : 1 );
	if ( !str.data ) {
		printError( "Memory allocation failed.\n" );
		return (string_s){ NULL, 0 };
	}

	// copy buffer to string data
	memcpy( str.data, buffer, len );
	str.len = len;
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
bool getBoolInput ( void ) {

	while ( 1 ) {
		// Read single character, convert to lowercase
		int c = getCharInput();

		if ( c == EOF ) {
			printError( "EOF detected. Returning false by default.\n" );
			return false;
		}

		// we avoid using tolower() due to EOF potentially triggering UB
		if ( c == 'Y' || c == 'y' ) return true;
		if ( c == 'N' || c == 'n' ) return false;
		printError( "Invalid input. Enter 'y' or 'n'.\n" );
	}
}
