#include "utils.h"
#include "constants.h"
#include "structs.h"

using namespace std;

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
            if (y >= ship->start_box.y && y <= ship->start_box.y + ship->size - 1) {
                //ship->hit_count++;
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
            if (x >= ship->start_box.x && x <= ship->start_box.x + ship->size - 1) {
                //ship->hit_count++;
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
           (*it).hit_count++;
           cout << "hit count is " << it->hit_count << endl;
           if (it->hit_count == it->size) {
               *remaining_ships = *remaining_ships - 1;
               (*it).afloat = false;
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

bool not_hit_yet(int x, int y, set<box> boxes) {
    return boxes.find(box(x,y)) == boxes.end();
}

void change_status_box_all_boxes(int x, int y, set<box> *boxes_hit, list<ship> *ships) {
    for (list<ship>::iterator it = ships->begin(); it != ships->end(); it++) {
       if ( contains_box(&(*it), x, y) ) {
           // the box belong to this ship
           int start_x = it->start_box.x;
           int start_y = it->start_box.y;
           for (int i = 0; i < it->size; i++) {
               if (it->orientation == VERTICAL) {
                   (*boxes_hit).find(box(start_x, start_y + i))->status = SUNK_STATUS_CODE;
               }
               else {
                   (*boxes_hit).find(box(start_x + i, start_y))->status = SUNK_STATUS_CODE;
               }
           }
       }
   }
}

void create_shots_with_ships(set<box> *all_boxes_hit, set<box> *shots_with_ships) {
    (*shots_with_ships).clear();
    for (set<box>::iterator it = all_boxes_hit->begin(); it != all_boxes_hit->end(); it++) {
        if (it->status != SUNK_STATUS_CODE) {
            (*shots_with_ships).insert(box(it->x, it->y));
        }
    }
}
