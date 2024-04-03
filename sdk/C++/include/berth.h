#ifndef BERTH_H
#define BERTH_H

class Berth {
public:
    int x, y;            // 泊位左上角坐标
    int transport_time;  // 运输时间
    int loading_speed;   // 装载速度
    int num;             // 泊位货物数量
    int price;           // 泊位货物价值
    int id;              // 泊位id
    int history_cnt;    // 泊位历史货物数量
    bool lastTransport;  // 泊位是否是最后一次运输
    int lux, luy, rdx, rdy;       // 泊位左上角和右下角坐标

public:
    Berth();

    void close();
};

#endif