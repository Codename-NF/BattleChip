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
#include "Graphics.h"
#include "Wifi.h"

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
        if (inputs.device_num == PLAYER1) {
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

        if (!path_empty(x_in, y_in, length, orientation, (*currently_setting_up)->all_boxes_on_board)) {
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
                inputs.x = rand() % (BOUNDARY_MAX + 1);
                inputs.y = rand() % (BOUNDARY_MAX + 1);
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
        if (x_in < BOUNDARY_MIN || y_in < BOUNDARY_MIN || x_in > BOUNDARY_MAX || y_in > BOUNDARY_MAX) {
            cout << "Can't hit here buddy. Please try again." << endl;
            start1 = !start1;
            continue;
        }
        int current_attacking = came_from_player1 ? 1: 2;
        int next_up = came_from_player1 ? 2: 1;
        list<player>::iterator current_under_attack = came_from_player1 ? *p2: *p1;

        

        if (not_hit_yet(x_in, y_in, current_under_attack->boxes_hit)) {

            int status = check_hit_what(x_in, y_in, &(current_under_attack->ships_list), &(current_under_attack->remaining_ships), &(current_under_attack->ships_alive));

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

    // setting up ships 
    // keep getting random place until it works? 
    // 5 ships, 2 3 3 4 5 
    int ship_sizes[5]= {2, 3, 3, 4, 5};

    int i = 0;
    while (i < NUM_OF_SHIPS) {
        srand (time(0));
        int x_in = rand() % (BOUNDARY_MAX + 1);
        int y_in = rand() % (BOUNDARY_MAX + 1);
        int orientation = (rand() % 2) + 1;
        int length = ship_sizes[i];
        if (out_of_bound(x_in, y_in, length, orientation)) {
            //cout << "out of bound" << endl;
            continue;
        }

        if (!path_empty(x_in, y_in, length, orientation, (*AI)->all_boxes_on_board)) {
            //cout << "another ship in the way for " << (*AI)->player_name << endl;
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
        for (int j = 0; j < length; j++) {
            (*AI)->all_boxes_on_board.insert(box(x_in + (offset_x * j), y_in + (offset_y * j)));
        }

        i++;
        cout << "(" << x_in << " " << y_in << " " << length << " " << orientation << ")" << endl;
    }

}

createmessage create_lobby() {
    createmessage input1;
    int got_msg = 0;
    do {
        got_msg = get_create_message_BT(&input1);
        // int input2 = get_join_message_BT();
        // if (input2 != -1) {
        //     send_join_reponse_BT(FAILURE);
        // }
    } while (!got_msg);
    

    if (input1.keywrod == 'c') {
        send_create_response_BT(input1.playing_mode, SUCCESS);
    }
    else {
        send_create_response_BT(input1.playing_mode, FAILURE);
    }

    return input1;
}

int wait_for_player2() {
    int input2;

    do {
        input2 = get_join_message_BT();
    }
    while (input2 == -1);

    send_join_reponse_BT(SUCCESS);
    return input2;
}

void reject_player2() {
    int input2 = get_join_message_BT();
    if (input2 != -1) {
        send_join_reponse_BT(FAILURE);
    }
}


void setting_player_id(list<player>::iterator *p1, list<player>::iterator *p2, int player1_id, int player2_id) {
    (*p1)->player_id = player1_id;
    (*p2)->player_id = player2_id;
}

void assign_ships(list<player>::iterator *player, list<setupvalues> values) {
    list<ship>::iterator ships_being_set_up = (*player)->ships_list.begin();
    int x_in;
    int y_in;
    int length;
    int orientation;

    for (list<setupvalues>::iterator it = values.begin(); it != values.end(); it++) {
        x_in = it->x;
        y_in = it->y;
        length = it->size;
        orientation = it->orientation;

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
            cout << "inserting (" << x_in+(offset_x*i) << ", " << y_in + (offset_y*i) << ")" << endl;
            (*player)->all_boxes_on_board.insert(box(x_in + (offset_x * i), y_in + (offset_y * i)));
        }
        for (set<box>::iterator it = (*player)->all_boxes_on_board.begin(); it != (*player)->all_boxes_on_board.end(); it++) {
            cout << "All the boxes I have: \n";
            cout << "(" << it->x << ", " << it->y << ") \n";
        }

    }
    
}

void setting_up_ships_BT(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode) {
    list<setupvalues> list_of_placement;
    int success = 0;
    do {
        success = get_placement_message_BT(&list_of_placement, PLAYER1);
    }
    while (!success);

    assign_ships(p1, list_of_placement);
    
    if (single_player_mode) {
        AI_setting_up(p2);
        return;
    }

    success = 0;

    do {
        success = get_placement_message_BT(&list_of_placement, PLAYER2);
    }
    while (!success);
    
    assign_ships(p2, list_of_placement);

}

void reveal_ships(list<player>::iterator *p1, list<player>::iterator *p2) {
    // reveal player 1
    for (set<box>::iterator it = (*p1)->all_boxes_on_board.begin(); it != (*p1)->all_boxes_on_board.end(); it++) {
        set<box>::iterator element = (*p1)->boxes_hit.find(box(it->x, it->y));
        if ( element == (*p1)->boxes_hit.end()) {
            // if it has not been hit
            squaremapper(it->x, it->y, PLAYER1, LIME);
        }
    }

    // reveal player 2
    for (set<box>::iterator it = (*p2)->all_boxes_on_board.begin(); it != (*p2)->all_boxes_on_board.end(); it++) {
        set<box>::iterator element = (*p2)->boxes_hit.find(box(it->x, it->y));
        if ( element == (*p2)->boxes_hit.end()) {
            // if it has not been hit
            squaremapper(it->x, it->y, PLAYER2, LIME);
        }
    }
}


void playing_game_BT(list<player>::iterator *p1, list<player>::iterator *p2, bool single_player_mode, int mode) {
    bool game_finished = false;

    bool turn_1 = true;
    send_game_start_status_BT(turn_1, single_player_mode);

    while (!game_finished) {
        shootvalues inputs;
        int got_message = 0;
        if (turn_1) {
            do {
                got_message = get_shoot_message_BT(&inputs, PLAYER1);
            } while (!got_message);
            turn_1 = false;
        }
        else {
            if (!single_player_mode) {
                do {
                    got_message = get_shoot_message_BT(&inputs, PLAYER2);
                } while (!got_message);
                turn_1 = true;
            }
            else {
                // Get input from HARDWARE AI algorithm
                int magic_number;
                if (mode = EASY_AI_MODE) {
                    srand (time(0));
                    magic_number = rand() % 100;
                }
                else if (mode == HARD_AI_MODE) {
                    set<box> fired;
                    set<box> hits;
                    create_fired_for_AI(&((*p1)->boxes_hit), &fired);
                    create_hits_for_AI(&((*p1)->boxes_hit), &hits);
                    magic_number = where_to_shoot_AI(fired, (*p1)->ships_alive, hits);
                }
                
                inputs.x = magic_number % 10;
                inputs.y = magic_number / 10;
                inputs.device_num = 2;
                turn_1 = true;
            }
            
        }

        // check for forfeit 
        if (inputs.p1_forfeit) {
            game_finished = true;
            send_win_by_forfeit_BT(PLAYER2);
            int score1 = get_score((*p2)->boxes_hit);
            int score2 = get_score((*p1)->boxes_hit);
            displaywinner(PLAYER2);
            postgameresults((*p1)->player_id, (*p2)->player_id, (*p2)->player_id, score1, score2);
            break;
        }
        if (inputs.p2_forfeit) {
            game_finished = true;
            send_win_by_forfeit_BT(PLAYER1);
            int score1 = get_score((*p2)->boxes_hit);
            int score2 = get_score((*p1)->boxes_hit);
            displaywinner(PLAYER1);
            postgameresults((*p1)->player_id, (*p2)->player_id, (*p1)->player_id, score1, score2);
            break;
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
        
        int current_attacking = came_from_player1 ? PLAYER1: PLAYER2;
        int next_up = came_from_player1 ? PLAYER2: PLAYER1;
        list<player>::iterator current_under_attack = came_from_player1 ? *p2: *p1;
        int status;
        
        if (not_hit_yet(x_in, y_in, current_under_attack->boxes_hit)) {

            status = check_hit_what(x_in, y_in, &(current_under_attack->ships_list), &(current_under_attack->remaining_ships), &(current_under_attack->ships_alive));

            current_under_attack->boxes_hit.insert(box(x_in, y_in, status));

            if (status == SUNK_STATUS_CODE) {
                // need to go through all boxes with that ship
                ship sunk_ship = change_status_box_all_boxes(x_in, y_in, &(current_under_attack->boxes_hit), &(current_under_attack->ships_list));

                // only if it's sunk it would be a possibly of gameover 
                if (current_under_attack->remaining_ships == 0) {
                    game_finished = true;
                    int player1_score = get_score((*p2)->boxes_hit);
                    int player2_score = get_score((*p1)->boxes_hit);
                    int winnerid;
                    if (current_attacking == 1) {
                        winnerid = (*p1)->player_id;
                    }
                    else {
                        winnerid = (*p2)->player_id;
                    }
                    postgameresults((*p1)->player_id, (*p2)->player_id, winnerid, player1_score, player2_score);
                }
                send_result_message_BT(current_attacking, x_in, y_in, game_finished, status, sunk_ship.start_box.x, sunk_ship.start_box.y, sunk_ship.size, sunk_ship.orientation, single_player_mode);
                send_targeted_message_BT(next_up, x_in, y_in, game_finished, status, sunk_ship.start_box.x, sunk_ship.start_box.y, sunk_ship.size, sunk_ship.orientation, single_player_mode);
                squaremapper(x_in, y_in, next_up, HIT_COLOR);
                squaremappership(next_up, sunk_ship.start_box.x, sunk_ship.start_box.y, sunk_ship.size, sunk_ship.orientation, game_finished, SUNK_CROSS_COLOR);
                continue;
            }
            send_result_message_BT(current_attacking, x_in, y_in, game_finished, status, single_player_mode);
            send_targeted_message_BT(next_up, x_in, y_in, game_finished, status, single_player_mode);
            if (status == HIT_STATUS_CODE) {
                squaremapper(x_in, y_in, next_up, HIT_COLOR);
            }
            else {
                squaremapper(x_in, y_in, next_up, MISS_COLOR);
            }
            

        }

        
    }
    // reavl ships when game finished
    reveal_ships(p1, p2);
}
