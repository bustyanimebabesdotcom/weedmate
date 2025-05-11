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

	// Open a file with the name assigned in macros.h. The 'w' means we're writing to file.
	FILE *file = fopen( SAVE_FILE_NAME, "w" );
	if ( !file ) {
		CLEAR_SCREEN();
		fprintf( stderr, "Could not open %s for writing.\n", SAVE_FILE_NAME );
		return;
	}

	// Write a little note at the top :)
	fprintf( file, "[Note]\n");
	fprintf( file, "# weedmate's save file can be edited quite easily.\n");
	fprintf( file, "# If you screw too much with the formatting,\n");
	fprintf( file, "# the save file will be deleted upon launch.\n");

	// write section header
	fprintf( file, "\n[Strains]\n" );

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

	// Save prices
	fprintf( file, "\n[Prices]\n" );
	for ( int i = 0; i < STRAIN_COUNT; i++ ) {
		fprintf( file, "price_%02d=%u\n", i + 1, strains[i].price );
	}


	// Save city in its own section
	fprintf( file, "\n[City]\n" );
	fprintf( file, "index=%d\n", currentCityIndex );

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
	char section [32] = "";

	while ( fgets( line, sizeof( line ), file ) ) {

		// strip newline
		line[strcspn( line, "\n" )] = '\0';

		// Section header?
		if ( line[0] == '[' ) {
			sscanf( line, "[%31[^]]", section );
			continue;
		}

		// Skip empty or comment lines
		if ( line[0] == '#' || line[0] == '\0' )
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

		// Copy strain names, only in [Strains]
		if ( strcmp( section, "Strains" ) == 0 && strncmp( key, "strain_", 7 ) == 0 ) {
			int index = atoi( key + 7 ) - 1;

			if ( index >= 0 && index < STRAIN_COUNT ) {
				memcpy( strains[index].name, value, len+1 );
			}
		}

		// Load prices in [Prices]
	if ( strcmp( section, "Prices" ) == 0 && strncmp( key, "price_", 6 ) == 0 ) {
		int index = atoi( key + 6 ) - 1;
		if ( index >= 0 && index < STRAIN_COUNT ) {
			unsigned int price = (unsigned int) strtoul( value, NULL, 10 );
			strains[index].price = price;
		}
	}


		// Load city index in [City]
		if ( strcmp( section, "City" ) == 0 && strcmp( key, "index" ) == 0 ) {
			int id = atoi( value );
			if ( id >= 0 && id < CITY_COUNT ) {
				currentCityIndex = id;
			}
		}
	}

	fclose( file );
}
