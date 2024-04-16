#include "robot.h"
#include "globals.h"
#include "berth.h"
#include "utils.h"

Robot::Robot() {
    initialize();
}

Robot::Robot(int x, int y) {
    initialize();
    this->x = x;
    this->y = y;
}

void Robot::initialize() {
    id = robot_num - 1;
    goods = 0;
    price = 0;
    mbx = -1;
    mby = -1;
    x = -1;
    y = -1;
    haveTarget = false;
    searchGdsNumber = 10;
    findLength = 0;
    // for(int i = 0; i < berth_num; i++) boards.push_back(i);
    is_first_move = true;

    if(map_index == 0 || map_index == 1) boards = board_seq[map_index][id];
    else {
        for(int i = 0; i < berth_num; i++) {
            boards.push_back(i);
        }
    }
    info << "机器人" << id << "可以去的泊位有：";
    for(int i = 0; i < boards.size(); i++) {
        info << boards[i] << " ";
    }
    info << endl;
}

int Robot::goto_mbp() {
    queue<pair<int, int>> q;
    q.push({x, y});
    bool vis[N][N];
    memset(vis, 0, sizeof(vis));
    vis[x][y] = 1;
    int path[N][N];
    memset(path, -1, sizeof(path));
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = x + d[i].x, ny = y + d[i].y;
            if (!check(nx, ny) || vis[nx][ny] || !check_robot_can_loc(nx, ny)) continue;
            vis[nx][ny] = 1;
            q.push({nx, ny});
            path[nx][ny] = i;
            if (nx == mbx && ny == mby) {
                int cx = nx, cy = ny, pre = i;
                while (path[cx][cy] != -1) {
                    int tmp = path[cx][cy];
                    pre = path[cx][cy];
                    cx -= d[tmp].x;
                    cy -= d[tmp].y;
                }
                return pre;
            }
        }
    }
    error << "机器人没有找到点" << endl;
    return -1;
}

int Robot::get_good() {// 捡起货物
    info << "get_good: 机器人" << id << "在" << x << " " << y << "捡起货物: " << gds[x][y] << endl;
    if (gds[x][y] == 0) return 0;
    goods_infos.erase(std::remove_if(goods_infos.begin(), goods_infos.end(),
        [&](const GoodsInfo &o){
            return o.x == x && o.y == y;
        }),
        goods_infos.end()
    );
    goods = 1;
    mbx = -1;
    mby = -1;
    price = gds[x][y];
    lock_goods[x][y] = 0;
    gds[x][y] = 0;
    printf("get %d\n", id);
    return 1;
}

void Robot::put_good() {
    if (goods == 0) {
        error << "机器人没有货物" << endl;
        return ;
    }
    info << "机器人:" << id << " " << getBerthId() << endl;
    mbx = -1;
    mby = -1;
    if (getBerthId() != -1){
        int berth_id = getBerthId();
        berth[berth_id]->num++;
        berth[berth_id]->price += price;
        berth[berth_id]->berthGoodsPrice.push_back(price);
        goods = 0;
        printf("pull %d\n", id);
        total_banyun_num ++;
        ban_value_radio[price / 10] ++;
        berth[berth_id]->history_cnt ++;
        // goods_put.push_back({x, y, price, step});
        // info << "机器人在" << x << " " << y << "放下货物价值" << price << endl;
        price = 0;
    }
}

// 移动指令
void Robot::move() {
    if (movePath.empty()) {
        mbx = -1;
        mby = -1;
        error << id << "号机器人轨迹为空" << endl;
        return;
    }
    int op = movePath[0];
    exec(op);
}

void Robot::exec(int op) {
    printf("move %d %d\n", id, op);
    movePath.erase(movePath.begin());
    info << "机器人在" << x << " " << y << "执行指令" << op << endl;
    /*if (movePath.empty()) {
        mbx = -1;
        mby = -1;
        return;
    }
    int nextX = x + d[op].x;
    int nextY = y + d[op].y;
    if (nextX == mbx && nextY == mby) {
        if(grid[nextX][nextY] == 'B') {
            put_good();
        }
        else {
            get_good();
        }
    }*/
}

int Robot::getBerthId() {
/*    for (int i = 0; i < boards.size(); i++) {
        if (x >= berth[boards[i]]->lux && x <= berth[boards[i]]->rdx && y >= berth[boards[i]]->luy && y <= berth[boards[i]]->rdy) {
            return berth[boards[i]]->id;
        }
    }*/
    auto it = Berth2ID.find({x, y});
    if (it == Berth2ID.end()) return -1;
    for (int i = 0; i < boards.size(); i++) {
        if (it->second == berth[boards[i]]->id) {
            return berth[boards[i]]->id;
        }
    }
    return -1;
}

/*
 * 1: 正常
 * -1: 货物消失
 * -2: 找不到路径
 */
