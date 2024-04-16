//
// Created by HP on 2024/4/1.
//

#include "execute.h"
#include "globals.h"

execute::execute() {

}

execute::~execute() {

}

void execute::execute_robot() {
    for (int i = 0; i < robot_num; i++) {
        if(robot[i]->mbx == -1 && robot[i]->mby == -1) {
            if (robot[i]->goods == 0) {
                int res = -1;
                while(res == -1) {
                    info << "set_goods_for_mb begin" << endl;
                    info << "robot[" << i << "]->mbx:" << robot[i]->mbx << " robot[" << i << "]->mby:" << robot[i]->mby << endl;
                    robot[i]->set_goods_for_mb();
                    info << "robot[" << i << "]->mbx:" << robot[i]->mbx << " robot[" << i << "]->mby:" << robot[i]->mby << endl;
                    info << "set_goods_for_mb end" << endl;

                    error << "find_mbGoodsPoint begin" << endl;
                    res = robot[i]->find_mbGoodsPoint();
                    if (res == -1) {
                        goods_infos.erase(std::remove_if(goods_infos.begin(), goods_infos.end(),
                            [&](const GoodsInfo &o){
                                return o.x == robot[i]->mbx && o.y == robot[i]->mby;
                            }),
                            goods_infos.end());
                        robot[i]->mbx = -1;
                        robot[i]->mby = -1;
                    }
                    error << "find_mbGoodsPoint end" << endl;
                }
            }
            else if (robot[i]->goods == 1 ) {

                info << "set_berth_for_mb begin" << endl;
                info << "robot[" << i << "]->mbx:" << robot[i]->mbx << " robot[" << i << "]->mby:" << robot[i]->mby << endl;
                int target_berthId = robot[i]->set_berth_for_mb();
                info << "robot[" << i << "]->mbx:" << robot[i]->mbx << " robot[" << i << "]->mby:" << robot[i]->mby << endl;
                info << "set_berth_for_mb end" << endl;

                error << "find berth begin" << endl;
                if (target_berthId != -1) robot[i]->find_mbBerthPoint(target_berthId);
                error << "find berth end" << endl;
            }
        }
        else if (robot[i]->x == robot[i]->mbx && robot[i]->y == robot[i]->mby) {
            if(grid[robot[i]->x][robot[i]->y] == 'B') {
                info << "robot[i]->x:" << robot[i]->x << " robot[i]->y:" << robot[i]->y << " goods:" << robot[i]->goods << endl;
                robot[i]->put_good();
            }
            else{
                robot[i]->get_good();
                info << "robot " << i << "goods:" << robot[i]->goods << endl;
            }
        }
        else {
            robot[i]->move();
        }
    }
}

void schedule_map1_1(int i){
    if (boat[i]->goods_num == 0 && boat[i]->nextBerth.empty()) {
        boat[i]->nextBerth = {1, 4};
    } else {
        boat[i]->nextBerth = {2, 3, 0};
    }

    // 无论是否载有货物，都执行以下逻辑
    // 确保nextBerth不为空
    if (!boat[i]->nextBerth.empty()) {
        int targetBerth = boat[i]->nextBerth.front(); // 取出第一个元素为目标泊位
        boat[i]->nextBerth.erase(boat[i]->nextBerth.begin()); // 从列表中删除第一个元素

        info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                             berth[targetBerth]->y, 0);
    } else {
        // 如果nextBerth为空，则打印错误或执行其他逻辑
        error << "Boat " << boat[i]->id << " has no berth target" << endl;
        int target=boat[i]->find_nearest_delivery_id();
        boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                             delivery_point[target].second, 1);
    }
}

