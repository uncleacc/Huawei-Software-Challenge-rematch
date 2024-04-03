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
    mbdir = -1;

}

Boat::Boat(int x, int y) {
    goods_num = 0;
    status = 0;
    id = boat_num - 1;
    dir = 0;
    mbx = -1;
    mby = -1;
    mbdir = -1;
}
bool Boat::navigation() {
    if (mbx == -1 || mby == -1) {
        outFile << "Boat " << id << " has no target!!!" << endl;
        return false;
    }
    if(x == mbx && y == mby) return false;
    outFile << "Boat " << id << " is navigating from "  << x <<" " << " " << y << " to "<< mbx << " " << mby << endl;
    operation_list.clear();

    priority_queue<boat_sport_node> q;
    q.push({x, y, dir, 0,0});
    memset(boat_vis, 0, sizeof(boat_vis));
    boat_vis[x][y][dir] = true;
    memset(boat_node_path, -1, sizeof(boat_node_path));

    while (!q.empty()) {
        boat_sport_node now = q.top();
        q.pop();

        // outFile << now.x << " " << now.y << " " << now.dir << endl
        if (now.x == mbx && now.y == mby) {
            while (boat_node_path[now.x][now.y][now.dir].x != -1) {
                operation_list.push_back(get_operation(boat_node_path[now.x][now.y][now.dir].dir, now.dir));
                now = boat_node_path[now.x][now.y][now.dir];
            }
            reverse(operation_list.begin(), operation_list.end());

            if (!operation_list.empty()){
                outFile << "移动轨迹：" ;
                for (int i = 0; i < operation_list.size(); i++) {
                    outFile << operation_list[i] << " ";
                }
                outFile << endl;
            }
            outFile << "operation_list size: " << operation_list.size() << endl;
            outFile << "now.x: " << now.x << " now.y: " << now.y << " now.dir: " << now.dir << endl;
            return true;
        }


        int nx, ny, rt_dir;
        std::pair<int, int> rotated_xy;

        //顺时针旋转
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR);
        rt_dir = get_clockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        if (can_place_boat(nx, ny, rt_dir) && !boat_vis[nx][ny][rt_dir]) {
            boat_vis[nx][ny][rt_dir] = true;
            if(check_boat_loc_slow(nx, ny, rt_dir)) {
                int gCost = now.gCost + 2;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                q.push({nx, ny, rt_dir,  gCost, hCost});
            }
            else {
                int gCost = now.gCost + 1;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, rt_dir,  gCost, hCost});
            }
            boat_node_path[nx][ny][rt_dir] = {now.x, now.y, now.dir};
        }

        //逆时针旋转
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR);
        rt_dir = get_anticlockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        if (can_place_boat(nx, ny, rt_dir) && !boat_vis[nx][ny][rt_dir]) {
            boat_vis[nx][ny][rt_dir] = true;
            if(check_boat_loc_slow(nx, ny, rt_dir)) {
                int gCost = now.gCost + 2;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, rt_dir, gCost, hCost });
            }
            else {
                int gCost = now.gCost + 1;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, rt_dir, gCost, hCost});
            }
            boat_node_path[nx][ny][rt_dir] = {now.x, now.y, now.dir};
        }

        //前进
        nx = now.x + d[now.dir].x;
        ny = now.y + d[now.dir].y;
        if (can_place_boat(nx, ny, now.dir) && !boat_vis[nx][ny][now.dir]) {
            boat_vis[nx][ny][now.dir] = true;
            if(check_boat_loc_slow(nx, ny, now.dir)) {
                int gCost = now.gCost + 2;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, now.dir,  gCost, hCost});
            }
            else {
                int gCost = now.gCost + 1;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, now.dir,  gCost, hCost});
            }
            boat_node_path[nx][ny][now.dir] = {now.x, now.y, now.dir};
        }

    }
}
/*
bool Boat::navigation() {
    if (mbx == -1 || mby == -1) {
        outFile << "Boat " << id << " has no target!!!" << endl;
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

        // outFile << now.x << " " << now.y << " " << now.dir << endl;

        if (now.x == mbx && now.y == mby) {

            while(boat_path[now.x][now.y][now.dir].x != -1) {
                operation_list.push_back(get_operation(boat_path[now.x][now.y][now.dir].dir, now.dir));
                now = boat_path[now.x][now.y][now.dir];
            }
            reverse(operation_list.begin(), operation_list.end());

            // outFile << "移动轨迹：" << endl;
            // for (int i = 0; i < operation_list.size(); i++) {
            //     outFile << operation_list[i] << " ";
            // }
            // outFile << endl;
            return true;
        }

        int nx, ny, rt_dir;
        std::pair<int, int> rotated_xy;

        //顺时针旋转
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR);
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
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR);
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
 * @param choice : 0-berthID， 1:DeliveryPointId， -1: 留作扩展
 * @return
 */
