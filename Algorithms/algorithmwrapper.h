#ifndef ALGORITHMWRAPPER_H
#define ALGORITHMWRAPPER_H

#include "field.h"

class AlgorithmWrapper
{

public:
    AlgorithmWrapper(const procon::Field& field, bool side);
    virtual std::vector<procon::MoveState> agentAct() = 0;

    bool getSide(){return side;}

protected:
    const procon::Field& field;
    bool side;
};

#endif // ALGORITHMWRAPPER_H
