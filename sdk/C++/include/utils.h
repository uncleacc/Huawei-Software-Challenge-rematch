#ifndef UTILS_H
#define UTILS_H
#include <vector>

bool check(int x, int y);

int get_nearest_berth_length(int x, int y, const std::vector<int> &boards);

int getBerthIdByPoint(int x, int y, const std::vector<int> &boards);

void closeAllRobot(int pos);

bool check_robot_can_loc(int x, int y);

bool check_boat_can_loc(int x, int y);

bool can_place_boat(int x, int y, int dir_index);

int get_clockwise(int dir);

int get_anticlockwise(int dir);

int get_opposite(int dir);

std::pair<int, int> get_rotated_point(int x, int y, int dir_index, int rotate_dir);

int get_operation(int pdir, int cdir);

bool check_boat_loc_slow(int x, int y, int dir);
#endif
