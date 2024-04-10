#include "boat.h"
#include "globals.h"

Boat::Boat() {
    goods_num = 0;
    status = 0;
    id = boat_num - 1;
    x = -1;
    y = -1;
    dir = 0;
    mbx = -1;
    mby = -1;
    mbBerthId = -1;
    goodsPrice = 0;
    // stepCompensate = 0;
    goodsPrice = 0;
}

Boat::Boat(int x1, int y1) {
    goods_num = 0;
    status = 0;
    id = boat_num - 1;
    dir = 0;
    mbx = -1;
    mby = -1;

    x = x1;
    y = y1;
    goodsPrice = 0;
    // stepCompensate = 0;
}
// bool Boat::navigation() {
//     if (mbx == -1 || mby == -1) {
//         info << "Boat " << id << " has no target!!!" << endl;
//         return false;
//     }
//     if(x == mbx && y == mby) return false;
//     info << "Boat " << id << " is navigating from "  << x <<" " << " " << y << " to "<< mbx << " " << mby << endl;
//     operation_list.clear();
//
//     priority_queue<boat_sport_node> q;
//     q.push({x, y, dir, 0,0});
//     memset(boat_vis, 0, sizeof(boat_vis));
//     boat_vis[x][y][dir] = true;
//     memset(boat_node_path, -1, sizeof(boat_node_path));
//
//     while (!q.empty()) {
//         boat_sport_node now = q.top();
//         q.pop();
//
//         // info << now.x << " " << now.y << " " << now.dir << endl
//         if (now.x == mbx && now.y == mby) {
//             while (boat_node_path[now.x][now.y][now.dir].x != -1) {
//                 operation_list.push_back(get_operation(boat_node_path[now.x][now.y][now.dir].dir, now.dir));
//                 now = boat_node_path[now.x][now.y][now.dir];
//             }
//             reverse(operation_list.begin(), operation_list.end());
//
//             if (!operation_list.empty()){
//                 info << "移动轨迹：" ;
//                 for (int i = 0; i < operation_list.size(); i++) {
//                     info << operation_list[i] << " ";
//                 }
//                 info << endl;
//             }
//             info << "operation_list size: " << operation_list.size() << endl;
//             info << "now.x: " << now.x << " now.y: " << now.y << " now.dir: " << now.dir << endl;
//             return true;
//         }
//
//
//         int nx, ny, rt_dir;
//         std::pair<int, int> rotated_xy;
//
//         //顺时针旋转
//         rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR_OP);
//         rt_dir = get_clockwise(now.dir);
//         nx = rotated_xy.first; ny = rotated_xy.second;
//         if (can_place_boat(nx, ny, rt_dir) && !boat_vis[nx][ny][rt_dir]) {
//             boat_vis[nx][ny][rt_dir] = true;
//             if(check_boat_loc_slow(nx, ny, rt_dir)) {
//                 int gCost = now.gCost + 2;
//                 int hCost = abs(nx - mbx) + abs(ny - mby);
//                 q.push({nx, ny, rt_dir,  gCost, hCost});
//             }
//             else {
//                 int gCost = now.gCost + 1;
//                 int hCost = abs(nx - mbx) + abs(ny - mby);
//                 int fCost = gCost + hCost;
//                 q.push({nx, ny, rt_dir,  gCost, hCost});
//             }
//             boat_node_path[nx][ny][rt_dir] = {now.x, now.y, now.dir};
//         }
//
//         //逆时针旋转
//         rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR_OP);
//         rt_dir = get_anticlockwise(now.dir);
//         nx = rotated_xy.first; ny = rotated_xy.second;
//         if (can_place_boat(nx, ny, rt_dir) && !boat_vis[nx][ny][rt_dir]) {
//             boat_vis[nx][ny][rt_dir] = true;
//             if(check_boat_loc_slow(nx, ny, rt_dir)) {
//                 int gCost = now.gCost + 2;
//                 int hCost = abs(nx - mbx) + abs(ny - mby);
//                 int fCost = gCost + hCost;
//                 q.push({nx, ny, rt_dir, gCost, hCost });
//             }
//             else {
//                 int gCost = now.gCost + 1;
//                 int hCost = abs(nx - mbx) + abs(ny - mby);
//                 int fCost = gCost + hCost;
//                 q.push({nx, ny, rt_dir, gCost, hCost});
//             }
//             boat_node_path[nx][ny][rt_dir] = {now.x, now.y, now.dir};
//         }
//
//         //前进
//         nx = now.x + d[now.dir].x;
//         ny = now.y + d[now.dir].y;
//         if (can_place_boat(nx, ny, now.dir) && !boat_vis[nx][ny][now.dir]) {
//             boat_vis[nx][ny][now.dir] = true;
//             if(check_boat_loc_slow(nx, ny, now.dir)) {
//                 int gCost = now.gCost + 2;
//                 int hCost = abs(nx - mbx) + abs(ny - mby);
//                 int fCost = gCost + hCost;
//                 q.push({nx, ny, now.dir,  gCost, hCost});
//             }
//             else {
//                 int gCost = now.gCost + 1;
//                 int hCost = abs(nx - mbx) + abs(ny - mby);
//                 int fCost = gCost + hCost;
//                 q.push({nx, ny, now.dir,  gCost, hCost});
//             }
//             boat_node_path[nx][ny][now.dir] = {now.x, now.y, now.dir};
//         }
//
//     }
//     return false;
// }
/*
bool Boat::navigation() {
    if (mbx == -1 || mby == -1) {
        info << "Boat " << id << " has no target!!!" << endl;
        return false;
    }
    if(x == mbx && y == mby) return false;
    priority_queue<boat_sport_status> q;
    q.push({x, y, dir, 0});
    memset(boat_vis, 0, sizeof(boat_vis));
    boat_vis[x][y][dir] = true;
    memset(boat_path, -1, sizeof(boat_path));

    while(!q.empty()) {
        boat_sport_status now = q.top();
        q.pop();

        // info << now.x << " " << now.y << " " << now.dir << endl;

        if (now.x == mbx && now.y == mby) {

            while(boat_path[now.x][now.y][now.dir].x != -1) {
                operation_list.push_back(get_operation(boat_path[now.x][now.y][now.dir].dir, now.dir));
                now = boat_path[now.x][now.y][now.dir];
            }
            reverse(operation_list.begin(), operation_list.end());

            // info << "移动轨迹：" << endl;
            // for (int i = 0; i < operation_list.size(); i++) {
            //     info << operation_list[i] << " ";
            // }
            // info << endl;
            return true;
        }

        int nx, ny, rt_dir;
        std::pair<int, int> rotated_xy;

        //顺时针旋转
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR_OP);
        rt_dir = get_clockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        if (can_place_boat(nx, ny, rt_dir) && !boat_vis[nx][ny][rt_dir]) {
            boat_vis[nx][ny][rt_dir] = true;
            if(check_boat_loc_slow(nx, ny, rt_dir)) {
                q.push({nx, ny, rt_dir, now.dis + 2});
            }
            else {
                q.push({nx, ny, rt_dir, now.dis + 1});
            }
            boat_path[nx][ny][rt_dir] = {now.x, now.y, now.dir};
        }

        //逆时针旋转
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR_OP);
        rt_dir = get_anticlockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        if (can_place_boat(nx, ny, rt_dir) && !boat_vis[nx][ny][rt_dir]) {
            boat_vis[nx][ny][rt_dir] = true;
            if(check_boat_loc_slow(nx, ny, rt_dir)) {
                q.push({nx, ny, rt_dir, now.dis + 2});
            }
            else {
                q.push({nx, ny, rt_dir, now.dis + 1});
            }
            boat_path[nx][ny][rt_dir] = {now.x, now.y, now.dir};
        }

        //前进
        nx = now.x + d[now.dir].x;
        ny = now.y + d[now.dir].y;
        if (can_place_boat(nx, ny, now.dir) && !boat_vis[nx][ny][now.dir]) {
            boat_vis[nx][ny][now.dir] = true;
            if(check_boat_loc_slow(nx, ny, now.dir)) {
                q.push({nx, ny, now.dir, now.dis + 2});
            }
            else {
                q.push({nx, ny, now.dir, now.dis + 1});
            }
            boat_path[nx][ny][now.dir] = {now.x, now.y, now.dir};
        }
    }
    return true;
}
*/
void Boat::set_mbp(int x, int y) {
    mbx = x;
    mby = y;
}

