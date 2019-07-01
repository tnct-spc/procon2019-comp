#ifndef SOFTMAXSEARCHER_H
#define SOFTMAXSEARCHER_H

#include "algorithmwrapper.h"
#include "softmaxsearchnode.h"
#include "evaluaterobject.h"

class SoftMaxSearcher : public AlgorithmWrapper
{
    SoftMaxSearcher(const procon::Field& field, bool side);
public:
    std::vector<procon::MoveState> agentAct();
    void search(const procon::Field& field);
private:
    int search_count = 1e5;
    double temp = 0.1;
    EvaluaterObject evaluator;
    std::shared_ptr<SoftMaxSearchNode> root;
};

#endif // SOFTMAXSEARCHER_H
