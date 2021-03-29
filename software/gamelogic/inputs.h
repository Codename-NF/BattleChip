#include <string>
#include "structs.h"

string ask_for_name();

setupvalues ask_for_setup(int player_num);

shootvalues ask_for_shoot(int player_num);

int get_placement_message_BT(list<setupvalues> *list_setupval, int device_num);

shootvalues get_shoot_message_BT(int device_num);

void send_game_start_status_BT(bool start1);

void send_result_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus);

void send_result_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus, int destroyed_start_x, int destroyed_start_y, int length, int orientation);

void send_targeted_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus);

void send_targeted_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus, int destroyed_start_x, int destroyed_start_y, int length, int orientation);

createmessage get_create_message_BT(); 

void send_create_response_BT(int num_players, int status);

string get_join_message_BT();

void send_join_reponse_BT(int status);

void send_ready_messaeg_BT();
