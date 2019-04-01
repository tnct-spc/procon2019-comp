#ifndef STATES_H
#define STATES_H

#include <vector>

namespace procon{

// constexprのために生配列を使う (C++20ならstd::vectorがconstexprで使える)
constexpr int dx[9] = {-1, -1, -1, 0, 0, 1, 1, 1, 0};
constexpr int dy[9] = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

struct Point
{
    constexpr Point() : x(0), y(0){}
    constexpr Point(int x, int y) : x(x), y(y){}

    Point& operator+=(const Point& p);
    Point& operator-=(const Point& p);

    friend Point operator+(const Point& p, const Point& q){return Point(p) += q;}
    friend Point operator-(const Point& p, const Point& q){return Point(p) -= q;}

    friend bool operator==(const Point& p, const Point& q);
    friend bool operator!=(const Point& p, const Point& q){return !(p == q);}

    friend bool operator<(const Point& p, const Point& q);
    friend bool operator<=(const Point& p, const Point& q){return p == q || p < q;}
    friend bool operator>(const Point& p, const Point& q){return !(p <= q);}
    friend bool operator>=(const Point& p, const Point& q){return !(p < q);}

    int x, y;
};

constexpr Point getMove(int move_index);

struct MoveState{
};


}

#endif // STATES_H
