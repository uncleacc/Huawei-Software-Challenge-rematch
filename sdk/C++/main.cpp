#include "globals.h"
using namespace std;

int money, boat_capacity, step;
int berth_num, robot_num, boat_num;

vector<pair<int, int>> delivery_point;

int all_goods_cnt;
long long all_goods_val;

std::map<std::pair<int, int>, bool> slow_points;

char grid[N][N];
int gds[N][N], goods_vanish_time[N][N];

bool boat_vis[n][n][5];
boat_sport_status boat_path[n][n][4];

map<pair<int, int>, char> originalPosition;
map<pair<int, int>, int> originalValue;

execute *exec;
Robot *robot[M];
Boat *boat[M];
Berth *berth[M];
Buy *buy;

std::ofstream outFile;

Dir d[4] = {{0,  1},
            {0,  -1},
            {-1, 0},
            {1,  0}};

Debug *debug;

/**************************************************************************************************************************************/

void ProcessMap() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 'R')
                buy->append_robot_point(i, j);
            else if (grid[i][j] == 'S')
                buy->append_boat_point(i, j);
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
}


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
    }
    ProcessMap();
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

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

    if (step < 1000) {
        for (int i = 0; i < robot_num; i++)
            robot[i]->searchGdsNumber = 5;
    } else {
        for (int i = 0; i < robot_num; i++)
            robot[i]->searchGdsNumber = 15;
    }
}


int main() {
    Init();
    while (scanf("%d", &step) != EOF) {
        outFile << endl << "step: " << step << endl;
        Input();
        outFile << "input done" << endl;
        update(step);
        outFile << "update done" << endl;

        // exec->execute_robot();
        // outFile << "execute robot done" << endl;

        exec->execute_boat();
        outFile << "execute boat done" << endl;

        exec->execute_buy();
        outFile << "execute buy done" << endl;

        outFile << "boat_num:"<< boat_num << " robot_num:" << robot_num << endl;

        puts("OK");
        fflush(stdout);
    }
    outFile.close();
    return 0;
}
