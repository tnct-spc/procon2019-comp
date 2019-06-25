#ifndef RANDOMEVALUATOR_H
#define RANDOMEVALUATOR_H

#include "PolicyPlayer/Evaluator/evaluatorwrapper.h"

class RandomEvaluator : public EvaluatorWrapper
{
    using EvaluatorWrapper::EvaluatorWrapper;

protected:
    boost::python::numpy::ndarray calcInsidePolicy(const procon::Field& field) const override;
};

#endif // RANDOMEVALUATOR_H
