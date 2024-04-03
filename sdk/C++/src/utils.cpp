#include "utils.h"
#include "globals.h"
#include <queue>

// 检测是否越界
bool check(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

// 通过坐标获取boards中的泊位id
int getBerthIdByPoint(int x, int y, const vector<int> &boards) {
    for (int i = 0; i < boards.size(); i++) {
        if (x >= berth[boards[i]]->lux && x <= berth[boards[i]]->rdx && y >= berth[boards[i]]->luy && y <= berth[boards[i]]->rdy) {
            return berth[boards[i]]->id;
        }
    }
    return -1;
}

// 得到(x, y)最近的港口长度
int get_nearest_berth_length(int x, int y, const std::vector<int> &boards) {
    // bfs
    queue<pair<int, int>> q;
    q.push({x, y});
    bool vis[N][N];
    int dis[N][N];
    memset(dis, 0x3f, sizeof dis);
    memset(vis, 0, sizeof(vis));
    vis[x][y] = true;
    dis[x][y] = 0;
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = x + d[i].x, ny = y + d[i].y;
            if (!check(nx, ny) || vis[nx][ny] || (grid[nx][ny] != '.' && grid[nx][ny] != 'B' && grid[nx][ny] != 'A')) continue;
            vis[nx][ny] = true;
            q.push({nx, ny});
            dis[nx][ny] = min(dis[x][y] + 1, dis[nx][ny]);

            if (grid[nx][ny] == 'B' && getBerthIdByPoint(nx, ny, boards) != -1) {  // 只能找列表里面的泊位
                return dis[nx][ny];
            }
        }
    }
    return -1;  // 机器人找不到货物
}

void closeAllRobot(int pos) {
    for (int j = 0; j < robot_num; ++j) {
        robot[j]->close(pos);
    }
}

bool check_robot_can_loc(int x, int y) {
    if(!check(x, y)) return false;
    if( grid[x][y] == '.'
        || grid[x][y] == 'B'
        || grid[x][y] == '>'
        || grid[x][y] == 'R'
        || grid[x][y] == 'C'
        || grid[x][y] == 'c'
        )
        return true;
    return false;
}

bool check_boat_can_loc(int x, int y) {
    if(!check(x, y)) return false;
    if( grid[x][y] == '*'
        || grid[x][y] == '~'
        || grid[x][y] == 'C'
        || grid[x][y] == 'c'
        || grid[x][y] == 'K'
        || grid[x][y] == 'T'
        || grid[x][y] == 'S'
        || grid[x][y] == 'B'
        )
        return true;
    return false;
}

bool can_place_boat(int x, int y, int idx) {
    std::vector<pair<int, int>> points;
    points.push_back({x, y});
    points.push_back({x + d[idx].x, y + d[idx].y});
    points.push_back({x + 2 * d[idx].x, y + 2 * d[idx].y});
    int nx = x + d[get_clockwise(idx)].x, ny = y + d[get_clockwise(idx)].y;
    points.push_back({nx, ny});
    points.push_back({nx + d[idx].x, ny + d[idx].y});
    points.push_back({nx + 2 * d[idx].x, ny + 2 * d[idx].y});
    for(int i = 0; i < points.size(); i++) {
        if(!check_boat_can_loc(points[i].first, points[i].second)) return false;
    }
    return true;
}

int get_clockwise(int dir) {
    if(dir == RIGHT) return DOWN;
    if(dir == LEFT) return UP;
    if(dir == UP) return RIGHT;
    if(dir == DOWN) return LEFT;
    return -1;
}

int get_anticlockwise(int dir) {
    if(dir == RIGHT) return UP;
    if(dir == LEFT) return DOWN;
    if(dir == UP) return LEFT;
    if(dir == DOWN) return RIGHT;
    return -1;
}

int get_opposite(int dir) {
    if(dir == RIGHT) return LEFT;
    if(dir == LEFT) return RIGHT;
    if(dir == UP) return DOWN;
    if(dir == DOWN) return UP;
    return -1;
}

// 获取旋转后的核心点坐标
std::pair<int, int> get_rotated_point(int x, int y, int dir_index, int rotate_dir) {
    std::pair<int, int> res = {-1, -1};
    if(rotate_dir == RIGHT) {
        res = {x + 2 * d[dir_index].x, y + 2 * d[dir_index].y};
    }
    else if(rotate_dir == LEFT) {
        res = {x + d[dir_index].x + d[get_opposite(get_anticlockwise(dir_index))].x, y + d[dir_index].y + d[get_opposite(get_anticlockwise(dir_index))].y};
    }
    else {
        outFile << "ERROR: 错误的旋转方向" << endl;
    }
    return res;
}

int get_operation(int pdir, int cdir) {
    if(pdir == cdir) return FORWARD;
    if(get_clockwise(pdir) == cdir) return CLOCKWISE_DIR;
    if(get_anticlockwise(pdir) == cdir) return ANTICLOCKWISE_DIR;
    outFile << "ERROR: 前面的状态转移不到后面的状态" << endl;
    return -1;
}

bool check_boat_loc_slow(int x, int y, int idx) {
    std::vector<pair<int, int>> points;
    points.push_back({x, y});
    points.push_back({x + d[idx].x, y + d[idx].y});
    points.push_back({x + 2 * d[idx].x, y + 2 * d[idx].y});
    int nx = x + d[get_clockwise(idx)].x, ny = y + d[get_clockwise(idx)].y;
    points.push_back({nx, ny});
    points.push_back({nx + d[idx].x, ny + d[idx].y});
    points.push_back({nx + 2 * d[idx].x, ny + 2 * d[idx].y});
    for(int i = 0; i < points.size(); i++) {
        if(slow_points.find({points[i].first, points[i].second}) != slow_points.end()) return true;
    }
    return false;
}
