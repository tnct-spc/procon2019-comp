#include "policyplayer.h"

PolicyPlayer::PolicyPlayer(const procon::Field& field, bool side) :
    AlgorithmWrapper(field, side),
    last_raw_result(boost::python::numpy::zeros(boost::python::tuple(), boost::python::numpy::dtype::get_builtin<float>()))
{

}

std::vector<procon::MoveState> PolicyPlayer::agentAct(){
    assert(searcher);

    auto raw_result = searcher->search(field);
    last_raw_result = raw_result;
    auto result_vec = selector(field, raw_result);

    int agent_count = field.getAgentCount();
    std::vector<procon::MoveState> result_movestate(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        result_movestate.at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index), result_vec.at(agent_index));

    return result_movestate;
}

void PolicyPlayer::setSearcher(std::shared_ptr<SearcherWrapper> _searcher){
    searcher = _searcher;
}

void PolicyPlayer::setSelector(std::function<std::vector<int>(const procon::Field&, const boost::python::numpy::ndarray&)> func){
    selector = func;
}

boost::python::numpy::ndarray PolicyPlayer::getLastSearchResult(){
    return last_raw_result;
}
