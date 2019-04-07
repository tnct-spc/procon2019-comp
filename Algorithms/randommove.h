#ifndef RANDOMMOVE_H
#define RANDOMMOVE_H

#include "algorithmwrapper.h"

class RandomMove : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    std::vector<procon::MoveState> agentAct() override;
};

#endif // RANDOMMOVE_H
