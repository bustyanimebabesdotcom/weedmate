// input.h

#ifndef INPUT_H
#define INPUT_H

// === INPUT BUFFER ===
#define INPUT_BUFFER_SIZE				128

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

#endif // INPUT_H
