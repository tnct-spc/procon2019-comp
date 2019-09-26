#ifndef NEWALGORITHM_H
#define NEWALGORITHM_H

#include <map>
#include <numeric>
#include "algorithmwrapper.h"

class NewAlgorithm : public AlgorithmWrapper
{
public:
    using AlgorithmWrapper::AlgorithmWrapper;
    // NewAlgorithm(const procon::Field& field, bool side);
public:
    std::vector<procon::MoveState> agentAct() override;
    std::vector<procon::MoveState> agentAct(std::vector<std::vector<bool>>& select_flag);

private:
    const int max_depth = 5;
    const int max_width = 30;
};

#endif // NEWALGORITHM_H
