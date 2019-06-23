#ifndef POLICYPLAYER_H
#define POLICYPLAYER_H

#include "algorithmwrapper.h"
#include "Searcher/searcherwrapper.h"

class PolicyPlayer : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;

public:
    std::vector<procon::MoveState> agentAct() override;
    void setSearcher(std::shared_ptr<SearcherWrapper> _searcher);
    void setSelector(std::function<std::vector<procon::MoveState>(boost::python::numpy::ndarray&)> func);

private:
    std::shared_ptr<SearcherWrapper> searcher;
    std::function<std::vector<procon::MoveState>(boost::python::numpy::ndarray&)> selector;

    // プレイアウト時にpolicyを取得するため
    boost::python::numpy::ndarray last_raw_result;
};

#endif // POLICYPLAYER_H
