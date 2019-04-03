#ifndef TESTALGORITHM_H
#define TESTALGORITHM_H

#include "algorithmwrapper.h"

class TestAlgorithm : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    procon::MoveState agentAct() override;
};

#endif // TESTALGORITHM_H
