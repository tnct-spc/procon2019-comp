#ifndef EXPANDEDGAMESIMULATOR_H
#define EXPANDEDGAMESIMULATOR_H

#include "gamesimulator.h"

class ExpandedGameSimulator : public GameSimulator
{
public:
    ExpandedGameSimulator() : GameSimulator(), start_field(*field){}
    ExpandedGameSimulator(const procon::Field& field) : GameSimulator(field), start_field(field){}
    template<typename... Args>
    ExpandedGameSimulator(Args... args) : GameSimulator(args...), start_field(*field){}

    void changeTurn(bool calc_score_flag) override;

private:
    const procon::Field start_field;
    std::vector<std::vector<std::vector<procon::MoveState>>> acts_log;
};

#endif // EXPANDEDGAMESIMULATOR_H
