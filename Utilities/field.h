#ifndef FIELD_H
#define FIELD_H

#include <bitset>
#include <set>
#include <queue>
#include "utils.h"


namespace procon{

class Field
{
public:
    Field(Point size = Point(10, 10));
    Field(int size_x, int size_y);

    const FieldState& getState(Point p) const;
    const FieldState& getState(int x, int y) const{return getState(Point(x, y));}

    void setTile(Point p, int value);
    void setTile(int x, int y, int value){setTile(Point(x, y), value);}

    void setTileSide(Point p, bool side){setTile(p, side + 1);}
    void setTileSide(int x, int y, bool side){setTile(Point(x, y), side + 1);}

    void setTileEmpty(Point p){setTile(p, 0);}
    void setTileEmpty(int x, int y){setTile(Point(x, y), 0);}

    const Point& getSize() const{return size;}
    int getAgentCount() const{return agents.size();}

    const Point& getAgent(bool side, int agent_index) const{return agents.at(agent_index)[side];}
    void setAgent(bool side, int agent_index, Point agent_data);
    void setAgent(bool side, int agent_index, int agent_x, int agent_y){setAgent(side, agent_index, Point(agent_x, agent_y));}

    const std::array<Score, 2>& getScores() const{return scores;}
    const Score& getScore(bool side) const{return scores[side];}

    std::vector<std::vector<std::bitset<2>>> reCalcRegion();
    void calcRegionPoint();

    MoveState makeMoveState(bool side, const Point& p, int move_index) const;

    std::pair<bool, Point> outOfRangeCheck(Point p) const;

    std::bitset<2> getIsRegion(Point p) const;
    std::bitset<2> getIsRegion(int x, int y) const{return getIsRegion(Point(x, y));}

    void setNowTurn(int now_turn);
    void setFinalTurn(int final_turn);
    void incrementTurn();

    const Turn& getTurn() const{return turn;}

    void setSize(const Point& size){this->size = size;}
    void setTurn(const Turn& turn){this->turn = turn;}
    void setScores(const std::array<Score, 2>& scores){this->scores = scores;}
    void setStates(const std::vector<std::vector<FieldState>>& states){this->states = states;}
    void setAgents(const std::vector<std::array<Point, 2>>& agents){this->agents = agents;}

    std::vector<std::vector<int>> getValidMoves(bool side) const;

    constexpr int pointToInt(const Point& p) const{return p.x * size.y + p.y;}
    constexpr Point intToPoint(const int i) const{return Point(i / size.y, i % size.y);}

    static Field generateRandomField(Point size = Point(0, 0), size_t agent_count = 0, int min_value = -8, int max_value = 8, double minus_per = 0.25);

    Field getSideReversedField() const;

private:

    Point size;
    Turn turn;
    std::array<Score, 2> scores;
    std::vector<std::vector<FieldState>> states;
    std::vector<std::array<Point, 2>> agents;
    std::vector<std::vector<std::bitset<2>>> regions;
};

}

#endif // FIELD_H
