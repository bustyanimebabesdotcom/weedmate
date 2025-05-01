// common.h - contains function declarations
// project: weedmate

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

// === strain_t: represents weed strains ( name + price) ===
typedef struct {
	char						*name;
	unsigned int				price;
} strain_t;

// === Init / Exit ===
void motd						( void );
void exitWeedMate				( void );

// === Display ===
void printStrainList			( void );

// === Menus / Interaction ===
void budTenderMenu				( void );
void handleStrainPriceLookup	( void );
void strainPriceAdjust			( strain_t* strains, int mode );

// === Input handling ===
int getIntInput					( void );
unsigned int getUIntInput		( void );
char getCharInput				( void );
float getFloatInput				( void );
void flushInputBuffer			( void );

// === Math shit, cause why not ===
void weedCalc					( void );

// === Global strain array ( defined in functions.c ) ===
extern strain_t					strains[STRAIN_COUNT];

#endif