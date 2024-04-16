#include <thread>
#include "globals.h"
using namespace std;

/**
 * 判题器交互输入的全局变量
 */
int money, boat_capacity, step, zhen;  // 当前金钱数、船的容量、当前实际运行帧数(系统读入，会进行覆盖)
int berth_num, robot_num, boat_num, total_value;  // 码头数、机器人数、船数(系统读入，会进行覆盖)
int robot_max_num;
int boat_max_num;
int map_index;
bool init_finished[20], all_init_finished;

string maps[] = {
    "#.#............#.##.##.##.##.##.##.##.#..........###..........#.##.##.##.##.##.##.##.#...........KKKK********************************************************************************************************************************************************************************************************........##.....##.................##.....##.............##.....##.................##.....##.....#.#",
    "###...........############.############...........############.############............############.############...........############.############...........############.############.....#***~~~~***###...........#.......................#...........#.......................#............#.......................#...........#.......................#...........#.......................#.....#***~~~~***",
    "****...........#.......................#...........#.......................#....................***********.................#.......................#...........#.......................#...........********...........#.......................#...........#.......................#....................***********.................#.......................#...........#.......................#...........****"
};

//todo:机器人所属泊位，与机器人数量匹配
vector<vector<int>> board_seq[3] ={
        {
                {4},
                 {4},
                 {1},
                 {1},
                 {0},
                 {0},
                 {2,3},
                 {2,3},
                 {4},
                 {4},
                 {4},
                 {1, 4},
                 {1, 4},
                 {1, 4},
                 {0, 1, 3},
                 {0, 1, 3},
                 {0, 1, 2, 3, 4},
                 {0, 1, 2, 3, 4}
           /*
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4},
                {0, 1, 2, 3, 4}
                */
        },
        {       {0},
                {0},
                {1},
                {2, 3},
                {2, 3},
                {3, 4},
                {4, 5},
                {3, 4},

                {2},
                {2},
                {1},
                {2},
                {0},
                {1},
                {0},
                {1},
                {3},
                {4},
                {5},
        },
        {
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3},
                {0, 1, 2, 3}
        }
};

int belong_berth_num[10];

/*
 * 地图货物信息，机器人专用定义区
 */
char grid[N][N];
int gds[N][N], lock_goods[N][N], goods_vanish_time[N][N];
int all_goods_cnt;
long long all_goods_val;
int total_banyun_num;
int value_radio[21];
int ban_value_radio[21];


map<std::pair<int, int>, int> Berth2ID;
map<pair<int, int>, int> KaoBerth2ID;
vector<GoodsInfo> goods_infos;

/**
 * 地图信息，轮船专用定义区
 */
vector<pair<int, int>> delivery_point;   // 轮船的货物交货点
std::map<std::pair<int, int>, bool> slow_points;  // 轮船的慢速行驶点
bitset<15001> boat_exist_obstacle[N][N]; // 避撞的障碍物信息
bitset<15001> robot_exist_obstacle[N][N]; // 避撞的障碍物信息
// char exist_id[N][N][15001];  // 地图在ts时刻的船只信息

/*int boat_to_deliver_hCost[11][N][N];
int boat_to_berth_hCost[15][N][N];*/

int boat_to_deliver_hCost[11][N][N][5];
int boat_to_berth_hCost[15][N][N][5];

int robot_to_berth_hCost[15][N][N];

bool finalTime = false;
/**
 * 轮船进行A*算法的全局变量，定义在此为了避免栈内存空间不够用
 */
bool boat_vis[n][n][5];
int boat_dis[n][n][5];
boat_sport_node boat_node_path[n][n][4];

vector<pair<int, int>> robot_purchase_point;
vector<pair<int, int>> boat_purchase_point;


map<pair<int, int>, char> originalPosition;
map<pair<int, int>, int> originalValue;

execute *exec;
Robot *robot[M];
Boat *boat[M];
Berth *berth[M];
Buy *buy;

Debug info(LogLevel::INFO);
Debug warn(LogLevel::WARNING);
Debug error(LogLevel::ERROR);

