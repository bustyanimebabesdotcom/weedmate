// macros.h - contains macros
// project: weedmate
#include <weedmate/terminal.h>

#ifndef MACROS_H_
#define MACROS_H_

// === Exit codes ===
#define EXIT_CODE				0
#define CRASH_CODE				1
#define DECIMAL_EXIT			0.0

// === CONSTANTS ===
#define STRAIN_COUNT			15
#define CITY_COUNT				4
#define FENT_KUSH				3
#define MAX_STRAIN_LENGTH		21
#define MAX_STRAIN_PRICE		1000
#define MIN_STRAIN_PRICE		0
#define USER_INPUT_OFFSET		1

// === Menu flow ===
typedef enum budtender_e {
	BUDTENDER_BREAK 	= 		-1,
	BUDTENDER_CONTINUE 	= 		0,
	BUDTENDER_OK 		= 		1
} budtender_e;

typedef enum strainprice_e {
	ADJUST_DOUBLE	=			1,
	ADJUST_HALVE	=		   -1
} strainprice_e;

// === File i/o ===
#define SAVE_FILE_NAME			".weedrc"

#endif // MACROS_H_
