#include "testalgorithm.h"

std::vector<procon::MoveState> TestAlgorithm::agentAct(){

    return testMakeConflict();

    // return std::vector<procon::MoveState>(field.getAgentCount());
}

std::vector<procon::MoveState> TestAlgorithm::testMakeConflict(int conflict_flag){

    const int const_max_depth = 10;
    const int max_width_1 = 30;
    const int max_width_2 = 30;

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
                tile_scores.at(x_index).at(y_index) = field.getState(x_index, y_index).equalSide(side) ? 0 : field.getState(x_index, y_index).value;
            else
                tile_scores.at(x_index).at(y_index) = -1e3;
        }

    // 相手の移動候補マスを除外、相手のいるマスを除外した上で、各エージェント毎にBFSをする
    auto calc_one_agent = [&](int agent_index, int first_move){
        // SimpleBeamSearchのほぼ移植なので、気が向いたらコピペ実装を改善する ってずっと言ってる
        int max_depth = std::min(const_max_depth, field.getTurn().final - field.getTurn().now) - 1;
        const auto& start_point = field.getAgent(side, agent_index).getAppliedPosition(first_move);

        if(field.outOfRangeCheck(start_point).first || enemy_agent_points.find(start_point) != enemy_agent_points.end())
            return -1e9;
        if(enemy_conflict_points.find(start_point) != enemy_conflict_points.end()){
            if(conflict_flag == 1)
                return 1e6;
            else if(conflict_flag == 2)
                return -1e6;
            bool has_advantage = field.getScore(side).getSum() >= field.getScore(!side).getSum();
            return has_advantage ? 1e6 : -1e6;
        }

        using que_type = std::pair<double, std::vector<procon::Point>>;
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

                while(static_cast<int>(next_que.size()) > max_width_1)
                    next_que.pop();
                while(static_cast<int>(next_next_que.size()) > max_width_1)
                    next_next_que.pop();
            }
            now_que = std::move(next_que);
            next_que = std::move(next_next_que);
        }
        if(now_que.empty())
            return 1e-9;

        // デフォルトでgreaterなため、pri_queの末尾の値が最も重要な値になる
        while(now_que.size() >= 2)
            now_que.pop();
        return now_que.top().first;
    };

    std::vector<std::vector<double>> scores(agent_count, std::vector<double>(8));
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        for(int first_move = 0; first_move < 8; ++first_move){
            scores.at(agent_index).at(first_move) = calc_one_agent(agent_index, first_move);
        }

    // マッチング可能な候補に対して辺を張ると、適当に燃やす埋める問題に帰着することができる
    // と思っていたのですが、制約条件の都合で帰着できないらしい？(二部グラフであるという性質を使えばいけるかも？)
    // とりあえず諦めて、素直にbeamsearchを書きます

    using matching_que_type = std::pair<double, std::vector<int>>;
    std::priority_queue<matching_que_type, std::vector<matching_que_type>, std::greater<matching_que_type>> matching_que;
    matching_que.emplace(0.0, std::vector<int>(0));

    for(int agent_index = 0; agent_index < agent_count; ++agent_index){
        decltype(matching_que) next_matching_que;
        while(!matching_que.empty()){
            std::set<procon::Point> used_points;
            auto [score, moves] = matching_que.top();
            matching_que.pop();
            for(int index = 0; index < agent_index; ++index){
                auto point = field.getAgent(side, index).getAppliedPosition(moves.at(index));
                used_points.emplace(point);
            }
            for(int move_index = 0; move_index < 8; ++move_index){
                auto point = field.getAgent(side, agent_index).getAppliedPosition(move_index);
                if(field.outOfRangeCheck(point).first == true || used_points.find(point) != used_points.end())
                    continue;
                auto after_moves = moves;
                after_moves.emplace_back(move_index);
                next_matching_que.emplace(score + scores.at(agent_index).at(move_index), after_moves);
            }
            while(static_cast<int>(next_matching_que.size()) > max_width_2)
                next_matching_que.pop();
        }
        matching_que = std::move(next_matching_que);
    }
    while(static_cast<int>(matching_que.size()) >= 2)
        matching_que.pop();

    std::vector<procon::MoveState> ret_moves(agent_count);

    auto move_indexes = matching_que.top().second;

    for(int agent_index = 0; agent_index < agent_count; ++agent_index){
        int move_index = move_indexes.at(agent_index);
        auto agent_pos = field.getAgent(side, agent_index);
        ret_moves.at(agent_index) = field.makeMoveState(side, agent_pos, move_index);

    }

    return ret_moves;

}
