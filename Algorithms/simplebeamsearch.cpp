#include "simplebeamsearch.h"

std::vector<procon::MoveState> SimpleBeamSearch::agentAct(){

    // 各エージェント毎にそれぞれbeamsearchを行い、それらの結果を合わせて最良のものを探索する
    int agent_count = field.getAgentCount();
    std::vector<std::vector<std::pair<int, std::vector<procon::Point>>>> results(agent_count);

    std::vector<procon::MoveState> return_vector;
    // (field.getAgentCount(), procon::MoveState(procon::random::call(8), false));

    for(int agent_index = 0; agent_index < agent_count; ++agent_index){
        results.at(agent_index) = beamSearch(agent_index);
        const procon::Point& start_point = field.getAgent(side, agent_index);
        const procon::Point& moved_point = results.at(agent_index).at(0).second.at(1);
        return_vector.emplace_back(field.makeMoveState(side, start_point, start_point.getMoveIndex(moved_point)));
    }
    return return_vector;
}

std::vector<std::pair<int, std::vector<procon::Point>>> SimpleBeamSearch::beamSearch(int agent_index){
    int max_depth = std::min(max_depth, field.getTurn().final - field.getTurn().now);
    const procon::Point& start_point = field.getAgent(side, agent_index);

    // 敵味方問わず、他エージェントの考慮を行わない 途中の盤面の変化の考慮も行わない

    using que_type = std::pair<int, std::vector<procon::Point>>;
    std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> now_que;
    std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> next_que;
    now_que.emplace(0.0, std::vector<procon::Point>(1, start_point));

    for(int depth = 0; depth < max_depth; ++depth){
        std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> next_next_que;
        while(!now_que.empty()){
            auto [value, moves] = now_que.top();
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
    std::vector<que_type> result;
    while(!now_que.empty()){
        result.emplace_back(now_que.top());
        now_que.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}
