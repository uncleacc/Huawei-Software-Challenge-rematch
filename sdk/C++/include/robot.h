#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <vector>
using namespace std;

class Robot {
public:
    int x, y, goods;  // 机器人坐标，货物持有状态
    int price;        // 手里的货物价值
    int mbx, mby; //目标点坐标
    int id;  // 机器人的id
    bool haveTarget; // 是否选定食物
    int searchGdsNumber; // 搜索的数量
    int findLength; // 锁定的货物距离
    bool is_first_move; // 是否是第一次移动


    std::vector<int> movePath;
    vector<int> boards; // 机器人可以去的泊位

public:
    Robot();
    Robot(int x, int y);
    void initialize();

    int goto_mbp();
    int get_good();
    void put_good(int ts);
    void move(int dir);
    int getBerthId();
    int find_nearest_good();
    int find_nearest_berth();
    void close(int berthID);
    bool is_locate_Berth();
};

#endif