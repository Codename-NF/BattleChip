#include <iostream>
#include "inputs.h"
#include <string>
#include <set> 
#include <list>
#include "structs.h"
#include "game.h"
#include "utils.h"
#include "constants.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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

void setting_up_ships(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode) {
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
            if (!single_player_mode) {
                inputs = ask_for_setup(2);
            }
            else {
                cout << "Player 1 done setting up ships." << endl;
                return;
            }
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

void playing_game(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode) {
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
            if (!single_player_mode) {
                inputs = ask_for_shoot(2);
                start1 = !start1;
            }
            else {
                // Get input from HARDWARE AI algorithm 

                // for now be random shoots
                /* initialize random seed: */
                cout << "AI to shoot......." << endl;
                srand (time(0));
                inputs.x = rand() % 10;
                inputs.y = rand() % 10;
                inputs.device_num = 2;

                cout << "AI shoots at (" << inputs.x << "," << inputs.y << ")" << endl;
                start1 = !start1;
            }
            
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

            if (status == SUNK_STATUS_CODE) {
                // need to go through all boxes with that ship
                change_status_box_all_boxes(x_in, y_in, &(current_under_attack->boxes_hit), &(current_under_attack->ships_list));
            }

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


void AI_setting_up(list<player>::iterator *AI) {
    list<ship>::iterator ships_being_set_up = (*AI)->ships_list.begin();
    (*AI)->player_name = "Milo";

    cout << "AI name is " << (*AI)->player_name << endl;

    // setting up ships 
    // keep getting random place until it works? 
    // 5 ships, 2 3 3 4 5 
    int ship_sizes[5]= {2, 3, 3, 4, 5};

    int i = 0;
    while (i < NUM_OF_SHIPS) {
        srand (time(0));
        int x_in = rand() % 10;
        int y_in = rand() % 10;
        int orientation = (rand() % 2) + 1;
        int length = ship_sizes[i];
        if (out_of_bound(x_in, y_in, length, orientation)) {
            cout << "out of bound" << endl;
            continue;
        }

        if (!path_empty(x_in, y_in, (*AI)->all_boxes_on_board)) {
            cout << "another ship in the way for " << (*AI)->player_name << endl;
            continue;
        }

        // setting the ship
        if (ships_being_set_up->size == 0) {
            ships_being_set_up->orientation = orientation;
            ships_being_set_up->size = length;
            ships_being_set_up->start_box = box(x_in, y_in);
            ships_being_set_up++;
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
            (*AI)->all_boxes_on_board.insert(box(x_in + (offset_x * i), y_in + (offset_y * i)));
        }

        i++;
    }

}