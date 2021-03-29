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
    if (DEBUG) { // CMD playing debug only 
        int player_num;
        bool single_player = false;
        // get player_number from selecting playing mode
        cout << "How many players? (Enter 1 for Single player or 2 for Multi player)" << endl;
        cin >> player_num;
        if (player_num == 2) {
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
            // cout << p1->player_name << endl;
            // cout << p2->player_name << endl;
            setting_up_ships(&p1, &p2, single_player);


            playing_game(&p1, &p2, single_player);
        }
        else {
            // if player_num == 1 -> plyaing with AI
            single_player = true;
            battleship game = battleship(2);
            list<player>::iterator p1, AI;
            for (list<player>::iterator it = game.players.begin(); it != game.players.end(); it++) {
                if (it->player_num == 1) {
                    p1 = it;
                }
                else if (it->player_num == 2) {
                    AI = it;
                }
            }
            
            getting_names(&game, player_num);
            
            setting_up_ships(&p1, &AI, single_player);

            AI_setting_up(&AI);

            playing_game(&p1, &AI, single_player);
        }
        
        return 0;
    }
    else { // dealing with UI / BT
        int player_num;
        bool single_player = false;
        createmessage input1 = create_lobby();
        
        player_num = input1.numplayer;

        if (player_num == 2) {
            string input2 = wait_for_player2();

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
            
            setting_emails(&game, player_num, input1.email, input2);
            
            send_ready_messaeg_BT();
            
            setting_up_ships_BT(&p1, &p2, single_player);


            playing_game_BT(&p1, &p2, single_player);
        }
        else {
            // if player_num == 1 -> plyaing with AI
            reject_player2();
            single_player = true;
            battleship game = battleship(2);
            list<player>::iterator p1, AI;
            for (list<player>::iterator it = game.players.begin(); it != game.players.end(); it++) {
                if (it->player_num == 1) {
                    p1 = it;
                }
                else if (it->player_num == 2) {
                    AI = it;
                }
            }
            
            setting_emails(&game, player_num, input1.email);
            
            setting_up_ships_BT(&p1, &AI, single_player);

            playing_game_BT(&p1, &AI, single_player);
        }
        
        return 0;
    }
}