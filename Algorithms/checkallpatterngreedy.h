#ifndef CHECKALLPATTERNGREEDY_H
#define CHECKALLPATTERNGREEDY_H

#include "algorithmwrapper.h"

class CheckAllPatternGreedy : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    std::vector<procon::MoveState> agentAct() override;

private:
    const int before_calc_depth = 3;
};

#endif // CHECKALLPATTERNGREEDY_H
