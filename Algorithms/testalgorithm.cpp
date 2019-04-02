#include "testalgorithm.h"

procon::MoveState TestAlgorithm::agentAct(){
    // ランダムウォークで移動のみを行う(タイル削除は行わない)
    return procon::MoveState(procon::random::call(8), false);
}
