#include <string>
#include "structs.h"

/**
 * CLI use only
 * prompts the player for the name
 * parameters: none
 * return: a string representing player's name
 * 
**/
string ask_for_name();


/**
 * CLI use only
 * prompts the player to enter where to set up ships
 * parameters: which player to ask
 * return: setupvalues struct
**/
setupvalues ask_for_setup(int player_num);


/**
 * CLI use only
 * prompts the player to enter where to shoot
 * parameters: which player to ask
 * return: shootvalues struct
**/
shootvalues ask_for_shoot(int player_num);


/**
 * Bluetooth use
 * get all setupvalues from UI through bluetooth firmware
 * parameters: 
 *      *list_setupval: reference to a list of setupvalues to modify
 *      device_num: which device to get from    
 * return: 
 *      1 on SUCCESS
 *      0 on FAILURE (nothing is there to receive)
**/
int get_placement_message_BT(list<setupvalues> *list_setupval, int device_num);


/**
 * Bluetooth use
 * get shootvalues from UI through bluetooth firmware
 * parameters: 
 *      *input: reference to a shootvalue struct to modify
 *      device_num: which device to get from    
 * return: 
 *      1 on SUCCESS
 *      0 on FAILURE (nothing is there to receive)
**/
int get_shoot_message_BT(shootvalues *input, int device_num);


/**
 * Bluetooth use
 * sends game start message to UI through bluetooth firmware
 * parameters: 
 *      start1: whether or not player 1 is starting first 
 *      single_player_mode: whether or not it is single player
 * return: 
**/
void send_game_start_status_BT(bool start1, bool single_player_mode);


/**
 * Bluetooth use
 * sends result message to shooting player through bluetooth firmware
 * parameters: 
 *      device_num: device which is just shooting
 *      x: x coordinate shooting on
 *      y: y coordinate shooting on
 *      gamestatus: if it resulted game over
 *      hitstatus: the hit status of the hit (MISS, HIT, SUNK)
 * return: 
**/
void send_result_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus, bool single_player_mode);


/**
 * Bluetooth use
 * sends result message to shooting player through bluetooth firmware (when a ship is sunk)
 * parameters: 
 *      device_num: device that is just shooting
 *      x: x coordinate shooting on
 *      y: y coordinate shooting on
 *      gamestatus: if it resulted game over
 *      hitstatus: the hit status of the hit (MISS, HIT, SUNK)
 *      destroyed_start_x: x coordinate of the start of ship sunk
 *      destroyed_start_y: y coordinate of the start of ship sunk
 *      length: length of the ship sunk
 *      orientation: orientation of the ship sunk
 * return: 
**/
void send_result_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus, int destroyed_start_x, int destroyed_start_y, int length, int orientation, bool single_player_mode);


/**
 * Bluetooth use
 * sends targeted message to player that got attacked through bluetooth firmware
 * parameters: 
 *      device_num: device that got shot on
 *      x: x coordinate shot on
 *      y: y coordinate shot on
 *      gamestatus: if it resulted game over
 *      hitstatus: the hit status of the hit (MISS, HIT, SUNK)
 * return: 
**/
void send_targeted_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus, bool single_player_mode);


/**
 * Bluetooth use
 * sends targeted message to player that got attacked through bluetooth firmware when a ship is sunk
 * parameters: 
 *      device_num: device that got shot on
 *      x: x coordinate shot on
 *      y: y coordinate shot on
 *      gamestatus: if it resulted game over
 *      hitstatus: the hit status of the hit (MISS, HIT, SUNK)
 *      destroyed_start_x: x coordinate of the start of ship sunk
 *      destroyed_start_y: y coordinate of the start of ship sunk
 *      length: length of the ship sunk
 *      orientation: orientation of the ship sunk
 * return: 
**/
void send_targeted_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus, int destroyed_start_x, int destroyed_start_y, int length, int orientation, bool single_player_mode);


/**
 * Bluetooth use
 * get create message from UI through bluetooth firmware
 * parameters: 
 *      *msg: reference to a create message to modify
 * return: 
 *      1 on SUCCESS
 *      0 on FAILURE (nothing is there to receive)
**/
int get_create_message_BT(createmessage *msg); 


/**
 * Bluetooth use
 * sends create respone to UI through bluetooth firmware
 * parameters: 
 *      num_players: number of players the lobby is created for
 *      status: 1 for success and 0 for failure (game in progress) 
 * return: 
**/
void send_create_response_BT(int num_players, int status);


/**
 * Bluetooth use
 * get create message from UI through bluetooth firmware
 * parameters: 
 *      *msg: reference to a create message to modify
 * return: 
 *      user id on SUCCESS
 *      -1 on FAILURE (nothing is there to receive)
**/
int get_join_message_BT();


/**
 * Bluetooth use
 * sends join respone to UI through bluetooth firmware
 * parameters: 
 *      status: 1 for success and 0 for failure (no lobby) 
 * return: 
**/
void send_join_reponse_BT(int status);


/**
 * Bluetooth use
 * sends ready message to both players through bluetooth firmware
 * parameters: 
 * return: 
**/
void send_ready_message_BT();


/**
 * Bluetooth use
 * sends forfiet message to winning player indicating the other person has forfeited through bluetooth firmware
 * parameters: 
 *      device_num: the device which won by default
 * return: 
**/
void send_win_by_forfeit_BT(int device_num);

int get_magic_number_AI(set<box> boxes_hit, bitset<5> ships_alive, set<box> shots_with_ships);
