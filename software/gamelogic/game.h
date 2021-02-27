#include <iostream>
#include <list>
#include <set>
#include <string>
#include <sys/socket.h>

void getting_names(battleship *game, int player_num);
void setting_up_ships(list<player>::iterator *p1, list<player>::iterator *p2);
void playing_game(list<player>::iterator *p1, list<player>::iterator *p2);