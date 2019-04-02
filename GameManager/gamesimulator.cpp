#include "gamesimulator.h"

GameSimulator::GameSimulator()
{
    field = procon::Field::generateRandomField();
    acts.resize(2, std::vector<procon::MoveState>(field.getAgentCount()));
}

void GameSimulator::addAgentAct(bool side, const std::vector<procon::MoveState>& moves){
    acts_flag.set(side);

    acts[side] = moves;

    if(acts_flag.all())
        changeTurn();
}

void GameSimulator::changeTurn(){
    acts_flag.reset();
}
