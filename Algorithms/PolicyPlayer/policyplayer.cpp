#include "policyplayer.h"

std::vector<procon::MoveState> PolicyPlayer::agentAct(){
    assert(searcher);

    auto raw_result = searcher->search(field);
    last_raw_result = raw_result;
    auto result_vec = selector(raw_result);

    int agent_count = field.getAgentCount();
    std::vector<procon::MoveState> result_movestate(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        result_movestate.at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index), result_vec.at(agent_index));

    return result_movestate;
}

void PolicyPlayer::setSearcher(std::shared_ptr<SearcherWrapper> _searcher){
    searcher = _searcher;
}

void PolicyPlayer::setSelector(std::function<std::vector<int>(boost::python::numpy::ndarray&)> func){
    selector = func;
}
