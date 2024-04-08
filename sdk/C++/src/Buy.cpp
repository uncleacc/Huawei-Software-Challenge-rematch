#include "Buy.h"
#include "globals.h"

Buy::Buy() {
}

Buy::~Buy() {

}

bool Buy::buy_robot(int idx) {
    if (money < robot_price) {
        info <<"step" << step << " Not enough money to buy robot" << endl;
        return false;
    }
    int x = robot_purchase_point[idx].first, y = robot_purchase_point[idx].second;
    printf("lbot %d %d\n", robot_purchase_point[idx].first, robot_purchase_point[idx].second);
    money -= robot_price;
    robot_num++;
    robot[robot_num - 1] = new Robot(x, y);
    originalPosition[{x, y}] = 'R';
    info <<"step" << step << " Buy Robot" << boat_num - 1 << " in " << idx
            << " robot_purchase_point ("<< robot_purchase_point[idx].first << "," << robot_purchase_point[idx].second << ")" << endl;
    return true;
}

bool Buy::buy_boat(int idx) {
    if (money < boat_price) {
        info <<"step" << step << " Not enough money to buy boat" << endl;
        return false;
    }
    int x = boat_purchase_point[idx].first, y = boat_purchase_point[idx].second;
    printf("lboat %d %d\n", x, y);
    money -= boat_price;
    boat_num++;
    boat[boat_num - 1] = new Boat(x, y);
    // TODO 给船赋予可去的区间
    if (idx == 0) {
        for(int i = 0; i < berth_num; i++) {
            if(berth[i]->is_locked == false)
                boat[boat_num - 1]->add_berth(i);
        }
    } else {
        boat[boat_num - 1]->berthBoard.push_back(0);
        boat[boat_num - 1]->berthBoard.push_back(1);
        // boat[boat_num - 1]->berthBoard.push_back(2);
        // boat[boat_num - 1]->berthBoard.push_back(3);
        // boat[boat_num - 1]->berthBoard.push_back(4);

        // TODO 清空函数
        boat[0]->berthBoard.clear();
        // boat[0]->berthBoard.push_back(0);
        // boat[0]->berthBoard.push_back(1);
        boat[0]->berthBoard.push_back(2);
        boat[0]->berthBoard.push_back(3);
        boat[0]->berthBoard.push_back(4);
    }
    info <<"step" << step << " Buy Boat" << boat_num - 1 << " in " << idx
        << " boat_purchase_point ("<< boat_purchase_point[idx].first << "," << boat_purchase_point[idx].second << ")" << endl;
    return true;
}
