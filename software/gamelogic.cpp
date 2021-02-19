#include <iostream>
#include <list>
#include <string>
#include <sys/socket.h>
#include </lib/bluetooth.h> 

using namespace std;

const int NUM_OF_SHIPS = 5;
const int SUNK_STATUS_CODE = 2;
const int HIT_STATUS_CODE = 1;
const int MISS_STATUS_CODE = 0;

enum orientations {VERTICAL = 1, HORIZONTAL = 2};

struct ship {
    public: 
        box start_box;
        int size; 
        int orientation;
        bool afloat;
        int hit_count;
    
        ship() {
            this->size = 0;
            this->afloat = true;
            this->hit_count = 0;
        }
} ;

struct box {
    public:
        int x;
        int y;
        int status;

        box(int x, int y) {
            this->x = x;
            this->y = y;
        }

        box(int x, int y, int status) {
            this->x = x;
            this->y = y;
            this->status = status;
        }
} ;

struct player {
    public: 
        string player_name = ""; 
        int player_num;
        list<ship> ships_list;
        list<box> boxes_hit;
        int remaining_ships;

        player(int player_num) {
            for (int i = 0; i < NUM_OF_SHIPS; i++) {
                this->ships_list.push_back(ship());
            }
            this->player_num = player_num;
            this->remaining_ships = NUM_OF_SHIPS
        }
} ;

struct battleship {
    public: 
        list<player> players;

        battleship(int num_player) {
            for (int i = 0; i < num_player; i++) {
                this->players.push_back(player(i));
            }
        }
} ;




void declare_win (bool p1_status, bool p2_status);
int check_in_bound(int x_start, int y_start, int size, int orientation); 
int check_path_empty(int x_start, int y_start, int size, int orientation, list<ship> ships); 
int check_hit_what(int x, int y, list<ship> ships);
bool all_ships_destroyed(list<ship> ships);
bool not_hit_yet(int x, int y);
/**
 * For AI(Eleiah):
 * 1. Boxes that's already hit (probably 1 update at a time instead of putting all in memory)
 * 2. Hit status
 * 3. Remaining ships
 * 
 * 
 * For display(Mario):
 * 1. Places hit
 * 2. Hit Stuats 
 **/
int main () {
    bool all_players_joined = false;
    bool setup_finished = false;
    bool game_finished = false;

    int player_num;
    // get player_number from selecting playing mode
    player_num = 2 // default to 2 for now 
    battleship game(player_num);

    list<player>::iterator p1, p2;

    for (list<player>::iterator it = game.players.begin(); it != game.players.end(); it++) {
        if (it->player_num == 1) {
            p1 = it;
        }
        else if (it->player_num == 2) {
            p2 = it;
        }
    }

    while (!all_players_joined) {
        // name input from bluetooth
        string name = "Neo";
        list<player>::iterator player_it = game.players.begin();
        if (have_nwe_input && player_it != game.players.end() && it->player_name.empty()) {
            it->player_name = name;
            it++;
        }

        if (it == game.players.end()) {
            all_players_joined = true;
        }
    }

    list<ship>::iterator shipsp1 = p1->ships_list.begin();
    list<ship>::iterator shipsp2 = p2->ships_list.begin();

    while (!setup_finished) {
        // *** place holder for bluetooth inputs ***
        int x_in = 10;
        int y_in = 10;
        int length = 10;
        int orientation = VERTICAL;
        bool came_from_player1 = true;
        // *** place holder for bluetooht inputs ***
        
        err = check_in_bound(x_in, y_in, length, orientation);

        if (err) {
            cout << "out of bound" << endl;
        }
        
        list<player>::iterator currently_setting_up = came_from_player1 ? p1: p2;
        list<ship>::iterator *ships_being_set_up = came_from_player1 ? &shipsp1: &shipsp2;

        err = check_path_empty(x_in, y_in, length, orientation, currently_setting_up->ships_list);

        if (err) {
            cout << "another ship in the way for player 1 " << endl;
        }
        
        if ((*ships_being_set_up)->sizse == 0) {
            (*ships_being_set_up)->orientation = orientation;
            (*ships_being_set_up)->size = length;
            (*ships_being_set_up)->start_box = box(x_in, y_in);
            (*ships_being_set_up)++;
        }

        if (shipsp1 == p1->ships_list.end() && shipsp2 == p2->ships_list.end()) {
            setup_finished = true;
            cout << "Both players finished setting up their ships" << endl;
            break;
        }
    }

    while (!game_finished) {
        // *** place holder for bluetooth inputs ***
        int x_in = 10;
        int y_in = 10;
        int length = 10;
        int orientation = VERTICAL;
        bool came_from_player1 = true;
        // *** place holder for bluetooht inputs ***

        int current_attacking = came_from_player1 ? 1: 2;
        int next_up = came_from_player1 ? 2: 1;
        list<player>::iterator current_under_attack = came_from_player1 ? p2: p1;

        cout << "Next up is player " << next_up << endl;

        if (not_hit_yet(x_in, y_in, current_under_attack->boxes_hit)) {

            int status = check_hit_what(x_in, y_in, current_under_attack->ships_list);

            current_under_attack->boxes_hit.push_back(box(x_in, y_in, status));

            cout << "Your hit status is " << status << endl;

        }
        else {
            cout << "Can't hit here buddy" << endl;
        }

        if (current_under_attack->remaining_ships == 0) {
            declare_win(current_attacking);
            game_finished = true;
        }
    }

  return 0;
}

