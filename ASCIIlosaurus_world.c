// R Jesse Chaney
// rchaney@pdx.edu

// Developed in congunction with Google AI Studio

/*
########################################################################
########################################################################
####
#   #
#    #   ####
#    #  #    #
#    #  #    #
#   #   #    #
####     ####



#     #   ###   #######
##    #  #   #     #
# #   # #     #    #
#  #  # #     #    #
#   # # #     #    #
#    ##  #   #     #
#     #   ###      #



##   ##              #    #       ###
# # # #              #           #
#  #  #  ####    #####   ##      #       #   #
#     # #    #  #    #    #     ####     #   #
#     # #    #  #    #    #      #       #   #
#     # #    #  #    #    #      #        ###
#     #  ####    #### # #####    #          #
                                         ###
########################################################################
########################################################################
*/

#include <ncurses.h>
#include <stdlib.h>

#include "ASCIIlosaurus_world.h"

void
setup_ui(void)
{
    initscr();              // Start ncurses
	if (LINES < GRID_H + 5 || COLS < GRID_W + 2) {
        endwin();
        fprintf(stderr, "Terminal too small! Please resize to at least %dx%d\n"
				, GRID_W+2, GRID_H+5);
        exit(EXIT_FAILURE);
    }
    cbreak();               // Disable line buffering
    noecho();               // Don't show what we type
    curs_set(0);            // Hide the cursor
    timeout(0);             // Make getch() non-blocking
    keypad(stdscr, TRUE);   // Enable arrow keys
}

void
teardown_ui(void)
{
    endwin();               // Restore terminal
}

void
draw_world(world_state_t *state)
{
    clear(); // Clear the screen buffer

    // Draw Borders
    for(int x = 0; x < GRID_W + 2; x++) {
		mvaddch(0, x, '#');
		mvaddch(GRID_H + 1, x, '#');
	}
    for(int y = 0; y < GRID_H + 2; y++) {
		mvaddch(y, 0, '#');
		mvaddch(y, GRID_W + 1, '#');
	}

    // Draw Players
	for (int i = 0; i < MAX_PLAYERS; i++) {
        if (state->players[i].active) {
			if (state->players[i].cloaked == false) {
				// +1 accounts for the border

				mvaddch(state->players[i].y + 1
						, state->players[i].x + 1
						, state->players[i].symbol);
			}
        }
    }
    
    // Draw Instructions
	//mvprintw(GRID_H + 3, 2, "Use WASD to move. Q to quit.");
	mvprintw(GRID_H + 2, 2, "you are %c", state->client_char);
	mvprintw(GRID_H + 3, 2, "q to quit.");
	mvprintw(GRID_H + 4, 2, "  w k <up arrow>   : move up");
	mvprintw(GRID_H + 5, 2, "  a h <left arrow> : move left");
	mvprintw(GRID_H + 6, 2, "  s j <down arrow> : move down");
	mvprintw(GRID_H + 7, 2, "  d l <right arrow>: move right");
    
    refresh(); // Push buffer to screen
}

int
get_input(void)
{
    return getch(); // Wraps ncurses getch
}
