#include <string>
#include "structs.h"

string ask_for_name();

setupvalues ask_for_setup(int player_num);

shootvalues ask_for_shoot(int player_num);

void get_input_UI(list<setupvalues> *list_setupval, string inputstring, int device_num);