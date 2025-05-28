// args.c
// project: weedmate

#include <stdio.h>
#include <string.h>
#include <weedmate/common.h>
#include <weedmate/args.h>
#include <weedmate/version.h>

// === Command-to-flag mapping ===
static const argmap_t commandMap[] = {

	{ "-r", "--remove",	"Delete save file.", 		CMD_DELETE	},
	{ "-c", "--create",	"Create save file.", 		CMD_CREATE	},
	{ "-h", "--help",	"Show this help message.",	CMD_HELP	},
	{ "-v", "--version","Print current version.",	CMD_VERSION	},
	{ NULL,	NULL,		NULL,						CMD_NONE	}
};

// === Declarations for file structure ===
static void showHelp			( void );
static cmdtype_e parseCommand	( const char *arg );
static void printVersion		( void );
static void deleteSaveFile		( void );
static void createSaveFile		( void );

// === Code ===
int handleArgs ( int argc, char *argv[] ) {

	if ( argc < 2 ) return ARGS_NOT_HANDLED;
	cmdtype_e cmd = parseCommand( argv[1] );

	switch ( cmd ) {

		case CMD_DELETE:
			deleteSaveFile();
			return ARGS_HANDLED;

		case CMD_CREATE:
			createSaveFile();
			return ARGS_HANDLED;

		case CMD_HELP:
			showHelp();
			return ARGS_HANDLED;

		case CMD_VERSION:
			printVersion();
			return ARGS_HANDLED;

		default:
			fprintf( stderr,
				"Usage: weedmate [OPTION]\n"
				"Try 'weedmate --help' for more information.\n"
			);
			return ARGS_INVALID;

	}
}

static cmdtype_e parseCommand ( const char *arg ) {

	if ( !arg ) return CMD_NONE;
	
	for ( int i = 0; commandMap[i].shortFlag != NULL; i++ ) {
		if ( 	strcmp ( arg, commandMap[i].shortFlag ) == 0 ||
			 	strcmp ( arg, commandMap[i].longFlag )  == 0 ) {
			return commandMap[i].cmdtype;
		}
	}

	return CMD_NONE;
}

void deleteSaveFile ( void ) {
	
	removeSaveFile() == 0
		? printf( "Deleted %s successfully.\n", SAVE_FILE_NAME )
		: fputs( "No save file found on disk.\n", stderr );
}

void createSaveFile ( void ) {

	FILE *file = fopen( SAVE_FILE_NAME , "r" );

	if ( !file ) {
		saveToFile();
		printf( "Created %s successfully.\n", SAVE_FILE_NAME );
		return;
	}

	fclose( file );
	fputs( "Save file already exists.\n", stderr );
}


static void printVersion ( void ) {

	printf(
		"weedmate %s\n"
		"License: The Unlicense <https://unlicense.org/>.\n"
		"This is free software: you are free to change and redistribute it.\n"
		"There is NO WARRANTY, to the extent permitted by law.\n\n"
		"Written by bustyanimebabesdotcom; see\n"
		"<https://github.com/bustyanimebabesdotcom>.\n",
		WEEDMATE_VERSION_STRING
	);
}

static void showHelp ( void ) {

	puts( "Usage: weedmate [OPTION]" );
	puts( "\nGeneral purpose:" );
	for ( int i = 0; commandMap[i].shortFlag != NULL; i++ ) {
		printf( "  %s, %-22s%s\n", 
			commandMap[i].shortFlag,
			commandMap[i].longFlag,
			commandMap[i].description );
	}
}
