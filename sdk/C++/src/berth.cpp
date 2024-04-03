#include "globals.h"

Berth::Berth() {
    lastTransport = false;
    num = 0;
    history_cnt = 0;
    price = 0;
    x = -1;
    y = -1;
    transport_time = -1;
    loading_speed = -1;
}

// 关闭码头
void Berth::close() {
    for(int j = 0; j < robot_num; j ++) {
        robot[j]->close(id);
    }
}

