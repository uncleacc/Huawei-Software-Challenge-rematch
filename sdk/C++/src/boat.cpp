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
}

Boat::Boat(int x, int y) {
    goods_num = 0;
    status = 0;
    id = boat_num - 1;
    dir = 0;
    mbx = -1;
    mby = -1;
    mbBerthId = -1;
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
                set_obstacle(now.x, now.y, now.dir, step + now.gCost);
                now = boat_node_path[now.x][now.y][now.dir];
                int pre_ts = now.gCost;
                if(pre_ts + 2 == cur_ts) {  // 经过了减速区
                    set_obstacle(now.x, now.y, now.dir, step + now.gCost + 1);
                }
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
            hCost = abs(nx + 2 * d[rt_dir].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby) ;
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
            hCost = abs(nx +  2 * d[rt_dir].x - mbx) + abs(ny +  2 * d[rt_dir].y - mby);
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
            hCost = abs(nx + 2 * d[now.dir].x - mbx) + abs(ny +  2 * d[now.dir].y - mby);
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
        set_obstacle(x, y, dir, step); // 船只没有动，设置当前位置为障碍物
    }
    if(!operation_list.empty()) {
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
        open_berth(getBerthIdByPoint());
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
 * 根据坐标点判断当前是在那个泊位上
 */
 int Boat::getBerthIdByPoint(){
      for (int i = 0; i < berth_num; i++) {
        if (berth[i]->klux <= x && berth[i]->kluy <= y
            && berth[i]->krdx >= x  && berth[i]->krdy>= y) {
            return i;
        }
    }
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
    int maxBerthId = berthBoard[0];
    int maxBerthGoodsNum = berth[maxBerthId]->num;
    for (int j = 1; j < berthBoard.size(); j++) {
        if (berth[berthBoard[j]]->num > maxBerthGoodsNum) {
            maxBerthId = berthBoard[j];
            maxBerthGoodsNum = berth[maxBerthId]->num;
        }
    }
    return maxBerthId;
}
