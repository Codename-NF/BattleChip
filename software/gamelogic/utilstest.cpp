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


void test_check_hit_what(void) {
    list<ship> mylist;
    for (int i = 0; i < 5; i++) {
        mylist.push_back(ship());
    }
    int remainingships = 5;

    bitset<5> onehot = 31; // 5'b11111
    TEST_CHECK_(check_hit_what(0,0,&mylist,&remainingships, &onehot) == MISS_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", 0,0,5,31, MISS_STATUS_CODE);
    TEST_CHECK(remainingships == 5);
    TEST_CHECK(onehot == 31);

    // initializing 
    int i = 0;
    int sizes[5] = {2, 3, 3, 4, 5};

    for (list<ship>::iterator it = mylist.begin(); it != mylist.end(); it++) {
        (*it).start_box = box(i,i);
        (*it).size = sizes[i];
        (*it).orientation = HORIZONTAL;
        i++;
    }

    int inputx;
    int inputy;

    inputx = 0;
    inputy = 0;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 5);
    TEST_CHECK(onehot == 31);

    inputx = 1;
    inputy = 0;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == SUNK_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), SUNK_STATUS_CODE);
    TEST_CHECK(remainingships == 4);
    TEST_CHECK(onehot == 30); // 5'b11110

    inputx = 9;
    inputy = 9;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == MISS_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), MISS_STATUS_CODE);
    TEST_CHECK(remainingships == 4);
    TEST_CHECK(onehot == 30); // 5'b11110

    inputx = 1;
    inputy = 1;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 4);
    TEST_CHECK(onehot == 30); // 5'b11110

    inputx = 2;
    inputy = 1;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 4);
    TEST_CHECK(onehot == 30); // 5'b11110

    inputx = 3;
    inputy = 1;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == SUNK_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), SUNK_STATUS_CODE);
    TEST_CHECK(remainingships == 3);
    TEST_CHECK(onehot == 26); // 5'b11010

    inputx = 2;
    inputy = 2;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 3);
    TEST_CHECK(onehot == 26); // 5'b11010

    inputx = 3;
    inputy = 2;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 3);
    TEST_CHECK(onehot == 26); // 5'b11010

    inputx = 4;
    inputy = 2;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == SUNK_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), SUNK_STATUS_CODE);
    TEST_CHECK(remainingships == 2);
    TEST_CHECK(onehot == 24); // 5'b11000

    inputx = 3;
    inputy = 3;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 2);
    TEST_CHECK(onehot == 24); // 5'b11000

    inputx = 4;
    inputy = 3;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 2);
    TEST_CHECK(onehot == 24); // 5'b11000

    inputx = 5;
    inputy = 3;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 2);
    TEST_CHECK(onehot == 24); // 5'b11000

    inputx = 6;
    inputy = 3;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == SUNK_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), SUNK_STATUS_CODE);
    TEST_CHECK(remainingships == 1);
    TEST_CHECK(onehot == 16); // 5'b10000

    inputx = 4;
    inputy = 4;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 1);
    TEST_CHECK(onehot == 16); // 5'b10000

    inputx = 5;
    inputy = 4;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 1);
    TEST_CHECK(onehot == 16); // 5'b10000

    inputx = 6;
    inputy = 4;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 1);
    TEST_CHECK(onehot == 16); // 5'b10000

    inputx = 7;
    inputy = 4;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == HIT_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), HIT_STATUS_CODE);
    TEST_CHECK(remainingships == 1);
    TEST_CHECK(onehot == 16); // 5'b10000

    inputx = 8;
    inputy = 4;

    TEST_CHECK_(check_hit_what(inputx,inputy,&mylist,&remainingships, &onehot) == SUNK_STATUS_CODE, "check_hit_what(%d, %d, myship, %d, %d)==%d", inputx,inputy,remainingships,(int)(onehot.to_ulong()), SUNK_STATUS_CODE);
    TEST_CHECK(remainingships == 0);
    TEST_CHECK(onehot == 0); // 5'b00000
    


};