/**
 * 找寻目标点，并将路径加入到boat的移动队列中
 * @param berth_id
 * @param DeliveryPointId
 * @param oriMbx : 初始目标点x，可能会因为泊位的靠泊区而更新
 * @param oriMby : 初始目标点y，可能会因为泊位的靠泊区而更新
 * @param choice : 0-berthID， 1:DeliveryPointId -1: 留作扩展
 * @return
 */
// bool Boat::go_mb_point(int berth_id, int DeliveryPointId, int oriMbx, int oriMby, int choice) {
//     if (choice == -1){
//         error << "findMbPoint出现问题" << endl;
//         return false;
//     }
//     if(x == oriMbx && y == oriMby) {
//         info << "Boat " << id << " is already at the target" << endl;
//         return false;
//     }
//     auto start_time = time_start_debug();
//     operation_list.clear();  // 清空当前路径
//     mbx = oriMbx;
//     mby = oriMby;
//
//     priority_queue<boat_sport_node> q;
//     q.push({x, y, dir, 0, 0});
//     memset(boat_vis, 0, sizeof(boat_vis));
//     boat_vis[x][y][dir] = true;
//     memset(boat_node_path, -1, sizeof(boat_node_path));
//     info << "go_mb_point" << endl;
//     while (!q.empty()) {
//         boat_sport_node now = q.top();
//         q.pop();
//
//         if ((now.x == mbx && now.y == mby)
//             || (choice == 0 && locate_berth_area(now.x, now.y, berth_id))) {
//
//             mbx = now.x;
//             mby = now.y;
//             // stepCompensate = 0;
//
//             if (choice == 0) {  // 如果选择的是泊位，最后一个指令是靠泊指令
//                 operation_list.push_back(BERTH_OP);
//             }
//             while (boat_node_path[now.x][now.y][now.dir].x != -1) {
//                 operation_list.push_back(get_operation(boat_node_path[now.x][now.y][now.dir].dir, now.dir));
//                 int cur_ts = now.gCost;
//                 boat_set_obstacle(now.x, now.y, now.dir, step + now.gCost);
//                 now = boat_node_path[now.x][now.y][now.dir];
//                 int pre_ts = now.gCost;
//                 if(pre_ts + 2 == cur_ts) // 经过了减速区
//                     boat_set_obstacle(now.x, now.y, now.dir, step + now.gCost + 1);
//             }
//             reverse(operation_list.begin(), operation_list.end());
//             info << id << "号船找到了目标，操作序列如下" << endl;
//             for (int i = 0; i < operation_list.size(); i++) {
//                 info << step + i << ": " << get_operation_debug(operation_list[i]) << endl;
//             }
//             if(choice == 0) {
//                 info << id << "号船要去" << berth_id << "号泊位" << endl;
//                 close_berth(berth_id);   //其他的船不可以去berth_id
//                 add_berth(berth_id);
//                 berth[berth_id]->set_locked(true);
//             }
//             else if(choice == 1) info << id << "号船要去" << DeliveryPointId << "号交货点" << endl;
//
//             info << id << "号船寻路花费了 " << time_end_debug(start_time) << " 毫秒" << endl;
//             return true;
//         }
//
//         if(check_boat_loc_slow(x, y, now.dir)) { //从缓慢区出去的时候会停滞一帧
//             now.gCost ++;
//         }
//
//         int nx, ny, rt_dir, gCost, hCost;
//         std::pair<int, int> rotated_xy;
//
//         //顺时针旋转
//         rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR_OP);
//         rt_dir = get_clockwise(now.dir);
//         nx = rotated_xy.first; ny = rotated_xy.second;
//         gCost = now.gCost + 1;
//         if (can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir]) {
//             boat_vis[nx][ny][rt_dir] = true;
//             if(choice == 0) {
//                 vector<pair<int, int>> ps = get_boat_loc(nx, ny, rt_dir);
//                 hCost = 0;
//                 // for(int i = 0; i < ps.size(); i++) {
//                 //     hCost += boat_to_berth_hCost[berth_id][ps[i].first][ps[i].second];
//                 // }
//             }
//             else if(choice == 1) {
//                 vector<pair<int, int>> ps = get_boat_loc(nx, ny, rt_dir);
//                 hCost = 0;
//                 // for(int i = 0; i < ps.size(); i++) {
//                 //     hCost += boat_to_deliver_hCost[DeliveryPointId][ps[i].first][ps[i].second];
//                 // }
//             }
//             q.push({nx, ny, rt_dir,  gCost, hCost});
//             boat_node_path[nx][ny][rt_dir] = now;
//         }
//
//         //逆时针旋转
//         rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR_OP);
//         rt_dir = get_anticlockwise(now.dir);
//         nx = rotated_xy.first; ny = rotated_xy.second;
//         gCost = now.gCost + 1;
//         if (can_place_boat(nx, ny, rt_dir, gCost + step) && !boat_vis[nx][ny][rt_dir]) {
//             boat_vis[nx][ny][rt_dir] = true;
//             if(choice == 0) {
//                 vector<pair<int, int>> ps = get_boat_loc(nx, ny, rt_dir);
//                 hCost = 0;
//                 // for(int i = 0; i < ps.size(); i++) {
//                 //     hCost += boat_to_berth_hCost[berth_id][ps[i].first][ps[i].second];
//                 // }
//             }
//             else if(choice == 1) {
//                 vector<pair<int, int>> ps = get_boat_loc(nx, ny, rt_dir);
//                 hCost = 0;
//                 // for(int i = 0; i < ps.size(); i++) {
//                 //     hCost += boat_to_deliver_hCost[DeliveryPointId][ps[i].first][ps[i].second];
//                 // }
//             }
//             q.push({nx, ny, rt_dir, gCost, hCost});
//             boat_node_path[nx][ny][rt_dir] = now;
//         }
//
//         //前进
//         nx = now.x + d[now.dir].x;
//         ny = now.y + d[now.dir].y;
//         gCost = now.gCost + 1;
//         if (can_place_boat(nx, ny, now.dir, gCost + step) && !boat_vis[nx][ny][now.dir]) {
//             boat_vis[nx][ny][now.dir] = true;
//             if(choice == 0) {
//                 vector<pair<int, int>> ps = get_boat_loc(nx, ny, rt_dir);
//                 hCost = 0;
//                 // for(int i = 0; i < ps.size(); i++) {
//                 //     hCost += boat_to_berth_hCost[berth_id][ps[i].first][ps[i].second];
//                 // }
//             }
//             else if(choice == 1) {
//                 vector<pair<int, int>> ps = get_boat_loc(nx, ny, rt_dir);
//                 hCost = 0;
//                 // for(int i = 0; i < ps.size(); i++) {
//                 //     hCost += boat_to_deliver_hCost[DeliveryPointId][ps[i].first][ps[i].second];
//                 // }
//             }
//             q.push({nx, ny, now.dir,  gCost, hCost});
//             boat_node_path[nx][ny][now.dir] = now;
//         }
//     }
//
//     info << id << "号船寻路花费了 " << time_end_debug(start_time) << " 毫秒（且没有找到路径）" << endl;
//     return false;
// }

