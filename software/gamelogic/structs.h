#include <list>
#include <set>
#include <string>
#include "constants.h"
#include <bitset> 

using namespace std;

#ifndef STRUCTS_H
#define STRUCTS_H

/**
 * box struct that represent each square on the board
 * x: x coordinate
 * y: y coordinate
 * status: status of the box 
 * 
**/
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


/**
 * ship struct that represent each ship placed on the board
 * start_box: starting box of the ship
 * orientation: orientation of the ship
 * afloat: if the at least one of box of the ship is still not hit
 * hit_count: number of times this ship has been hit 
 * 
**/
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
        ship(int x, int y, int size, int orientation) {
            this->start_box = box(x,y);
            this->size = size;
            this->orientation = orientation;
            this->afloat = true;
            this->hit_count = 0;

        }
} ;


/**
 * player struct that represent each player of the game
 * player_name: player's name (for command line interface only)
 * plyaer_id: unique identifier for player in the database(for POST command)
 * player_num: player number (player1 or player2)
 * ships_list: list of ships this player has
 * boxes_hit: all the boxes that has been fired upon
 * remaining_ships: count of ships still afloat on the board
 * all_boxes_on_board: all the boxes that has ship on it
 * ships_alive: 5 bit one hot representation for which ship is still alive
 * 
**/
struct player {
    public: 
        string player_name; // for CLI only
        int player_id;
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


/**
 * battleship struct that represent the game
 * players: list of players in the game
 * 
**/
struct battleship {
    public: 
        list<player> players;

        battleship(int num_player) {
            for (int i = 1; i <= num_player; i++) {
                this->players.push_back(player(i));
            }
        }
} ;

/**
 * setupvalues struct that represent the values for each ship set up
 * x: x coordinates
 * y: y coordinates
 * size: size of ship
 * orientation: orientation of the ship
 * device_num: came from which device
 * 
**/
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

/**
 * shootvalues struct that represent where to shoot
 * x: x coordinates
 * y: y coordinates
 * device_num: came from which device
 * p1_forfeit: if player 1 forfeited
 * p2_forfeit: if player 2 forfeited
 * 
**/
struct shootvalues {
    int x;
    int y;
    int device_num;
    bool p1_forfeit = false;
    bool p2_forfeit = false;

    shootvalues(int x, int y, int device_num) {
        this->x = x;
        this->y = y;
        this->device_num = device_num;
    }

    shootvalues(bool p1_forfeit, bool p2_forfeit) {
        this->p1_forfeit = p1_forfeit;
        this->p2_forfeit = p2_forfeit;
    }
};

/**
 * createmessage struct that represent a create message from BT
 * keyword: the message keyword 
 * numplayer: number of players for the lobby
 * playerid: unique id for database
 * 
**/
struct createmessage {
    char keywrod;
    int numplayer;
    int playerid;

    createmessage() {
        this->keywrod = 'f';
        this->numplayer = -1;
        this->playerid = -1;
    }

    createmessage(char keyword, int numplayer, int playerid) {
        this->keywrod = keyword;
        this->numplayer = numplayer;
        this->playerid = playerid;
    }
};


#endif