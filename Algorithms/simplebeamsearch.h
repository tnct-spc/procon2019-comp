#ifndef SIMPLEBEAMSEARCH_H
#define SIMPLEBEAMSEARCH_H

#include <algorithm>
#include <set>
#include <queue>
#include <tuple>
#include <cmath>
#include "algorithmwrapper.h"

class SimpleBeamSearch : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    std::vector<procon::MoveState> agentAct();

private:

    std::vector<double> beamSearch(int agent_index);

    const int max_depth = 5;
    const int max_width = 30;
};

#endif // SIMPLEBEAMSEARCH_H
