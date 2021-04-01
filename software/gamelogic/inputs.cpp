#include "inputs.h"
#include "constants.h"
#include "structs.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <bitset>
#include "RS232.h" // may need to change path 

using namespace std;

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
// taking in a list of inputs 
/*
Format: 
Placement message (app to console)
“p coordinate1 bitfield1 coordinate2 bitfield2 ...~” 
Note: These are shown with spaces in between, but they are not sent in reality
Example: “~”
coordinate is a byte taking on values from 0 to 99 inclusive, where the coordinate of the ship is actually at (coordinate / 10, coordinate % 10)
bitfield contains two attributes
Upper 4 bits are unused
shipOrientation (0/1) (1 bit) -> 0 is vertical and 1 is horizontal 
shipLength (2 - 5) (3 bits)
Bit 0 is shipOrientation, bit 1-3 are shipLength
1011 is a 5 length ship that is horizontal
0100 is a 2 length ship that is vertical

*/
int get_placement_message_BT(list<setupvalues> *list_setupval, int device_num) {
    char receive_char[BT_RECEIVE_SIZE];
    if (device_num == 1) {
        BT_receive_0(receive_char);
    }
    else {
        BT_receive_1(receive_char);
    }

    if (receive_char[0] != 'p') {
        return FAILURE;
    }
    for (int i = 1; i < 10; i += 2) {
        int x,y,size, orientation;
        x = receive_char[i] % 10;
        y = receive_char[i] / 10;
        bitset<8> bitfield(receive_char[i+1]);
        orientation = bitfield[0] + 1;
        size = bitfield[3] * 4 + bitfield[2] * 2 + bitfield[1];

        (*list_setupval).push_back(setupvalues(x, y, size, orientation, device_num));

        // std::cout << "x: " << x << '\n';
        // std::cout << "y: " << y << '\n';
        // std::cout << "size: " << size << '\n';
        // std::cout << "orientation: " << orientation << '\n';
        // std::cout << ss.str() << endl; // converting string stream to string 

    }
    return SUCCESS;
}

/*
Format:
“shoot xCoordinate yCoordinate\n\n”
*/
int get_shoot_message_BT(shootvalues *input, int device_num) {
    int x,y;

    char receive_char[BT_RECEIVE_SIZE];
    char forfeit_char[BT_RECEIVE_SIZE];
    int first_success = 0;
    int second_success = 0;
    if (device_num == 1) {
        first_success = BT_receive_0(receive_char);
        second_success = BT_receive_1(forfeit_char);
    }
    else {
        first_success = BT_receive_1(receive_char);
        second_success = BT_receive_0(forfeit_char);
    }

    if (first_success == 0) {
        return FAILURE;
    }

    // checking for forfeit messages at the beginning of each turn 
    if (receive_char[0] = 'f') {
        if (device_num == 1) {
            (*input).p1_forfeit = true;
            return SUCCESS;
        }
        (*input).p2_forfeit = true;
        return SUCCESS;
    }

    if (second_success && forfeit_char[0] == 'f') {
        if (device_num == 1) {
            (*input).p2_forfeit = true;
            return SUCCESS;
        }
        (*input).p1_forfeit = true;
        return SUCCESS;
    }

    if (receive_char[0] == 's') {
        return FAILURE;
    }

    (*input).x = receive_char[2];
    (*input).y = receive_char[4];
    (*input).device_num = device_num;

    return SUCCESS;
}

/*
Format:
“start isTurn~”
isTurn is 1 if the current player is attacking first, 0 otherwise
*/
void send_game_start_status_BT(bool start1) {
    string message_to_1 = "start ";
    string message_to_2 = "start ";

    
    message_to_1 += to_string(start1);
    message_to_2 += to_string(!start1);

    message_to_1 += "~";
    message_to_2 += "~";

    int n = message_to_1.length();
 
    // declaring character array with space for termination character 
    char message_to_1_char[n + 1];
    char message_to_2_char[n + 1];
 
    // copying the contents of the string to char array
    strcpy(message_to_1_char, message_to_1.c_str());
    strcpy(message_to_2_char, message_to_2.c_str());

    BT_send_0(message_to_1_char);
    BT_send_1(message_to_2_char);

}

/*
Format: (the result for attacking)
“result xCoordinate yCoordinate gamestatus hitstatus~”
*/
void send_result_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus) {
    string message = "result ";

    message += to_string(x);
    message += " ";
    message += to_string(y);
    message += " ";
    message += to_string(gamestatus);
    message += " ";
    message += to_string(hitstatus);

    message += "~";

    int n = message.length();
 
    // declaring character array with space for termination character 
    char message_char[n + 1];
 
    // copying the contents of the string to char array
    strcpy(message_char, message.c_str());

    if (device_num == 1) {
        BT_send_0(message_char);
    }
    else {
        BT_send_1(message_char);
    }


}

