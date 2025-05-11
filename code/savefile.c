// savefile.c
// project: weedmate

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <weedmate/common.h>

/**
 * saveToFile - writes strain names to SAVE_FILE_NAME
 *
 * Overwrites file. Falls back to default names if invalid.
 */
void saveToFile ( void ) {

	FILE *file = fopen( SAVE_FILE_NAME, "w" );
	if ( !file ) {
		CLEAR_SCREEN();
		fprintf( stderr, "Could not open %s for writing.\n", SAVE_FILE_NAME );
		return;
	}

	// write section header
	fprintf( file, "[Strains]\n" );

	// fallback if somehow NULL or too long
	for ( int i = 0; i < STRAIN_COUNT; i++ ) {
		size_t len = strlen( strains[i].name );

		if ( len == 0 || len >= MAX_STRAIN_LENGTH ) {
			fprintf( file, "strain_%02d=UnnamedStrain%d\n", i+1, i+1 );
			continue;
		}

		// Write strain name in key=value format
		fprintf( file, "strain_%02d=%s\n", i+1, strains[i].name );
	}

	// Save city in its own section
	fprintf( file, "\n[City]\n" );
	fprintf( file, "current=%s\n", cities[currentCityIndex].name );

	fclose( file );
}

/**
 * loadSaveFile - loads strain names from SAVE_FILE_NAME
 *
 * Deletes file if any line exceeds MAX_STRAIN_LENGTH.
 */
void loadSaveFile( void ) {

	FILE *file = fopen( SAVE_FILE_NAME, "r" );
	if ( !file ) return; // No file? Skip. Use defaults.

	char line [128];
	int i = 0;

	while ( fgets( line, sizeof( line ), file ) ) {

		// strip newline
		line[strcspn( line, "\n" )] = '\0';

		// Skip comments, section headers, and empty lines
		if ( line[0] == '#' || line[0] == '\0' || line[0] == '[' )
			continue;

		// Parse key=value
		char *equal = strchr( line, '=' );
		if ( !equal ) continue;

		*equal = '\0';
		char *key = line;
		char *value = equal + 1;

		size_t len = strlen( value );

		// Bail out if string exceeds max length
		if ( len >= MAX_STRAIN_LENGTH ) {
			fclose(file);
			remove( SAVE_FILE_NAME );

			CLEAR_SCREEN();
			fputs( "[!] Save file corrupt or line length exceeded.\n", stderr );
			fprintf( stderr, "%s has been deleted to prevent further issues.\n", SAVE_FILE_NAME );
			fputs( "\nPress any key to continue...\n", stderr );

			getchar();													// Wait for any key
			for ( int ch; (ch = getchar()) != '\n' && ch != EOF; );		// Drain buffer

			CLEAR_SCREEN();
			return;
		}

		// Load strain names
		if ( strncmp( key, "strain_", 7 ) == 0 && i < STRAIN_COUNT ) {
			memcpy( strains[i].name, value, len+1 );
			i++;
		}

		// Load city name and match it to index
		if ( strcmp( key, "current" ) == 0 ) {
			for ( int j = 0; j < CITY_COUNT; j++ ) {
				if ( strcmp( value, cities[j].name ) == 0 ) {
					currentCityIndex = j;
					break;
				}
			}
		}
	}

	fclose( file );
}