void test_not_hit_yet(void) {
    set<box> boxes;
    for (int i = 0; i < 10; i++) {
        boxes.insert(box(i,i));
    }

    int inputx, inputy;

    for (int i = 0; i < 10; i++) {
        inputx = i;
        inputy = i;
        TEST_CHECK_(not_hit_yet(inputx, inputy, boxes) == false, "not_hit_yet(%d, %d, boxes)==%d", inputx, inputy, false);
    }
    
    inputx = 1;
    inputy = 0;

    TEST_CHECK_(not_hit_yet(inputx, inputy, boxes) == true, "not_hit_yet(%d, %d, boxes)==%d", inputx, inputy, true);

    inputx = 2;
    inputy = 0;

    TEST_CHECK_(not_hit_yet(inputx, inputy, boxes) == true, "not_hit_yet(%d, %d, boxes)==%d", inputx, inputy, true);

    inputx = 3;
    inputy = 0;

    TEST_CHECK_(not_hit_yet(inputx, inputy, boxes) == true, "not_hit_yet(%d, %d, boxes)==%d", inputx, inputy, true);

    inputx = 9;
    inputy = 0;

    TEST_CHECK_(not_hit_yet(inputx, inputy, boxes) == true, "not_hit_yet(%d, %d, boxes)==%d", inputx, inputy, true);

    inputx = 0;
    inputy = 9;

    TEST_CHECK_(not_hit_yet(inputx, inputy, boxes) == true, "not_hit_yet(%d, %d, boxes)==%d", inputx, inputy, true);

    inputx = 3;
    inputy = 8;

    TEST_CHECK_(not_hit_yet(inputx, inputy, boxes) == true, "not_hit_yet(%d, %d, boxes)==%d", inputx, inputy, true);

};

void test_change_status_box_all_boxes(void) {
    int inputx, inputy;
    set<box> boxes_hit;
    list<ship> ships;

    // initializing
    int i = 0;
    int sizes[5] = {2, 3, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        ships.push_back(ship());
    }

    for (list<ship>::iterator it = ships.begin(); it != ships.end(); it++) {
        (*it).start_box = box(i,i);
        (*it).size = sizes[i];
        (*it).orientation = HORIZONTAL;
        i++;
    }

    // Let hit all ships until 1 boxes until remains 
    i = 0;
    for (int w = 0; w < 5; w++) {
        for (int j = 0; j < sizes[w] - 1; j++) {
            // insert box 
            boxes_hit.insert(box(i+j, i, HIT_STATUS_CODE));
        }
        i++;
    }

    // hit the rest
    inputx = 1;
    inputy = 0;
    boxes_hit.insert(box(inputx, inputy, SUNK_STATUS_CODE));

    change_status_box_all_boxes(inputx,inputy,&boxes_hit, &ships);

    inputx = 3;
    inputy = 1;
    boxes_hit.insert(box(inputx, inputy, SUNK_STATUS_CODE));

    change_status_box_all_boxes(inputx,inputy,&boxes_hit, &ships);

    inputx = 4;
    inputy = 2;
    boxes_hit.insert(box(inputx, inputy, SUNK_STATUS_CODE));

    change_status_box_all_boxes(inputx,inputy,&boxes_hit, &ships);

    inputx = 6;
    inputy = 3;
    boxes_hit.insert(box(inputx, inputy, SUNK_STATUS_CODE));

    change_status_box_all_boxes(inputx,inputy,&boxes_hit, &ships);

    inputx = 8;
    inputy = 4;
    boxes_hit.insert(box(inputx, inputy, SUNK_STATUS_CODE));

    change_status_box_all_boxes(inputx,inputy,&boxes_hit, &ships);


    for (set<box>::iterator it = boxes_hit.begin(); it != boxes_hit.end(); it++) {
        TEST_CHECK(it->status == SUNK_STATUS_CODE);
    }
    
};


TEST_LIST = {
    
    {"bool out_of_bound(int, int, int, int);", test_out_of_bound},
    {"bool path_empty(int, int, set<box>);", test_path_empty},
    {"bool contains_box(ship, int, int);", test_contains_box},
    {"int check_hit_what(int, int, list<ship>*, int *, bitset<5> *);", test_check_hit_what},
    {"bool not_hit_yet(int, int, set<box>);", test_not_hit_yet},
    {"void change_status_box_all_boxes(int, int, set<box> *, list<ship> *);", test_change_status_box_all_boxes},
    {0} // terminating test 
};
