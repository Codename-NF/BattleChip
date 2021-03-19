#include <list>
#include <set>
#include <string>
#include "constants.h"
#include <bits/stdc++.h> 

using namespace std;

#ifndef STRUCTS_H
#define STRUCTS_H

struct box {
    public:
        int x;
        int y;
        mutable int status;

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
        bitset<5> ships_alive;  

        player(int player_num) {
            for (int i = 0; i < NUM_OF_SHIPS; i++) {
                this->ships_list.push_back(ship());
            }
            this->player_name = "";
            this->player_num = player_num;
            this->remaining_ships = NUM_OF_SHIPS;
            this->all_boxes_on_board = {};
            this->ships_alive = 31; // 5'b11111
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


// ******************* bluetooth place holder *******************

#endif