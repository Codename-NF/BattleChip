#include "utils.h"
#include "constants.h"
#include "structs.h"
#include "inputs.h"
#include "game.h"
/**
 * 
 * fyi setupvalues only work for a single ship, i'm guessing you're expecting bluetooth to pass in a list of those?
 * Why don't you just make it contain a ship anyways...
 * 
 * 
 * 
 * */

/**
 * For AI(Eleiah):
 * 1. Boxes that's already hit (probably 1 update at a time instead of putting all in memory)
 * 2. Hit status
 * 3. Remaining ships
 * 
 * 
 * For display(Mario):
 * 1. Places hit
 * 2. Hit Stuats 
 **/
int main () {
    int player_num;
    // get player_number from selecting playing mode
    player_num = 2; // default to 2 for now 
    battleship game = battleship(player_num);
    list<player>::iterator p1, p2;
    for (list<player>::iterator it = game.players.begin(); it != game.players.end(); it++) {
        if (it->player_num == 1) {
            p1 = it;
        }
        else if (it->player_num == 2) {
            p2 = it;
        }
    }
    
    getting_names(&game, player_num);
    
    setting_up_ships(&p1, &p2);


    playing_game(&p1, &p2);
  return 0;
}