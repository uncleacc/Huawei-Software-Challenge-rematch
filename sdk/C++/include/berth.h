#ifndef BERTH_H
#define BERTH_H

class Berth {

public:
    Berth();

    void close();
    void set_locked(bool locked);


public:
    int x, y;            // 泊位左上角坐标
    int loading_speed;   // 装载速度
    int num;             // 泊位货物数量
    int price;           // 泊位货物价值
    int id;              // 泊位id
    int history_cnt;    // 泊位历史货物数量
    bool lastTransport;  // 泊位是否是最后一次运输
    int lux, luy, rdx, rdy;       // 泊位左上角和右下角坐标

    int klux, kluy, krdx, krdy;   // 靠泊区左上角和右下角坐标
    bool is_locked;        //有船要来

    vector<pair<int, int>> berthCoordinates;
    vector<pair<int, int>> kaoBerthCoordinates;
    vector<int> berthGoodsPrice;

};

#endif