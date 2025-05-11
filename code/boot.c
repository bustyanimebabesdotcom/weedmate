#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <weedmate/common.h>

/**
 * bootAnimation - Displays a loading animation.
 * Can be commented out from the boot sequence in main.c
 * it's purely visual. our program is blazingly fast
 *
 */
void bootAnimation( void ) {

	const char *message = "Loading";
	const char *frames[] = { "     ", ".    ", ". .  ", ". . ." };
	const int frameCount = 4;

	// Get terminal size
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int termWidth  = w.ws_col;
	int termHeight = w.ws_row;

	int msgLen = strlen(message);
	int row = termHeight / 2;
	int col = (termWidth - msgLen - 1) / 2; // -1 for spinner space

	// Hide cursor
	printf("\x1b[?25l");

	// Move cursor to center
	printf("\x1b[%d;%dH", row, col);

	// Print message
	printf("%s", message);
	fflush(stdout);

	// Spinner animation
	for (int i = 0; i < 16; i++) {
		printf("%s\b\b\b\b\b", frames[i % frameCount]); // enough \b's to clear max width
		fflush(stdout);
		usleep(100000);
	}

	// Clean up
	printf("     \b\b\b\b\b");	// clear before exiting
	printf("\x1b[?25h\n");		// show cursor again
}

/**
 * motd - Displays the Message of the Day (program instructions).
 *
 * Called at startup or when the user presses 'm'.
 */
void motd ( void ) {

	int useColor = isatty(STDOUT_FILENO);

	puts( "  ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" );
	puts( "  ██ ███ ██ ▄▄▄██ ▄▄▄██ ▄▄▀██ ▄▀▄ █ ▄▄▀█▄▄ ▄▄██ ▄▄▄██" );
	puts( "  ██ █ █ ██ ▄▄▄██ ▄▄▄██ ██ ██ █ █ █ ▀▀ ███ ████ ▄▄▄██" );
	puts( "  ██▄▀▄▀▄██ ▀▀▀██ ▀▀▀██ ▀▀ ██ ███ █ ██ ███ ████ ▀▀▀██" );
	puts( "  ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" );
	puts( "=======================================================\n" );

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
