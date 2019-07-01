#ifndef EVALUATEROBJECT_H
#define EVALUATEROBJECT_H

#include "field.h"

class EvaluaterObject
{
public:
    EvaluaterObject();
    void append(const procon::Field& field);
    void append(const procon::Field& field, bool side, int moves);
    std::vector<double> calc();
};

#endif // EVALUATEROBJECT_H
