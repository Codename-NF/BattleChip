#include <iostream>
#include <list>
#include <string>
#include <sys/socket.h>
#include <set> 

using namespace std;
/**
 * 
 * fyi setupvalues only work for a single ship, i'm guessing you're expecting bluetooth to pass in a list of those?
 * Why don't you just make it contain a ship anyways...
 * 
 * 
 * also main really shouldn't be 150 lines
 * you should split loops into their own functions
 * 
 * 
 * */
const int NUM_OF_SHIPS = 1;
const int SUNK_STATUS_CODE = 2;
const int HIT_STATUS_CODE = 1;
const int MISS_STATUS_CODE = 0;
const int BOUNDARY_MIN = 0;
const int BOUNDARY_MAX = 9;

enum orientations {VERTICAL = 1, HORIZONTAL = 2};

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

        bool operator<(const box& rhs) const
        {
            int score = (this->x)*(BOUNDARY_MAX + 1) + this->y;
            int score_rhs = (rhs.x)*(BOUNDARY_MAX + 1) + rhs.y;
            return score < score_rhs;
        }
} ;

struct ship {
    public: 
        box start_box = box(-1, -1);
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



struct player {
    public: 
        string player_name; 
        int player_num;
        list<ship> ships_list;
        set<box> boxes_hit;
        int remaining_ships;
        set<box> all_boxes_on_board;

        player(int player_num) {
            for (int i = 0; i < NUM_OF_SHIPS; i++) {
                this->ships_list.push_back(ship());
            }
            this->player_name = "";
            this->player_num = player_num;
            this->remaining_ships = NUM_OF_SHIPS;
            this->all_boxes_on_board = {};
        }
} ;

struct battleship {
    public: 
        list<player> players;

        battleship(int num_player) {
            for (int i = 1; i <= num_player; i++) {
                this->players.push_back(player(i));
            }
        }
} ;


// ******************* bluetooth place holder *******************
struct setupvalues {
    int x;
    int y;
    int size;
    int orientation;
    int device_num;

    setupvalues(int x, int y, int size, int orientation, int device_num) {
        this->x = x;
        this->y = y;
        this->size = size;
        this->orientation = orientation;
        this->device_num = device_num;
    }
};
struct shootvalues {
    int x;
    int y;
    int device_num;

    shootvalues(int x, int y, int device_num) {
        this->x = x;
        this->y = y;
        this->device_num = device_num;
    }
};

string ask_for_name();

setupvalues ask_for_setup(int player_num);

shootvalues ask_for_shoot(int player_num);
// ******************* bluetooth place holder *******************
void declare_win (int player_who_won);
bool out_of_bound(int x_start, int y_start, int size, int orientation); 
bool path_empty(int x_start, int y_start, set<box> all_boxes_on_board);
bool contains_box(ship *ship, int x, int y);
int check_hit_what(int x, int y, list<ship> *ships, int *remaining_ships);
bool all_ships_destroyed(list<ship> ships);
bool not_hit_yet(int x, int y, set<box> boxes);

void getting_names(battleship *game, int player_num);
void setting_up_ships(list<player>::iterator *p1, list<player>::iterator *p2);
void playing_game(list<player>::iterator *p1, list<player>::iterator *p2);
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
    int player_num;
    // get player_number from selecting playing mode
    player_num = 2; // default to 2 for now 
    battleship game = battleship(player_num);
    list<player>::iterator p1, p2;
    for (list<player>::iterator it = game.players.begin(); it != game.players.end(); it++) {
        if (it->player_num == 1) {
            p1 = it;
        }
        else if (it->player_num == 2) {
            p2 = it;
        }
    }
    
    getting_names(&game, player_num);
    
    setting_up_ships(&p1, &p2);


    playing_game(&p1, &p2);
  return 0;
}

bool not_hit_yet(int x, int y, set<box> boxes) {
    return boxes.find(box(x,y)) == boxes.end();
}

void declare_win (int player_who_won) {
    cout << "Player number " << player_who_won << " won" << endl;
}

bool out_of_bound(int x_start, int y_start, int size, int orientation) {
    if (x_start < BOUNDARY_MIN || y_start < BOUNDARY_MIN || x_start > BOUNDARY_MAX || y_start > BOUNDARY_MAX) {
        return true;
    }
    int axis_value = orientation == VERTICAL ? y_start : x_start;
    if (axis_value + (size - 1) > BOUNDARY_MAX) {
        return true;
    }
    
    // only get here if no error
    return false;
}
bool path_empty(int x_start, int y_start, set<box> all_boxes_on_board) {
    return all_boxes_on_board.find(box(x_start, y_start)) == all_boxes_on_board.end();
}

bool contains_box(ship *ship, int x, int y) {
    if (ship->orientation == VERTICAL) {
        if (x != ship->start_box.x) {
            return false;
        }
        else {
            if (y >= ship->start_box.y && y <= ship->start_box.y + ship->size) {
                ship->hit_count++;
                return true;
            }
            return false;
        }
    }
    else { // if HORINZONTAL 
        if (y != ship->start_box.y) {
            return false;
        }
        else {
            if (x >= ship->start_box.x && x <= ship->start_box.x + ship->size) {
                ship->hit_count++;
                return true;
            }
            return false;
        }
    }
}
int check_hit_what(int x, int y, list<ship> *ships, int *remaining_ships) {
    /*
    return 0 for miss 
    return 1 for hit a box of a ship
    return 2 if the entire ship is sunk
    -> Decrements remaining count each time a sunk appears 
    */
   

   for (list<ship>::iterator it = ships->begin(); it != ships->end(); it++) {
       if ( contains_box(&(*it), x, y) ) {
           cout << "hit count is " << it->hit_count << endl;
           if (it->hit_count == it->size) {
               *remaining_ships = *remaining_ships - 1;
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
        //TODO: set up error needs to be handled more carefully to makesure all ships are in place when finished.
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
            //TODO: "feature" if you hit the same place again you loose your turn ;) 
            cout << "Can't hit here buddy" << endl;
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


// ******************* bluetooth place holder *******************
string ask_for_name() {
    string name; 
    cout << "Please type your name: "; // Type a number and press enter
    cin >> name; // Get user input from the keyboard
    cout << "Your name is: " << name << endl; // Display the input value

    return name;
}

setupvalues ask_for_setup(int player_num) {
    int x;
    int y;
    int size;
    int orientation;
    int device_num;

    cout << "Player " << player_num << endl;
    cout << "What x position would you like to place your ship: ";
    cin >> x;
    cout << "What y position would you like to place your ship: ";
    cin >> y;
    cout << "What size is the ship: ";
    cin >> size;
    cout << "What orientation is the ship (1 for vertical and 2 for horizntaal): ";
    cin >> orientation;

    device_num = player_num;

    return setupvalues(x, y, size, orientation, device_num);
}

shootvalues ask_for_shoot(int player_num) {
    int x;
    int y;
    int device_num;

    cout << "Player " << player_num << endl;
    cout << "What x position would you like to shoot: ";
    cin >> x;
    cout << "What y position would you liek to shoot: ";
    cin >> y;

    device_num = player_num;

    return shootvalues(x, y, device_num);

}

// ******************* bluetooth place holder *******************