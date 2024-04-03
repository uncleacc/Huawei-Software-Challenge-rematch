#ifndef BOAT_H
#define BOAT_H

#include <vector>
#include <stdio.h>


class Boat {
public:
    Boat();
    Boat(int x, int y);

    bool navigation();
    void set_mbp(int x, int y);
    void move();
    void exec(int op);


public:
    int goods_num, status;
    int id;    // 船只id
    int x, y;  // 核心点位置
    int dir;   // 正方方向 (0-右, 1-左, 2-上, 3-下)
    int mbx, mby, mbdir;  // 目标位置

    std::vector<int> operation_list;  // 0: 前进，1: 左转，2: 右转
};

#endif