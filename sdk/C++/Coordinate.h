#ifndef CODECRAFTSDK_COORDINATE_H
#define CODECRAFTSDK_COORDINATE_H


class Coordinate {
public:
    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}

    // 赋值运算符重载
    Coordinate& operator=(const Coordinate& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    // 相等运算符重载
    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }

    // 不等运算符重载
    bool operator!=(const Coordinate& other) const {
        return !(*this == other);
    }

    // 加法运算符重载
    Coordinate operator+(const Coordinate& other) const {
        return Coordinate(x + other.x, y + other.y);
    }

    // 减法运算符重载
    Coordinate operator-(const Coordinate& other) const {
        return Coordinate(x - other.x, y - other.y);
    }

    // // 输出流运算符重载
    // friend std::ostream& operator<<(std::ostream& os, const Coordinate& coord) {
    //     os << "(" << coord.x << ", " << coord.y << ")";
    //     return os;
    // }

private:
    int x;
    int y;
};



#endif //CODECRAFTSDK_COORDINATE_H
