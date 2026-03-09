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

#ifndef _ASCIILOSAURUS_WORLD_H
# define _ASCIILOSAURUS_WORLD_H

#include <stdint.h>

// The Grid Dimensions
# define GRID_W 60
# define GRID_H 20
# define MAX_PLAYERS 10

# define DEFAULT_PORT 10011

// The Protocol (Shared by Client/Server)
typedef struct __attribute__((packed)) {
	int x;            // X coordinate (0 to GRID_W-1)
	int y;            // Y coordinate (0 to GRID_H-1)
	char symbol;      // The character to display (e.g., '@', 'X')
	uint8_t active;   // true = Active, false = Inactive
	                  // this might be redundant if I use id correctly.
	
	// You won't be doing anything with these
	// DO NOT REMOVE THEM
	uint8_t cloaked;  // is the client currently cloaked
	int cloak_countdown; // how many refresh cycles are left in this cloak.
} player_t;


typedef struct __attribute__((packed)) {
	char client_char;
	player_t players[MAX_PLAYERS]; // Max 10 players for simplicity
} world_state_t;

// --- UI FUNCTIONS ---
void setup_ui(void);                 // Call once at start
void teardown_ui(void);              // Call once at end
void draw_world(world_state_t *);    // Call every loop to update screen
int  get_input(void);                // Returns key pressed (or ERR if none)

#endif // _ASCIILOSAURUS_WORLD_H