bool not_hit_yet(int x, int y, list<box> boxes) {
    for (list<box>::iterator it = boxes.begin(); it != boxes.end(); it++) {
        if (x == it->x && y == it->y) {
            return false;
        }
    }

    return true;
}

void declare_win (int player_who_won) {
    cout << "Player number " << player_who_won " won" << endl;
}

int check_in_bound(int x_start, int y_start, int size, int orientation) {
    if (x_start < 0 || y_start < 0 || x_start > 9 || y_start > 9) {
        return 1;
    }
    switch (orientation) {
        case VERTICAL:
            if (y_start + (size - 1) > 9 ) {
                return 1;
            }
            break;
        case HORIZONTAL:
            if (x_start + (size - 1) > 9 ) {
                return 1;
            }
            break;
        default:
            cout << "What else direction can you have?" << endl;
            break;
    }

    // only get here if no error
    return 0;
}
int check_path_empty(int x_start, int y_start, int size, int orientation, list<ship> ships) {
    for (list<ship>::iterator it=ships.begin(); it != ships.end(); it++ ) {
        if ( it->size != 0 ) {
            if ( orientation == VERTICAL && it->orientation == VERTICAL ) {
                // if both vertical
                if (x_start != it->start_box.x) {
                        return 0;
                }
                else {
                    // if on the same line 
                    if (y_start + sizse - 1 >= it->start_box.y) {
                       return 1;
                    }
                    else {
                        return 0;
                    }
                } 
            }
            else if ( orientation == HORIZONTAL && it->orientation == HORIZONTAL ) {
                // if both horizontal
                if (y_start != it->start_box.y) {
                    return 0;
                }
                else {
                    // if on the same line 
                    if (x_start + sizse - 1 >= it->start_box.x) {
                        return 1;
                    }
                    else {
                        return 0;
                    }
                }
            }
            else if ( orientation == HORIZONTAL && it->orientation == VERTICAL ) {
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < it->size; j++) {
                        if ( it->start_box.y + j == y_start && it->start_box.x == x_start + i) {
                            return 1;
                        }
                    }
                }
            }
            else if ( orientation == VERTICAL && it->orientation == HORIZONTAL ) {
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < it->size; j++) {
                        if ( it->start_box.x + j == x_start && it->start_box.y == y_start + i) {
                            return 1;
                        }
                    }
                }
            }
        }
    }

    // reaches here if nothing is wrong
    return 0;
}

bool contains_box(ship ship, int x, int y) {
    if (ship.orientation == VERTICAL) {
        if (x != ship.start_box.x) {
            return false;
        }
        else {
            if (y >= ship.start_box.y && y <= ship.start_box.y + ship.size) {
                ship.hit_count++;
                return true;
            }
            return false;
        }
    }
    else { // if HORINZONTAL 
        if (y != ship.start_box.y) {
            return false;
        }
        else {
            if (x >= ship.start_box.x && x <= ship.start_box.x + ship.size) {
                ship.hit_count++;
                return true;
            }
            return false;
        }
    }
}
int check_hit_what(int x, int y, list<ship> ships, int *remaining_ships) {
    /*
    return 0 for miss 
    return 1 for hit a box of a ship
    return 2 if the entire ship is sunk
    -> Decrements remaining count each time a sunk appears 
    */
   

   for (list<ship>::iterator it = ships.begin(); it != ships.end(); it++) {
       if ( contains_box(it, x, y) ) {
           if (it->hit_count == it->size) {
               return SUNK_STATUS_CODE;
           }
           else {
               return HIT_STATUS_CODE;
           }
       }
   }

    // if all ships are a miss
    return MISS_STATUS_CODE;
}

