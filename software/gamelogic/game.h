#include <iostream>
#include <list>
#include <set>
#include <string>


/**
 * CLI use only
 * get the names of each players and set them 
 * parameters: 
 *      *game: reference to the battleship struct to modify
 *      player_num: the number of players needed to modify
 * return: 
**/
void getting_names(battleship *game, int player_num);


/**
 * CLI use only
 * get and set up ships accordingly 
 * parameters: 
 *      *p1: reference to the iterator of player 1 
 *      *p2: reference to the iterator of player 2
 *      single_player_mode: to indicate if it's single player 
 * return: 
**/
void setting_up_ships(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);


/**
 * CLI use only
 * play the game
 * parameters: 
 *      *p1: reference to the iterator of player 1 
 *      *p2: reference to the iterator of player 2
 *      single_player_mode: to indicate if it's single player 
 * return: 
**/
void playing_game(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);


/**
 * setting up ships for AI
 * parameters: 
 *      *AI: reference to the iterator of player 2 (AI)
 * return: 
**/
void AI_setting_up(list<player>::iterator *AI);


/**
 * Creates lobby
 * parameters: 
 * return: 
 *      createmessage indicating playing mode and user id
**/
createmessage create_lobby();


/**
 * When multi-player mode is selected, wait for another player to join
 * parameters: 
 * return: 
 *      int indicating the user id of the joined player
**/
int wait_for_player2();


/**
 * Sends reject message when player tries to join without a lobby 
 * parameters: 
 * return: 
**/
void reject_player2();


/**
 * Sets player id in the player struct
 * parameters: 
 *      *p1: reference to player 1
 *      *p2: reference to player 2
 *      player1_id: id of player 1
 *      player2_id: id of player 2
 * return: 
**/
void setting_player_id(list<player>::iterator *p1, list<player>::iterator *p2, int player1_id, int player2_id);


/**
 * Helper function to assign ships for to a player given a ship
 * parameters: 
 *      *player: reference to a player to modify
 *      it: iterator to the setupvalues in a list
 * return: 
**/
void assign_ships(list<player>::iterator *player, list<setupvalues> values);


/**
 * Called by main function to set up ships for both players 
 * parameters: 
 *      *p1: reference to player 1
 *      *p2: reference to player 2
 *      single_player_mode: indicating if it is single player mode 
 * return: 
**/
void setting_up_ships_BT(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode);


/**
 * Called by main function to play game
 * parameters: 
 *      *p1: reference to player 1
 *      *p2: reference to player 2
 *      single_player_mode: indicating if it is single player mode 
 * return: 
**/
void playing_game_BT(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode, int mode);