Dir d[4] = {{0,  1},    // 右
            {0,  -1},   // 左
            {-1, 0},    // 上
            {1,  0}};   // 下

/**
 * 对输入的地图信息数据进行预处理
 */
void ProcessMapBerthPoint() {
    bool vis[n][n];
    for (int i = 0; i < berth_num; i++) {
        memset(vis, 0, sizeof(vis));
        queue <pair<int, int>> q;
        berth[i]->berthCoordinates.push_back({berth[i]->x, berth[i]->y});
        q.push({berth[i]->x, berth[i]->y});
        vis[berth[i]->x][berth[i]->y] = true;
        Berth2ID[{berth[i]->x, berth[i]->y}] = i;

        while (!q.empty()) {
            pair<int, int> now = q.front();
            q.pop();
            // info << "now.x" << now.first << " now.y" << now.second << endl;
            for (int j = 0; j < 4; j++) {
                int nx = now.first + d[j].x;
                int ny = now.second + d[j].y;

                if (!check(nx, ny) || vis[nx][ny] || (grid[nx][ny] != 'B' && grid[nx][ny] != 'K')) continue;

                vis[nx][ny] = true;
                q.push({nx, ny});
                if (grid[nx][ny] == 'B') {
                    berth[i]->berthCoordinates.push_back({nx, ny});
                    Berth2ID[{nx, ny}] = i;
                }
                if (grid[nx][ny] == 'K') {
                    berth[i]->kaoBerthCoordinates.push_back({nx, ny});
                    KaoBerth2ID[{nx, ny}] = i;
                }
            }
        }
    }
     /*for (int i = 0; i < berth_num; ++i) {
        for (int j = 0; j < berth[i]->berthCoordinates.size(); ++j) {
            info << "berth[" << i << "]  point:(" << berth[i]->berthCoordinates[j].first << " " << berth[i]->berthCoordinates[j].second << ")" << endl;
        }
         for (int j = 0; j < berth[i]->kaoBerthCoordinates.size(); ++j) {
             info << "berth[" << i << "]  kao point:(" << berth[i]->kaoBerthCoordinates[j].first << " " << berth[i]->kaoBerthCoordinates[j].second << ")" << endl;
         }
    }*/
}

void ProcessMap() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 'R')
                robot_purchase_point.push_back(make_pair(i, j));
            if (grid[i][j] == 'S')
                boat_purchase_point.push_back(make_pair(i, j));
            if (grid[i][j] == 'T')
                delivery_point.push_back(make_pair(i, j));
            /*if (grid[i][j] == 'B') {
                if (check(i - 2, j - 1) && grid[i -2][j - 1] == 'B') {
                    for (int k = 0; k < berth_num; k++) {
                        if (berth[k]->x >= i - 2 && berth[k]->x <= i && berth[k]->y >= j - 1 && berth[k]->y <= j) {
                            berth[k]->lux = i - 2;
                            berth[k]->luy = j - 1;
                            berth[k]->rdx = i;
                            berth[k]->rdy = j;

                            break;
                        }
                    }
                }
                else if(check(i - 1, j - 2) && grid[i - 1][j - 2] == 'B') {
                    for (int k = 0; k < berth_num; k++) {
                        if (berth[k]->x >= i - 1 && berth[k]->x <= i && berth[k]->y >= j - 2 && berth[k]->y <= j) {
                            berth[k]->lux = i - 1;
                            berth[k]->luy = j - 2;
                            berth[k]->rdx = i;
                            berth[k]->rdy = j;
                            break;
                        }
                    }
                }
            }*/
            if(grid[i][j] == 'c' || grid[i][j] == '~' || grid[i][j] == 'S' || grid[i][j] == 'K' || grid[i][j] == 'T' || grid[i][j] == 'B') {
                slow_points[make_pair(i, j)] = true;
            }
        }
    }

    /*// 对泊位的靠泊区进行预处理
    for (int i = 0; i < berth_num; i++) {
        // TODO 后续再考虑是否出现越界的极端情况
        if  (grid[berth[i]->lux - 3][berth[i]->luy - 2] == 'K') {
            berth[i]->klux = berth[i]->lux - 3;
            berth[i]->kluy = berth[i]->luy - 2;
            berth[i]->krdx = berth[i]->rdx + 3;
            berth[i]->krdy = berth[i]->luy;
        }
        else if  (grid[berth[i]->lux - 2][berth[i]->luy - 3] == 'K') {
            berth[i]->klux = berth[i]->lux - 2;
            berth[i]->kluy = berth[i]->luy - 3;
            berth[i]->krdx =  berth[i]->rdx;
            berth[i]->krdy = berth[i]->luy + 3;
        }
        else if  (grid[berth[i]->rdx + 3][berth[i]->rdy + 2] == 'K') {
            berth[i]->klux = berth[i]->lux - 3;
            berth[i]->kluy = berth[i]->luy;
            berth[i]->krdx = berth[i]->rdx + 3;
            berth[i]->krdy = berth[i]->rdy + 2;
        }
        else if  (grid[berth[i]->rdx + 2][berth[i]->rdy + 3] == 'K') {
            berth[i]->klux = berth[i]->lux;
            berth[i]->kluy = berth[i]->rdy - 3;
            berth[i]->krdx = berth[i]->rdx + 2;
            berth[i]->krdy = berth[i]->rdy + 3;
        }
    }*/
    std::thread threads_2(ProcessMapBerthPoint);
    threads_2.detach();
}