/*bool Boat::go_mb_point(int berth_id, int DeliveryPointId, int oriMbx, int oriMby, int choice) {
    if (choice == -1){
        error << "findMbPoint出现问题" << endl;
        return false;
    }
    if(x == oriMbx && y == oriMby) {
        info << "Boat " << id << " is already at the target" << endl;
        return false;
    }
    auto start_time = time_start_debug();
    int qCount = 0;
    operation_list.clear();  // 清空当前路径
    mbx = oriMbx;
    mby = oriMby;

    priority_queue<boat_sport_node> q;
    q.push({x, y, dir, 0, 0});
    memset(boat_node_path, -1, sizeof(boat_node_path));

    memset(boat_dis, 0x3f, sizeof(boat_dis));

    info << "go_mb_point" << endl;
    boat_dis[x][y][dir] = 0;
    while (!q.empty()) {
        boat_sport_node now = q.top();
        q.pop();
        qCount ++;
        auto start_time_1 = time_start_debug();

        if ((now.x == mbx && now.y == mby)
            || (choice == 0 && locate_berth_area(now.x, now.y, berth_id))
            // || qCount >= 1000) {
            // || now.gCost >= 100) {
                ){

            mbx = now.x;
            mby = now.y;

            // stepCompensate = 0;


            if (choice == 0 && locate_berth_area(now.x, now.y, berth_id)){
                operation_list.push_back(BERTH_OP);
                mbBerthId = -1;
            }
            while (boat_node_path[now.x][now.y][now.dir].x != -1) {
                operation_list.push_back(get_operation(boat_node_path[now.x][now.y][now.dir].dir, now.dir));
                int cur_ts = now.gCost;
                boat_set_obstacle(now.x, now.y, now.dir, step + now.gCost);
                now = boat_node_path[now.x][now.y][now.dir];
                int pre_ts = now.gCost;

                if(pre_ts + 2 == cur_ts) // 经过了减速区
                    boat_set_obstacle(now.x, now.y, now.dir, step + now.gCost + 1);

            }
            reverse(operation_list.begin(), operation_list.end());
            info << id << "号船找到了目标，操作序列长度如下:" << operation_list.size() << endl;
            // for (int i = 0; i < operation_list.size(); i++) {
            //     info << step + i << ": " << get_operation_debug(operation_list[i]) << endl;
            // }
            if(choice == 0) {
                info << id << "号船要去" << berth_id << "号泊位" << endl;
                close_berth(berth_id);   //其他的船不可以去berth_id
                add_berth(berth_id);
                berth[berth_id]->set_locked(true);
            }
            else if(choice == 1) info << id << "号船要去" << DeliveryPointId << "号交货点" << endl;


            for(int i = 0; i < n; i ++) {
                for(int j = 0; j < n; j++) {
                    info << boat_dis[i][j][UP] << " ";
                }
                info << endl;
            }
            info << id << "号船寻路花费了 " << time_end_debug(start_time) << " 毫秒" << endl;
            return true;
        }



        int nx, ny, rt_dir, gCost, hCost, tmp_gCost = 0;
        std::pair<int, int> rotated_xy;
        bool flag11 = true;
        bool flag2 = true;



        if(check_boat_loc_slow(x, y, now.dir)) { //从缓慢区出去的时候会停滞一帧
            tmp_gCost = now.gCost + 1;
        }

        //顺时针旋转
        flag11 = true;
        flag2 = true;
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR_OP);

        rt_dir = get_clockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;

        gCost = tmp_gCost + 1;
        if (can_place_boat(nx, ny, rt_dir, step + gCost) && boat_dis[nx][ny][rt_dir] > gCost) {
            boat_dis[nx][ny][rt_dir] = gCost;

            q.push({nx, ny, rt_dir,  gCost, hCost});
            boat_node_path[nx][ny][rt_dir] = now;

        }

        //逆时针旋转
        flag11 = true;
        flag2 = true;
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR_OP);
        rt_dir = get_anticlockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;

        gCost = tmp_gCost + 1;
        if (can_place_boat(nx, ny, rt_dir, gCost + step) && boat_dis[nx][ny][rt_dir] > gCost) {
            boat_dis[nx][ny][rt_dir] = gCost;
            q.push({nx, ny, rt_dir, gCost, hCost});
            boat_node_path[nx][ny][rt_dir] = now;

        }

        //前进
        flag11 = true;
        flag2 = true;
        nx = now.x + d[now.dir].x;
        ny = now.y + d[now.dir].y;

        gCost = tmp_gCost + 1;
        if (can_place_boat(nx, ny, now.dir, gCost + step) && boat_dis[nx][ny][now.dir] > gCost) {
            boat_dis[nx][ny][now.dir] = gCost;
            q.push({nx, ny, now.dir,  gCost, hCost});
            boat_node_path[nx][ny][now.dir] = now;

        }
        // info << "一次遍历花费了" << time_end_debug(start_time_1) << "微秒" << endl;

    }

    info << id << "号船寻路花费了 " << time_end_debug(start_time) << " 毫秒（且没有找到路径）" << endl;
    return false;
}*/

