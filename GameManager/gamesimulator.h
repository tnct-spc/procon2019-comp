#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include <algorithm>
#include <queue>
#include <tuple>
#include <memory>
#include "algorithmwrapper.h"

class GameSimulator
{
public:
    template<typename... Args>
    GameSimulator(Args... args);

    const procon::Field& getField(){return field;}

    void addAgentAct(bool side, const std::vector<procon::MoveState>& moves);

    bool isSimulationEnded(){const auto& turn = field.getTurn(); return turn.now == turn.final;}

    template<typename... Args>
    static procon::Field runSimulation(std::shared_ptr<AlgorithmWrapper> algo_1, std::shared_ptr<AlgorithmWrapper> algo_2, Args... args);

private:
    void changeTurn();

    procon::Field field;
    std::vector<std::vector<procon::MoveState>> acts;
    std::bitset<2> acts_flag;
};

#endif // GAMESIMULATOR_H