void schedule_map1_2(int i){
    if(step<200){
        return;
    }
    else if(step<800){
        int targetBerth=1;
        info << "Boat " << " 标记一下预备港口，准备前往 " << targetBerth << endl;
        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                             berth[targetBerth]->y, 0);
        return;
    }
    else if(step<1000 &&boat[i]->goods_num>0){
        int targetBerth=0;
        info << "Boat " << " 标记一下预备港口，准备前往 " << targetBerth << endl;
        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                             berth[targetBerth]->y, 0);
        return;
    }
    //首先去货物最多的地方
    if (boat[i]->goods_num==0&&boat[i]->nextBerth.empty()){
        int targetBerth=boat[i]->find_max_goods();
        if(targetBerth==-1){
            targetBerth=0;
        }
        info << "Boat " << " 标记一下预备港口，准备前往 " << targetBerth << endl;
        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                             berth[targetBerth]->y, 0);
    }
    if(boat[i]->goods_num>0&&boat[i]->nextBerth.empty())
    {
        if(boat[i]->goods_num*4.8>boat_capacity*3.8){
            int target=boat[i]->find_nearest_delivery_id();
            boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                                 delivery_point[target].second, 1);
        }
        else{
            if(boat[i]->y<50||boat[i]->y>150){
                if(berth[0]->num>(boat_capacity-boat[i]->goods_num)){
                    int targetBerth=0;
                    info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
                    boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                         berth[targetBerth]->y, 0);
                }
                else{
                    if(berth[0]->num*4.8>boat_capacity){
                        int targetBerth=0;
                        info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
                        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                             berth[targetBerth]->y, 0);
                    }
                    else{
                        int target=boat[i]->find_nearest_delivery_id();
                        boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                                             delivery_point[target].second, 1);
                    }
                }
            }else{
                if(berth[1]->num>berth[2]->num){
                    if(berth[2]->num>(boat_capacity-boat[i]->goods_num)){
                        int targetBerth=2;
                        info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
                        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                             berth[targetBerth]->y, 0);
                    }
                    else if(berth[1]->num>(boat_capacity-boat[i]->goods_num)){
                        int targetBerth=1;
                        info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
                        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                             berth[targetBerth]->y, 0);
                    }
                    else{
                        if(berth[1]->num*4.8>boat_capacity){
                            int targetBerth=1;
                            info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
                            boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                                 berth[targetBerth]->y, 0);
                        }
                        else{
                            int target=boat[i]->find_nearest_delivery_id();
                            boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                                                 delivery_point[target].second, 1);
                        }
                    }
                }
                else{
                    if(berth[1]->num>(boat_capacity-boat[i]->goods_num)){
                        int targetBerth=1;
                        info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
                        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                             berth[targetBerth]->y, 0);
                    }
                    else if(berth[2]->num>(boat_capacity-boat[i]->goods_num)){
                        int targetBerth=2;
                        info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
                        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                             berth[targetBerth]->y, 0);
                    }
                    else{
                        if(berth[2]->num*4.8>boat_capacity){
                            int targetBerth=2;
                            info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
                            boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                                 berth[targetBerth]->y, 0);
                        }
                        else{
                            int target=boat[i]->find_nearest_delivery_id();
                            boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                                                 delivery_point[target].second, 1);
                        }
                    }
                }
            }

        }
    }
}

void schedule_map1_3(int i){
    if (boat[i]->goods_num==0&&boat[i]->nextBerth.empty()){
        int target=boat[i]->find_max_goods();
        boat[i]->nextBerth = {target};
    }
    // 无论是否载有货物，都执行以下逻辑
    // 确保nextBerth不为空
    if (!boat[i]->nextBerth.empty()) {
        int targetBerth = boat[i]->nextBerth.front(); // 取出第一个元素为目标泊位
        boat[i]->nextBerth.erase(boat[i]->nextBerth.begin()); // 从列表中删除第一个元素

        info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
        boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                             berth[targetBerth]->y, 0);
    } else {
        // 如果nextBerth为空，则打印错误或执行其他逻辑
        error << "Boat " << boat[i]->id << " has no berth target" << endl;
//        int target=boat[i]->find_nearest_delivery_id();
        int target=-1;
        if (boat[i]->y<50){
            target=0;
        }
        else if(boat[i]->y>150){
            target=1;
        }
        else{
            if(berth[1]->num>berth[2]->num){
                target=0;
            }
            else{
                target=1;
            }
        }
        boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                             delivery_point[target].second, 1);
    }
}


