#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>
#include <chrono>

/**************** debug用的函数 **********/

std::string get_operation_debug(int op);
bool other_boat_is_here_debug(int x, int y, int dir);
std::string getCurrentTime();

typedef std::chrono::steady_clock::time_point time_point;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::microseconds microseconds;

time_point time_start_debug();
int time_end_debug(time_point start);

/**************** end **********/

/*
 * 在ts检测(x, y)处是否可以放船
 */
bool can_place_boat(int x, int y, int dir, int ts);

/*
 * 检测是否越界
 */
bool check(int x, int y);

/*
 * 检测机器人是否可以在(x, y)处
 */
bool check_robot_can_loc(int x, int y);

/*
 * 在ts检测(x, y)处是否存在障碍（包括船只和机器人）
 */
bool check_boat_can_loc(int x, int y, int ts);

bool check_boat_can_loc_dir(int x, int y, int dir);

/*
 * 检测(x, y)处是否有slow点
 */
bool check_boat_loc_slow(int x, int y, int idx);

/*
 * 所有机器人不能去pos港口
 */
void closeAllRobot(int pos);

/*
 * 关闭berth_id
 */
void close_berth(int berth_id);

/*
 * 得到(x, y)最近的港口长度
 */
int get_nearest_berth_length(int x, int y, const std::vector<int> &boards);

/*
 * 通过坐标获取boards中的泊位id
 */
int getBerthIdByPoint(int x, int y, const std::vector<int> &boards);

/*
 * 获取(x, y, dir)船只的位置
 */
std::vector<std::pair<int, int>> get_boat_loc(int x, int y, int dir);

/*
 * 获取dir方向的顺时针方向
 */
int get_clockwise(int dir);

/*
 * 获取dir方向的逆时针方向
 */
int get_anticlockwise(int dir);

/*
 * 获取dir方向的相反方向
 */
int get_opposite(int dir);

/*
 * 获取dir方向的旋转后的点
 */
std::pair<int, int> get_rotated_point(int x, int y, int dir_index, int rotate_dir);

/*
 * 从pdir到cdir如何操作
 */
int get_operation(int pdir, int cdir);

/*
 * 检测(x, y)处是否有障碍
 */
bool locate_berth_area(int x, int y, int id);

/*
 * 打开berth_id
 */
void open_berth(int berth_id);

/*
 * 将(x,y,dir)在ts时刻设置为障碍
 */
void boat_set_obstacle(int x, int y, int dir, int ts);
void robot_set_obstacle(int x, int y, int ts);

/*
 * 帧率补偿计算
 *//*
void step_compensate_compute(int time);*/

void pre_process();
void ProcessMapBerthPoint();
/*void sea_flood_algorithm(int id, int x, int y);*/
void sea_flood_algorithm(int id, int x, int y);

void berth_flood_algorithm(int id);

void robot_berth_flood_algorithm(int berth_id);

bool is_sea(int x, int y);

int getDirByTwoPoint(int px, int py, int cx, int cy);

// double get_metrics(int value, int distance, int vanishtime);
double get_metrics(int value, int distance, int dis_1, int dis_2, int vanishtime);

#endif
