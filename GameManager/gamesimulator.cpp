#include "gamesimulator.h"

template <typename... Args>
GameSimulator::GameSimulator(Args... args)
{
    field = procon::Field::generateRandomField(std::forward<Args>(args)...);
    acts.resize(2, std::vector<procon::MoveState>(field.getAgentCount()));
}

bool GameSimulator::addAgentAct(bool side, const std::vector<procon::MoveState>& moves){
    acts_flag.set(side);

    acts[side] = moves;

    if(acts_flag.all()){
        changeTurn();
        return true;
    }else{
        return false;
    }
}

void GameSimulator::changeTurn(){
    acts_flag.reset();
    field.incrementTurn();
}