/**
 * 项目初始化
 */
void Init() {


    exec = new execute();
    buy = new Buy();

    string curMap;
    for (int i = 0; i < n; i++) {
        scanf("%s", grid[i]);
        if(i == 41 || i == 123) curMap += string(grid[i]);
    }
    for(int i = 0; i < 3; i++) {
        if(maps[i] == curMap) map_index = i;
    }
    if(map_index == 0) {
        robot_max_num = 18;
        boat_max_num = 2;
    }
    else if(map_index == 1) {
        robot_max_num = 19;
        boat_max_num = 2;
    }
    else if(map_index == 2) {
        robot_max_num = 18;
        boat_max_num = 1;
    }
    else {
        robot_max_num = 18;
        boat_max_num = 1;
    }
    warn << "当前地图是" << map_index << endl;


    scanf("%d", &berth_num);
    for (int i = 0; i < berth_num; i++) {
        berth[i] = new Berth();
    }
    for (int i = 0; i < berth_num; i++) {
        scanf("%d%d%d%d", &berth[i]->id, &berth[i]->x, &berth[i]->y, &berth[i]->loading_speed);
        info << "berth[" << i << "]:" << berth[i]->id << " " << berth[i]->x << " " << berth[i]->y << " " << berth[i]->loading_speed << endl;
    }
    ProcessMap();  // 对地图进行预处理

    scanf("%d", &boat_capacity);
    info  << "boat_capacity:" << boat_capacity << endl;
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}
/**
 * 每一帧执行的输入
 */
