#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include <algorithm>
#include <tuple>
#include <memory>
#include "algorithmwrapper.h"
#include "testalgorithm.h"

class GameSimulator
{
public:
    GameSimulator();
    GameSimulator(const procon::Field& field);
    template<typename... Args>
    GameSimulator(Args... args);

    const procon::Field& getField(){return *field;}
    const std::shared_ptr<const procon::Field> getFieldPtr(){return field;}

    void addAgentAct(bool side, const std::vector<procon::MoveState>& moves);

    bool isSimulationEnded(){const auto& turn = field->getTurn(); return turn.now == turn.final;}

    static procon::Field runSimulation(std::shared_ptr<AlgorithmWrapper> algo_1, std::shared_ptr<AlgorithmWrapper> algo_2, bool calc_score_flag);
    template<typename... Args>
    static procon::Field runSimulation(std::shared_ptr<AlgorithmWrapper> algo_1, std::shared_ptr<AlgorithmWrapper> algo_2, bool calc_score_flag, Args... args);

    void turnSimulation(std::shared_ptr<AlgorithmWrapper> algo_1, std::shared_ptr<AlgorithmWrapper> algo_2, bool calc_score_flag);

    void setField(const procon::Field& field){this->field = std::make_shared<procon::Field>(field);}

    void changeTurn(bool calc_score_flag);

private:

    std::shared_ptr<procon::Field> field;
    std::vector<std::vector<procon::MoveState>> acts;
    std::bitset<2> acts_flag;
};

#endif // GAMESIMULATOR_H
