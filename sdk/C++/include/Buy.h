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
};


#endif //CODECRAFTSDK_BUY_H
