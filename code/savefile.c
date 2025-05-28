// savefile.c
// project: weedmate

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <weedmate/common.h>
#include <weedmate/terminal.h>

// === Declarations for file structure ===
static void waitForAnyKey			( void );
static void handleCorruptSaveFile	( FILE *file, const char *msg );
static void writeSaveHeader			( FILE *file );
static void writeSaveData			( FILE *file );
static void handleSaveData			( const char *section, const char *key, const char *value, size_t len );
static int parseLine				( FILE *file, char *line, char *section );

// === Code ===
void saveToFile ( void ) {

	// Open a file with the name assigned in macros.h. The 'w' means we're writing to file.
	FILE *file = fopen( SAVE_FILE_NAME, "w" );
	if ( !file ) {
		CLEAR_SCREEN();
		fprintf( stderr, "Could not open %s for writing.\n", SAVE_FILE_NAME );
		return;
	}

	// write our data and close our file
	writeSaveHeader( file );
	writeSaveData( file );
	fclose( file );
}

void loadSaveFile ( void ) {

	FILE *file = fopen( SAVE_FILE_NAME, "r" );
	if ( !file ) return; // No file? Skip. Use defaults.

	char line [128];
	char section [32] = "";

	while ( fgets( line, sizeof( line ), file ) ) {

		// strip newline
		line[strcspn( line, "\n" )] = '\0';
		if ( parseLine ( file, line, section ) < 0 ) return;

	}

	fclose( file );
}

/**
 * writeSaveHeader - writes file header to savefile
 */
static void writeSaveHeader ( FILE *file ) {

	fprintf( file, "[Note]\n");
	fprintf( file, "# weedmate's save file can be edited quite easily.\n");
	fprintf( file, "# If you screw too much with the formatting,\n");
	fprintf( file, "# the save file will be deleted upon launch.\n");
}

static void writeSaveData ( FILE *file ) {

	// Strains
	fputs( "\n[Strains]\n", file );
	for ( int i = 0; i < STRAIN_COUNT; i++ ) {
		const char *name = strains[i].name;

		if ( name[0] == '\0' || strlen( name ) >= MAX_STRAIN_LENGTH )
			fprintf( file, "strain_%02d=UnnamedStrain%d\n", i+1, i+1 );
		else
			fprintf( file, "strain_%02d=%s\n", i+1, strains[i].name );

	}
	
	// Prices
	fputs( "\n[Prices]\n", file );
	for ( int i = 0; i < STRAIN_COUNT; i++ ) {
		fprintf( file, "price_%02d=%u\n", i+1, strains[i].price );
	}

	// City
	fprintf( file, "\n[City]\nindex=%d\n", currentCityIndex );
}

static void handleSaveData ( const char *section, const char *key, const char *value, size_t len ) {

	// Copy names in [Strains]
	if ( strcmp( section, "Strains" ) == 0 && strncmp( key, "strain_", 7 ) == 0 ) {
		int i = atoi( key+7 )-1;
		if ( i >= 0 && i < STRAIN_COUNT ) memcpy( strains[i].name, value, len+1 );
	}

	// Load prices in [Prices]
	else if ( strcmp( section, "Prices" ) == 0 && strncmp( key, "price_", 6 ) == 0 ) {
		int i = atoi( key+6 )-1;
		if ( i >= 0 && i < STRAIN_COUNT ) strains[i].price = (unsigned)strtoul( value, NULL, 10 );
	}
	
	// Load city index in [City]
	if ( strcmp( section, "City" ) == 0 && strcmp( key, "index" ) == 0 ) {
		int i = atoi( value );
		if ( i >= 0 && i < CITY_COUNT ) currentCityIndex = i;
	}
}

static int parseLine ( FILE *file, char *line, char *section ) {
	
	// Section header?
	if ( line[0] == '[' ) {
		sscanf( line, "[%31[^]]", section );
		return 0;
	}

	// Skip empty or comment lines
	if ( line[0] == '#' || line[0] == '\0' ) return 0;

	// Parse key=value
	char *equal = strchr( line, '=' );
	if ( !equal ) {
		handleCorruptSaveFile( file, "Missing '=' in line." );
		return EOF;
	}

	*equal = '\0';
	char *key = line;
	char *value = equal+1;

	size_t len = strlen( value );
	if ( len >= MAX_STRAIN_LENGTH ) {
		handleCorruptSaveFile( file, "Value too long." );
		return EOF; // Corrupt
	}

	handleSaveData( section, key, value, len );
	return 0; // OK
}

static void handleCorruptSaveFile ( FILE *file, const char *msg ) {

	int useColor = isatty(STDOUT_FILENO);

	fclose(file);
	removeSaveFile();

	ENTER_ALT_SCREEN();
	CLEAR_SCREEN();

	fputs( "[!] Save file corrupt.\n", stderr );
	if ( msg ) fprintf( stderr, "%s\n", msg );
	fprintf( stderr, "\n%s has been deleted to prevent further issues.\n", SAVE_FILE_NAME );

	ANY_KEY_MSG();
	waitForAnyKey();

	CLEAR_SCREEN();
	EXIT_ALT_SCREEN();
}

int removeSaveFile ( void ) {
	
	FILE *file = fopen( SAVE_FILE_NAME, "r" );
	if ( !file ) return EOF;
	fclose(file);

	return remove( SAVE_FILE_NAME );
}

static void waitForAnyKey ( void ) {
	
	struct termios oldt, newt;

	// get current terminal settings
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;

	// Disable canonical mode and echo
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );

	// Read one char
	(void)getchar();

	// Restore original settings
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
}
