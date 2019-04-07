#include "randommove.h"

std::vector<procon::MoveState> RandomMove::agentAct(){
    // ランダムウォークで移動のみを行う(タイル削除は行わない)
    return std::vector<procon::MoveState>(field.getAgentCount(), procon::MoveState(procon::random::call(8), false));
}
