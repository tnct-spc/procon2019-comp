#include "testalgorithm.h"

std::vector<procon::MoveState> TestAlgorithm::agentAct(){

    return testMakeConflict();

    // return std::vector<procon::MoveState>(field.getAgentCount());
}

std::vector<procon::MoveState> TestAlgorithm::testMakeConflict(){

    const int const_max_depth = 10;
    const int max_width = 30;
    const long double max_value = 1e9;
    const long double inf_cost = 1e15;

    SimpleBeamSearch enemy(field, !side);
    std::vector<procon::MoveState> moves(enemy.agentAct());

    const auto& size = field.getSize();
    int agent_count = field.getAgentCount();

    std::set<procon::Point> enemy_agent_points, enemy_conflict_points;

    // 相手が移動を選んだ場合(タイルの置かれていないマスに移動とした場合)、コンフリクトに回せる場合はそれをします
    for(int agent_index = 0; agent_index < agent_count; ++agent_index){
        auto before_pos = field.getAgent(!side, agent_index);
        auto after_pos = before_pos.getAppliedPosition(moves.at(agent_index).move_index);
        enemy_agent_points.emplace(before_pos);
        enemy_conflict_points.emplace(after_pos);
    }

    std::vector<std::vector<int>> tile_scores(size.x, std::vector<int>(size.y, 0));
    for(int x_index = 0; x_index < size.x; ++x_index)
        for(int y_index = 0; y_index < size.y; ++y_index){
            if(enemy_agent_points.find(procon::Point(x_index, y_index)) == enemy_agent_points.end() && enemy_conflict_points.find(procon::Point(x_index, y_index)) == enemy_conflict_points.end())
                tile_scores.at(x_index).at(y_index) = std::max(field.getState(x_index, y_index).value, 0);
            else
                tile_scores.at(x_index).at(y_index) = -1e3;
        }

    int invalid_move_count = 0;
    // 相手の移動候補マスを除外、相手のいるマスを除外した上で、各エージェント毎にBFSをする
    auto calc_one_agent = [&](int agent_index, int first_move){
        // SimpleBeamSearchのほぼ移植なので、気が向いたらコピペ実装を改善する ってずっと言ってる
        int max_depth = std::min(const_max_depth, field.getTurn().final - field.getTurn().now) - 1;
        const auto& start_point = field.getAgent(side, agent_index).getAppliedPosition(first_move);

        if(field.outOfRangeCheck(start_point).first || enemy_agent_points.find(start_point) != enemy_agent_points.end() || enemy_conflict_points.find(start_point) != enemy_conflict_points.end()){
            ++invalid_move_count;
            return static_cast<long double>(-inf_cost);
        }

        using que_type = std::pair<long double, std::vector<procon::Point>>;
        std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> now_que;
        std::priority_queue<que_type, std::vector<que_type>, std::greater<que_type>> next_que;
        auto first_vec = {field.getAgent(side, agent_index), start_point};
        now_que.emplace(tile_scores.at(start_point.x).at(start_point.y) * 2, first_vec);

        for(int depth = 0; depth < max_depth - 1; ++depth){
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

                    bool conflict_flag = (enemy_agent_points.find(moved_point) != enemy_agent_points.end() || enemy_conflict_points.find(moved_point) != enemy_conflict_points.end());

                    if(conflict_flag)
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
                while(static_cast<int>(next_next_que.size()) > max_width)
                    next_next_que.pop();
            }
            now_que = std::move(next_que);
            next_que = std::move(next_next_que);
        }
        // デフォルトでgreaterなため、pri_queの末尾の値が最も重要な値になる
        while(now_que.size() >= 2)
            now_que.pop();
        return now_que.top().first;
    };

    std::vector<std::vector<long double>> scores(agent_count, std::vector<long double>(8));
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        for(int first_move = 0; first_move < 8; ++first_move)
            scores.at(agent_index).at(first_move) = calc_one_agent(agent_index, first_move);

    // マッチング可能な候補に対して辺を張ると、適当に燃やす埋める問題に帰着することができる
    procon::Dinic<long double> max_flow(agent_count * 8 + 2);
    std::map<procon::Point, std::vector<int>> field_map;
    int start_index = agent_count * 8;
    int end_index = start_index + 1;
    for(int agent_index = 0; agent_index < agent_count; ++agent_index){
        for(int first_move = 0; first_move < 8; ++first_move){

            int index = agent_index * first_move;
            const auto& agent_pos = field.getAgent(side, agent_index).getAppliedPosition(first_move);

            // 行きの辺が消去されてはいけないので、この辺のコストは最大値にする
            max_flow.addEdge(start_index, agent_index * 8 + first_move, inf_cost);

            assert(max_value >= scores.at(agent_index).at(first_move));
            max_flow.addEdge(index, end_index, max_value - scores.at(agent_index).at(first_move));

            // 同じ位置を複数回踏む必要はないため、ペナルティを設ける
            for(auto& same_point_index : field_map[agent_pos]){
                max_flow.addEdge(index, same_point_index, inf_cost);
                max_flow.addEdge(same_point_index, index, inf_cost);
            }

            field_map[agent_pos].emplace_back(index);

            // 同じエージェントが複数の動作を同時に行う事はできないため、ペナルティを設ける
            for(int target_move = first_move + 1; target_move < 8; ++target_move){
                max_flow.addEdge(index, agent_index * 8 + target_move, inf_cost);
                max_flow.addEdge(agent_index * 8 + target_move, index, inf_cost);
            }
        }
    }
    auto max_f = max_flow.calcMaxFlow(start_index, end_index);
    std::cout << "max advantage : " << invalid_move_count * inf_cost + (long double)(8) * agent_count * max_value - max_f << std::endl;

    for(auto& edge : max_flow.graph.at(start_index))
        if(edge.is_rev == false){
            auto& rev = max_flow.graph[edge.to][edge.rev];
            // std::cout << start_index << " -> " << edge.to << " : " << rev.cap << " / " << edge.cap + rev.cap << std::endl;
            std::cout << start_index << " -> " << edge.to << " : " << max_value - rev.cap << " / " << max_value - (edge.cap + rev.cap) << std::endl;
            if(edge.to % 8 == 7)
                std::cout << std::endl;
        }

    return std::vector<procon::MoveState>(field.getAgentCount());

}
