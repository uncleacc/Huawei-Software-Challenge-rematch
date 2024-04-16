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
    info << "mbx:" <<  mbx  << " mby:" << mby << endl;
    info << "x:" <<  x  << " y:" << y << endl;

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
            ) {

            mbx = now.x;
            mby = now.y;


            if (choice == 0 && locate_berth_area(now.x, now.y, berth_id)){
                operation_list.push_back(BERTH_OP);
                if (!nextBerth.empty()) {
                    info << berth_id << "号泊位已经被锁定" << endl;
                    berth[berth_id]->is_locked = true;
                    nextBerth.erase(find(nextBerth.begin(), nextBerth.end(), berth_id)); // 从列表中删除此泊位
                }
            }
            vector<int> op_debug;
            while (boat_node_path[now.x][now.y][now.dir].x != -1) {
                operation_list.push_back(get_operation(boat_node_path[now.x][now.y][now.dir].dir, now.dir));
                    op_debug.push_back(get_operation(boat_node_path[now.x][now.y][now.dir].dir, now.dir));
                int cur_ts = now.gCost;
                boat_set_obstacle(now.x, now.y, now.dir, step + now.gCost);
                int preX = now.x, preY = now.y, preDir = now.dir;
                now = boat_node_path[now.x][now.y][now.dir];
                int pre_ts = now.gCost;
                if(pre_ts + 2 == cur_ts) {  // 经过了减速区
                    boat_set_obstacle(preX, preY, preDir, step + now.gCost + 1);
                        op_debug.push_back(9);
                }
            }
            reverse(operation_list.begin(), operation_list.end());

            info << id << "号船找到了目标，操作序列长度如下:" << operation_list.size() << endl;
            reverse(op_debug.begin(), op_debug.end());
            int tmpX = now.x, tmpY = now.y, tmpDir = now.dir;
            for (int i = 0; i < op_debug.size(); i++) {
                info << step + i << ": " << get_operation_debug(op_debug[i]) << endl;
                if(op_debug[i] == FORWARD_OP) {
                    tmpX = tmpX + d[tmpDir].x;
                    tmpY = tmpY + d[tmpDir].y;
                }
                else if(op_debug[i] == CLOCKWISE_DIR_OP) {
                    tmpX = get_rotated_point(tmpX, tmpY, tmpDir, CLOCKWISE_DIR_OP).first;
                    tmpY = get_rotated_point(tmpX, tmpY, tmpDir, CLOCKWISE_DIR_OP).second;
                    tmpDir = get_clockwise(tmpDir);
                }
                else if(op_debug[i] == ANTICLOCKWISE_DIR_OP) {
                    tmpX = get_rotated_point(tmpX, tmpY, tmpDir, ANTICLOCKWISE_DIR_OP).first;
                    tmpY = get_rotated_point(tmpX, tmpY, tmpDir, ANTICLOCKWISE_DIR_OP).second;
                    tmpDir = get_anticlockwise(tmpDir);
                }
                info << step + i << ": " << tmpX << " " << tmpY << " " << tmpDir << endl;
            }

            if(choice == 0) {
                info << id << "号船要去" << berth_id << "号泊位" << endl;
                info << berth_id << "号泊位已经被锁定" << endl;
                berth[berth_id]->set_locked(true);
                mbBerthId = berth_id;
            }
            else if(choice == 1) info << id << "号船要去" << DeliveryPointId << "号交货点" << endl;

            info << id << "号船寻路花费了 " << time_end_debug(start_time) << " 毫秒,一共遍历了" << qCount << "个点"<< endl;
            return true;
        }



        int nx, ny, gCost, hCost, ndir;
        std::pair<int, int> rotated_xy;
        bool flag11 = true;
        bool flag2 = true;
        bool flag3 = false;
        vector<pair<int, int>> points;


        //顺时针旋转===========================================================================================
        flag11 = true;
        flag2 = true;
        flag3 = false;
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR_OP);

        ndir = get_clockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        gCost = now.gCost + 1;
        points = get_boat_loc(nx, ny, ndir);    //获取六个点

        flag11 = can_place_boat(points, step + gCost)
                && can_place_boat(points, step + gCost)
                && can_place_boat(points, step + now.gCost)
                && !boat_vis[nx][ny][ndir];
        flag3 = check_boat_can_loc_dir(points);
        if(check_boat_loc_slow(points)) { //从缓慢区出去的时候会停滞一帧
            gCost++;
            flag2 = can_place_boat(points, step + gCost) && !boat_vis[nx][ny][ndir];
        }

        if (flag11 && flag2 && flag3) {
            boat_vis[nx][ny][ndir] = true;
            if(choice == 0) hCost =  boat_to_berth_hCost[berth_id][nx][ny][ndir];
            else if(choice == 1) hCost = boat_to_deliver_hCost[DeliveryPointId][nx][ny][ndir];
            q.push({nx, ny, ndir,  gCost, hCost});
            boat_node_path[nx][ny][ndir] = now;

        }

        //逆时针旋转===========================================================================================
        flag11 = true;
        flag2 = true;
        flag3 = false;

        rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR_OP);
        ndir = get_anticlockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        gCost = now.gCost + 1;
        points = get_boat_loc(nx, ny, ndir);
        flag11 = can_place_boat(points, step + gCost)
                && can_place_boat(points, step + gCost)
                && can_place_boat(points, step + now.gCost)
                && !boat_vis[nx][ny][ndir];
        flag3 = check_boat_can_loc_dir(points);

        if(check_boat_loc_slow(points)) { //从缓慢区出去的时候会停滞一帧
            gCost++;
            flag2 = can_place_boat(points, step + gCost) && !boat_vis[nx][ny][ndir];
        }
        // if (can_place_boat(nx, ny, rt_dir, step + gCost) && !boat_vis[nx][ny][rt_dir]) {
        if (flag11 && flag2 && flag3) {
            boat_vis[nx][ny][ndir] = true;
            if(choice == 0) hCost =  boat_to_berth_hCost[berth_id][nx][ny][ndir];
            else if(choice == 1) hCost = boat_to_deliver_hCost[DeliveryPointId][nx][ny][ndir];
            q.push({nx, ny, ndir, gCost, hCost});
            boat_node_path[nx][ny][ndir] = now;

        }

        //前进===========================================================================================
        flag11 = true;
        flag2 = true;
        flag3 = false;
        nx = now.x + d[now.dir].x;
        ny = now.y + d[now.dir].y;
        ndir = now.dir;
        gCost = now.gCost + 1;
        points = get_boat_loc(nx, ny, ndir);

        flag11 = can_place_boat(points, step + gCost)
                && can_place_boat(points, step + gCost)
                && can_place_boat(points, step + now.gCost)
                && !boat_vis[nx][ny][ndir];
        flag3 = check_boat_can_loc_dir(points);

        if(check_boat_loc_slow(points)) { //从缓慢区出去的时候会停滞一帧
            gCost++;
            flag2 = can_place_boat(points, step + gCost) && !boat_vis[nx][ny][now.dir];
        }

        if (flag11 && flag2 && flag3) {
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
    }

    info << id << "号船寻路花费了 " << time_end_debug(start_time) << " 毫秒（且没有找到路径）,一共遍历了" << qCount<< endl;
    return false;
}

