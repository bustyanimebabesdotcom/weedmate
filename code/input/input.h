// input.h
#ifndef INPUT_H
#define INPUT_H

// === INPUT BUFFER ===
#define INPUT_BUFFER_SIZE		128

// === Input handling ===
int	getIntInput					( void );
unsigned int getUIntInput		( void );

float getFloatInput				( void );
long getLongInput				( void );
unsigned long getULongInput		( void );
double getDoubleInput			( void );

char getCharInput				( void );
char getCharInputFiltered		( const char *allowed );

#endif
