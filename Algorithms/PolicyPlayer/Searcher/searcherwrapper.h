#ifndef SEARCHERWRAPPER_H
#define SEARCHERWRAPPER_H

#include "PolicyPlayer/Evaluator/evaluatorwrapper.h"

class SearcherWrapper
{
public:
    SearcherWrapper(std::shared_ptr<EvaluatorWrapper> evalator);
    virtual boost::python::numpy::ndarray search(const procon::Field& field) = 0;
    void setEvaluator(std::shared_ptr<EvaluatorWrapper> _evaluator);

private:
    std::shared_ptr<EvaluatorWrapper> evaluator;
};

#endif // SEARCHERWRAPPER_H
