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

namespace random{

    unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;

    unsigned long call(unsigned long mod){
        unsigned long t = (x ^ (x << 11));
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return w > mod ? w % mod : mod;
    }
}

}
