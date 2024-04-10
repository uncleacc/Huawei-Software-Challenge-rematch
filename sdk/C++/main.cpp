#include <thread>
#include "globals.h"
using namespace std;

/**
 * 判题器交互输入的全局变量
 */
int money, boat_capacity, step, zhen;  // 当前金钱数、船的容量、当前实际运行帧数(系统读入，会进行覆盖)
int berth_num, robot_num, boat_num, total_value;  // 码头数、机器人数、船数(系统读入，会进行覆盖)

/*
 * 地图货物信息，机器人专用定义区
 */
char grid[N][N];
int gds[N][N], goods_vanish_time[N][N];
int all_goods_cnt;
long long all_goods_val;

/**
 * 地图信息，轮船专用定义区
 */
vector<pair<int, int>> delivery_point;   // 轮船的货物交货点
std::map<std::pair<int, int>, bool> slow_points;  // 轮船的慢速行驶点
bitset<15001> exist_obstacle[N][N]; // 避撞的障碍物信息
// char exist_id[N][N][15001];  // 地图在ts时刻的船只信息

/*int to_deliver_hCost[11][N][N];
int to_berth_hCost[15][N][N];*/

int to_deliver_hCost[11][N][N][5];
int to_berth_hCost[15][N][N][5];


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
            {-1, 0},    // 下
            {1,  0}};   // 上

/**
 * 对输入的地图信息数据进行预处理
 */
void ProcessMap() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 'R')
                robot_purchase_point.push_back(make_pair(i, j));
            else if (grid[i][j] == 'S')
                boat_purchase_point.push_back(make_pair(i, j));
            else if (grid[i][j] == 'T')
                delivery_point.push_back(make_pair(i, j));
            else if (grid[i][j] == 'B') {
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
            }
            else if(grid[i][j] == 'c' || grid[i][j] == '~' || grid[i][j] == 'S') {
                slow_points[make_pair(i, j)] = true;
            }
        }
    }

    // 对泊位的靠泊区进行预处理
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
    }

}

/**
 * 项目初始化
 */
void Init() {
    exec = new execute();
    buy = new Buy();

    for (int i = 0; i < n; i++) {
        scanf("%s", grid[i]);
    }
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
    all_goods_cnt += num;
    for (int i = 0; i < num; i++) {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        gds[x][y] = val;
        all_goods_val += val;
        total_value += val;
        goods_vanish_time[x][y] = step + 1000;
        total_value += val;
    }
    scanf("%d", &robot_num);
    for (int i = 0; i < robot_num; i++) {
        scanf("%d%d%d%d", &robot[i]->id, &robot[i]->goods, &robot[i]->x, &robot[i]->y);
    }
    scanf("%d", &boat_num);
    for (int i = 0; i < boat_num; i++) {
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
                all_goods_cnt--;
                gds[i][j] = 0;
            }
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

int main() {
    Init();

    pre_process();

    int preStep = 0;
    for(step = 1; step <= 15000; step++) {
        scanf("%d", &step);
        info << endl << "step: " << step << " begin" << endl;

        if(step - preStep > 1) {
            error << "跳帧: " << step - preStep << endl;
        }
        preStep = step;

        // 输入
        Input();

        // 更新
        update(step);


        // 执行机器人指令
        info << "execute robot begin" << endl;
        exec->execute_robot();
        info << "execute robot done" << endl;

        // 执行轮船指令
        info << "execute boat begin" << endl;
        exec->execute_boat();
        info << "execute boat done" << endl;

        // 执行购买指令
        info << "execute buy begin" << endl;
        exec->execute_buy();
        info << "execute buy done" << endl;

        info << "boat_num:"<< boat_num << " robot_num:" << robot_num << endl;
        info << "step: " << step << " end" << endl;

        puts("OK");
        fflush(stdout);
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

    info << "total_goods_value:" << total_value << endl;
    info << "total_Berth_value:" << totalBerthPrice << endl;
    info << "money:" << money << endl;
    info << "totalMoneyProfit:" << money + boat_num * 8000 + robot_num * 2000 - 25000 << endl;
    return 0;
}