#include "acutest.h"
#include "utils.h"
#include "structs.h"
#include <iostream>
#include "constants.h"
using std::cout;
using std::endl;

void test_out_of_bound(void) {
    TEST_CHECK_(out_of_bound(0,0,2,2) == false, "out_of_bound(%d,%d,%d,%d)==%d", 0,0,2,2, false);

    TEST_CHECK_(out_of_bound(5,5,5,2) == false, "out_of_bound(%d,%d,%d,%d)==%d", 5,5,5,2, false);
    TEST_CHECK_(out_of_bound(5,5,5,1) == false, "out_of_bound(%d,%d,%d,%d)==%d", 5,5,5,1, false);

    TEST_CHECK_(out_of_bound(6,6,5,2) == true, "out_of_bound(%d,%d,%d,%d)==%d", 6,6,5,2, true);
    TEST_CHECK_(out_of_bound(6,6,5,1) == true, "out_of_bound(%d,%d,%d,%d)==%d", 6,6,5,1, true);

    TEST_CHECK_(out_of_bound(10,10,2,2) == true, "out_of_bound(%d,%d,%d,%d)==%d", 10,10,2,2, true);
    TEST_CHECK_(out_of_bound(10,10,2,1) == true, "out_of_bound(%d,%d,%d,%d)==%d", 10,10,2,1, true);

    TEST_CHECK_(out_of_bound(9,9,1,2) == false, "out_of_bound(%d,%d,%d,%d)==%d", 9,9,1,2, false);
    TEST_CHECK_(out_of_bound(9,9,1,1) == false, "out_of_bound(%d,%d,%d,%d)==%d", 9,9,1,1, false);
};


void test_path_empty(void) {
    set<box> myset;
    cout << "  Empty set" << endl;
    TEST_CHECK_(path_empty(0,0,myset) == true, "path_empty(%d,%d, myset)==%d", 0,0, true);
    TEST_CHECK_(path_empty(1,1,myset) == true, "path_empty(%d,%d, myset)==%d", 1,1, true);
    cout << "  Inserting (0,0)" << endl;
    myset.insert(box(0,0));
    TEST_CHECK_(path_empty(0,0,myset) == false, "path_empty(%d,%d, myset)==%d", 0,0, false);
    cout << "  Inserting (1,1)" << endl;
    myset.insert(box(1,1));
    cout << "  Inserting (2,2)" << endl;
    myset.insert(box(2,2));
    cout << "  Inserting (3,2)" << endl;
    myset.insert(box(3,2));
    cout << "  Inserting (5,5)" << endl;
    myset.insert(box(5,5));
    cout << "  Inserting (5,9)" << endl;
    myset.insert(box(5,9));
    TEST_CHECK_(path_empty(1,1,myset) == false, "path_empty(%d,%d, myset)==%d", 1,1, false);
    TEST_CHECK_(path_empty(2,2,myset) == false, "path_empty(%d,%d, myset)==%d", 2,2, false);
    TEST_CHECK_(path_empty(3,2,myset) == false, "path_empty(%d,%d, myset)==%d", 3,2, false);
    TEST_CHECK_(path_empty(5,5,myset) == false, "path_empty(%d,%d, myset)==%d", 5,5, false);
    TEST_CHECK_(path_empty(5,9,myset) == false, "path_empty(%d,%d, myset)==%d", 5,9, false);

    TEST_CHECK_(path_empty(2,3,myset) == true, "path_empty(%d,%d, myset)==%d", 2,3, true);
    TEST_CHECK_(path_empty(3,3,myset) == true, "path_empty(%d,%d, myset)==%d", 3,3, true);
    TEST_CHECK_(path_empty(4,4,myset) == true, "path_empty(%d,%d, myset)==%d", 4,4, true);
    TEST_CHECK_(path_empty(9,5,myset) == true, "path_empty(%d,%d, myset)==%d", 9,5, true);
    TEST_CHECK_(path_empty(6,6,myset) == true, "path_empty(%d,%d, myset)==%d", 6,6, true);
    
};

void test_contains_box(void) {
    ship *myship;
    cout << "  New ship has not been initialized" << endl;
    TEST_CHECK_(contains_box(myship,0,0) == false, "contains_box(myship, %d, %d)==%d", 0,0, false);
    cout << "  Setting the ship at (0,0) with size of 5 and HORIZONTAL" << endl;
    (*myship).start_box.x = 0;
    (*myship).start_box.y = 0;
    (*myship).size = 5;
    (*myship).orientation = HORIZONTAL;
    TEST_CHECK_(contains_box(myship,0,0) == true, "contains_box(myship, %d, %d)==%d", 0,0, true);
    TEST_CHECK_(contains_box(myship,1,0) == true, "contains_box(myship, %d, %d)==%d", 1,0, true);
    TEST_CHECK_(contains_box(myship,2,0) == true, "contains_box(myship, %d, %d)==%d", 2,0, true);
    TEST_CHECK_(contains_box(myship,3,0) == true, "contains_box(myship, %d, %d)==%d", 3,0, true);
    TEST_CHECK_(contains_box(myship,4,0) == true, "contains_box(myship, %d, %d)==%d", 4,0, true);
    TEST_CHECK_(contains_box(myship,5,0) == false, "contains_box(myship, %d, %d)==%d", 4,0, false);

    cout << "  Setting the ship at (0,0) with size of 5 and VERTICAL" << endl;
    (*myship).orientation = VERTICAL;
    TEST_CHECK_(contains_box(myship,0,0) == true, "contains_box(myship, %d, %d)==%d", 0,0, true);
    TEST_CHECK_(contains_box(myship,0,1) == true, "contains_box(myship, %d, %d)==%d", 0,1, true);
    TEST_CHECK_(contains_box(myship,0,2) == true, "contains_box(myship, %d, %d)==%d", 0,2, true);
    TEST_CHECK_(contains_box(myship,0,3) == true, "contains_box(myship, %d, %d)==%d", 0,3, true);
    TEST_CHECK_(contains_box(myship,0,4) == true, "contains_box(myship, %d, %d)==%d", 0,4, true);
    TEST_CHECK_(contains_box(myship,0,5) == false, "contains_box(myship, %d, %d)==%d", 0,5, false);


};


TEST_LIST = {
    
    {"bool out_of_bound(int, int, int, int);", test_out_of_bound},
    {"bool path_empty(int, int, set<box>);", test_path_empty},
    {"bool contains_box(ship, int, int);", test_contains_box},
    {0} // terminating test 
};
