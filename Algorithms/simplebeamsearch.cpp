#include "simplebeamsearch.h"

#include <iostream>

std::vector<procon::MoveState> SimpleBeamSearch::agentAct(){

    // 各エージェント毎にそれぞれbeamsearchを行い、それらの結果を合わせて最良のものを探索する
    int agent_count = field.getAgentCount();
    std::vector<procon::MoveState> return_vector(agent_count);

    using que_type = std::tuple<double, unsigned long, std::pair<int,int>>;
    std::priority_queue<que_type> value_que;

    for(int agent_index = 0; agent_index < agent_count; ++agent_index){
        auto result = beamSearch(agent_index);
        for(int move_index = 0; move_index < 9; ++move_index)
            value_que.emplace(result.at(move_index), procon::random::call(), std::make_pair(agent_index, move_index));
    }
    std::set<procon::Point> used_points;
    std::bitset<8> set_flag;
    while(!value_que.empty()){
        auto [agent_index, move_index] = std::get<2>(value_que.top());
        value_que.pop();
        procon::Point moved_point = field.getAgent(side, agent_index).getAppliedPosition(move_index);
        if(field.outOfRangeCheck(moved_point).first == true || used_points.find(moved_point) != used_points.end() || set_flag[agent_index])
            continue;
        if(move_index != 8)
            set_flag.set(agent_index);
        used_points.insert(moved_point);
        return_vector.at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index), move_index);
    }
    return return_vector;
}

std::vector<double> SimpleBeamSearch::beamSearch(int agent_index){
    int max_depth = std::min(this->max_depth, field.getTurn().getRemainTurn());
    const procon::Point& start_point = field.getAgent(side, agent_index);

    // 敵味方問わず、他エージェントの考慮を行わない 途中の盤面の変化の考慮も行わない

    using que_type = std::pair<double, std::vector<procon::Point>>;
    std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> now_que;
    std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> next_que;
    now_que.emplace(0.0, std::vector<procon::Point>(1, start_point));

    for(int depth = 0; depth < max_depth; ++depth){
        std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> next_next_que;
        while(!now_que.empty()){
            auto [value, moves] = now_que.top();
            value *= 1.4;
            now_que.pop();

            // 行動した回数
            std::set<procon::Point> change_points;
            for(auto& point : moves)
                change_points.insert(point);

            const procon::Point& now_point = moves.back();
            for(int move_index = 0; move_index < 9; ++move_index){
                procon::Point moved_point = now_point.getAppliedPosition(move_index);
                if(field.outOfRangeCheck(moved_point).first == true)
                    continue;
                auto new_moves = moves;
                new_moves.emplace_back(moved_point);

                bool already_moved = (change_points.find(moved_point) != change_points.end());
                auto state = field.getState(moved_point);

                if(state.isEmpty() == true && already_moved == false)
                    next_que.emplace(value + state.value, new_moves);
                else if((state.isEmpty() == true && already_moved == true) || state.getDecrementedSide() == side)
                    next_que.emplace(value, new_moves);
                else
                    next_next_que.emplace(value + 2 * state.value, new_moves);
            }

            while(static_cast<int>(next_que.size()) > max_width)
                next_que.pop();
        }
        now_que = std::move(next_que);
        next_que = std::move(next_next_que);
    }
    std::vector<double> result(9, 0);
    std::vector<int> result_count(9, 0);
    while(!now_que.empty()){
        int index = (static_cast<int>(now_que.top().second.size()) == 1) ? 8 : now_que.top().second.at(0).getMoveIndex(now_que.top().second.at(1));
        result.at(index) += now_que.top().first;
        ++result_count.at(index);
        now_que.pop();
    }
    for(int move_index = 0; move_index < 9; ++move_index)
        result.at(move_index) *= std::pow(1.3, result.at(move_index));

    return result;
}
