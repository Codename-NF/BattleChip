#ifndef CONSTANTS_H
#define CONSTANTS_H

const int NUM_OF_SHIPS = 1;
const int SUNK_STATUS_CODE = 2;
const int HIT_STATUS_CODE = 1;
const int MISS_STATUS_CODE = 0;
const int BOUNDARY_MIN = 0;
const int BOUNDARY_MAX = 9;
const int DEBUG = 1;
const int SUCCESS = 1; //BT success
const int FAILURE = 0; // BT nothing is there
const int CRITICAL = 2; // BT critical failure connection failed
const int BT_RECEIVE_SIZE = 16;

const int BLACK			= 0;
const int WHITE			= 1;
const int RED			= 2;
const int LIME			= 3;
const int BLUE			= 4;
const int YELLOW        = 5;
const int CYAN			= 6;
const int MAGENTA		= 7;

const int MISS_COLOR    = WHITE;
const int HIT_COLOR     = RED;
const int SUNK_CROSS_COLOR    = BLACK;

const int PLAYER1       = 1;
const int PLAYER2       = 2;

const int AI_PLAYER_ID = 0;


enum orientations {VERTICAL = 1, HORIZONTAL = 2};

#endif 