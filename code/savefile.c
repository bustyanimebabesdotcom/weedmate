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

	// fallback if somehow NULL or too long
	for ( int i = 0; i < STRAIN_COUNT; i++ ) {
		size_t len = strlen( strains[i].name );

		if ( len == 0 || len >= MAX_STRAIN_LENGTH ) {
			fprintf( file, "UnnamedStrain%d\n", i+1 );
			continue;
		}
		// Write name with newline
		fwrite( strains[i].name, 1, len, file );
		fputc( '\n', file );
	}

	// Save city as final line
	size_t cityLen = strlen(cities[currentCityIndex].name);
	fwrite(cities[currentCityIndex].name, 1, cityLen, file);
	fputc('\n', file);


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

	while ( i < STRAIN_COUNT && fgets( line, sizeof( line ), file )) {

		// strip newline
		line[strcspn( line, "\n" )] = '\0';

		size_t len = strlen( line );

		// Skip empty or garbage lines

		if ( len == 0 ) {
			i++;
			continue;
		}
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
		
		// Copy line into strain struct safely
		memcpy( strains[i].name, line, len+1 );
		i++;
	}

	if (fgets(line, sizeof(line), file)) {
	line[strcspn(line, "\n")] = '\0';
	for (int j = 0; j < CITY_COUNT; j++) {
		if (strcmp(line, cities[j].name) == 0) {
			currentCityIndex = j;
			break;
		}
	}
}

	fclose( file );

}
