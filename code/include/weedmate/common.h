// common.h - contains common function declarations
// project: weedmate

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "terminal.h"

// === strain_t: represents weed strains ( name + price) ===
typedef struct {
	char						name[MAX_STRAIN_LENGTH];
	unsigned int				price;
}	strain_t;

// === Global strain array ( defined in functions.c ) ===
extern strain_t					strains[STRAIN_COUNT];

// === Init / Exit ===
void motd						( void );
void exitWeedMate				( void );

// === Display ===
void printStrainList			( void );

// === Menus / Interaction ===
void budTenderMenu				( void );
void handleStrainPriceLookup	( void );
void strainPriceAdjust			( strain_t* strains, int mode );
void renameStrain				( void );

// === Signals ===
void installSignalHandlers		( void );

// === Math shit, cause why not ===
void weedCalc					( void );

// === FILE IO ===
void loadSaveFile				( void );

#endif // COMMON_H