int Robot::find_mbGoodsPoint() {

    if (boards.size() <= 0) {
        return -2;
    }
    if (mbx == -1 && mby == -1) {
        return -2;
    }
    if(x == mbx && y == mby) {
        return 1;
    }
    auto time_start = time_start_debug();
    struct GroundNode {
        int x, y, dis, hCost;
        bool operator<(const GroundNode &b) const {
            return dis + hCost > b.dis + b.hCost;
        }
    };


    bool vis[N][N];
    GroundNode path[N][N];
    priority_queue<GroundNode> q;
    movePath.clear();

    memset(vis, 0, sizeof vis);
    memset(path, -1, sizeof path);
    q.push({x, y, 0, 0});
    vis[x][y] = true;
    int qCount = 0;

    while(!q.empty()) {
        GroundNode now = q.top();
        q.pop();
        if(step + now.dis >= 15000) continue;
        ++qCount;

        if(now.x == mbx && now.y == mby) {
            if(goods_vanish_time[now.x][now.y] - 1 < step + now.dis) {
                error << "货物消失:"<<mbx << " " << mby << " " << step + now.dis <<" "<< goods_vanish_time[mbx][mby] << endl;
                return -1;
            }
            lock_goods[now.x][now.y] = 1;

            while(path[now.x][now.y].x != -1) {
                movePath.push_back(getDirByTwoPoint(path[now.x][now.y].x, path[now.x][now.y].y, now.x, now.y));

                if(step + now.dis < 15000) {
                    robot_set_obstacle(now.x, now.y, step + now.dis);
                }
                now = path[now.x][now.y];
            }
            reverse(movePath.begin(), movePath.end());
            info << "movePath:";
            for (const auto &item: movePath) {
                info  << item << " ";
            }
            info << endl;

            for (auto &item: goods_infos) {
                if (item.x == mbx && item.y == mby) {
                    item.locked = true;
                }
            }

            auto time_end = time_end_debug(time_start);
            info << "遍历的点数量：" << qCount << endl;
            info << "find_mbGoodsPoint time:" << time_end << endl;
            return 1;
        }

        for(int i = 0; i < 4; i++) {
            int nx = now.x + d[i].x, ny = now.y + d[i].y;
            // if (nx == 112 && ny == 142) {
            //     info <<"sss" << nx << " " << ny <<  << (robot_exist_obstacle[nx][ny].test(now.dis + 1) == 1) << endl;
            // }
            bool isObstacle = (robot_exist_obstacle[nx][ny].test(step + now.dis + 1) == 1);
            // if (nx == 112 && ny == 142) {
            //     info <<"sss" << nx << " " << ny << (robot_exist_obstacle[nx][ny].test(step + now.dis + 1) == 1) << endl;
            // }
            bool isDZ = ((robot_exist_obstacle[nx][ny].test(step + now.dis) == 1) && (robot_exist_obstacle[now.x][now.y].test(step + now.dis + 1) == 1));
            if(!check(nx, ny) || vis[nx][ny] || !check_robot_can_loc(nx, ny) || isObstacle || isDZ) continue;

            vis[nx][ny] = true;
            int hCost = abs(nx - mbx) + abs(ny - mby);
            q.push({nx, ny, now.dis + 1, hCost});
            path[nx][ny] = now;
        }
    }

    error << id << "号机器人没有找到点" << endl;
    auto time_end = time_end_debug(time_start);
    info << "寻路花费的时间:" << time_end << " 找的点数量: " << qCount << endl;

    return -2;
}

bool Robot::find_mbBerthPoint(int berth_id) {
    if (boards.size() <= 0) {
        return -1;
    }

    struct GroundNode {
        int x, y, dis, hCost;
        bool operator<(const GroundNode &b) const {
            return hCost > b.hCost;
        }
    };

    int qCount = 0;
    bool vis[N][N];
    GroundNode path[N][N];
    priority_queue<GroundNode> q;
    movePath.clear();

    memset(vis, 0, sizeof vis);
    memset(path, -1, sizeof path);
    q.push({x, y, 0, 0});
    vis[x][y] = true;

    while(!q.empty()) {
        GroundNode now = q.top();
        q.pop();
        if(step + now.dis >= 15000) continue;
        qCount ++;

        if(getBerthIdByPoint(now.x, now.y, boards) == berth_id) {
            mbx = now.x;
            mby = now.y;

            while(path[now.x][now.y].x != -1) {
                movePath.push_back(getDirByTwoPoint(path[now.x][now.y].x, path[now.x][now.y].y, now.x, now.y));
                if(step + now.dis < 15000) {
                    robot_set_obstacle(now.x, now.y, step + now.dis);
                }

                now = path[now.x][now.y];
            }
            reverse(movePath.begin(), movePath.end());

            info << id << "号机器人" << "find_mbBerthPoint qCount:" << qCount << " movePath:" << movePath.size() << endl;

            return true;
        }

        for(int i = 0; i < 4; i++) {
            int nx = now.x + d[i].x, ny = now.y + d[i].y;
            bool isObstacle = robot_exist_obstacle[nx][ny].test(step + now.dis + 1) == 1;
            bool isDZ = robot_exist_obstacle[nx][ny].test(step + now.dis) == 1 && robot_exist_obstacle[now.x][now.y].test(step + now.dis + 1) == 1;
            if(!check(nx, ny) || vis[nx][ny] || !check_robot_can_loc(nx, ny) || isObstacle || isDZ) continue;

            vis[nx][ny] = true;
            int hCost = robot_to_berth_hCost[berth_id][nx][ny];
            q.push({nx, ny, now.dis + 1, hCost});
            path[nx][ny] = now;
        }
    }
    return false;
}

