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

#include "robot.h"
#include "boat.h"
#include "berth.h"
#include "execute.h"
#include "Debug.h"
#include "Buy.h"
#include "utils.h"

#define CLOCKWISE_DIR 0
#define ANTICLOCKWISE_DIR 1
#define LEFT 1
#define RIGHT 0
#define UP 2
#define DOWN 3
#define FORWARD 0
#define CLRO 2      // 顺时针
#define ATCLRO 1     // 逆时针

using namespace std;

const int n = 200;
const int N = 210;
const int M = 100;
const int boat_price = 8000, robot_price = 2000;
const int robot_max_num = 10;
const int boat_max_num = 5;
// const int CLOCKWISE_DIR = 0;
// const int ANTICLOCKWISE_DIR = 1;

extern int robot_num, boat_num, berth_num;
extern int money, boat_capacity, step;
extern int all_goods_cnt;
extern long long all_goods_val;
extern double r1[3], r2[3], r3[3];

extern char grid[N][N];
extern int gds[N][N], goods_vanish_time[N][N];
extern std::map<std::pair<int, int>, char> originalPosition;
extern std::map<std::pair<int, int>, int> originalValue;
extern std::map<std::pair<int, int>, bool> slow_points;

struct Dir {
    int x, y;
    bool operator==(const Dir &b) {
        return x == b.x && y == b.y;
    }
};

struct boat_sport_status{
    int x, y, dir, dis;
    bool operator<(const boat_sport_status &b) const {
        return dis > b.dis;
    }
};

extern Dir d[4];

extern std::ofstream outFile;

extern Robot* robot[M];
extern Boat* boat[M];
extern Berth* berth[M];
extern Buy* buy;

extern execute* exec;

extern Debug* debug;
extern std::ofstream outFile;

extern vector<pair<int, int>> robot_purchase_point;
extern vector<pair<int, int>> boat_purchase_point;
extern vector<pair<int, int>> delivery_point;
extern bool boat_vis[n][n][5];
extern boat_sport_status boat_path[n][n][4];

#endif