void Input() {
    scanf("%d", &money);
    int num;
    scanf("%d", &num);
    // all_goods_cnt += num;
    for (int i = 0; i < num; i++) {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        if(val <= 0) continue;

        value_radio[val / 10] ++;

        all_goods_cnt ++;
        gds[x][y] = val;
        all_goods_val += val;
        total_value += val;
        goods_vanish_time[x][y] = step + 995;

        int dis = 0x3f3f3f3f;
        int nearest_berth = -1;
        if (all_init_finished) {
            for(int j = 0; j < berth_num; j++) {
                int dis_temp = robot_to_berth_hCost[j][x][y] >= 10000 ? abs(x - berth[j]->x) + abs(y - berth[j]->y) : robot_to_berth_hCost[j][x][y];
                if (dis_temp < dis) {
                    dis = dis_temp;
                    nearest_berth = j;
                }
            }
            belong_berth_num[nearest_berth] ++;
            info << "goods_infos.size():" << goods_infos.size() << endl;
            goods_infos.push_back({x, y, val, step + 995, false, nearest_berth});
            info << "goods_infos.size():" << goods_infos.size() << endl;
        }
        else {
            for(int j = 0; j < berth_num; j++) {
                int dis_temp = robot_to_berth_hCost[j][x][y] >= 10000 ? abs(x - berth[j]->x) + abs(y - berth[j]->y) : robot_to_berth_hCost[j][x][y];
                if (dis_temp < dis) {
                    dis = dis_temp;
                    nearest_berth = j;
                }
            }
            belong_berth_num[nearest_berth] ++;
            info << "goods_infos.size():" << goods_infos.size() << endl;
            goods_infos.push_back({x, y, val, step + 995, false, nearest_berth});
            info << "goods_infos.size():" << goods_infos.size() << endl;
        }

    }
    scanf("%d", &robot_num);
    for (int i = 0; i < robot_num; i++) {
        scanf("%d%d%d%d", &robot[i]->id, &robot[i]->goods, &robot[i]->x, &robot[i]->y);
        info << "robot[" << i << "]:" << robot[i]->id << " " << robot[i]->goods << " " << robot[i]->x << " " << robot[i]->y << endl;
    }
    scanf("%d", &boat_num);
    for (int i = 0; i < boat_num; i++) {
        info << "boat[" << i << "]:" << boat[i]->id << " " << boat[i]->goods_num << " " << boat[i]->x << " " << boat[i]->y << " " << boat[i]->dir << " " << boat[i]->status << endl;
        scanf("%d%d%d%d%d%d\n", &boat[i]->id, &boat[i]->goods_num, &boat[i]->x, &boat[i]->y, &boat[i]->dir, &boat[i]->status);
        info << "boat[" << i << "]:" << boat[i]->id << " " << boat[i]->goods_num << " " << boat[i]->x << " " << boat[i]->y << " " << boat[i]->dir << " " << boat[i]->status << endl;
    }

    char okk[100];
    scanf("%s", okk);
}

void update(int step) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (gds[i][j] > 0 && goods_vanish_time[i][j] <= step) {
                all_goods_val -= gds[i][j];
                gds[i][j] = 0;
            }
        }
    }
    goods_infos.erase(std::remove_if(goods_infos.begin(), goods_infos.end(), [&](const GoodsInfo &o){ return o.vanish_time - 5 <= step; }), goods_infos.end());
    all_init_finished = true;
    for(int i = 0; i < berth_num; i++) {
        if(init_finished[i] == false) {
            all_init_finished = false;
        }
    }
}