void schedule_map2_1(int i){
    if (boat[i]->goods_num == 0 && boat[i]->nextBerth.empty()) {
        int target = boat[i]->find_max_goods();
        if (target == 0) {
            boat[i]->nextBerth = {0, 1};
        } else if (target == 1) {
            if (berth[1]->num < 42) {
                boat[i]->nextBerth = {1, 5};
            } else {
                boat[i]->nextBerth = {1};
            }
        } else if (target == 2) {
            boat[i]->nextBerth = {2, 3};
        } else if (target == 3) {
            if (berth[4]->num + berth[5]->num > berth[1]->num) {
                boat[i]->nextBerth = {3, 4, 5};
            } else {
                boat[i]->nextBerth = {3, 1};
            }
        } else if (target == 4) {
            boat[i]->nextBerth = {4, 5};
        } else if (target == 5) {
            if (berth[1]->num < 42) {
                if (berth[4]->num > berth[1]->num)
                {
                    boat[i]->nextBerth = {4, 5};
                }
                else{
                    boat[i]->nextBerth = {5, 1};
                }
            } else {
                boat[i]->nextBerth = {5};
            }
        }
    }
    // 无论是否载有货物，都执行以下逻辑
    // 确保nextBerth不为空
    if (!boat[i]->nextBerth.empty()) {
        int targetBerth = boat[i]->nextBerth.front(); // 取出第一个元素为目标泊位
        if (berth[targetBerth]->is_locked == false) {
            // boat[i]->nextBerth.erase(boat[i]->nextBerth.begin()); // 从列表中删除第一个元素
            info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
            boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                 berth[targetBerth]->y, 0);
            // berth[targetBerth]->is_locked = true;
        } else {
            boat[i]->nextBerth.erase(boat[i]->nextBerth.begin()); // 从列表中删除第一个元素
        }

    } else {
        // 如果nextBerth为空，则打印错误或执行其他逻辑
        error << "Boat " << boat[i]->id << " has no berth target!!!" << endl;
        int target=boat[i]->find_nearest_delivery_id();
        boat[i]->nextBerth.clear();
        boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                             delivery_point[target].second, 1);
    }
}
void schedule_map2_2(int i){
    if (boat[i]->goods_num == 0 && boat[i]->nextBerth.empty()) {
        // vector<vector<int>> seqs = {{1, 4}, {0, 3, 2}};   // map1
        vector<vector<int>> seqs = {{0, 4, 1}, {2, 3, 5}};   // map2
        // vector<vector<int>> seqs = {{0}, {1}, {2}, {3}};   // map3
        int seq_id = boat[i]->find_max_seq(seqs);
        vector<int> seq = seqs[seq_id];
        if(boat_to_berth_hCost[seq[0]][boat[i]->x][boat[i]->y][boat[i]->dir] < boat_to_berth_hCost[seq[seq.size() - 1]][boat[i]->x][boat[i]->y][boat[i]->dir]) {
            boat[i]->nextBerth = seq;
        } else {
            reverse(seq.begin(), seq.end());
            boat[i]->nextBerth = seq;
        }
        /*  if (berthtarget==1||berthtarget==4){
              if (boat_to_berth_hCost[1][boat[i]->x][boat[i]->y][boat[i]->dir] < boat_to_berth_hCost[4][boat[i]->x][boat[i]->y][boat[i]->dir]) {
                  boat[i]->nextBerth = {1, 4};
              }
              else {
                  boat[i]->nextBerth = {4, 1};
              }
          }
          else{
              if (boat_to_berth_hCost[0][boat[i]->x][boat[i]->y][boat[i]->dir] < boat_to_berth_hCost[2][boat[i]->x][boat[i]->y][boat[i]->dir]) {
                  boat[i]->nextBerth = {0, 3, 2};
              }
              else{
                  boat[i]->nextBerth = {2, 3, 0};
              }
          }*/
    }

    // 无论是否载有货物，都执行以下逻辑
    // 确保nextBerth不为空
    if (!boat[i]->nextBerth.empty()) {
        int targetBerth = boat[i]->nextBerth.front(); // 取出第一个元素为目标泊位
        if (berth[targetBerth]->is_locked == false) {
            // boat[i]->nextBerth.erase(boat[i]->nextBerth.begin()); // 从列表中删除第一个元素
            info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
            boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                 berth[targetBerth]->y, 0);
            // berth[targetBerth]->is_locked = true;
        } else {
            boat[i]->nextBerth.erase(boat[i]->nextBerth.begin()); // 从列表中删除第一个元素
        }

    } else {
        // 如果nextBerth为空，则打印错误或执行其他逻辑
        error << "Boat " << boat[i]->id << " has no berth target!!!" << endl;
        int target=boat[i]->find_nearest_delivery_id();
        boat[i]->nextBerth.clear();
        boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                             delivery_point[target].second, 1);
    }
}
void schedule_map3_2(int i){
    if (boat[i]->goods_num == 0 && boat[i]->nextBerth.empty()) {
        // vector<vector<int>> seqs = {{1, 4}, {0, 3, 2}};   // map1
        // vector<vector<int>> seqs = {{0, 4, 1}, {2, 3, 5}};   // map2
        vector<vector<int>> seqs = {{0}, {1}, {2}, {3}};   // map3
        int seq_id = boat[i]->find_max_seq(seqs);
        vector<int> seq = seqs[seq_id];
        if(boat_to_berth_hCost[seq[0]][boat[i]->x][boat[i]->y][boat[i]->dir] < boat_to_berth_hCost[seq[seq.size() - 1]][boat[i]->x][boat[i]->y][boat[i]->dir]) {
            boat[i]->nextBerth = seq;
        } else {
            reverse(seq.begin(), seq.end());
            boat[i]->nextBerth = seq;
        }
        /*  if (berthtarget==1||berthtarget==4){
              if (boat_to_berth_hCost[1][boat[i]->x][boat[i]->y][boat[i]->dir] < boat_to_berth_hCost[4][boat[i]->x][boat[i]->y][boat[i]->dir]) {
                  boat[i]->nextBerth = {1, 4};
              }
              else {
                  boat[i]->nextBerth = {4, 1};
              }
          }
          else{
              if (boat_to_berth_hCost[0][boat[i]->x][boat[i]->y][boat[i]->dir] < boat_to_berth_hCost[2][boat[i]->x][boat[i]->y][boat[i]->dir]) {
                  boat[i]->nextBerth = {0, 3, 2};
              }
              else{
                  boat[i]->nextBerth = {2, 3, 0};
              }
          }*/
    }

    // 无论是否载有货物，都执行以下逻辑
    // 确保nextBerth不为空
    if (!boat[i]->nextBerth.empty()) {
        int targetBerth = boat[i]->nextBerth.front(); // 取出第一个元素为目标泊位
        if (berth[targetBerth]->is_locked == false) {
            // boat[i]->nextBerth.erase(boat[i]->nextBerth.begin()); // 从列表中删除第一个元素
            info << "Boat " << " 预备港口，准备前往 " << targetBerth << endl;
            boat[i]->go_mb_point(targetBerth, -1, berth[targetBerth]->x,
                                 berth[targetBerth]->y, 0);
            // berth[targetBerth]->is_locked = true;
        } else {
            boat[i]->nextBerth.erase(boat[i]->nextBerth.begin()); // 从列表中删除第一个元素
        }

    } else {
        // 如果nextBerth为空，则打印错误或执行其他逻辑
        error << "Boat " << boat[i]->id << " has no berth target!!!" << endl;
        int target=boat[i]->find_nearest_delivery_id();
        boat[i]->nextBerth.clear();
        boat[i]->go_mb_point(-1, target, delivery_point[target].first,
                             delivery_point[target].second, 1);
    }
}

