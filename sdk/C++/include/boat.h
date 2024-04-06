#ifndef BOAT_H
#define BOAT_H

#include <vector>
#include <stdio.h>


class Boat {
public:
    Boat();
    Boat(int x, int y);

    // bool navigation();
    void set_mbp(int x, int y);
    void move();
    void exec(int op);
    void add_berth(int berth_id);
    void remove_berth(int berth_id);
    bool can_go_berth(int berth_id);
    bool go_mb_point(int berth_id, int DeliveryPointId, int oriMbx, int oriMby, int choice);
    int find_max_goods();
    int getBerthIdByPoint();


public:
    int goods_num, status;
    int id;
    int x, y;
    int dir;                // 正方方向 (0-右, 1-左, 2-上, 3-下)
    int mbx, mby;

    std::vector<int> berthBoard;
    std::vector<int> operation_list;  // 0: 前进，1: 左转，2: 右转
};

#endif