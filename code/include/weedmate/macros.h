// macros.h - contains macros
// project: weedmate

#ifndef MACROS_H
#define MACROS_H

// === Exit codes ===
#define EXIT_CODE				0
#define CRASH_CODE				1
#define DECIMAL_EXIT			0.0

// === UI ===
#define RETURN_TO_MENU_MSG()	puts( "\npress 'm' to return to main menu." );

// === Strain constants ===
#define STRAIN_COUNT			15
#define FENT_KUSH				3
#define MAX_STRAIN_LENGTH		21
#define MAX_STRAIN_PRICE		1000
#define MIN_STRAIN_PRICE		0
#define USER_INPUT_OFFSET		1

// === Menu flow ===
#define BUDTENDER_CONTINUE		0
#define BUDTENDER_BREAK			-1
#define BUDTENDER_OK			1

#endif // MACROS_H
