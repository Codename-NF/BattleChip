#include <iostream>
#include <list>
#include <set>
#include <string>
#include "constants.h"
#include "structs.h"
#include <bitset> 


/**
 * CLI use only
 * prints out player who won
 * parameters: 
 *      player_who_won: player number who won
 * return: 
**/
void declare_win (int player_who_won);

/**
 * determine if a ship placement is out of bound
 * parameters: 
 *      x_start: x coordinate of the start of the ship
 *      y_start: y coordinate of the start of the ship
 *      size: size of the ship
 *      orientation: orientation of the ship
 * return: 
 *      true if it is out of bound
 *      false if not
**/
bool out_of_bound(int x_start, int y_start, int size, int orientation); 


/**
 * determine if there are other box in the path of this ship
 * parameters: 
 *      x_start: x coordinate of the start of the ship
 *      y_start: y coordinate of the start of the ship
 *      all_boxes_on_board: all the boxes that's on the board
 * return: 
 *      true if it is empty
 *      false if not
**/
bool path_empty(int x_start, int y_start, set<box> all_boxes_on_board);


/**
 * determine if a ship contain this box
 * parameters: 
 *      *ship: reference of a ship
 *      x: x coordinate of the box
 *      y: y coordinate of the box
 * return: 
 *      true if the ship contains this box
 *      false if not
**/
bool contains_box(ship *ship, int x, int y);


/**
 * determine the hit status of a firing shot
 * parameters: 
 *      x: x coordinate of the shot
 *      y: y coordinate of the shot
 *      *ships: refrenece to a list of ships of the player
 *      *remaining_ships: reference to the remaining ships count of the player
 *      *ships_alive: reference to the 5 bit one hot representation of which ships are alive
 * return: 
 *      0 on miss
 *      1 on hit
 *      2 on sunk
**/
int check_hit_what(int x, int y, list<ship> *ships, int *remaining_ships, bitset<5> *ships_alive);


/**
 * determine if a box has been hit or not
 * parameters:
 *      x: x coordinate of the box
 *      y: y coordinate of the box
 *      boxes: all the boxes that has been hit 
 * return: 
 *      true if it has not
 *      false if yes
**/
bool not_hit_yet(int x, int y, set<box> boxes);


/**
 * changes the status of all the boxes of the ship that has been hit to sunk status code when a ship has been sunk
 * parameters:
 *      x: x coordinate of the box
 *      y: y coordinate of the box
 *      *boxes_hit: reference to all the boxes that has been hit 
 *      *ships: reference to the list of ships
 * return: 
 *      ship struct that the ship has been sunk
**/
ship change_status_box_all_boxes(int x, int y, set<box> *boxes_hit, list<ship> *ships);


/**
 * for AI algorithm
 * creates all the shots that is an active hit
 * parameters:
 *      *all_boxes_hit: reference to all the boxes that has been hit 
 *      *hits: shots that are active hits
 * return: 
**/
void create_hits_for_AI(set<box> *all_boxes_hit, set<box> *hits);


/**
 * for AI algorithm
 * creates all the shots that is either miss or sunk
 * parameters:
 *      *all_boxes_hit: reference to all the boxes that has been hit 
 *      *fired: shots that are either missed or sunk
 * return: 
**/
void create_fired_for_AI(set<box> *all_boxes_hit, set<box> *fired);


/**
 * for POST records after game finished
 * get the score of the players 
 * parameters:
 *      all_boxes_hit: all the boxes that has been hit
 * return:
 *      score of the player
**/
int get_score(set<box> all_boxes_hit);
