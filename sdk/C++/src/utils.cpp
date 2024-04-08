#include "utils.h"
#include "globals.h"
#include <queue>

// 获取当前时间
std::string getCurrentTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%m-%d.%H.%M.%S", localTime);
    return buffer;
}

string get_operation_debug(int op) {
    if(op == FORWARD_OP) return "前进";
    else if(op == CLOCKWISE_DIR_OP) return "顺时针旋转";
    else if(op == ANTICLOCKWISE_DIR_OP) return "逆时针旋转";
    else if(op == DEPT_OP) return "离开泊位";
    else if(op == BERTH_OP) return "靠泊";
    return "错误的操作";
}

time_point time_start_debug() {
    return std::chrono::steady_clock::now();
}

int time_end_debug(time_point start) {
    time_point end = std::chrono::steady_clock::now();
    // milliseconds duration = std::chrono::duration_cast<milliseconds>(end - start);
    microseconds duration = std::chrono::duration_cast<microseconds>(end - start);
    return static_cast<int>(duration.count());
}

/*
 * 检测(x, y)处是否有其他船只
 */
bool other_boat_is_here_debug(int x, int y, int dir) {
    std::map<pair<int, int>, bool> vis;
    std::vector<std::pair<int, int>> pos_list = get_boat_loc(x, y, dir);
    for(int i = 0; i < pos_list.size(); i++) {
        vis[pos_list[i]] = true;
    }
    for(int i = 0; i < boat_num; i++) {
        if(boat[i]->x != x && boat[i]->y != y) {
            std::vector<std::pair<int, int>> other_pos_list = get_boat_loc(boat[i]->x, boat[i]->y, boat[i]->dir);
            for(int j = 0; j < other_pos_list.size(); j++) {
                if(vis.find(other_pos_list[j]) != vis.end()) return true;
            }
        }
    }
    return false;
}

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

std::vector<std::pair<int, int>> get_boat_loc(int x, int y, int dir) {
    std::vector<pair<int, int>> points;
    points.push_back({x, y});
    points.push_back({x + d[dir].x, y + d[dir].y});
    points.push_back({x + 2 * d[dir].x, y + 2 * d[dir].y});
    int nx = x + d[get_clockwise(dir)].x, ny = y + d[get_clockwise(dir)].y;
    points.push_back({nx, ny});
    points.push_back({nx + d[dir].x, ny + d[dir].y});
    points.push_back({nx + 2 * d[dir].x, ny + 2 * d[dir].y});
    return points;
}

bool check_boat_can_loc(int x, int y, int ts) {
    if(!check(x, y)) return false;
    if(exist_obstacle[x][y].test(ts) == 1) {
        return false;
    }
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

/*
 * 在ts时刻(x, y, dir)方向的船是否可以放置
 */
bool can_place_boat(int x, int y, int idx, int ts) {
    if(ts > 15000) return false;
    std::vector<pair<int, int>> points = get_boat_loc(x, y, idx);
    for(int i = 0; i < points.size(); i++) {
        if(!check_boat_can_loc(points[i].first, points[i].second, ts)) return false;
    }
    return true;
}

/*
 * 顺时针旋转后的方向
 */
int get_clockwise(int dir) {
    if(dir == RIGHT) return DOWN;
    if(dir == LEFT) return UP;
    if(dir == UP) return RIGHT;
    if(dir == DOWN) return LEFT;
    return -1;
}

/*
 * 逆时针旋转后的方向
 */
int get_anticlockwise(int dir) {
    if(dir == RIGHT) return UP;
    if(dir == LEFT) return DOWN;
    if(dir == UP) return LEFT;
    if(dir == DOWN) return RIGHT;
    return -1;
}

/*
 * 逆置方向
 */
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
        error << "错误的旋转方向" << endl;
    }
    return res;
}

/*
 * 获取操作
 */
int get_operation(int pdir, int cdir) {
    if(pdir == cdir)
        return FORWARD_OP;
    if(get_clockwise(pdir) == cdir)
        return CLOCKWISE_DIR_OP;
    if(get_anticlockwise(pdir) == cdir)
        return ANTICLOCKWISE_DIR_OP;
    error << "前面的状态转移不到后面的状态" << endl;
    return -1;
}

/*
 * 检测(x, y)处是否有slow点
 */
bool check_boat_loc_slow(int x, int y, int idx) {
    std::vector<pair<int, int>> points = get_boat_loc(x, y, idx);
    bool res = false;
    for(int i = 0; i < points.size(); i++) {
        if(check(points[i].first, points[i].second) == false) return false;
        if(slow_points.find({points[i].first, points[i].second}) != slow_points.end()) res = true;
    }
    return res;
}

void set_obstacle(int x, int y, int dir, int ts) {
    std::vector<pair<int, int>> points = get_boat_loc(x, y, dir);
    for(int i = 0; i < points.size(); i++) {
        // info << "set_obstacle: " << points[i].first << " " << points[i].second << " " << ts << endl;
        exist_obstacle[points[i].first][points[i].second].set(ts);
    }
}

void open_berth(int berth_id) {
    berth[berth_id]->set_locked(false);
    for(int i = 0; i < boat_num; i++) {
        boat[i]->add_berth(berth_id);
    }
}

void close_berth(int berth_id) {
    for(int i = 0; i < boat_num; i++) {
        boat[i]->remove_berth(berth_id);
    }
}

/**
 * (x, y)是否是berth[berth_id]的靠泊区
 */
bool locate_berth_area(int x, int y, int berth_id) {
    if (berth[berth_id]->klux <= x && berth[berth_id]->kluy <= y
        && berth[berth_id]->krdx >= x  && berth[berth_id]->krdy>= y
        ) {

        return true;
    }
    return false;
}

/*
void step_compensate_compute(int time) {
    for(int i = 0; i < boat_num; i++) {
        boat[i]->stepCompensate = time;
    }
}*/
