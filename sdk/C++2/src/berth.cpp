#include "globals.h"
#include "berth.h"


Berth::Berth() {
    lastTransport = false;
    num = 0;
    history_cnt = 0;
    price = 0;
    x = -1;
    y = -1;
    loading_speed = -1;
    is_locked = false;
}

// 关闭码头
void Berth::close() {
    for(int j = 0; j < robot_num; j ++) {
        robot[j]->close(id);
    }
}

void Berth::set_locked(bool locked) {
    is_locked = locked;
}

