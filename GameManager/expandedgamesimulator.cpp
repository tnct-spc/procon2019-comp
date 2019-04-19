#include "expandedgamesimulator.h"


void ExpandedGameSimulator::changeTurn(bool calc_score_flag){
    acts_log.emplace_back(acts);
    GameSimulator::changeTurn(calc_score_flag);
}
