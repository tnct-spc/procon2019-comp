#ifndef SIMPLEFASTGREEDY_H
#define SIMPLEFASTGREEDY_H

#include "algorithmwrapper.h"
#include <math.h>
#include <numeric>

class SimpleFastGreedy : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    std::vector<procon::MoveState> agentAct() override;
private:
    int agent_move_bound = 8;
    double temperature = 0.01;
};

#endif // SIMPLEFASTGREEDY_H