int threadNum = 0;
void thread_1() {
    info << "子线程1  begin" << endl;
    this_thread::sleep_for(chrono::milliseconds(20 * 10));
    threadNum = 100;
    info << "子线程1  end" << endl;
}
bool firstOpenFlag = false;
void closeOperation() {
    // if (step == robot_max_num) {
    //     for (int i = 0; i < robot_num; i++) {
    //         robot[i]->close(0);
    //         robot[i]->close(1);
    //     }
    // }
    // if (robot_num == robot_max_num && !firstOpenFlag) {
    //     for (int i = 0; i < robot_num; i++) {
    //         robot[i]->openBerthID(0);
    //         robot[i]->openBerthID(1);
    //         firstOpenFlag = true;
    //     }
    // }
}
int main() {
    Init();

    pre_process();

    auto start = time_start_debug();

    int preStep = 0;
    for(step = 1; step <= 15000; step++) {
        scanf("%d", &step);

        /*if(step == 3000) {
            for(int k = 0; k < berth_num; k++) {
                info << "berth[" << k << "]:" << endl;
                for(int i = 0; i < n; i++) {
                    for(int j = 0; j < n; j++) {
                        if(robot_to_berth_hCost[k][i][j] == 0x3f3f3f3f
                        // || boat_to_berth_hCost[2][i][j][k] == 1061109567
                        ) {
                            info << "-1" << "\t";
                        } else {
                            info << robot_to_berth_hCost[k][i][j] << "\t";
                        }
                    }
                    info << endl;
                }
                info << endl << endl<<endl<<endl<<endl;
            }
        }
*/
        info << endl << "step: " << step << " begin" << endl;

        if(step - preStep > 1) {
            error << "跳帧: " << step - preStep << endl;
        }
        preStep = step;

        // 输入
        Input();


        // 更新
        update(step);

        //关闭指令
        closeOperation();

        // 执行机器人指令
        info << "execute robot begin" << endl;
        if (step > 10) exec->execute_robot();
        info << "execute robot done" << endl;

        // 执行轮船指令
        // 执行轮船指令

        info << "execute boat begin" << endl;
        if (step > 200) {exec->execute_boat();}
        info << "execute boat done" << endl;


        // 执行购买指令
        info << "execute buy begin" << endl;
        if (step > 2) exec->execute_buy();
        info << "execute buy done" << endl;


        info << "boat_num:"<< boat_num << " robot_num:" << robot_num << endl;
        info << "step: " << step << " end" << endl;

        puts("OK");
        fflush(stdout);
    }
/*    for(int  k = 0; k < 4; k++) {
        info << "方向: " << k << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if(boat_to_deliver_hCost[0][i][j][k] == 0x3f3f3f3f) info << -1 << "\t";
                else info << boat_to_deliver_hCost[0][i][j][k] << "\t";
            }
            info << endl;
        }
        info << endl << endl << endl;
    }

    for(int  k = 0; k < 4; k++) {
        info << "方向: " << k << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if(boat_to_deliver_hCost[1][i][j][k] == 0x3f3f3f3f) info << -2 << "\t";
                else info << boat_to_deliver_hCost[1][i][j][k] << "\t";
            }
            info << endl;
        }
        info << endl << endl << endl;
    }*/

    for(int l = 0; l < berth_num; l++) {
        info << "泊位: " << l << endl;
        for(int k = 0; k < 4; k++) {
            info << "方向: " << k << endl;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if(boat_to_berth_hCost[l][i][j][k] == 0x3f3f3f3f) info << -2 << "\t";
                    else info << boat_to_berth_hCost[l][i][j][k] << "\t";
                }
                info << endl;
            }
            info << endl << endl << endl;
        }
        info << "+==============================================" << endl;
        info << "+==============================================" << endl;
        info << "+==============================================" << endl;
    }



    /*int total_price = 0;
    for (int i = 0; i < berth_num; i++) {
        info << "berth[" << i << "]  point:(" << berth[i]->x << " " << berth[i]->y << ") num:" << berth[i]->num << " price:" << berth[i]->price << endl;
        total_price += berth[i]->price;
    }
    info << "total_price:" << total_price << endl;*/

    int totalBerthPrice = 0;
    for (int i = 0; i < berth_num; i++) {
        info << "berth[" << i << "]  point:(" << berth[i]->x << " " << berth[i]->y << ") num:" << berth[i]->num << " price:" << berth[i]->price << endl;
        totalBerthPrice += berth[i]->price;
    }

    for(int i = 0; i < 20; i++) {
        info << i * 10 << "到" << (i + 1) * 10 << "的总货物数量:" << value_radio[i] << endl;
        info << i * 10 << "到" << (i + 1) * 10 << "的已搬运货物数量:" << ban_value_radio[i] << endl;
    }

    for(int i = 0; i < berth_num; i++) {
        info << "泊位" << i << "的货物数量：" << belong_berth_num[i] << endl;
    }

    for(int i = 0; i < boat_num; i++) {
        info << "船只" << i << "的货物数量：" << boat[i]->goods_num << ", 货物价值：" << boat[i]->goodsPrice << endl;
    }
    info << "总货物数量: " << all_goods_cnt << endl;
    info << "运到码头的货物数量: " << total_banyun_num << endl;
    info << "总货物价值:" << total_value << endl;
    info << "搬运到码头的货物总价值:" << totalBerthPrice << endl;
    info << "交易的分数:" << money + boat_num * 8000 + robot_num * 2000 - 25000 << endl;
    info << "本金:25000,机器人成本:"  << 2000 * robot_num << ",轮船成本:" << 8000 * boat_num<<",总成本:" << 2000 * robot_num + 8000 * boat_num << endl;
    info << "成本:" << boat_num * 8000 + robot_num * 2000 << endl;
    info << "最终分数:" << money << endl;

    return 0;
}