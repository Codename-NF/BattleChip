#include <iostream>
#include "inputs.h"
#include <string>
#include <set> 
#include <list>
#include "structs.h"
#include "game.h"
#include "utils.h"
#include "constants.h"

using namespace std;

void getting_names(battleship *game, int player_num) {
    
    bool all_players_joined = false;

    list<player>::iterator player_it = (*game).players.begin();
    int count = 0;
    while (!all_players_joined) {
// ******************* bluetooth place holder *******************
        string name = ask_for_name();
// ******************* bluetooth place holder *******************
        if (player_it->player_name.empty()) {
            player_it->player_name = name;
            player_it++;
            count++;
        }
        if (count == player_num) {
            all_players_joined = true;
        }
    }
}

void setting_up_ships(list<player>::iterator *p1, list<player>::iterator *p2) {
    bool setup_finished = false;

    list<ship>::iterator shipsp1 = (*p1)->ships_list.begin();
    list<ship>::iterator shipsp2 = (*p2)->ships_list.begin();

    int p1count = 0;
    while (!setup_finished) {
// ******************* bluetooth place holder *******************
        setupvalues inputs = setupvalues(-1, -1, -1, -1, -1);
        if (p1count < NUM_OF_SHIPS) {
            inputs = ask_for_setup(1);
            p1count++;
        }
        else {
            inputs = ask_for_setup(2);
        }
        int x_in = inputs.x;
        int y_in = inputs.y;
        int length = inputs.size;
        int orientation = inputs.orientation;
        bool came_from_player1;
        if (inputs.device_num == 1) {
            came_from_player1 = true;
        }
        else {
            came_from_player1 = false;
        }
// ******************* bluetooth place holder *******************
        if (out_of_bound(x_in, y_in, length, orientation)) {
            cout << "out of bound" << endl;
            p1count--;
            continue;
        }
        
        
        list<player>::iterator *currently_setting_up = came_from_player1 ? p1: p2;
        list<ship>::iterator *ships_being_set_up = came_from_player1 ? &shipsp1: &shipsp2;

        if (!path_empty(x_in, y_in, (*currently_setting_up)->all_boxes_on_board)) {
            cout << "another ship in the way for " << (*currently_setting_up)->player_name << endl;
            p1count--;
            continue;
        }
        
        // setting the ship
        if ((*ships_being_set_up)->size == 0) {
            (*ships_being_set_up)->orientation = orientation;
            (*ships_being_set_up)->size = length;
            (*ships_being_set_up)->start_box = box(x_in, y_in);
            (*ships_being_set_up)++;
        }

        int offset_x, offset_y;
        if (orientation == VERTICAL) {
            offset_x = 0;
            offset_y = 1;
        }
        else {
            offset_x = 1;
            offset_y = 0;
        }

        // adding all the boxes into the set
        for (int i = 0; i < length; i++) {
            (*currently_setting_up)->all_boxes_on_board.insert(box(x_in + (offset_x * i), y_in + (offset_y * i)));
        }

        if (shipsp1 == (*p1)->ships_list.end() && shipsp2 == (*p2)->ships_list.end()) {
            setup_finished = true;
            cout << "Both players finished setting up their ships" << endl;
            break;
        }
    }

}

void playing_game(list<player>::iterator *p1, list<player>::iterator *p2) {
    bool game_finished = false;

    bool start1 = true;
    while (!game_finished) {
// ******************* bluetooth place holder *******************
        shootvalues inputs = shootvalues(-1, -1, -1);
        if (start1) {
            inputs = ask_for_shoot(1);
            start1 = !start1;
        }
        else {
            inputs = ask_for_shoot(2);
            start1 = !start1;
        }
        
        int x_in = inputs.x;
        int y_in = inputs.y;
        bool came_from_player1;
        if (inputs.device_num == 1) {
            came_from_player1 = true;
        }
        else {
            came_from_player1 = false;
        }
// ******************* bluetooth place holder *******************

        int current_attacking = came_from_player1 ? 1: 2;
        int next_up = came_from_player1 ? 2: 1;
        list<player>::iterator current_under_attack = came_from_player1 ? *p2: *p1;

        

        if (not_hit_yet(x_in, y_in, current_under_attack->boxes_hit)) {

            int status = check_hit_what(x_in, y_in, &(current_under_attack->ships_list), &(current_under_attack->remaining_ships));

            current_under_attack->boxes_hit.insert(box(x_in, y_in, status));

            cout << "Your hit status is " << status << endl;

        }
        else {
            cout << "Can't hit here buddy. Please try again." << endl;
            start1 = !start1;
            continue;
        }

        if (current_under_attack->remaining_ships == 0) {
            declare_win(current_attacking);
            game_finished = true;
        }
        else {
            cout << "Next up is player " << next_up << endl;
        }
    }

}