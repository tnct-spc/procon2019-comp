#include "policyplayer.h"

std::vector<procon::MoveState> PolicyPlayer::agentAct(){
    assert(searcher);

    auto raw_result = searcher->search(field);
    last_raw_result = raw_result;
    auto result_movestate = selector(raw_result);

    return result_movestate;
}

void PolicyPlayer::setSearcher(std::shared_ptr<SearcherWrapper> _searcher){
    searcher = _searcher;
}

void PolicyPlayer::setSelector(std::function<std::vector<procon::MoveState>(boost::python::numpy::ndarray&)> func){
    selector = func;
}
