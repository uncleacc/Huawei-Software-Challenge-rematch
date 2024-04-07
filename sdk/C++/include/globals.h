#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <deque>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <bitset>


#include "robot.h"
#include "boat.h"
#include "berth.h"
#include "execute.h"
#include "Buy.h"
#include "utils.h"
#include "Debug.h"

#define CLOCKWISE_DIR_OP 0
#define ANTICLOCKWISE_DIR_OP 1

/**
 * 轮船当前行经方向
 */
#define LEFT 1
#define RIGHT 0
#define UP 2
#define DOWN 3

/**
 * 轮船运动操作
 */
#define CLOCKWISE_DIR_OP 0  // 顺时针
#define ANTICLOCKWISE_DIR_OP 1  // 逆时针
#define FORWARD_OP 2   // 前进
#define DEPT_OP 3   // 尝试将对应船位置重置到主航道上，会导致船进入恢复状态。
#define BERTH_OP 4  // 尝试将对应船位置重置到船坞上，会导致船进入恢复状态。

using namespace std;



const int n = 200;
const int N = 210;
const int M = 100;
const int boat_price = 8000, robot_price = 2000;

const int robot_max_num = 15;
const int boat_max_num = 5;


extern int robot_num, boat_num, berth_num;
extern int money, boat_capacity, step;
extern int all_goods_cnt;
extern long long all_goods_val;
extern double r1[3], r2[3], r3[3];

extern char grid[N][N];
extern int gds[N][N], goods_vanish_time[N][N];
extern bitset<15001> exist_obstacle[N][N];
extern std::map<std::pair<int, int>, char> originalPosition;
extern std::map<std::pair<int, int>, int> originalValue;
extern std::map<std::pair<int, int>, bool> slow_points;

struct Dir {
    int x, y;
    bool operator==(const Dir &b) {
        return x == b.x && y == b.y;
    }
};


/*
 * A*算法的运动节点
 */
struct boat_sport_node{
    int x, y, dir;  // 当前点坐标，当前方向
    int gCost;  // 起始点到当前点的实际代价
    int hCost;  // 从当前点到目标点的预估代价----曼哈顿距离
    bool operator<(const boat_sport_node &b) const {
        return gCost + hCost > b.gCost + b.hCost;
    }
};


extern Dir d[4];

extern Robot* robot[M];
extern Boat* boat[M];
extern Berth* berth[M];
extern Buy* buy;

extern Debug info;
extern Debug warn;
extern Debug error;


extern execute* exec;

extern vector<pair<int, int>> robot_purchase_point;
extern vector<pair<int, int>> boat_purchase_point;
extern vector<pair<int, int>> delivery_point;
extern bool boat_vis[n][n][5];
extern boat_sport_node  boat_node_path[n][n][4];
#endif
