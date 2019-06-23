#ifndef POLICYPLAYER_H
#define POLICYPLAYER_H

#include "algorithmwrapper.h"
#include "Searcher/searcherwrapper.h"
#include "selectors.h"

class PolicyPlayer : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;

public:
    std::vector<procon::MoveState> agentAct() override;
    void setSearcher(std::shared_ptr<SearcherWrapper> _searcher);
    void setSelector(std::function<std::vector<int>(const procon::Field&, const boost::python::numpy::ndarray&)> func);
    boost::python::numpy::ndarray getLastSearchResult();

private:
    std::shared_ptr<SearcherWrapper> searcher;
    std::function<std::vector<int>(const procon::Field&, const boost::python::numpy::ndarray&)> selector = procon::selector::argmax_selector;

    // プレイアウト時にpolicyを取得するため
    boost::python::numpy::ndarray last_raw_result;
};

#endif // POLICYPLAYER_H