/*
Format: (the result for attacking)
“targeted xCoordinate yCoordinate gameStatus hitstatus (destroyedShipXCoordinate destroyedShipYCoordinate shipLength shipOrientation)~”
*/
void send_result_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus, int destroyed_start_x, int destroyed_start_y, int length, int orientation) {
    string message = "result ";

    message += to_string(x);
    message += " ";
    message += to_string(y);
    message += " ";
    message += to_string(gamestatus);
    message += " ";
    message += to_string(hitstatus);
    message += " ";
    message += to_string(destroyed_start_x);
    message += " ";
    message += to_string(destroyed_start_y);
    message += " ";
    message += to_string(length);
    message += " ";
    message += to_string(orientation);

    message += "~";

    int n = message.length();
 
    // declaring character array with space for termination character 
    char message_char[n + 1];
 
    // copying the contents of the string to char array
    strcpy(message_char, message.c_str());

    if (device_num == 1) {
        BT_send_0(message_char);
    }
    else {
        BT_send_1(message_char);
    }

}

/*
Format: (the result of being attacked)
“targeted xCoordinate yCoordinate gameStatus hitstatus (destroyedShipXCoordinate destroyedShipYCoordinate shipLength shipOrientation)~”
*/
void send_targeted_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus) {
    string message = "targeted ";

    message += to_string(x);
    message += " ";
    message += to_string(y);
    message += " ";
    message += to_string(gamestatus);
    message += " ";
    message += to_string(hitstatus);

    message += "~";

    int n = message.length();
 
    // declaring character array with space for termination character 
    char message_char[n + 1];
 
    // copying the contents of the string to char array
    strcpy(message_char, message.c_str());

    if (device_num == 1) {
        BT_send_0(message_char);
    }
    else {
        BT_send_1(message_char);
    }

}

/*
Format: (the result of being attacked)
“targeted xCoordinate yCoordinate gameStatus hitstatus (destroyedShipXCoordinate destroyedShipYCoordinate shipLength shipOrientation)~”
*/
void send_targeted_message_BT(int device_num, int x, int y, int gamestatus, int hitstatus, int destroyed_start_x, int destroyed_start_y, int length, int orientation) {
    string message = "targeted ";

    message += to_string(x);
    message += " ";
    message += to_string(y);
    message += " ";
    message += to_string(gamestatus);
    message += " ";
    message += to_string(hitstatus);
    message += " ";
    message += to_string(destroyed_start_x);
    message += " ";
    message += to_string(destroyed_start_y);
    message += " ";
    message += to_string(length);
    message += " ";
    message += to_string(orientation);

    message += "~";

    int n = message.length();
 
    // declaring character array with space for termination character 
    char message_char[n + 1];
 
    // copying the contents of the string to char array
    strcpy(message_char, message.c_str());

    if (device_num == 1) {
        BT_send_0(message_char);
    }
    else {
        BT_send_1(message_char);
    }

}


/*
Format:
Create message (app to console)
“c playerID numPlayers~” 
playerID is the ID of the player
numPlayers is 1 or 2, 1 for vs AI, 2 for multiplayer
Only sent to BluetoothChip0

*/
int get_create_message_BT(createmessage *msg) {
    char receive_char[BT_RECEIVE_SIZE];

    
    if (BT_receive_0(receive_char) == SUCCESS) {
        if (receive_char[0] != 'c') { 
            return FAILURE;
        } 
        (*msg).keywrod = receive_char[0];
        (*msg).playerid = receive_char[2];
        (*msg).numplayer = receive_char[4];
        return SUCCESS;
    }
    return FAILURE;
    
}

/*
Format:
“create numPlayers status~”
numPlayers is 1 or 2, 1 for vs AI, 2 for multiplayer
status is “1” or “0”
“0” if a game is already in progress
Else “1” if multiplayer lobby created successfully
*/
void send_create_response_BT(int num_players, int status) {
    string message = "create ";

    message += to_string(num_players);
    message += " ";
    message += to_string(status);

    message += "~";

    int n = message.length();
 
    // declaring character array with space for termination character 
    char message_char[n + 1];
 
    // copying the contents of the string to char array
    strcpy(message_char, message.c_str());

    
    BT_send_0(message_char);
    
}


/*
Format:
Join message (app to console)
“j playerID~”
playerID is the player ID
Can only join a pre-existing lobby
Only sent to BluetoothChip1

*/
int get_join_message_BT() {
    char receive_char[BT_RECEIVE_SIZE];

    
    if (BT_receive_1(receive_char) == SUCCESS) {
        if (receive_char[0] == 'j') {
            return receive_char[2];
        }
    }
    return -1;
    
}

void send_join_reponse_BT(int status) {
    string message = "join ";

    message += to_string(status);

    message += "~";

    int n = message.length();
 
    // declaring character array with space for termination character 
    char message_char[n + 1];
 
    // copying the contents of the string to char array
    strcpy(message_char, message.c_str());

    
    BT_send_1(message_char);

}


/*
Format:
“ready~”
Only sent for multiplayer lobbies
Sent when the multiplayer lobby is full and the players may begin placement
*/

void send_ready_messaeg_BT() {
    BT_send_0("ready~");
    BT_send_1("ready~");
}

void send_win_by_forfiet_BT(int device_num) {
    if (device_num == 1) {
        BT_send_0("f~");
    }
    else {
        BT_send_1("f~");
    }
}
