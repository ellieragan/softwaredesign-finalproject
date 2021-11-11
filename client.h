/* client.h - header file for client module
 *
 * Ellie Boyd
 * Nov 10 2021
 */

#include <stdio.h>
#include "support/message.h"
/******** parseArgs ********/
/* checks if message module can be initialized
 * checks for correct number of arguments
 * verify window size
 * 
 * Caller provides:
 *  argc, argv
 *
 * We return:
 *  true if in player mode, false if in spectator mode
 *
 */
bool parseArgs(const int argc, char* argv[]);

/******** handleInput ********/
/* takes keyboard input from a player and turns it into characters to send to the server
 *
 * Caller provides:
 *  a void pointer
 *  
 * We return:
 *  true to terminate looping, false to keep looping
 */
static bool handleInput(void* arg);

/******** handleMessage ********/
/* takes messages from the server and updates the screen accordingly
 *
 * Caller provides:
 *  void pointer, const addr_t, and a char* message
 *
 * We return:
 *  true to terminate looping, false to keep looping
 */
static bool handleMessage(void* arg, const addr_t from, const char* message);

/******** outputMap ********/
/* print map onto the screen for the player using ncurses function
 *
 * Caller provides:
 *  char* string of the map
 *
 * We return:
 *  nothing
 */
static void outputMap(char* map);
