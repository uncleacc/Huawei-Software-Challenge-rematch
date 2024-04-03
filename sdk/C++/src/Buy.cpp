#include "Buy.h"
#include "globals.h"

Buy::Buy() {
}

Buy::~Buy() {

}

bool Buy::buy_robot(int idx) {
    if (money < robot_price) {
//        debug->error("Not enough money to buy robot");
        return false;
    }
    int x = robot_purchase_point[idx].first, y = robot_purchase_point[idx].second;
    printf("lbot %d %d\n", robot_purchase_point[idx].first, robot_purchase_point[idx].second);
    outFile << "lbot " << robot_purchase_point[idx].first << " " << robot_purchase_point[idx].second << endl;
    money -= robot_price;
    robot_num++;
    robot[robot_num - 1] = new Robot(x, y);
    originalPosition[{x, y}] = 'R';
    return true;
}

bool Buy::buy_boat(int idx) {
    if (money < boat_price) {
//        debug->error("Not enough money to buy boat");
        return false;
    }
    outFile << "idx:" << idx << endl;
    int x = boat_purchase_point[idx].first, y = boat_purchase_point[idx].second;
    printf("lboat %d %d\n", x, y);
    money -= boat_price;
    boat_num++;
    boat[boat_num - 1] = new Boat(x, y);
    return true;
}

void Buy::append_robot_point(int x, int y) {
    robot_purchase_point.push_back(make_pair(x, y));
}

void Buy::append_boat_point(int x, int y) {
    boat_purchase_point.push_back(make_pair(x, y));
}
