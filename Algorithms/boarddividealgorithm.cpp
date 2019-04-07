#include "boarddividealgorithm.h"

std::vector<procon::MoveState> BoardDivideAlgorithm::agentAct(){

    int agent_count = field.getAgentCount();
    const auto& size = field.getSize();
    std::vector<std::vector<int>> near_agents(size.x, std::vector<int>(size.y, -1));

    std::queue<std::pair<int, procon::Point>> point_que;

    auto add_to_que = [this, &point_que, &near_agents](const procon::Point& p, int agent_index){
        if(field.outOfRangeCheck(p).first == false && near_agents.at(p.x).at(p.y) == -1){
            near_agents.at(p.x).at(p.y) = agent_index;
            point_que.emplace(agent_index, p);
        }
    };

    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        add_to_que(field.getAgent(side, agent_index), agent_index);

    while(!point_que.empty()){
        auto [agent_index, point] = point_que.front();
        point_que.pop();
        for(int move_index = 0; move_index < 8; ++move_index)
            add_to_que(point.getAppliedPosition(move_index), agent_index);
    }

    auto calc_one_agent = [&](int agent_index){
        std::vector<std::vector<int>> tile_scores(size.x, std::vector<int>(size.y, 0));
        for(int x_index = 0; x_index < size.x; ++x_index)
            for(int y_index = 0; y_index < size.y; ++y_index){
                if(near_agents.at(x_index).at(y_index) == agent_index)
                    tile_scores.at(x_index).at(y_index) += std::max(field.getState(x_index, y_index).value, 0);
            }
        // ここからは単純にBFSをすればよい
        // SimpleBeamSearchのほぼ移植なので、気が向いたらコピペ実装を改善する

        using que_type = std::pair<double, std::vector<procon::Point>>;
        std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> now_que;
        std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> next_que;
        now_que.emplace(0.0, std::vector<procon::Point>(1, field.getAgent(side, agent_index)));

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
                    auto tile_score = tile_scores.at(moved_point.x).at(moved_point.y);

                    if(state.isEmpty() == true && already_moved == false)
                        next_que.emplace(value + tile_score, new_moves);
                    else if((state.isEmpty() == true && already_moved == true) || state.getDecrementedSide() == side)
                        next_que.emplace(value, new_moves);
                    else
                        next_next_que.emplace(value + 2 * tile_score, new_moves);
                }

                while(static_cast<int>(next_que.size()) > max_width)
                    next_que.pop();
            }
            now_que = std::move(next_que);
            next_que = std::move(next_next_que);
        }
        return (static_cast<int>(now_que.top().second.size()) == 1) ? 8 : now_que.top().second.at(0).getMoveIndex(now_que.top().second.at(1));
    };
    std::vector<procon::MoveState> moves(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        moves.at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index), calc_one_agent(agent_index));

    return moves;

}
