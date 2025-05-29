#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <weedmate/common.h>

/**
 * motd - Displays the Message of the Day (program instructions).
 *
 * Called at startup or when the user presses 'm'.
 */
void motd ( void ) {

	int useColor = isatty(STDOUT_FILENO);

	puts(
		"  ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n"
		"  ██ ███ ██ ▄▄▄██ ▄▄▄██ ▄▄▀██ ▄▀▄ █▀▄▄▀█▄▄ ▄▄██ ▄▄▄██\n"
		"  ██ █ █ ██ ▄▄▄██ ▄▄▄██ ██ ██ █ █ █ ▀▀ ███ ████ ▄▄▄██\n"
		"  ██▄▀▄▀▄██ ▀▀▀██ ▀▀▀██ ▀▀▄██ ███ █ ██ ███ ████ ▀▀▀██\n"
		"  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n"
		"=======================================================\n" );

	printf( "               Welcome to " GREEN "weedmate" RESET "!\n\n");

	COLOR_KEY( 'l', "browse the strain list." );
	COLOR_KEY( 'p', "check the price of individual strains." );
	COLOR_KEY( 'r', "rename strains." );
	COLOR_KEY( 'b', "open the budtender menu.\n" );

	COLOR_KEY( '+', "double prices." );
	COLOR_KEY( '-', "cut prices in half." );
	COLOR_KEY( 'c', "open the calculator." );
	COLOR_KEY( 's', "select your location.\n" );

	COLOR_KEY( 'm', "reprint this message." );
	QUIT_KEY( 'q', "quit.\n" );

	printf( "> " );
}