void Robot::close(int berthID) {
    auto it = std::find(boards.begin(), boards.end(), berthID);
    if (it != boards.end()) {
        boards.erase(it);
    }
}

void Robot::openBerthID(int berthID) {
    auto it = std::find(boards.begin(), boards.end(), berthID);
    if (it == boards.end() && berthID >= 0 && berthID < berth_num) {
        boards.push_back(berthID);
    }
}

void Robot::openAllBerth() {
    for (int i = 0; i < berth_num; i++) {
        openBerthID(i);
    }
}

bool Robot::is_locate_Berth() {
    return getBerthId() != -1;
}

void Robot::set_goods_for_mb() {
    info << id <<"号机器人set_goods_for_mb" << endl;

    double max_metrics = 0;
    for(auto &it : goods_infos) {
        if(it.locked || it.value < 10) continue;
        int gx = it.x, gy = it.y, gv = it.value, g_berthid = it.nearest_berth;
        // int g_gen_time = it.generate_time;
        if(std::find(boards.begin(), boards.end(), g_berthid) == boards.end()
            && find(closed_berth.begin(), closed_berth.end(), g_berthid) == closed_berth.end() ) {
            //todo: 可能存在问题，分配货物
            //机器人不能去g_berthid码头
            continue;
        }
        // if(g_gen_time > step + 100) {
        //     continue;
        // }
        // info << "gx:" << gx << " gy:" << gy << " gv:" << gv << " g_berthid:" << g_berthid << endl;
        int currentBerthId = getBerthId();
        int dis = 0;
        int dis_1 = 0;
        int dis_2 = 0;
        if (currentBerthId != -1) {
            dis = robot_to_berth_hCost[currentBerthId][gx][gy] + robot_to_berth_hCost[g_berthid][gx][gy];
            dis_1 = robot_to_berth_hCost[currentBerthId][gx][gy];
            dis_2 = robot_to_berth_hCost[g_berthid][gx][gy];
            if(dis_1 > 1000 || dis_2 > 1000) {
                continue;
            }
        } else {
            info << "currentBerthId = -1" << endl;
            dis = abs(x - gx) + abs(y - gy) + robot_to_berth_hCost[g_berthid][gx][gy];
            dis_1 = abs(x - gx) + abs(y - gy);
            dis_2 = robot_to_berth_hCost[g_berthid][gx][gy];
            if(dis_1 > 1000 || dis_2 > 1000) {
                continue;
            }
        }
        // if(get_metrics(it.value, dis, it.vanish_time) < 0) {
        if(get_metrics(gv, dis, dis_1, dis_2, it.vanish_time) < 0) {
            // error << "负数:" << get_metrics(it.value, dis, dis_1, dis_2, it.vanish_time) <<  endl;
        }
        if (get_metrics(gv, dis, dis_1, dis_2, it.vanish_time) > max_metrics) {
            // error << "正数:" << get_metrics(it.value, dis, dis_1, dis_2, it.vanish_time) <<  endl;
            max_metrics = get_metrics(gv, dis,dis_1, dis_2, it.vanish_time);
            mbx = gx;
            mby = gy;
        }
        /*if(gv * 1.0 / dis > max_metrics) {
            max_metrics = gv * 1.0 / dis;
            mbx = gx;
            mby = gy;
            // TODO 锁货物
        }*/
    }
    return ;
}


int Robot:: set_berth_for_mb() {
    int target_berthId = -1;
    int dis = 0x3f3f3f3f;
    // TODO
    if (boards.empty()) {
        for(int i = 0; i < berth_num; i++) {
            if(find(closed_berth.begin(), closed_berth.end(), i) != closed_berth.end()) {
                continue;
            }
            boards.push_back(i);
        }
    }
    for (int i = 0; i < boards.size(); i++) {
        if (find(closed_berth.begin(), closed_berth.end(), boards[i]) != closed_berth.end()) {
            continue;
        }
        int berthId = boards[i];
        int dis_temp = robot_to_berth_hCost[berthId][x][y];
        int flag = finalTime && berth[berthId]->num < boat_capacity;
        if (dis_temp < dis && !flag) {
            dis = dis_temp;
            target_berthId = berthId;
        }
    }

/*
    // TODO
    if (robot_num < robot_max_num) {
        int target_berthId = -1;
        int max_num = 0;
        for (int i = 0; i < berth_num; i++) {
            if (berth[i]->num > max_num) {
                max_num = berth[i]->num;
                target_berthId = i;
            }
        }
    }
*/

    return target_berthId;
}
