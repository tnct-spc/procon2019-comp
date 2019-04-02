#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <array>
#include <limits>

namespace procon{

constexpr std::array<int, 9> dx = {-1, -1, -1, 0, 0, 1, 1, 1, 0};
constexpr std::array<int, 9> dy = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

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

    explicit operator bool() const{return x || y;}

    int x, y;
};

constexpr Point getMove(int move_index){return Point(dx[move_index], dy[move_index]);}

struct MoveState{

    constexpr MoveState(int move_index, bool is_delete = false) : move_index(move_index), is_delete(is_delete){}

    int move_index;
    bool is_delete;

    constexpr bool isDelete(){return is_delete;}
    constexpr Point getMove(){return procon::getMove(move_index);}
};

struct FieldState{
    constexpr FieldState(int value, int tile = 0) : tile(tile), value(value){}
    int tile, value;
};

struct Score{
    constexpr Score(int tile = 0, int region = 0) : tile(tile), region(region){}
    int tile, region;
    constexpr int getSum() const{return tile + region;}
};

struct Turn{
    constexpr Turn(int now, int final) : now(now), final(final){}
    // 0-indexed, [0, final)
    int now, final;
};

namespace random{

    extern unsigned long x, y, z, w;

    unsigned long call(unsigned long mod = std::numeric_limits<unsigned long>::max());
}

}

#endif // UTILS_H
