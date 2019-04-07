#include "testalgorithm.h"

std::vector<procon::MoveState> TestAlgorithm::agentAct(){
    return std::vector<procon::MoveState>(field.getAgentCount());
}
