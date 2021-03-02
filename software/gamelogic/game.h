#include <iostream>
#include <list>
#include <set>
#include <string>
#include <sys/socket.h>

void getting_names(battleship *game, int player_num);
void setting_up_ships(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);
void playing_game(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);
void AI_setting_up(list<player>::iterator *AI);