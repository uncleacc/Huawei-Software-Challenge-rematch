#ifndef CODECRAFTSDK_BUY_H
#define CODECRAFTSDK_BUY_H

#include <vector>
#include <map>

class Buy {
public:
    Buy();
    ~Buy();

    bool buy_robot(int idx);   // 买机器人
    bool buy_boat(int idx);   // 买船

    void append_robot_point(int x, int y);  // 添加机器人的位置
    void append_boat_point(int x, int y);  // 添加船的位置



};


#endif //CODECRAFTSDK_BUY_H