/**
 * 找寻目标点，并将路径加入到boat的移动队列中
 * @param berth_id
 * @param DeliveryPointId
 * @param oriMbx : 初始目标点x，可能会因为泊位的靠泊区而更新
 * @param oriMby : 初始目标点y，可能会因为泊位的靠泊区而更新
 * @param choice : 0-berthID， 1:DeliveryPointId -1: 留作扩展
 * @return
 */
bool Boat::go_mb_point(int berth_id, int DeliveryPointId, int oriMbx, int oriMby, int choice) {
    if (choice == -1){
        error << "findMbPoint出现问题" << endl;
        return false;
    }
    if(x == oriMbx && y == oriMby) {
        info << "Boat " << id << " is already at the target" << endl;
        return false;
    }
    auto start_time = time_start_debug();
    int qCount = 0;
    operation_list.clear();  // 清空当前路径
    mbx = oriMbx;
    mby = oriMby;

    priority_queue<boat_sport_node> q;
    q.push({x, y, dir, 0, 0});
    memset(boat_vis, 0, sizeof(boat_vis));
    boat_vis[x][y][dir] = true;
    memset(boat_node_path, -1, sizeof(boat_node_path));

    info << "go_mb_point" << endl;
    while (!q.empty()) {
        boat_sport_node now = q.top();
        q.pop();
        qCount ++;
        auto start_time_1 = time_start_debug();

     /*   if(qCount > 1 && choice == 0) {
            error << "遍历的点：(" << now.x << " "<< now.y << "),dir: " << dir << ", 遍历次数：" << qCount <<",hCost1:" << boat_to_berth_hCost[berth_id][now.x][now.y][now.dir] << endl;
        }
        if(qCount > 1 && choice == 1) {
            error << "遍历的点：(" << now.x << " "<< now.y << "),dir: " << dir << ", 遍历次数：" << qCount <<",hCost1:" << boat_to_deliver_hCost[DeliveryPointId][now.x][now.y][now.dir] << endl;
        }*/
        if ((now.x == mbx && now.y == mby)
            || (choice == 0 && locate_berth_area(now.x, now.y, berth_id))
            // || qCount >= 1000) {
            // || now.gCost >= 100) {
            ) {

            mbx = now.x;
            mby = now.y;


            if (choice == 0 && locate_berth_area(now.x, now.y, berth_id)){
                operation_list.push_back(BERTH_OP);
            }
            while (boat_node_path[now.x][now.y][now.dir].x != -1) {
                operation_list.push_back(get_operation(boat_node_path[now.x][now.y][now.dir].dir, now.dir));
                int cur_ts = now.gCost;
                boat_set_obstacle(now.x, now.y, now.dir, step + now.gCost);
                int preX = now.x, preY = now.y, preDir = now.dir;
                now = boat_node_path[now.x][now.y][now.dir];
                int pre_ts = now.gCost;
                if(pre_ts + 2 == cur_ts) {  // 经过了减速区
                    boat_set_obstacle(preX, preY, preDir, step + now.gCost + 1);
                }
            }
            reverse(operation_list.begin(), operation_list.end());
            info << id << "号船找到了目标，操作序列长度如下:" << operation_list.size() << endl;
            // for (int i = 0; i < operation_list.size(); i++) {
            //     info << step + i << ": " << get_operation_debug(operation_list[i]) << endl;
            // }
            if(choice == 0) {
                info << id << "号船要去" << berth_id << "号泊位" << endl;
                // close_berth(berth_id);   //其他的船不可以去berth_id
                // add_berth(berth_id);
                berth[berth_id]->set_locked(true);
            }
            else if(choice == 1) info << id << "号船要去" << DeliveryPointId << "号交货点" << endl;

            info << id << "号船寻路花费了 " << time_end_debug(start_time) << " 毫秒,一共遍历了" << qCount << "个点"<< endl;
            return true;
        }



        int nx, ny, rt_dir, gCost, hCost;
        int ndir;
        std::pair<int, int> rotated_xy;
        bool flag11 = true;
        bool flag2 = true;


        //顺时针旋转
        flag11 = true;
        flag2 = true;
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR_OP);

        rt_dir = get_clockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        gCost = now.gCost + 1;
        flag11 = can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir];

        if(check_boat_loc_slow(nx, ny, rt_dir)) { //从缓慢区出去的时候会停滞一帧
            gCost++;
            flag2 = can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir];
        }
        // if (can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir]) {
        if (flag11 && flag2) {
            boat_vis[nx][ny][rt_dir] = true;
            // hCost = abs(nx + d[rt_dir].x - mbx) + abs(ny + d[rt_dir].y - mby);
            // hCost = abs(nx + 2 * d[rt_dir].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby) ;
            // hCost = abs(nx + 2 * d[rt_dir].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby)  + abs(nx + 2 * d[get_clockwise(rt_dir)].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby);
            // hCost = abs(nx + 2 * d[rt_dir].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby) ;
            if(choice == 0) hCost =  boat_to_berth_hCost[berth_id][nx][ny][rt_dir];
            else if(choice == 1) hCost = boat_to_deliver_hCost[DeliveryPointId][nx][ny][rt_dir];
            q.push({nx, ny, rt_dir,  gCost, hCost});
            boat_node_path[nx][ny][rt_dir] = now;

        }

        //逆时针旋转
        flag11 = true;
        flag2 = true;
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR_OP);
        rt_dir = get_anticlockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        gCost = now.gCost + 1;
        flag11 = can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir];

        if(check_boat_loc_slow(nx, ny, rt_dir)) { //从缓慢区出去的时候会停滞一帧
            gCost++;
            flag2 = can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir];
        }
        // if (can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir]) {
        if (flag11 && flag2) {
            boat_vis[nx][ny][rt_dir] = true;
            // hCost = abs(nx + d[rt_dir].x - mbx) + abs(ny + d[rt_dir].y - mby);
            // hCost = abs(nx +  2 * d[rt_dir].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby);
            // hCost = abs(nx + 2 * d[rt_dir].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby)  + abs(nx + 2 * d[get_clockwise(rt_dir)].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby);
            // hCost = abs(nx +  2 * d[rt_dir].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby);
            if(choice == 0) hCost =  boat_to_berth_hCost[berth_id][nx][ny][rt_dir];
            else if(choice == 1) hCost = boat_to_deliver_hCost[DeliveryPointId][nx][ny][rt_dir];
            q.push({nx, ny, rt_dir, gCost, hCost});
            boat_node_path[nx][ny][rt_dir] = now;

        }

        //前进
        flag11 = true;
        flag2 = true;
        nx = now.x + d[now.dir].x;
        ny = now.y + d[now.dir].y;
        ndir = now.dir;
        gCost = now.gCost + 1;
        flag11 = can_place_boat(nx, ny, ndir, step + gCost) && !boat_vis[nx][ny][now.dir];

        if(check_boat_loc_slow(nx, ny, ndir)) { //从缓慢区出去的时候会停滞一帧
            gCost++;
            flag2 = can_place_boat(nx, ny, ndir, step + gCost) && !boat_vis[nx][ny][now.dir];
        }
        // if (can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir]) {
        if (flag11 && flag2) {
            boat_vis[nx][ny][now.dir] = true;
            // hCost = abs(nx - mbx) + abs(ny - mby);
            // hCost = abs(nx + d[now.dir].x - mbx) + abs(ny + d[now.dir].y - mby);
            // hCost = abs(nx + 2 * d[now.dir].x - mbx) + abs(ny +  2 * d[now.dir].y - mby);
            // hCost = abs(nx + 2 * d[now.dir].x - mbx) + abs(ny +  2 * d[now.dir].y - mby)  + abs(nx + 2 * d[get_clockwise(now.dir)].x - mbx) + abs(ny +  2 * d[now.dir].y - mby);
            // hCost = abs(nx + 2 * d[now.dir].x - mbx) + abs(ny +  2 * d[now.dir].y - mby);
            if(choice == 0) hCost =  boat_to_berth_hCost[berth_id][nx][ny][now.dir];
            else if(choice == 1) hCost = boat_to_deliver_hCost[DeliveryPointId][nx][ny][now.dir];
            q.push({nx, ny, now.dir,  gCost, hCost});
            boat_node_path[nx][ny][now.dir] = now;
        }
        // info << "一次遍历花费了" << time_end_debug(start_time_1) << "微秒" << endl;

    }

    info << id << "号船寻路花费了 " << time_end_debug(start_time) << " 毫秒（且没有找到路径）" << endl;
    return false;
}

