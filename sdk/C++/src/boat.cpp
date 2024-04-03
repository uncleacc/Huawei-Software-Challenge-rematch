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

void Boat::set_mbp(int x, int y) {
    mbx = x;
    mby = y;
}

void Boat::move() {
    if(operation_list.empty()) {
        navigation();
    }
    if(!operation_list.empty()) {
        int op = operation_list[0];
        exec(op);
    }
}

void Boat::exec(int op) {
    outFile << "op: " << op << endl;
    std::pair<int, int> rotated_xy;
    if(op == FORWARD) {
        x += d[dir].x;
        y += d[dir].y;
        operation_list.erase(operation_list.begin());
        printf("ship %d\n", id);
    }
    else if(op == ATCLRO) {
        rotated_xy = get_rotated_point(x, y, dir, ANTICLOCKWISE_DIR);
        dir = get_anticlockwise(dir);
        x = rotated_xy.first;
        y = rotated_xy.second;
        operation_list.erase(operation_list.begin());
        printf("rot %d 1\n", id);
    }
    else if(op == CLRO) {
        rotated_xy = get_rotated_point(x, y, dir, CLOCKWISE_DIR);
        dir = get_clockwise(dir);
        x = rotated_xy.first;
        y = rotated_xy.second;
        operation_list.erase(operation_list.begin());
        printf("rot %d 0\n", id);
    }
}


