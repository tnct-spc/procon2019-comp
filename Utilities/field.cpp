#include "field.h"

namespace procon{

procon::Field::Field(Point size) :
    size(size),
    agents(2)
{
}

procon::Field::Field(int size_x, int size_y) :
    size(size_x, size_y),
    agents(2)
{
}


constexpr int Field::getState(Point p) const{
    assert(0 <= p.x && p.x < size.x && 0 <= p.y && p.y < size.y);
    auto shift_count = pointToInt(p) * 2;
    return (data[shift_count + 1] << 1) + data[shift_count];
}

}
