#ifndef FIELD_H
#define FIELD_H

#include <bitset>
#include <cassert>
#include <set>
#include "utils.h"

namespace procon{

class Field
{
public:
    Field(Point size = Point(10, 10));
    Field(int size_x, int size_y);

    int getState(Point p) const;
    int getState(int x, int y) const{return getState(Point(x, y));}

    void setState(Point p, int value);
    void setState(int x, int y, int value){setState(Point(x, y), value);}

    const Point& getSize() const{return size;}
    int getAgentCount() const{return agents.size();}

    const Point& getAgent(bool team, int agent_index) const{return agents.at(agent_index)[team];}
    void setAgent(bool team, int agent_index, Point agent_data);
    void setAgent(bool team, int agent_index, int agent_x, int agent_y){setAgent(team, agent_index, Point(agent_x, agent_y));}

    const Score& getScore(bool side)const {return scores[side];}
    void setScore(bool side, Score score){scores[side] = score;}

    constexpr int pointToInt(const Point& p) const{return p.x * size.y + p.y;}
    constexpr Point intToPoint(const int i) const{return Point(i / size.y, i % size.y);}

    static Field generateRandomField(Point size = Point(0, 0), size_t agent_count = 0, int min_value = -8, int max_value = 8);

private:

    Point size;
    std::array<Score, 2> scores;
    // ここは6bitあれば足りる(33の状態を表せればよい)ため、signed charとかに変えるかもしれない
    std::vector<std::vector<int>> values;
    std::vector<std::array<Point, 2>> agents;
    std::bitset<800> data;
};

}

#endif // FIELD_H
