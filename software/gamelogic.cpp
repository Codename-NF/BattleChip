#include <iostream>
#include <list>
#include <string>
#include <sys/socket.h>
#include </lib/bluetooth.h> 

using namespace std;

const int NUM_OF_SHIPS = 5;

struct ship {
    public: 
        box start_box;
        int size; 
        string orientation = "";

        bool afloat;

        int hit_count;
    
        ship() {
            afloat = true;
            hit_count = 0;
        }
} ;

struct box {
    public:
        int x;
        int y;

        box(int x, int y) {
            x = x;
            y = y;
        }
} ;

struct player {
    public: 

        string player_name = ""; 

        int player_num;

        list<ship> ships_list;

        list<box> boxes_hit;

        player(int player_num) {

            for (int i = 0; i < NUM_OF_SHIPS; i++) {
                ships_list.push_back(ship());
            }
            player_num = player_num;

        }
} ;

struct battleship {
    public: 
        list<player> players;

        battleship(int num_player) {

            for (int i = 0; i < num_player; i++) {
                players.push_back(player(i));
            }
        }

} ;




void declare_win (bool p1_status, bool p2_status);
int check_in_bound(int x_start, int y_start, int size, string orientation); 
int check_path_empty(int x_start, int y_start, int size, string orientation, list<ship> ships); 
int check_hit_what(int x, int y, list<ship> ships);
bool all_ships_destroyed(list<ship> ships);

int main () {


    bool all_players_joined = false;
    bool setup_finished = false;
    bool game_finished = false;

    int p1_remaining_ships = NUM_OF_SHIPS;
    int p2_remaining_ships = NUM_OF_SHIPS;

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

        if (it == gmae.players.end()) {
            all_players_joined = true;
        }
    }

    while (!setup_finished) {        

        // check setup bounds 
        // wait for inputs (x and y coordinates and length and orientation )
        
        // check if all points is in bound 
        err = check_in_bound(x_start_in, y_start_in, length, orientation);

        if (err) {
            cout << "out of bound";
        }
        

        list<ship>::iterator shipsp1 = p1->ships_list.begin();
        list<ship>::iterator shipsp2 = p2->ships_list.begin();

        if (came_from_player1) {
            // check if there are any other ship in the path of this ship 
            err = check_path_empty(x_start_in, y_start_in, length, orientation, p1->ships_list);

            if (err) {
                cout << "another ship in the way for player 1 ";
            }

            if (shipsp1->orientation.empty()) {
                shipsp1->orientation = orientation;
                shipsp1->size = length;
                shipsp1->start_box = box(x_start_in, y_start_in);
                shipsp1++;
            }
        }
        else if (came_from_player2) {
            // check if there are any other ship in the path of this ship 
            err = check_path_empty(x_start_in, y_start_in, length, orientation, p2->ships_list);

            if (err) {
                cout << "another ship in the way for player 2";
            }

            if (shipsp2->orientation.empty()) {
                shipsp2->orientation = orientation;
                shipsp2->size = length;
                shipsp2->start_box = box(x_start_in, y_start_in);
                shipsp2++;
            }
        }
        else {
            cout << "who are you??";
        }


        if (shipsp1 == p1->ships_list.end() && shipsp2 == p2->ships_list.end()) {
            setup_finished = true;
            cout << "Both players finished setting up their ships";
            break;
        }
    }

    while (!game_finished) {
        // wait for input
        // check / determine game state 

        if (came_from_player1) {
            cout << "Next up is player 2";

            p2->boxes_hit.push_back(box(x, y));

            status = check_hit_what(x, y, p2->ships_list);

            cout << "Your hit status is " << status ;
        }
        else if (came_from_player2) {
            cout << "Next up is player 1 ";

            p1->boxes_hit.push_back(box(x, y));

            status = check_hit_what(x, y, p1->ships_list);

            cout << "Your hit status is " << status ;
        }


        if (p1_remaining_ships == 0) {
            player2 = 2;
            declare_win(player2);
            game_finished = true;
        }
        if (p2_remaining_ships == 0) {
            player1 = 1;
            declare_win(player1);
            game_finished = true;
        }
        
    }

    

  return 0;
}


void declare_win (int player_who_won) {

    cout << "Player number " << player_who_won " won";

}

int check_in_bound(int x_start, int y_start, int size, string orientation) {

    if (x_start < 0 || y_start < 0 || x_start > 9 || y_start > 9) {
        return 1;
    }
    switch (orientation) {
        case "vertical":
            if (y_start + (size - 1) > 9 ) {
                return 1;
            }
            break;
        case "horizontal":
            if (x_start + (size - 1) > 9 ) {
                return 1;
            }
            break;
        default:
            cout << "What else direction can you have?";
            break;
    }

    // only get here if no error
    return 0;
}
int check_path_empty(int x_start, int y_start, int size, string orientation, list<ship> ships) {

    for (list<ship>::iterator it=ships.begin(); it != ships.end(); it++ ) {
        if (!it->orientation.empty()) {

            // if same oreintation
            if (orientation.compare(it->orientation) == 0 ) {
                // if both vertical or both horizontal 
                if (orientation.compare("vertical")) {
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
                else { // if both horizontal
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
            } 
            // if not same orientation have to check every pixel 
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < it->size; j++) {
                    if (orientation.compare("vertical") == 0 && it->orientation.compare("horizontal") == 0) {
                        if ( it->start_box.x + j == x_start && it->start_box.y == y_start + i) {
                            return 1;
                        }

                    }
                    else {
                        // this is when orientation = horizontal && it->orientation = vertical
                        if ( it->start_box.y + j == y_start && it->start_box.x == x_start + i) {
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

    if (ship.orientation.compare("vertical") == 0 ) {
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
    else {
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
   int SUNK_STATUS_CODE = 2;
   int HIT_STATUS_CODE = 1;
   int MISS_STATUS_CODE = 0;

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

