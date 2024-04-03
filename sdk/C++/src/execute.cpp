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
//                outFile << step << "时刻" <<  i << "号机器人没有移动" << endl;
            }
        }
        else {
            int dir = robot[i]->find_nearest_berth();
            if (dir != -1) {
                robot[i]->move(dir);
            }
//            else outFile << i << "号机器人没有移动" << endl;
        }
    }
}

void execute::execute_buy() {
//    if(boat_num < boat_max_num) buy->buy_boat(step % 2);
//    if(robot_num < robot_max_num) buy->buy_robot(0);

    // TODO 判断是否需要买船(后续封装成函数),返回值为-1表示不需要买船,返回0表示在第一个点买,返回1表示在第二个点买...
    // int isNeedBuyBoat = fun1();
    int isNeedBuyBoat = (money > 0 && boat_num < 1) ? step % 2 : -1;

    // TODO 判断是否需要买机器人(后续封装成函数),返回值为-1表示不需要买机器人,返回0表示在第一个点买,返回1表示在第二个点买...
    // int isNeedBuyRobot = fun2();
    int isNeedBuyRobot = (isNeedBuyBoat == -1 && money >= 2000) ? 0 : -1 ;

    if(isNeedBuyBoat != -1) {
        outFile << "Buy boat" << endl;
        buy->buy_boat(isNeedBuyBoat);   // TODO 买船,在那个泊位购买
    }
    if (isNeedBuyRobot != -1) {
        outFile << "Buy robot" << endl;
        buy->buy_robot(isNeedBuyRobot);       // TODO 买机器人

    }

}

void execute::execute_boat() {
    // TODO 船的执行逻辑,其中findMbPoint()函数和move()函数里最好考虑到BFS的避障和避撞。
    for (int i = 0; i < boat_num; ++i) {
        if (boat[i]->mbx == -1 && boat[i]->mby == -1) {
            boat[i]->set_mbp(berth[0]->x, berth[0]->y);
            // TODO 船没有目标，需要根据当前状态确定目标：1. 船找泊位。2. 船找出售点
            // TODO findMbPoint(boat[i]->id);
        }
        else {
            // TODO 船有目标，需要根据当前状态确定下一步行动：1. 船移动。2. 船装货。3. 船卸货
            // if 船在目标点
                // if 船在泊位
                    // TODO 船装货,更新泊位货物数量和船的数量(注意需要考虑船靠泊的时候的状态)
                    // if (装满了，或者达到某种需要走的条件)更新目标点(mbx=-1, mby=-1,mbdir=-1),并重新根据当前状态找寻目标findMbPoint(boat[i]->id);
                    //     boat[i]->mbx = -1;
                    //     boat[i]->mby = -1;
                    //     boat[i]->mbdir = -1;
                        // TODO findMbPoint(boat[i]->id);
                // else if 船在出售点
                    // 船卸货,并且更新目标点(mbx=-1, mby=-1, mbdir=-1),并重新根据当前状态找寻目标findMbPoint(boat[i]->id)
                    // boat[i]->mbx = -1;
                    // boat[i]->mby = -1;
                    // boat[i]->mbdir = -1;
                    // TODO findMbPoint(boat[i]->id);
        }
        // TODO 朝目标点移动,如果目标点为-1则不移动,如果当前位置和方向与目标点一致(x=mbx, y=mby, dir=mbdir则不移动)
        // TODO boat[i]->move();  // 船朝目标点移动: dept、berth、rot、ship
        outFile << "Boat " << boat[i]->id << " begin move" << endl;
        boat[i]->move();
        outFile << "Boat " << boat[i]->id << " move done" << endl;
    }
}



