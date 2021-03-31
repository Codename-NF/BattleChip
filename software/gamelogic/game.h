#include <iostream>
#include <list>
#include <set>
#include <string>

void getting_names(battleship *game, int player_num);
void setting_up_ships(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);
void playing_game(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);
void AI_setting_up(list<player>::iterator *AI);

createmessage create_lobby();
int wait_for_player2();
void reject_player2();
void setting_player_id(battleship *game, int player_num, int player1_id);
void setting_player_id(battleship *game, int player_num, int player1_id, int player2_id);
void assign_ship(list<player>::iterator *player, list<setupvalues>::iterator it);
void setting_up_ships_BT(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);
void playing_game_BT(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);

