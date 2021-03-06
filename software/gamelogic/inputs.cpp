#include "inputs.h"
#include "constants.h"
#include "structs.h"
#include <iostream>
#include <sstream>
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
void get_input_UI(list<setupvalues> *list_setupval, string inputstring, int device_num) {
    //string input = inputstring;
    string input = "0 0 2 2 \n1 1 2 3 \n5 5 2 2\n";
    stringstream ss;

    ss << input;
    
    for (int i = 0; i < NUM_OF_SHIPS; i++) {

        int x,y,size, orientation;
        ss >> x >> y >> size >> orientation;
        (*list_setupval).push_back(setupvalues(x, y, size, orientation, device_num));
        // std::cout << "x: " << x << '\n';
        // std::cout << "y: " << y << '\n';
        // std::cout << "size: " << size << '\n';
        // std::cout << "orientation: " << orientation << '\n';
        // std::cout << ss.str() << endl; // converting string stream to string 
    }
}
