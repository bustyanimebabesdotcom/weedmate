// input.h - version 1.0.6

#ifndef INPUT_H
#define INPUT_H

// === Includes ===
#include <stdbool.h>

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

char getCharInput						( void );
char getCharInputFiltered				( const char *allowed );

char *getStringInput					( void );

bool getBoolInput						( void );

#endif // INPUT_H
