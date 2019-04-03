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

Point Point::getAppliedPosition(const int move_index) const{
    return *this + getMove(move_index);
}

Point Point::getAppliedPosition(const MoveState &m) const{
    return getAppliedPosition(m.move_index);
}


bool operator==(const Point& p, const Point& q){
    return p.x == q.x && p.y == q.y;
}

bool operator<(const Point& p, const Point& q){
    return p.x < q.x || (p.x == q.x && p.y < q.y);
}

namespace random{

    bool once_called = false;
    unsigned long x = 123456789, y = 362436069, z = 521288629, w;

    unsigned long call(unsigned long mod){
        if(!once_called){
            w = time(nullptr);
            once_called = true;
            call();
        }
        unsigned long t = (x ^ (x << 11));
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        // 剰余をとっている都合上正確な分布にはならないが、64bitの範囲で均等に取られているので誤差の範囲内
        return w > mod ? w % mod : w;
    }
    long call(long min_value, long max_value){
        assert(min_value <= max_value);
        unsigned long length = max_value - min_value + 1;
        return min_value + call(length);
    }
}

}
