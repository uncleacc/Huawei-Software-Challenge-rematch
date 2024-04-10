//
// Created by HP on 2024/4/1.
//

#include "execute.h"
#include "globals.h"

execute::execute() {

}

execute::~execute() {

}

/*
void execute::execute_robot() {
    for (int i = 0; i < robot_num; i++) {
        // if(i < 6 && robot[i]->is_first_move) {
        //     robot[i]->mbx = 50;
        //     robot[i]->mby = 58;
        //     if(robot[i]->x == robot[i]->mbx && robot[i]->y == robot[i]->mby) {
        //         robot[i]->is_first_move = false;
        //     } else {
        //         int dir = robot[i]->goto_mbp();
        //         robot[i]->move(dir);
        //     }
        //     continue;
        // }
        if (robot[i]->goods == 0) {
            int res = robot[i]->get_good();
            if (res == 0) {
                int dir = 4;
                while(dir == 4) { // 需要重新搜索目标
                    dir = robot[i]->find_nearest_good();
                }
                if (dir != -1) {
                    robot[i]->move(dir);
                } else {

                }
                continue;
            }
        }
        // 拿到货物并且到达泊位
        if (robot[i]->goods == 1 && robot[i]->is_locate_Berth()) {
            robot[i]->put_good(step);  // 放下货物
            int dir = 4;
            while(dir == 4) { // 需要重新搜索目标
                dir = robot[i]->find_nearest_good();
            }
            if (dir != -1) {
                robot[i]->move(dir);
            }else {
//                info << step << "时刻" <<  i << "号机器人没有移动" << endl;
            }
        }
        else {
            int dir = robot[i]->find_nearest_berth();
            if (dir != -1) {
                robot[i]->move(dir);
            }
//            else info << i << "号机器人没有移动" << endl;
        }
    }
}
*/

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

                    error << "find goods begin" << endl;
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
                    error << "find goods end" << endl;

                }


                // info << "set_goods_for_mb begin" << endl;
                // info << "robot[" << i << "]->mbx:" << robot[i]->mbx << " robot[" << i << "]->mby:" << robot[i]->mby << endl;
                // robot[i]->set_goods_for_mb();
                // info << "robot[" << i << "]->mbx:" << robot[i]->mbx << " robot[" << i << "]->mby:" << robot[i]->mby << endl;
                // info << "set_goods_for_mb end" << endl;

                // error << "find goods begin" << endl;
                // bool flag = robot[i]->find_mbGoodsPoint();
                // if (flag == false) {
                //     robot[i]->mbx = -1;
                //     robot[i]->mby = -1;
                // }
                // error << "find goods end" << endl;
            }
            else if (robot[i]->goods == 1 ) {

                info << "set_berth_for_mb begin" << endl;
                info << "robot[" << i << "]->mbx:" << robot[i]->mbx << " robot[" << i << "]->mby:" << robot[i]->mby << endl;
                int target_berthId = robot[i]->set_berth_for_mb();
                info << "robot[" << i << "]->mbx:" << robot[i]->mbx << " robot[" << i << "]->mby:" << robot[i]->mby << endl;
                info << "set_berth_for_mb end" << endl;

                error << "find berth begin" << endl;
                robot[i]->find_mbBerthPoint(target_berthId);
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

void execute::execute_boat() {
    for (int i = 0; i < boat_num; ++i) {
        if (boat[i]->mbx == -1 && boat[i]->mby == -1) {
            for (int j = 0; j < delivery_point.size(); j++) {
                if (boat[i]->x == delivery_point[j].first && boat[i]->y == delivery_point[j].second) {
                    boat[i]->goodsPrice = 0;
                }
            }
            if (boat[i]->status == 2) {
                info  << "Boat " << boat[i]->id << " loading" << endl;
                int currentBerthId = boat[i]->getBerthIdByPoint();
                if (berth[currentBerthId]->num <= 0 || boat[i]->goods_num >= boat_capacity || step >= 14700) {  // TODO 后续要封装成是否离开当前泊位的函数   boat[i]->goods_num == boat_capacity
                    boat[i]->operation_list.push_back(DEPT_OP);
                } else {
                    // 先执行船舶命令，再进行装载!!!!!!!,一旦发送回到主航道的指令后续无法装载
                    int load_num = std::min(berth[currentBerthId]->num, std::min(berth[currentBerthId]->loading_speed, boat_capacity - boat[i]->goods_num));
                    boat[i]->goods_num += load_num;
                    berth[currentBerthId]->num -= load_num;
                    for(int load_i = 0; load_i < load_num; load_i++) {
                        boat[i]->goodsPrice += berth[currentBerthId]->berthGoodsPrice[0];
                        berth[currentBerthId]->berthGoodsPrice.erase(berth[currentBerthId]->berthGoodsPrice.begin());
                    }
                }
            }
            else if(boat[i]->status == 0) {
                bool isCanBuyRobot = (robot_num >= 8 && robot_num < robot_max_num && boat[i]->goodsPrice + money >= 2000);
                // 去送货点
                if(boat[i]->goods_num >= boat_capacity ||  step >= 14700 || isCanBuyRobot) {
                    info << "Boat " << boat[i]->id << "的价值: " << boat[i]->goodsPrice << " 数量: " << boat[i]->goods_num << endl;
                    int delivery_id = boat[i]->find_nearest_delivery_id();  // map2
                    info <<  "Boat " << boat[i]->id << " find nearest delivery id " << delivery_id << endl;
                    boat[i]->go_mb_point(-1,delivery_id, delivery_point[delivery_id].first, delivery_point[delivery_id].second, 1);
                }
                    // 去泊位
                else {
                    int maxBerthId = boat[i]->find_max_goods();
                    info << "Boat " << boat[i]->id << " find maxBerthId " << maxBerthId << endl;
                    if(maxBerthId != -1) boat[i]->go_mb_point(maxBerthId, -1, berth[maxBerthId]->x, berth[maxBerthId]->y,0);
                    else error << "船" << boat[i]->id << "没有找到目标泊位" << endl;                }
            }
        }
        info << "Boat " << boat[i]->id << " begin move" << endl;
        boat[i]->move();
        info << "Boat " << boat[i]->id << " move done" << endl;
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
                    int load_num = std::min(berth[currentBerthId]->num, std::min(berth[currentBerthId]->loading_speed, boat_capacity - boat[i]->goods_num));
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
    // for (int i = 0; i < boat_num; ++i) {
    //     info << "Boat " << boat[i]->id << " begin move" << endl;
    //     boat[i]->move();
    //     info << "Boat " << boat[i]->id << " move done" << endl;
    // }

    /*
    for (int i = 0; i < boat_num; ++i) {
        if (boat[i]->mbx == -1 && boat[i]->mby == -1) {
            info << "Boat " << boat[i]->id << " no mb point" << endl;
            // TODO 船没有目标，需要根据当前状态确定目标：1. 船找泊位。2. 船找出售点
            if (boat[i]->goods_num == 0) {
                info << "Boat " << boat[i]->id << " no goods" << endl;
                boat[i]->findMbBerthPointById(berth[0]->id);
            } else if (boat[i]->goods_num == boat_capacity) {
                info << "Boat " << boat[i]->id << " full" << endl;
                boat[i]->set_mbp(delivery_point[0].first, delivery_point[0].second);               // TODO go_mb_point(boat[i]->id);
            }

        }
        else {
            // TODO 船有目标，需要根据当前状态确定下一步行动：1. 船移动。2. 船装货。3. 船卸货
            // if (boat[i]->x == boat[i]->mbx && boat[i]->y == boat[i]->mby) {  // 船在目标点
            if (boat[i]->x >= berth[0]->klux &&  boat[i]->x <= berth[0]->krdx
                && boat[i]->y >= berth[0]->kluy && boat[i]->y <= berth[0]->krdy) {  // 船在目标点
                info << "Boat " << boat[i]->id << " in mb point" << endl;
                if (boat[i]->x >= berth[0]->klux &&  boat[i]->x <= berth[0]->krdx
                && boat[i]->y >= berth[0]->kluy && boat[i]->y <= berth[0]->krdy) {  // 船在泊位 TODO 判断
                    info << "Boat " << boat[i]->id << " in berth" << endl;
                    if (boat[i]->status == 0 && boat[i]->goods_num == 0) {
                        info << "Boat " << boat[i]->id << " exec berth op" << endl;
                        boat[i]->exec(BERTH_OP);
                    }
                    else if (boat[i]->status == 2){
                        int load_num = std::min(berth[0]->num,   std::min(berth[0]->loading_speed, boat_capacity - boat[i]->goods_num));
                        if (berth[0]->num != 0) {
                            info << "Berth " << berth[0]->id << " load " << load_num << " goods" << endl;
                        } else {
                            info << "Berth " << berth[0]->id << " load nothing" << endl;
                        }
                        if (load_num != 0) {
                            info << "Boat " << boat[i]->id << " load " << load_num << " goods" << endl;
                        } else{
                            info << "Boat " << boat[i]->id << " load nothing" << endl;
                        }
                        boat[i]->goods_num += load_num;
                        berth[0]->num -= load_num;
                        if (boat[i]->goods_num == 10) {
                            info <<  "Boat " << boat[i]->id << " exec dept op" << endl;
                            boat[i]->exec(DEPT_OP);
                        }
                    }
                    else if(boat[i]->status == 0 &&  boat[i]->goods_num >= 5){
                        boat[i]->set_mbp(delivery_point[0].first, delivery_point[0].second);
                        boat[i]->navigation();
                    }
                    // TODO 判断泊位是否有货物 printf("berth 0\n");

                    // TODO 船装货,更新泊位货物数量和船的数量(注意需要考虑船靠泊的时候的状态)


                } else if(boat[i]->x == delivery_point[0].first && boat[i]->y == delivery_point[0].second){ // 船在交货点  TODO 判断
                    info << "Boat " << boat[i]->id << " in delivery point" << endl;
                    boat[i]->goods_num = 0;
                    boat[i]->mbx = -1;
                    boat[i]->mby = -1;
                    boat[i]->mbdir = -1;
                }
            }
            // if 船在目标点
                // if 船在泊位
                    // TODO 船装货,更新泊位货物数量和船的数量(注意需要考虑船靠泊的时候的状态)
                    // if (装满了，或者达到某种需要走的条件)更新目标点(mbx=-1, mby=-1,mbdir=-1),并重新根据当前状态找寻目标findMbPoint(boat[i]->id);
                    //     boat[i]->mbx = -1;
                    //     boat[i]->mby = -1;
                    //     boat[i]->mbdir = -1;
                        // TODO go_mb_point(boat[i]->id);
                // else if 船在出售点
                    // 船卸货,并且更新目标点(mbx=-1, mby=-1, mbdir=-1),并重新根据当前状态找寻目标findMbPoint(boat[i]->id)
                    // boat[i]->mbx = -1;
                    // boat[i]->mby = -1;
                    // boat[i]->mbdir = -1;
                    // TODO go_mb_point(boat[i]->id);
        }

        // TODO 朝目标点移动,如果目标点为-1则不移动,如果当前位置和方向与目标点一致(x=mbx, y=mby, dir=mbdir则不移动)
        // TODO boat[i]->move();  // 船朝目标点移动: dept、berth、rot、ship
        info << "Boat " << boat[i]->id << " begin move" << endl;
        boat[i]->move();
        info << "Boat " << boat[i]->id << " move done" << endl;
    }
     */
}
/*
void execute::execute_boat() {
    // TODO 船的执行逻辑,其中findMbPoint()函数和move()函数里最好考虑到BFS的避障和避撞。
    for (int i = 0; i < boat_num; ++i) {
        if (boat[i]->mbx == -1 && boat[i]->mby == -1) {
            // TODO 船没有目标，需要根据当前状态确定目标：1. 船找泊位。2. 船找出售点
            if (boat[i]->goods_num == 0) {


                boat[i]->set_mbp(berth[0]->x, berth[0]->y);               // TODO go_mb_point(boat[i]->id);



            } else if (boat[i]->goods_num == boat_capacity) {
                boat[i]->set_mbp(delivery_point[0].first, delivery_point[0].second);               // TODO go_mb_point(boat[i]->id);
            }

        }
        else {
            // TODO 船有目标，需要根据当前状态确定下一步行动：1. 船移动。2. 船装货。3. 船卸货
            if (boat[i]->x == boat[i]->mbx && boat[i]->y == boat[i]->mby) {  // 船在目标点
                info << "Boat " << boat[i]->id << " in mb point" << endl;
                if (boat[i]->x == berth[0]->x && boat[i]->y == berth[0]->y) {  // 船在泊位 TODO 判断
                    info << "Boat " << boat[i]->id << " in berth" << endl;

                    // TODO 判断泊位是否有货物 printf("berth 0\n");

                    // TODO 船装货,更新泊位货物数量和船的数量(注意需要考虑船靠泊的时候的状态)
                    int load_num = std::min(berth[0]->num,   std::min(berth[0]->loading_speed, boat_capacity - boat[i]->goods_num));
                    if (berth[0]->num != 0) {
                        info << "Berth " << berth[0]->id << " load " << load_num << " goods" << endl;
                    } else {
                        info << "Berth " << berth[0]->id << " load nothing" << endl;
                    }
                    if (load_num != 0) {
                        info << "Boat " << boat[i]->id << " load " << load_num << " goods" << endl;
                    } else{
                        info << "Boat " << boat[i]->id << " load nothing" << endl;
                    }
                    boat[i]->goods_num += load_num;
                    berth[0]->num -= load_num;
                    if (boat[i]->goods_num == 10) {
                        info << "Boat " << boat[i]->id << " full" << endl;
                        boat[i]->mbx = -1;
                        boat[i]->mby = -1;
                        boat[i]->mbdir = -1;
                        boat[i]->set_mbp(delivery_point[0].first, delivery_point[0].second);    // TODO go_mb_point(boat[i]->id);
                    }

                } else if(boat[i]->x == delivery_point[0].first && boat[i]->y == delivery_point[0].second){ // 船在交货点  TODO 判断
                    info << "Boat " << boat[i]->id << " in delivery point" << endl;
                    boat[i]->goods_num = 0;
                    boat[i]->mbx = -1;
                    boat[i]->mby = -1;
                    boat[i]->mbdir = -1;

                    boat[i]->set_mbp(berth[0]->x, berth[0]->y);    // TODO go_mb_point(boat[i]->id);
                }
            }
            // if 船在目标点
                // if 船在泊位
                    // TODO 船装货,更新泊位货物数量和船的数量(注意需要考虑船靠泊的时候的状态)
                    // if (装满了，或者达到某种需要走的条件)更新目标点(mbx=-1, mby=-1,mbdir=-1),并重新根据当前状态找寻目标findMbPoint(boat[i]->id);
                    //     boat[i]->mbx = -1;
                    //     boat[i]->mby = -1;
                    //     boat[i]->mbdir = -1;
                        // TODO go_mb_point(boat[i]->id);
                // else if 船在出售点
                    // 船卸货,并且更新目标点(mbx=-1, mby=-1, mbdir=-1),并重新根据当前状态找寻目标findMbPoint(boat[i]->id)
                    // boat[i]->mbx = -1;
                    // boat[i]->mby = -1;
                    // boat[i]->mbdir = -1;
                    // TODO go_mb_point(boat[i]->id);
        }

        // TODO 朝目标点移动,如果目标点为-1则不移动,如果当前位置和方向与目标点一致(x=mbx, y=mby, dir=mbdir则不移动)
        // TODO boat[i]->move();  // 船朝目标点移动: dept、berth、rot、ship
        info << "Boat " << boat[i]->id << " begin move" << endl;
        boat[i]->move();
        info << "Boat " << boat[i]->id << " move done" << endl;
    }
}
 */

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