bool Boat::findMbPoint(int berth_id, int DeliveryPointId, int oriMbx, int oriMby, int choice) {
    if (choice == -1){
        outFile << "ERROR-findMbPoint出现问题" << endl;
        return false;
    }
    operation_list.clear();  // 清空当前路径
    mbx = oriMbx;
    mby = oriMby;

    priority_queue<boat_sport_node> q;
    q.push({x, y, dir, 0,0});
    memset(boat_vis, 0, sizeof(boat_vis));
    boat_vis[x][y][dir] = true;
    memset(boat_node_path, -1, sizeof(boat_node_path));
    while (!q.empty()) {
        boat_sport_node now = q.top();
        q.pop();
        if ((now.x == mbx && now.y == mby) || (choice == 0 && checkBerthingArea(now.x, now.y, berth_id))) {
            mbx = now.x;
            mby = now.y;
            if (choice == 0) {  // 如果选择的是泊位，最后一个指令是靠泊指令
                operation_list.push_back(BERTH_OP);
            }
            while (boat_node_path[now.x][now.y][now.dir].x != -1) {
                operation_list.push_back(get_operation(boat_node_path[now.x][now.y][now.dir].dir, now.dir));
                now = boat_node_path[now.x][now.y][now.dir];
            }
            reverse(operation_list.begin(), operation_list.end());
            outFile << "operation_list size: " << operation_list.size() << endl;
            outFile << "now.x: " << now.x << " now.y: " << now.y << " now.dir: " << now.dir << endl;
            return true;
        }


        int nx, ny, rt_dir;
        std::pair<int, int> rotated_xy;

        //顺时针旋转
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, CLOCKWISE_DIR);
        rt_dir = get_clockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        if (can_place_boat(nx, ny, rt_dir) && !boat_vis[nx][ny][rt_dir]) {
            boat_vis[nx][ny][rt_dir] = true;
            if(check_boat_loc_slow(nx, ny, rt_dir)) {
                int gCost = now.gCost + 2;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                q.push({nx, ny, rt_dir,  gCost, hCost});
            }
            else {
                int gCost = now.gCost + 1;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, rt_dir,  gCost, hCost});
            }
            boat_node_path[nx][ny][rt_dir] = {now.x, now.y, now.dir};
        }

        //逆时针旋转
        rotated_xy = get_rotated_point(now.x, now.y, now.dir, ANTICLOCKWISE_DIR);
        rt_dir = get_anticlockwise(now.dir);
        nx = rotated_xy.first; ny = rotated_xy.second;
        if (can_place_boat(nx, ny, rt_dir) && !boat_vis[nx][ny][rt_dir]) {
            boat_vis[nx][ny][rt_dir] = true;
            if(check_boat_loc_slow(nx, ny, rt_dir)) {
                int gCost = now.gCost + 2;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, rt_dir, gCost, hCost });
            }
            else {
                int gCost = now.gCost + 1;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, rt_dir, gCost, hCost});
            }
            boat_node_path[nx][ny][rt_dir] = {now.x, now.y, now.dir};
        }

        //前进
        nx = now.x + d[now.dir].x;
        ny = now.y + d[now.dir].y;
        if (can_place_boat(nx, ny, now.dir) && !boat_vis[nx][ny][now.dir]) {
            boat_vis[nx][ny][now.dir] = true;
            if(check_boat_loc_slow(nx, ny, now.dir)) {
                int gCost = now.gCost + 2;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, now.dir,  gCost, hCost});
            }
            else {
                int gCost = now.gCost + 1;
                int hCost = abs(nx - mbx) + abs(ny - mby);
                int fCost = gCost + hCost;
                q.push({nx, ny, now.dir,  gCost, hCost});
            }
            boat_node_path[nx][ny][now.dir] = {now.x, now.y, now.dir};
        }

    }


}


void Boat::move() {
    if(operation_list.empty()) {
        mbx = -1;
        mby = -1;
        mbdir = -1;
        outFile<< "Boat " << id << " " <<"船只轨迹为空" << endl;
    }
    if(!operation_list.empty()) {
        int op = operation_list[0];
        exec(op);
    }
}

void Boat::exec(int op) {
    if(status == 1) {
        outFile << "Boat " << id << " 状态正在恢复中" << endl;
        return;
    }
    outFile << "op: " << op << endl;
    std::pair<int, int> rotated_xy;
    if(op == FORWARD) {
        printf("ship %d\n", id);
        operation_list.erase(operation_list.begin());
        x += d[dir].x;
        y += d[dir].y;
        outFile <<  "Boat " << id << " ship" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
    else if(op == ANTICLOCKWISE_DIR) {
        printf("rot %d 1\n", id);
        operation_list.erase(operation_list.begin());
        rotated_xy = get_rotated_point(x, y, dir, ANTICLOCKWISE_DIR);
        dir = get_anticlockwise(dir);
        x = rotated_xy.first;
        y = rotated_xy.second;
        outFile <<  "Boat " << id << " rot" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;

    }
    else if(op == CLOCKWISE_DIR) {
        printf("rot %d 0\n", id);
        operation_list.erase(operation_list.begin());
        rotated_xy = get_rotated_point(x, y, dir, CLOCKWISE_DIR);
        dir = get_clockwise(dir);
        x = rotated_xy.first;
        y = rotated_xy.second;
        outFile <<  "Boat " << id << " rot" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;

    }
    else if (op == DEPT_OP) {
        printf("dept %d\n", id);
        operation_list.erase(operation_list.begin());
        mbx = -1;
        mby = -1;
        mbdir = -1;
        outFile <<  "Boat " << id << " dept" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;
    }
    else if (op == BERTH_OP) {
        printf("berth %d\n", id);
        operation_list.erase(operation_list.begin());
        mbx = -1;
        mby = -1;
        mbdir = -1;
        outFile <<  "Boat " << id << " berth" << " x:" <<  x  << " y:" << y << "  dir:" << dir << " mbx:" <<  mbx  << " mby:" << mby << endl;

    }

}

/**
 * 是否是berth[berth_id]的靠泊区
 */
bool Boat::checkBerthingArea(int x, int y, int berth_id) {
    if (berth[berth_id]->klux <= x && berth[berth_id]->kluy <= y
        && berth[berth_id]->krdx >= x  && berth[berth_id]->krdy>= y) {
        outFile << "Boat " << id << " " << "checkBerthingArea" <<  " " << x << " " << y << " " << berth_id << endl;
        return true;
    }
    return false;
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
