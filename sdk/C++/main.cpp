#include "globals.h"
using namespace std;

/**
 * 判题器交互输入的全局变量
 */
int money, boat_capacity, step;  // 当前金钱数、船的容量、当前实际运行帧数(系统读入，会进行覆盖)
int berth_num, robot_num, boat_num;  // 码头数、机器人数、船数(系统读入，会进行覆盖)

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


/**
 * 轮船进行A*算法的全局变量，定义在此为了避免栈内存空间不够用
 */
bool boat_vis[n][n][5];
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

std::ofstream outFile;

Dir d[4] = {{0,  1},   // 右
            {0,  -1},  // 左
            {-1, 0},  // 下
            {1,  0}};  // 上

Debug *debug;

/**
 * 对输入的地图信息数据进行预处理
 */
void ProcessMap() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 'R')
                // buy->append_robot_point(i, j);
                robot_purchase_point.push_back(make_pair(i, j));
            else if (grid[i][j] == 'S')
                // buy->append_boat_point(i, j);
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
//    debug = new Debug();
    buy = new Buy();

    outFile.open("out.txt", ios::out);
    if (!outFile) {
        std::cout << "open file error" << endl;
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        scanf("%s", grid[i]);
    }
    scanf("%d", &berth_num);
    for (int i = 0; i < berth_num; i++) {
        berth[i] = new Berth();
    }
    for (int i = 0; i < berth_num; i++) {
        scanf("%d%d%d%d", &berth[i]->id, &berth[i]->x, &berth[i]->y, &berth[i]->loading_speed);
        outFile << "berth[" << i << "]:" << berth[i]->id << " " << berth[i]->x << " " << berth[i]->y << " " << berth[i]->loading_speed << endl;
    }
    ProcessMap();  // 对地图进行预处理
    scanf("%d", &boat_capacity);
    outFile  << "boat_capacity:" << boat_capacity << endl;
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
        goods_vanish_time[x][y] = step + 1000;
    }
    scanf("%d", &robot_num);
    for (int i = 0; i < robot_num; i++) {
        scanf("%d%d%d%d", &robot[i]->id, &robot[i]->goods, &robot[i]->x, &robot[i]->y);
    }
    scanf("%d", &boat_num);
    for (int i = 0; i < boat_num; i++) {
        scanf("%d%d%d%d%d%d\n", &boat[i]->id, &boat[i]->goods_num, &boat[i]->x, &boat[i]->y, &boat[i]->dir, &boat[i]->status);
        outFile << "boat[" << i << "]:" << boat[i]->id << " " << boat[i]->goods_num << " " << boat[i]->x << " " << boat[i]->y << " " << boat[i]->dir << " " << boat[i]->status << endl;
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


int main() {
    Init();
    // TODO 跳帧判断
    while (scanf("%d", &step) != EOF) {
        outFile << endl << "step: " << step << " begin" << endl;

        // 输入
        Input();
        outFile << "input done" << endl;


        // 更新
        update(step);
        outFile << "update done" << endl;


        // 执行机器人指令
        exec->execute_robot();
        outFile << "execute robot done" << endl;

        // 执行轮船指令
        exec->execute_boat();
        outFile << "execute boat done" << endl;

        // 执行购买指令
        exec->execute_buy();
        outFile << "execute buy done" << endl;

        outFile << "boat_num:"<< boat_num << " robot_num:" << robot_num << endl;
        outFile << "step: " << step << " end" << endl;

        puts("OK");
        fflush(stdout);
    }
    outFile.close();
    return 0;
}
