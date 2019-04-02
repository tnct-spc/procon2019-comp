#ifndef FIELD_H
#define FIELD_H

#include <bitset>
#include <cassert>
#include <vector>
#include "utils.h"

namespace procon{

class Field
{
public:
    Field(Point size = Point(10, 10));
    Field(int size_x, int size_y);

    constexpr int getState(Point p) const;
    constexpr int getState(int x, int y) const{return getState(Point(x, y));}

    const Point& getAgent(bool team, int agent_index) const{return agents.at(agent_index)[team];}
    void setAgent(bool team, int agent_index, Point agent_data){agents.at(agent_index)[team] = agent_data;}

    constexpr int pointToInt(const Point& p) const{return p.x * size.y + p.y;}
    constexpr Point IntToPoint(const int i) const{return Point(i / size.y, i % size.y);}

private:

    Point size;
    std::vector<std::array<Point, 2>> agents;
    std::bitset<800> data;
};

}

#endif // FIELD_H
