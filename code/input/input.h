// input.h - version 1.0.8

#ifndef INPUT_H
#define INPUT_H

// === Includes ===
#include <stdbool.h>
#include <stddef.h> // for size_t

// === STRUCTs ===
typedef struct {
	char	*data;	// pointer to data
	size_t	len;	// byte counter
} string_t;

// === INPUT BUFFER ===
#define INPUT_BUFFER_SIZE				128
#define CHAR_INPUT_BUFFER_SIZE			4

// === Input handling ===
int	getIntInput							( void );
unsigned int getUIntInput				( void );

float getFloatInput						( void );
double getDoubleInput					( void );

long getLongInput						( void );
unsigned long getULongInput				( void );
long long getLongLongInput				( void );
unsigned long long getULongLongInput	( void );

int getCharInput						( void );
int getCharInputFiltered				( const char *allowed );

char *getCStringInput					( void );
string_t getStringInput					( void );

bool getBoolInput						( void );

#endif // INPUT_H