void execute::execute_boat() {
    /*info << "execute_boat() begin" << endl;*/
    for (int i = 0; i < boat_num; ++i) {
        for (int j = 0; j < delivery_point.size(); j++) {
            int delivery_id = j;  // map2
            if (boat[i]->x == delivery_point[delivery_id].first && boat[i]->y == delivery_point[delivery_id].second) {
                boat[i]->goodsPrice = 0;
            }
        }
        if (boat[i]->mbx == -1 && boat[i]->mby == -1) {
            for (int j = 0; j < delivery_point.size(); j++) {
                if (boat[i]->x == delivery_point[j].first && boat[i]->y == delivery_point[j].second) {
                    boat[i]->goodsPrice = 0;
                }
            }
            if (boat[i]->status == 2) {
                info << "Boat " << boat[i]->id << " load status" << endl;
                int currentBerthId = boat[i]->getBerthIdByPoint();

                int toDeliverID = -1, mi = 1000, x = boat[i]->x, y = boat[i]->y, dir = boat[i]->dir;;
                for (int j = 0; j < delivery_point.size(); j++) {
                    if (boat_to_deliver_hCost[j][x][y][dir] < mi) {
                        mi = boat_to_deliver_hCost[j][x][y][dir];
                        toDeliverID = j;
                    }
                }
                /*info << "最近的送货点是" << toDeliverID << " 最终时刻是: "
                     << 15000 - boat_to_deliver_hCost[toDeliverID][x][y][dir] - 10 << endl;*/
                if (step > 15000 - boat_to_deliver_hCost[toDeliverID][x][y][dir] - 10 &&
                    boat[i]->berthBoard.size() <= boat_num) {
                    boat[i]->operation_list.push_back(DEPT_OP);
                } else if (boat[i]->goods_num >=
                           boat_capacity) {  // TODO 后续要封装成是否离开当前泊位的函数   boat[i]->goods_num == boat_capacity
                    boat[i]->operation_list.push_back(DEPT_OP);
                }
                else if (berth[currentBerthId]->num <= 0) {
                    if (step + 2 * boat_to_deliver_hCost[toDeliverID][x][y][dir] < 15000) {
                        boat[i]->operation_list.push_back(DEPT_OP);
                    }
                }
                else {
                    // 先执行船舶命令，再进行装载!!!!!!!,一旦发送回到主航道的指令后续无法装载
                    /*info << "Boat " << boat[i]->id << " loading" << endl;*/
                    int load_num = std::min(berth[currentBerthId]->num, std::min(berth[currentBerthId]->loading_speed,
                                                                                 boat_capacity - boat[i]->goods_num));
                    boat[i]->goods_num += load_num;
                    berth[currentBerthId]->num -= load_num;
                    for (int load_i = 0; load_i < load_num; load_i++) {
                        boat[i]->goodsPrice += berth[currentBerthId]->berthGoodsPrice[0];
                        berth[currentBerthId]->berthGoodsPrice.erase(berth[currentBerthId]->berthGoodsPrice.begin());
                    }
                }
            } else if (boat[i]->status == 0) {
                if (boat[i]->berthBoard.empty() && boat[i]->goods_num == 0) {
                    boat[i]->go_mb_point(boat[i]->id, -1, berth[boat[i]->id]->x, berth[boat[i]->id]->y, 0);
                    continue;
                }

                bool isCanBuyRobot = (robot_num >= 8 && robot_num < robot_max_num &&
                                      boat[i]->goodsPrice + money >= 4000);
                int berth_id = boat[i]->getBerthIdByPoint();
                int toDeliverID = -1, mi = 1000, x = boat[i]->x, y = boat[i]->y, dir = boat[i]->dir;;
                for (int j = 0; j < delivery_point.size(); j++) {
                    if (boat_to_deliver_hCost[j][x][y][dir] < mi) {
                        mi = boat_to_deliver_hCost[j][x][y][dir];
                        toDeliverID = j;
                    }
                }
                // TODO 有bug
                bool isFinnalTime = (step >= 15000 - 2 * boat_to_deliver_hCost[toDeliverID][x][y][dir]);

                // 去送货点
                // if(boat[i]->goods_num >= boat_capacity ||  step >= 14700 || isCanBuyRobot) {
                if (boat[i]->goods_num >= boat_capacity || isCanBuyRobot || isFinnalTime) {
                    if (isFinnalTime) error << "最终时刻:" << step << endl;
                    boat[i]->nextBerth.clear();
                    /*info << "Boat " << boat[i]->id << "的价值: " << boat[i]->goodsPrice << " 数量: "
                         << boat[i]->goods_num << endl;*/
                    int delivery_id = boat[i]->find_nearest_delivery_id();  // map2
                    /*info << "Boat " << boat[i]->id << " find nearest delivery id " << delivery_id << endl;*/
                    boat[i]->go_mb_point(-1, delivery_id, delivery_point[delivery_id].first,
                                         delivery_point[delivery_id].second, 1);
                }
                    // 去泊位
                else {
                    if (map_index == 0) {
                        int maxBerthId = boat[i]->find_max_goods();
                        boat[i]->mbBerthId = maxBerthId;
                        /*info << "Boat " << boat[i]->id << " find maxBerthId " << maxBerthId << endl;*/
                        if (maxBerthId != -1)
                            boat[i]->go_mb_point(maxBerthId, -1, berth[maxBerthId]->x, berth[maxBerthId]->y, 0);
                        else {
                            /*error << "船" << boat[i]->id << "没有找到目标泊位" << endl;*/
                            int delivery_id = boat[i]->find_nearest_delivery_id();
                            /*info << "Boat " << boat[i]->id << " find nearest delivery id " << delivery_id << endl;*/
                            boat[i]->go_mb_point(-1, delivery_id, delivery_point[delivery_id].first,
                                                 delivery_point[delivery_id].second, 1);
                        }
                        // schedule_map1_1(i);
                        // info << "Boat " << boat[i]->id << " schedule_map1_1  " << map_index << endl;
                        // schedule_map1_2(i);
                    } else if (map_index == 1) {
                        /*info << "Boat " << boat[i]->id << " schedule_map2_1  " << map_index << endl;*/
                        // schedule_map2_1(i);
                        int maxBerthId = boat[i]->find_max_goods();
                        boat[i]->mbBerthId = maxBerthId;
                        /*info << "Boat " << boat[i]->id << " find maxBerthId " << maxBerthId << endl;*/
                        if (maxBerthId != -1)
                            boat[i]->go_mb_point(maxBerthId, -1, berth[maxBerthId]->x, berth[maxBerthId]->y, 0);
                        else {
                            /*error << "船" << boat[i]->id << "没有找到目标泊位" << endl;*/
                            int delivery_id = boat[i]->find_nearest_delivery_id();
                            /*info << "Boat " << boat[i]->id << " find nearest delivery id " << delivery_id << endl;*/
                            boat[i]->go_mb_point(-1, delivery_id, delivery_point[delivery_id].first,
                                                 delivery_point[delivery_id].second, 1);
                        }
                        // schedule_map2_2(i);
                    } else if (map_index == 2) {
                        /*info << "Boat " << boat[i]->id << " schedule_map3_1  " << map_index << endl;*/
                        // schedule_map3_2(i);
                        schedule_map1_1(i);
                    } else {
                        int maxBerthId = boat[i]->find_max_goods();
                        boat[i]->mbBerthId = maxBerthId;
                        /*info << "Boat " << boat[i]->id << " find maxBerthId " << maxBerthId << endl;*/
                        if (maxBerthId != -1)
                            boat[i]->go_mb_point(maxBerthId, -1, berth[maxBerthId]->x, berth[maxBerthId]->y, 0);
                        else {
                            /*error << "船" << boat[i]->id << "没有找到目标泊位" << endl;*/
                            int delivery_id = boat[i]->find_nearest_delivery_id();
                            /*info << "Boat " << boat[i]->id << " find nearest delivery id " << delivery_id << endl;*/
                            boat[i]->go_mb_point(-1, delivery_id, delivery_point[delivery_id].first,
                                                 delivery_point[delivery_id].second, 1);
                        }
                        // error << "地图出错了" << endl;
                        // exit(-1);
                        //todo: 盲图的方案
                    }
                }
            }
        }
        /*info << "Boat " << boat[i]->id << " begin move" << endl;*/
        boat[i]->move();
        /*info << "Boat " << boat[i]->id << " move done" << endl;*/
        /*if (boat[i]->mbx == -1 && boat[i]->mby == -1) {
            // info  << "Boat " << boat[i]->id << " no mb point" << endl;
            if (boat[i]->status == 0){

                if (boat[i]->goods_num <= boat_capacity - 2) {   // TODO 后续需要封装成去泊位的函数
                    int maxBerthId = boat[i]->find_max_goods();
                    info << "Boat " << boat[i]->id << " find maxBerthId " << maxBerthId << endl;
                    if(maxBerthId != -1) boat[i]->go_mb_point(maxBerthId, -1, berth[maxBerthId]->x, berth[maxBerthId]->y,0);
                    else error << "船" << boat[i]->id << "没有找到目标泊位" << endl;
                } else if (boat[i]->goods_num > boat_capacity - 2) {  // TODO 后续需要封装成去交货点的函数

                    int delivery_id = i == 0 ? delivery_point.size() -1 : 0;  // map2
                    boat[i]->go_mb_point(-1,delivery_id, delivery_point[delivery_id].first, delivery_point[delivery_id].second, 1);
                }else  if (boat[i]->goods_num <= boat_capacity - 2 ) {   // TODO 后续需要封装成去泊位的函数

                    // TODO
                    // int maxBerthId = boat[i]->find_max_goods();

                    int maxBerthId = boat[i]->mbBerthId == -1 ? boat[i]->find_max_goods() : boat[i]->mbBerthId;
                    boat[i]->mbBerthId = maxBerthId;

                    info << "Boat " << boat[i]->id << " find maxBerthId " << maxBerthId << endl;
                    if(maxBerthId != -1) boat[i]->go_mb_point(maxBerthId, -1, berth[maxBerthId]->x, berth[maxBerthId]->y,0);
                    else error << "船" << boat[i]->id << "没有找到目标泊位" << endl;
                }
            }
            else if (boat[i]->status == 2) {
                info  << "Boat " << boat[i]->id << " loading" << endl;
                // TODO 是否离开港口判定。
                // TODO 离开情况1：船已经装满了，离开前往售卖点
                // TODO 离开情况2：快到15000帧了，提前离开前往售卖点以满足售卖时间
                // TODO 离开情况3：当前港口已经没有货物了，离开当前港口寻找下一个港口装载货物
                // TODO 离开情况4：如果船装载的货物价值满足某个值，提前离开以便更早购买新的机器人或船。（如开局要多买机器人，攒够2000或其倍数先离开一趟）

                int currentBerthId = boat[i]->getBerthIdByPoint();
                if (berth[currentBerthId]->num == 0 || boat[i]->goods_num >= boat_capacity - 1 || step >= 14700) {  // TODO 后续要封装成是否离开当前泊位的函数   boat[i]->goods_num == boat_capacity
                    boat[i]->operation_list.push_back(DEPT_OP);
                } else {
                    // 先执行船舶命令，再进行装载!!!!!!!,一旦发送回到主航道的指令后续无法装载
                    int load_num = std::min(berth[currentBerthId]->num, std::min(berth[currentBerthId]->loading_speed,
                                                                                 boat_capacity - boat[i]->goods_num));
                    boat[i]->goods_num += load_num;
                    berth[currentBerthId]->num -= load_num;

                    for(int load_i = 0; load_i < load_num; load_i++) {

                        berth[currentBerthId]->berthGoodsPrice.erase(berth[currentBerthId]->berthGoodsPrice.begin());
                    }
                }
            }
            else if (boat[i]->status == 1) {
                // 无法操作,pass
            }
        }*/
    }
}

void execute::execute_buy() {
     int isNeedBuyBoat = -1;
     int isNeedBuyRobot = -1;

     if (robot_num < 8) {
         isNeedBuyBoat = -1;
         isNeedBuyRobot = step % robot_purchase_point.size() ;  // TODO 买机器人,具体在哪买待确定
     }
     else if (money >= 8000) {
        if (boat_num < 1) {
            isNeedBuyBoat = 0;  // TODO 开局买一艘船,具体在哪买待确定
        } else if (robot_num >= robot_max_num &&  boat_num < boat_max_num) {
            isNeedBuyBoat = 0;  // TODO 后面买的船,具体在哪买待确定
        }
    }
    if(money >= 2000 && isNeedBuyBoat == -1 && robot_num < robot_max_num && robot_num >= 8) {
        isNeedBuyRobot = step % robot_purchase_point.size() ;  // TODO 买机器人,具体在哪买待确定
    }

    if(isNeedBuyBoat != -1) {
        buy->buy_boat(isNeedBuyBoat);
    }
    if (isNeedBuyRobot != -1) {
        buy->buy_robot(isNeedBuyRobot);
    }
}






