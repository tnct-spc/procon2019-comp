#include "utils.h"

namespace procon{

Point& Point::operator+=(const Point& p){
    x += p.x;
    y += p.y;
    return *this;
}

Point& Point::operator-=(const Point& p){
    x -= p.x;
    y -= p.y;
    return *this;
}

bool operator==(const Point& p, const Point& q){
    return p.x == q.x && p.y == q.y;
}

bool operator<(const Point& p, const Point& q){
    return p.x < q.x || (p.x == q.x && p.y < q.y);
}

constexpr Point getMove(int move_index){
    return Point(dx[move_index], dy[move_index]);
}

}