void Boat::move() {
    if(operation_list.empty()) {
        mbx = -1;
        mby = -1;
        warn << "Boat " << id << " " <<"船只轨迹为空" << endl;
        // boat_set_obstacle(x, y, dir, step); // 船只没有动，设置当前位置为障碍物
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
        vector<pair<int, int>> points = get_boat_loc(x, y, dir);
        for (int i = 0; i < boat_num; i++) {
            if(i != id) {
                vector<pair<int, int>> otherPoints = get_boat_loc(boat[i]->x, boat[i]->y, boat[i]->dir);
                bool flag = false;
                for(int j = 0; j < points.size(); j++) {
                    if(find(otherPoints.begin(), otherPoints.end(), points[j]) != otherPoints.end()) {
                        flag = true;
                        break;
                    }
                }
                if(flag) error << "船" << id << "在" << step << "帧发生了碰撞" << endl;
            }
        }

        info <<  "Boat " << id << " ship" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
    else if(op == ANTICLOCKWISE_DIR_OP) {
        printf("rot %d 1\n", id);
        rotated_xy = get_rotated_point(x, y, dir, ANTICLOCKWISE_DIR_OP);
        dir = get_anticlockwise(dir);
        x = rotated_xy.first;
        y = rotated_xy.second;

        vector<pair<int, int>> points = get_boat_loc(x, y, dir);
        for (int i = 0; i < boat_num; i++) {
            if(i != id) {
                vector<pair<int, int>> otherPoints = get_boat_loc(boat[i]->x, boat[i]->y, boat[i]->dir);
                bool flag = false;
                for(int j = 0; j < points.size(); j++) {
                    if(find(otherPoints.begin(), otherPoints.end(), points[j]) != otherPoints.end()) {
                        flag = true;
                        break;
                    }
                }
                if(flag) error << "船" << id << "在" << step << "帧发生了碰撞" << endl;
            }
        }
        info <<  "Boat " << id << " rot" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
    else if(op == CLOCKWISE_DIR_OP) {
        printf("rot %d 0\n", id);
        rotated_xy = get_rotated_point(x, y, dir, CLOCKWISE_DIR_OP);
        dir = get_clockwise(dir);
        x = rotated_xy.first;
        y = rotated_xy.second;

        vector<pair<int, int>> points = get_boat_loc(x, y, dir);
        for (int i = 0; i < boat_num; i++) {
            if(i != id) {
                vector<pair<int, int>> otherPoints = get_boat_loc(boat[i]->x, boat[i]->y, boat[i]->dir);
                bool flag = false;
                for(int j = 0; j < points.size(); j++) {
                    if(find(otherPoints.begin(), otherPoints.end(), points[j]) != otherPoints.end()) {
                        flag = true;
                        break;
                    }
                }
                if(flag) {
                    error << "船" << id << "在" << step << "帧发生了碰撞" << endl;
                    boat[i]->mbBerthId = -1;

                }
            }
        }
        info <<  "Boat " << id << " rot" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;

    }
    else if (op == DEPT_OP) {
        printf("dept %d\n", id);
        // open_berth(getBerthIdByPoint());
        berth[getBerthIdByPoint()]->set_locked(false);
        mbx = -1;
        mby = -1;
        info <<  "Boat " << id << " dept" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;

        info << "船只" << id << "离开了泊位" << getBerthIdByPoint() << "在" << step << "时刻，货物数量" << goods_num <<",泊位数量还有："<< berth[getBerthIdByPoint()]->num << endl;
        if(step >= finish_time[map_index]) {
            error << "关闭码头: 船只" << id << "在" << step << "时刻最后一次离开了泊位" << getBerthIdByPoint() << ",船上货物数量为：" <<goods_num <<",泊位数量还有："<< berth[getBerthIdByPoint()]->num << endl;
        }
        mbBerthId = -1;
        close_berth_at_time(finish_time[map_index], getBerthIdByPoint());
    }
    else if (op == BERTH_OP) {
        printf("berth %d\n", id);
        mbx = -1;
        mby = -1;
        info <<  "Boat " << id << " berth" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
}

/**
 * 根据坐标点判断当前是在那个泊位上(包含靠泊区)
 */
 int Boat::getBerthIdByPoint(){
    /*for (int i = 0; i < berth_num; i++) {
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
    if(berthBoard.empty()) {
        error << "泊位为空，标记两下" << endl;
        return -1; // 如果船哪个泊位都不能去
    }
    int maxBerthId = -1;
    int maxBerthGoodsNum = -1;
    for (int j = 0; j < berthBoard.size(); j++) {
        if (berth[berthBoard[j]]->num > maxBerthGoodsNum && !berth[berthBoard[j]]->is_locked) {
            maxBerthId = berthBoard[j];
            maxBerthGoodsNum = berth[maxBerthId]->num;
        }
    }
    return maxBerthId;
}

int Boat::find_max_seq(std::vector<std::vector<int>> seqs) {
    if(seqs.empty()) return -1; // 序列为空
    int mx = -1, res_id = -1, id = 0;
    for(auto &v : seqs) {

        int sum = 0;
        error << "泊位序列" << endl;
        for(int i = 0; i < v.size(); i++) {
            error << v[i] << endl;
            if(!berth[v[i]]->is_locked) sum += berth[v[i]]->num;
        }
        if(sum > mx) {
            mx = sum;
            res_id = id;
        }
        id++;
    }
    error << "最终选择的序列: " << res_id << endl;
    return res_id;
}

int Boat::find_nearest_delivery_id() {
    int res = 0;
    int dis = 1000;
    for (int i = 0; i < delivery_point.size(); i++) {
        if (boat_to_deliver_hCost[i][x][y][dir] < dis) {
            res = i;
            dis = boat_to_deliver_hCost[i][x][y][dir];
        }
    }
    if (res == -1) error << "find_nearest_delivery_id: " << -1 << endl;
    return res;
}
int Boat::find_nearest_berth_id(){
    int res = -1;
    int dis = 1000;
    for (int i = 0; i < berthBoard.size(); i++) {
        if (boat_to_berth_hCost[i][x][y][dir] < dis && boat_to_berth_hCost[i][x][y][dir]>0) {
            res = i;
            dis = boat_to_berth_hCost[i][x][y][dir];
        }
    }

    return res;
}

void Boat::close_berth(int berth_id) {
    remove_berth(berth_id);
}