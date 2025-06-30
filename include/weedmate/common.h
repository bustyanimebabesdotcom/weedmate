// common.h - contains common function declarations
// project: weedmate

#ifndef COMMON_H_
#define COMMON_H_

#include "macros.h"

// === Structs ===
typedef struct strain_s {
	char						name[MAX_STRAIN_LENGTH];
	unsigned int				price;
} strain_s;

typedef struct city_s {
	const char					*name;
	float						modifier;
} city_s;

// === Globals ===
extern strain_s					strains[STRAIN_COUNT];
extern city_s 					cities[4];
extern int 						currentCityIndex;

// === Init / Exit ===
void motd						( void );
void exitWeedMate				( void );

// === Display ===
void printStrainList			( void );

// === Menus / Interaction ===
void budTenderMenu				( void );
void selectCity					( void );

// === Strain logic ===
void printNewStrainPrice		( int choice );
int getStrainChoice				( void );
void strainPriceAdjust			( strain_s* strains, int mode );
void renameStrain				( void );
void handleStrainPriceLookup	( void );

// === Signals ===
void installSignalHandlers		( void );

// === Math shit, cause why not ===
void weedCalc					( void );

// === FILE IO ===
void loadSaveFile				( void );
void saveToFile					( void );
int removeSaveFile				( void );

#endif // COMMON_H_
