#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "utils.h"
#include "field.h"

class AlgorithmWrapper
{

public:
    AlgorithmWrapper(const procon::Field& field, bool side);
    virtual procon::MoveState agentAct() = 0;

private:
    const procon::Field& field;
    bool side;
};

#endif // ALGORITHMWRAPPER_H