void Boat::move() {
    if(operation_list.empty()) {
        mbx = -1;
        mby = -1;
        warn << "Boat " << id << " " <<"船只轨迹为空" << endl;
        boat_set_obstacle(x, y, dir, step); // 船只没有动，设置当前位置为障碍物
    }
    else {
        int op = operation_list[0];
        exec(op);
    }
}

void Boat::exec(int op) {
    if(status == 1) {
        info << "Boat " << id << " 状态正在恢复中" << endl;
        return;
    }
    std::pair<int, int> rotated_xy;
    operation_list.erase(operation_list.begin());
    if(op == FORWARD_OP) {
        printf("ship %d\n", id);
        x += d[dir].x;
        y += d[dir].y;
        info <<  "Boat " << id << " ship" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
    else if(op == ANTICLOCKWISE_DIR_OP) {
        printf("rot %d 1\n", id);
        rotated_xy = get_rotated_point(x, y, dir, ANTICLOCKWISE_DIR_OP);
        dir = get_anticlockwise(dir);
        x = rotated_xy.first;
        y = rotated_xy.second;
        info <<  "Boat " << id << " rot" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
    else if(op == CLOCKWISE_DIR_OP) {
        printf("rot %d 0\n", id);
        rotated_xy = get_rotated_point(x, y, dir, CLOCKWISE_DIR_OP);
        dir = get_clockwise(dir);
        x = rotated_xy.first;
        y = rotated_xy.second;
        info <<  "Boat " << id << " rot" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;

    }
    else if (op == DEPT_OP) {
        printf("dept %d\n", id);
        // open_berth(getBerthIdByPoint());
        berth[getBerthIdByPoint()]->set_locked(false);
        mbx = -1;
        mby = -1;
        info <<  "Boat " << id << " dept" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
    else if (op == BERTH_OP) {
        printf("berth %d\n", id);
        mbx = -1;
        mby = -1;
        info <<  "Boat " << id << " berth" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
    if(other_boat_is_here_debug(x, y, dir)) {
        error << "Boat " << id << " 发生撞击" << endl;
    }
}

/**
 * 根据坐标点判断当前是在那个泊位上(包含靠泊区)
 */
 int Boat::getBerthIdByPoint(){
/*      for (int i = 0; i < berth_num; i++) {
        if (berth[i]->klux <= x && berth[i]->kluy <= y
            && berth[i]->krdx >= x  && berth[i]->krdy>= y) {
            return i;
        }
    }*/
    auto it1 = Berth2ID.find({x, y});
    auto it2 = KaoBerth2ID.find({x,y});
    if (it1 != Berth2ID.end()) return it1->second;
    if (it2 != KaoBerth2ID.end()) return it2->second;
    return -1;
 }

void Boat::add_berth(int berth_id) {
    if(berth_id < 0 || berth_id >= berth_num) {
        info << "Boat " << id << " add_berth error" << endl;
        return;
    }
    if(std::find(berthBoard.begin(), berthBoard.end(), berth_id) == berthBoard.end()) {
        berthBoard.push_back(berth_id);
    }
}

void Boat::remove_berth(int berth_id) {
    if(berth_id < 0 || berth_id >= berth_num) {
        info << "Boat " << id << " remove_berth error" << endl;
        return;
    }
    if(std::find(berthBoard.begin(), berthBoard.end(), berth_id) != berthBoard.end()) {
        berthBoard.erase(std::find(berthBoard.begin(), berthBoard.end(), berth_id));
    }
}

bool Boat::can_go_berth(int berth_id) {
    if(berth_id < 0 || berth_id >= berth_num) {
        info << "Boat " << id << " can_go_berth error" << endl;
        return false;
    }
    if(std::find(berthBoard.begin(), berthBoard.end(), berth_id) != berthBoard.end()) {
        return true;
    }
    return false;
}

int Boat::find_max_goods() {
    if(berthBoard.empty()) return -1; // 如果船哪个泊位都不能去
    int maxBerthId = -1;
    int maxBerthGoodsNum = 0;
    for (int j = 0; j < berthBoard.size(); j++) {
        if (berth[berthBoard[j]]->num > maxBerthGoodsNum && !berth[berthBoard[j]]->is_locked) {
            maxBerthId = berthBoard[j];
            maxBerthGoodsNum = berth[maxBerthId]->num;
        }
    }
    return maxBerthId;
}

int Boat::find_nearest_delivery_id() {
    int res = 0;
    int dis = boat_to_deliver_hCost[0][x][y][dir];
    if(id == 0) error << "find_nearest_delivery_id: " << dis << endl;
    for (int i = 1; i < delivery_point.size(); i++) {
        if (boat_to_deliver_hCost[i][x][y][dir] < dis) {
            res = i;
            dis = boat_to_deliver_hCost[i][x][y][dir];
        }
    }
    return res;
}