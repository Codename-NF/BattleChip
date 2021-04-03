#include "utils.h"
#include "constants.h"
#include "structs.h"
#include "inputs.h"
#include "game.h"
//#include "RS232.h"
#include "Graphics.h"
#include "Wifi.h"

int main () {
    while (1) {
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
                    if (it->player_num == PLAYER1) {
                        p1 = it;
                    }
                    else if (it->player_num == PLAYER2) {
                        p2 = it;
                    }
                }
                
                getting_names(&game, player_num);
                
                setting_up_ships(&p1, &p2, single_player);

                playing_game(&p1, &p2, single_player);
            }
            else {
                // if player_num == 1 -> plyaing with AI
                single_player = true;
                battleship game = battleship(2);
                list<player>::iterator p1, AI;
                for (list<player>::iterator it = game.players.begin(); it != game.players.end(); it++) {
                    if (it->player_num == PLAYER1) {
                        p1 = it;
                    }
                    else if (it->player_num == PLAYER2) {
                        AI = it;
                    }
                }
                
                getting_names(&game, player_num);
                
                setting_up_ships(&p1, &AI, single_player);

                AI_setting_up(&AI);

                playing_game(&p1, &AI, single_player);
            }
        }
        else { // dealing with UI / BT
            cout << "i'm here" << endl;
            Init_RS232();
            Init_RS232_WIFI();
            cout << "past init" << endl;
            int num_players;
            bool single_player = false;
            createmessage input1 = create_lobby();
            blankscreen( BLUE );
            num_players = input1.numplayer;

            if (num_players == 2) {
                int input2 = wait_for_player2();

                battleship game = battleship(num_players);
                list<player>::iterator p1, p2;
                for (list<player>::iterator it = game.players.begin(); it != game.players.end(); it++) {
                    if (it->player_num == PLAYER1) {
                        p1 = it;
                    }
                    else if (it->player_num == PLAYER2) {
                        p2 = it;
                    }
                }
                
                setting_player_id(&p1, &p2, input1.playerid, input2);
                
                send_ready_message_BT();
                
                setting_up_ships_BT(&p1, &p2, single_player);


                playing_game_BT(&p1, &p2, single_player);
            }
            else {
                // if player_num == 1 -> plyaing with AI
                //reject_player2();
                single_player = true;
                battleship game = battleship(2);
                list<player>::iterator p1, AI;
                for (list<player>::iterator it = game.players.begin(); it != game.players.end(); it++) {
                    if (it->player_num == PLAYER1) {
                        p1 = it;
                    }
                    else if (it->player_num == PLAYER2) {
                        AI = it;
                    }
                }
                
                setting_player_id(&p1, &AI, input1.playerid, AI_PLAYER_ID);
                
                setting_up_ships_BT(&p1, &AI, single_player);

                playing_game_BT(&p1, &AI, single_player);
            }
        }
    }
    return 0;
}
