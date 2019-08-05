#include "searcherwrapper.h"

SearcherWrapper::SearcherWrapper(std::shared_ptr<EvaluatorWrapper> evalator) :
    evaluator(evalator)
{

}

void SearcherWrapper::setEvaluator(std::shared_ptr<EvaluatorWrapper> _evaluator){
    evaluator = _evaluator;
}
