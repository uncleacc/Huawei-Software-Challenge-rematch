#ifndef CODECRAFTSDK_BUY_H
#define CODECRAFTSDK_BUY_H

#include <vector>


class Buy {
public:
    Buy();
    ~Buy();

    bool buy_robot(int idx);
    bool buy_boat(int idx);

    void append_robot_point(int x, int y);
    void append_boat_point(int x, int y);

public:
    std::vector<std::pair<int, int>> robot_purchase_point;
    std::vector<std::pair<int, int>> boat_purchase_point;
};


#endif //CODECRAFTSDK_BUY_H
