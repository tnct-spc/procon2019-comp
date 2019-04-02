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

    // TODO: randomの半開区間対応(random::call(st, en)で[st, en)の範囲を取り出したい)
    unsigned long call(unsigned long mod){
        unsigned long t = (x ^ (x << 11));
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        // 剰余をとっている都合上正確な分布にはならないが、64bitの範囲で均等に取られているので誤差の範囲内
        return w > mod ? w % mod : mod;
    }
}

}
