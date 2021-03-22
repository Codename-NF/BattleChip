#include <string>
#include "structs.h"

string ask_for_name();

setupvalues ask_for_setup(int player_num);

shootvalues ask_for_shoot(int player_num);

void get_placement_message_BT(list<setupvalues> *list_setupval, string inputstring, int device_num);

shootvalues get_shoot_message_BT(int device_num);

void send_game_start_status_BT(bool start1);

void send_result_message_BT(int device_num);

void send_targeted_message_BT(int device_num, int x, int y, int gamestatus, int shipstatus);

void send_targeted_message_BT(int device_num, int x, int y, int gamestatus, int shipstatus, int destroyed_start_x, int destroyed_start_y, int length, int orientation);

createmessage get_create_message_BT(int device_num); 

void send_create_message_response_BT(int device_num, int num_players, int status);

void send_ready_messaeg_BT();
