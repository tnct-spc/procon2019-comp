#ifndef BOARDDIVIDEALGORITHM_H
#define BOARDDIVIDEALGORITHM_H

#include <tuple>
#include "algorithmwrapper.h"

class BoardDivideAlgorithm : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    std::vector<procon::MoveState> agentAct() override;

private:
    // チェビジェフ距離が[0, always_check_dist)のものを最寄りエージェントに関わらず探索候補にする
    const int always_check_dist = 2;
    const int max_depth = 10;
    const int max_width = 30;
};

#endif // BOARDDIVIDEALGORITHM_H
