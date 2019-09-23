#include "newalgorithm.h"

std::vector<procon::MoveState> NewAlgorithm::agentAct(){
    // do not use
    abort();
    return std::vector<procon::MoveState>();
}

std::vector<procon::MoveState> NewAlgorithm::agentAct(std::vector<std::vector<bool>>& select_flag){

    int agent_count = field.getAgentCount();

    std::map<procon::Point, std::vector<int>> select_point_agents;
    for(int x_index = 0; x_index < field.getSize().x; ++x_index)
        for(int y_index = 0; y_index < field.getSize().y; ++y_index)
            if(select_flag.at(x_index).at(y_index)){
                procon::Point select_point(x_index, y_index);
                for(int agent_index = 0; agent_index < agent_count; ++agent_index){
                    auto agent_point = field.getAgent(side, agent_index);
                    auto point_diff = select_point - agent_point;
                    if(std::abs(point_diff.x) <= 1 && std::abs(point_diff.y) <= 1)
                        select_point_agents[select_point].emplace_back(agent_index);
                }
            }

    std::vector<procon::Point> select_points_vec;
    std::vector<int> select_points_agent_count;
    for(auto& elm : select_point_agents){
        select_points_vec.emplace_back(elm.first);
        select_points_agent_count.emplace_back(elm.second.size());
    }
    long prod = std::accumulate(select_points_agent_count.begin(), select_points_agent_count.end(), 1L, [](auto x, auto y){return x * y;});

    int points_count = select_points_vec.size();


    // calc all move value

    struct BsState{
        double value;
        procon::Point pos;
        std::set<procon::Point> used;
        BsState make(procon::Point nex_pos, double nex_value){
            auto nex_used = used;
            nex_used.emplace(nex_pos);
            return BsState{value * 1.3 + nex_value, nex_pos, nex_used};
        }
    };

    auto calc = [&](int agent_index, int move_index){
        std::priority_queue<BsState, std::vector<BsState>, std::function<bool(BsState&, BsState&)>> state_que([](auto& a, auto& b){return a.value > b.value;});
        { // init
            auto start_pos = field.getAgent(side, agent_index).getAppliedPosition(move_index);
            if(field.outOfRangeCheck(start_pos).first)
                return -1e9;
            std::set<procon::Point> first_set;
            first_set.emplace(start_pos);
            state_que.emplace(BsState{static_cast<double>(field.getState(start_pos).equalSide(side) ? 0 : field.getState(start_pos).value), start_pos, first_set});
        }

        for(int depth = 0; depth < max_depth; ++depth){
            decltype(state_que) nex_que([](auto& a, auto& b){return a.value > b.value;});
            while(!state_que.empty()){
                auto state = state_que.top();
                state_que.pop();
                for(int move_index = 0; move_index < 8; ++move_index){
                    auto nex_pos = state.pos.getAppliedPosition(move_index);
                    if(field.outOfRangeCheck(nex_pos).first)
                        continue;
                    auto nex_state = field.getState(nex_pos);
                    bool add_flag = (state.used.find(nex_pos) == state.used.end()) && !nex_state.equalSide(side);
                    nex_que.emplace(state.make(nex_pos, add_flag ? nex_state.value : 0));
                    if(static_cast<int>(nex_que.size()) > max_depth)
                        nex_que.pop();
                }
            }
            state_que = std::move(nex_que);
            assert(!state_que.empty());
        }
        while(state_que.size() > 1)
            state_que.pop();
        return state_que.top().value;
    };


    std::vector<std::vector<double>> eval_values(agent_count, std::vector<double>(8));
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        for(int move_index = 0; move_index < 8; ++move_index)
            eval_values.at(agent_index).at(move_index) = calc(agent_index, move_index);


    auto check = [&](std::vector<int>& point_agent_list){
        std::vector<int> moves(agent_count, -1);
        for(int point_index = 0; point_index < points_count; ++point_index){
            int agent_index = point_agent_list.at(point_index);
            moves.at(agent_index) = field.getAgent(side, agent_index).getMoveIndex(select_points_vec.at(point_index));
        }
        // bfs
        std::vector<std::vector<double>> values(8, std::vector<double>(8, -1e9));
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            if(moves.at(agent_index) != -1)
                continue;
            for(int move_index = 0; move_index < 8; ++move_index){
                auto moved_point = field.getAgent(side, agent_index).getAppliedPosition(move_index);
                if(select_point_agents.find(moved_point) != select_point_agents.end())
                    continue;

                values.at(agent_index).at(move_index) = eval_values.at(agent_index).at(move_index);
            }
        }
        // TODO: merge
        return 0;
    };


    for(int flag = 0; flag < prod; ++flag){
        std::vector<int> point_agent_list(points_count);
        int flag_div = flag;
        for(int point_index = 0; point_index < points_count; ++point_index){
            point_agent_list.at(point_index) = select_point_agents[select_points_vec.at(point_index)].at(flag_div % select_points_agent_count.at(point_index));
            flag_div /= select_points_agent_count.at(point_index);
        }
        check(point_agent_list);
    }


    return std::vector<procon::MoveState>();
}
