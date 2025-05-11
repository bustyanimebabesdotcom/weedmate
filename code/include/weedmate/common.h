// common.h - contains common function declarations
// project: weedmate

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "terminal.h"

// === Structs ===
typedef struct {
	char						name[MAX_STRAIN_LENGTH];
	unsigned int				price;
}	strain_t;

typedef struct {
	const char					*name;
	float						modifier;
} city_t;

// === Globals ===
extern strain_t					strains[STRAIN_COUNT];
extern city_t 					cities[4];
extern int 						currentCityIndex;


// === Init / Exit ===
void motd						( void );
void exitWeedMate				( void );

// === Display ===
void printStrainList			( void );

// === Menus / Interaction ===
void budTenderMenu				( void );
void selectCity					( void );
void bootAnimation				( void );

// === Strain logic ===
void printNewStrainPrice		( int choice );
int getStrainChoice				( void );
void strainPriceAdjust			( strain_t* strains, int mode );
void renameStrain				( void );
void handleStrainPriceLookup	( void );

// === Signals ===
void installSignalHandlers		( void );

// === Math shit, cause why not ===
void weedCalc					( void );

// === FILE IO ===
void loadSaveFile				( void );
void saveToFile					( void );

#endif // COMMON_H
