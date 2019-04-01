#ifndef FIELD_H
#define FIELD_H

#include <bitset>
#include <cassert>
#include "utils.h"

namespace procon{

class Field
{
public:
    constexpr Field(Point size = Point(10, 10));
    constexpr Field(int size_x, int size_y);

    constexpr int getState(Point p) const;
    constexpr int getState(int x, int y) const{return getState(Point(x, y));}

    constexpr int pointToInt(const Point& p) const{return p.x * size.y + p.y;}
    constexpr Point IntToPoint(const int i) const{return Point(i / size.y, i % size.y);}

private:

    Point size;
    std::bitset<800> data;
};

}

#endif // FIELD_H
