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
    lockTarget = true;
    searchGdsNumber = 5;
    findLength = 0;
    for(int i = 0; i < berth_num; i++) boards.push_back(i);
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
            if (!check(nx, ny) || vis[nx][ny] || (grid[nx][ny] != '.' && grid[nx][ny] != 'B')) continue;
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
    return -1;
}

int Robot::get_good() {// 捡起货物
     if (gds[x][y] == 0) return 0;
    if (x == mbx && y == mby) {
        if(!lockTarget) {
            all_goods_cnt -= 1;
            all_goods_val -= gds[x][y];
            price = gds[x][y];
        }
        originalValue.erase({mbx, mby}); // 删除备份的货物
        haveTarget = false;
        mbx = -1;
        mby = -1;
        goods = 1;
        gds[x][y] = 0;
        printf("get %d\n", id);
        return 1;
    }
    return 0;

}

void Robot::put_good(int ts) {
    if (goods == 0) {
        return;
    }
    info << "机器人:" << id << " " << getBerthId() << endl;
    int berth_id = getBerthId();
    berth[berth_id]->num++;
    berth[berth_id]->history_cnt ++;
    berth[berth_id]->price += price;
    printf("pull %d\n", id);

    berth[berth_id]->berthGoodsPrice.push_back(price);
}

// 移动指令
void Robot::move(int dir) {
    printf("move %d %d\n", id, dir);
    int nextX = x + d[dir].x;
    int nextY = y + d[dir].y;
    grid[x][y] = originalPosition[{x, y}];
    originalPosition[{nextX, nextY}] = grid[nextX][nextY];  // 备份地图
    grid[nextX][nextY] = 'A';
}


int Robot::getBerthId() {
    for (int i = 0; i < boards.size(); i++) {
        if (x >= berth[boards[i]]->lux && x <= berth[boards[i]]->rdx && y >= berth[boards[i]]->luy && y <= berth[boards[i]]->rdy) {
            return berth[boards[i]]->id;
        }
    }
    return -1;
}


int Robot::find_nearest_good() {
    if (boards.size() <= 0) {
        return -1;
    }
    struct goodsInfo {
        int value, direction, len1, len2; // 货物价值、路径第一个方向、到达货物路径长度，从货物到达码头的长度
        int mbx, mby; //目标点坐标
        int vanishTime; // 消失时间
    };

    int k = searchGdsNumber;
    queue<pair<int, int>> q;
    int path[N][N];
    bool vis[N][N];
    vector<goodsInfo> goodsList;

    q.push({x, y});
    memset(vis, 0, sizeof(vis));
    vis[x][y] = 1;
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
            if (!haveTarget && gds[nx][ny] > 0) {
                int cx = nx, cy = ny, pre = i, len = 0;
                while (path[cx][cy] != -1) {
                    int tmp = path[cx][cy];
                    len++;
                    pre = path[cx][cy];
                    cx -= d[tmp].x;
                    cy -= d[tmp].y;
                }
                if(step + len >= goods_vanish_time[nx][ny] ) continue; // 货物拿不到了
                goodsList.push_back({gds[nx][ny], pre, len, get_nearest_berth_length(nx, ny, boards), nx, ny, goods_vanish_time[nx][ny]});
                k -= 1;
                if(len > 70) {
                    k = 0;
                }
                if (k == 0) {
                    int idx = 0;
                    for (int j = 1; j < goodsList.size() ; j++) {
                        double R1 = 8, R2 = 2, R3 = 1;
                        if (R1 * goodsList[j].value * 1.0 / 200 + R2 * (70 - goodsList[j].len1) * 1.0 / 70 + R3 * (1000 - (goodsList[j].vanishTime - step)) * 1.0 / 1000
                            > R1 * goodsList[idx].value * 1.0 / 200 + R2 * (70 - goodsList[idx].len1) * 1.0 / 70 + R3 * (1000 - (goodsList[idx].vanishTime - step)) * 1.0 / 1000) {
                            idx = j;
                        }
                    }
                    mbx = goodsList[idx].mbx; // 设置机器人的目标点
                    mby = goodsList[idx].mby;
                    if(lockTarget) {
                        price = gds[mbx][mby];
                        all_goods_val -= gds[mbx][mby];
                        all_goods_cnt--;
                        haveTarget = true;
                        originalValue[{mbx, mby}] = gds[mbx][mby]; // 备份货物价值
                        findLength = goodsList[idx].len1;             // 记录锁定的货物距离
                        gds[mbx][mby] = -1;
                    }
                    return goodsList[idx].direction;
                }
            }
            else if(haveTarget && nx == mbx && ny == mby) {
                int cx = nx, cy = ny, pre = i, len = 0;
                while (path[cx][cy] != -1) {
                    int tmp = path[cx][cy];
                    len++;
                    pre = path[cx][cy];
                    cx -= d[tmp].x;
                    cy -= d[tmp].y;
                }
                if(len - findLength > 5) {  // 解除锁定的目标
                    price = 0;
                    all_goods_val += originalValue[{mbx, mby}];
                    all_goods_cnt++;
                    haveTarget = false;
                    gds[mbx][mby] = originalValue[{mbx, mby}]; // 恢复货物价值
                    originalValue.erase({mbx, mby});
                    return 4; // 机器人需要重新搜索目标
                }
                return pre;
            }

        }
    }

    if (q.empty() && goodsList.size() > 0) {
        int idx = 0;
        for (int j = 1; j < goodsList.size() ; j++) {
            double R1 = 1, R2 = 4, R3 = 2;
            if (R1 * goodsList[j].value * 1.0 / 200 + R2 * (70 - goodsList[j].len1) * 1.0 / 70 + R3 * (1000 - (goodsList[j].vanishTime - step)) * 1.0 / 1000
                > R1 * goodsList[idx].value * 1.0 / 200 + R2 * (70 - goodsList[idx].len1) * 1.0 / 70 + R3 * (1000 - (goodsList[idx].vanishTime - step)) * 1.0 / 1000) {
                idx = j;
            }
        }
        mbx = goodsList[idx].mbx;
        mby = goodsList[idx].mby;
        if(lockTarget) {
            price = gds[mbx][mby];
            all_goods_val -= gds[mbx][mby];
            all_goods_cnt--;
            haveTarget = true;
            originalValue[{mbx, mby}] = gds[mbx][mby];
            findLength = goodsList[idx].len1;
            gds[mbx][mby] = -1;
        }
        return goodsList[idx].direction;
    }

    return -1;
}

int Robot::find_nearest_berth() {
    if (boards.size() <= 0) {
        return -1;
    }
    bool vis[N][N];
    queue<pair<int, int>> q;
    int path[N][N];

    q.push({x, y});
    memset(vis, 0, sizeof(vis));
    vis[x][y] = true;
    memset(path, -1, sizeof(path));
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = x + d[i].x, ny = y + d[i].y;
            if (!check(nx, ny) || vis[nx][ny] || !check_robot_can_loc(nx, ny)) continue;
            vis[nx][ny] = true;
            q.push({nx, ny});
            path[nx][ny] = i;

            if (grid[nx][ny] == 'B' && getBerthIdByPoint(nx, ny, boards) != -1) {
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
    return -1;
}

void Robot::close(int berthID) {
    auto it = std::find(boards.begin(), boards.end(), berthID);
    if (it != boards.end()) {
        boards.erase(it);
    }
}

bool Robot::is_locate_Berth() {
    return getBerthId() != -1;
}


