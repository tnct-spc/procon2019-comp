#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "field.h"

class GameSimulator
{
public:
    GameSimulator();

    void addAgentAct(bool side, const std::vector<procon::MoveState>& moves);

private:
    void changeTurn();

    procon::Field field;
    std::vector<std::vector<procon::MoveState>> acts;
    std::bitset<2> acts_flag;
};

#endif // GAMESIMULATOR_H
