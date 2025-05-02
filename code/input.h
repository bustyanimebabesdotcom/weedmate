// input.h
#ifndef INPUT_H
#define INPUT_H

// === Input handling ===
int getIntInput					( void );
unsigned int getUIntInput		( void );
char getCharInput				( void );
char getCharInputFiltered		( const char *allowed );
float getFloatInput				( void );
void flushInputBuffer			( void );

#endif