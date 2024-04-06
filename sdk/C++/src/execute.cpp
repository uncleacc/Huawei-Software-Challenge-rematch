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


void execute::execute_boat() {
    for (int i = 0; i < boat_num; ++i) {
        if (boat[i]->mbx == -1 && boat[i]->mby == -1) {
             // info  << "Boat " << boat[i]->id << " no mb point" << endl;
            if (boat[i]->status == 0){
                // TODO 船没有目标，需要根据当前状态确定目标：1. 船找泊位。2. 船找出售点
                //  TODO 方案1：如果船没有装满,船去泊位列表的下一个泊位装载货物
                //  TODO 方案2：如果船没有装满，船去所有泊位中货物最多的泊位装载货物，并给予泊位的货物数量-船容量，其他船寻找货物最多泊位时可以不优先选他
                //  TODO 方案3：如果船没有装满，船去泊位列表中货物最多的泊位装载货物，主要起到一个分区的左右，节省船移动时间。
                if (boat[i]->goods_num <= boat_capacity - 2) {   // TODO 后续需要封装成去泊位的函数
                    int maxBerthId = boat[i]->find_max_goods();
                    info << "Boat " << boat[i]->id << " find maxBerthId " << maxBerthId << endl;
                    if(maxBerthId != -1) boat[i]->go_mb_point(maxBerthId, -1, berth[maxBerthId]->x, berth[maxBerthId]->y,0);
                    else error << "船" << boat[i]->id << "没有找到目标泊位" << endl;
                } else if (boat[i]->goods_num > boat_capacity - 2) {  // TODO 后续需要封装成去交货点的函数
                    // int delivery_id = i == 0 ? 0 : delivery_point.size() -1;  // map1
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
                }
            }
            else if (boat[i]->status == 1) {
                // 无法操作,pass
            }
        }
        info << "Boat " << boat[i]->id << " begin move" << endl;
        boat[i]->move();
        info << "Boat " << boat[i]->id << " move done" << endl;
    }
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
    // TODO 判断是否需要买船(后续封装成函数),返回值为-1表示不需要买船,返回0表示在第一个点买,返回1表示在第二个点买...
    //  TODO 判断是否需要买船，根据船的周期计算出船一趟可以运多少货物，根据机器人平均运货距离得到这个周期之内可以运输多少货物，如果超过该船的上限则增加船。
    // int isNeedBuyBoat = fun1();
    int isNeedBuyBoat = -1;
    int isNeedBuyRobot = -1;

    if (money >= 8000) {
        if (boat_num < 1) {
            isNeedBuyBoat = 0;  // TODO 开局买一艘船,具体在哪买待确定
        } else if (robot_num >= robot_max_num &&  boat_num < boat_max_num) {
            isNeedBuyBoat = boat_purchase_point.size() - 1;  // TODO 后面买的船,具体在哪买待确定
        }
    }
    // TODO 判断是否需要买机器人(后续封装成函数),返回值为-1表示不需要买机器人,返回0表示在第一个点买,返回1表示在第二个点买...
    // TODO 判断机器人运货比例，如果[1-（机器人平均运货价值/时间）/（货物平均生成价值 /时间）]*参数（例15000）>机器人价格 &&船没有达到运输能力的上限，购买新机器人
    //  TODO 还可以加几种不同的方式到时候试试。
    if(money >= 2000 && isNeedBuyBoat == -1 && robot_num < robot_max_num) {
        isNeedBuyRobot = step % robot_purchase_point.size() ;  // TODO 买机器人,具体在哪买待确定
    }

    if(isNeedBuyBoat != -1) {
        buy->buy_boat(0);
    }
    if (isNeedBuyRobot != -1) {
        info << isNeedBuyBoat <<" " << isNeedBuyRobot << endl;
        buy->buy_robot(isNeedBuyRobot);
    }
/*test    if(step == 1) {
        buy->buy_boat(true);
    }
    if(step > 1 && step < 99) {
        if(boat[0]->operation_list.empty()) boat[0]->go_mb_point(-1, -1, 111, 30, 2);
        boat[0]->move();
    }
    if(step == 100) {
        buy->buy_boat(true);
    }
    if(step > 100) {
        if(boat[0]->operation_list.empty()) boat[0]->go_mb_point(-1, -1, 111, 100, 2);
        boat[0]->move();
        if(boat[1]->operation_list.empty()) boat[1]->go_mb_point(-1, -1, 110, 10, 2);
        boat[1]->move();
    }*/
}






