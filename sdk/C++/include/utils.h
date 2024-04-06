#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>

std::string getCurrentTime();

bool check(int x, int y);

int get_nearest_berth_length(int x, int y, const std::vector<int> &boards);

int getBerthIdByPoint(int x, int y, const std::vector<int> &boards);

void closeAllRobot(int pos);

bool check_robot_can_loc(int x, int y);

bool check_boat_can_loc(int x, int y, int ts);

bool can_place_boat(int x, int y, int dir_index, int ts);

int get_clockwise(int dir);

int get_anticlockwise(int dir);

int get_opposite(int dir);

std::pair<int, int> get_rotated_point(int x, int y, int dir_index, int rotate_dir);

int get_operation(int pdir, int cdir);

bool check_boat_loc_slow(int x, int y, int idx);

void set_obstacle(int x, int y, int dir, int ts);

void open_berth(int berth_id);

void close_berth(int berth_id);

bool locate_berth_area(int x, int y, int id);


#endif
