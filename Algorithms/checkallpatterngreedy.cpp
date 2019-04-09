#include "checkallpatterngreedy.h"

std::vector<procon::MoveState> CheckAllPatternGreedy::agentAct(){

    const auto& size = field.getSize();
    int agent_count = field.getAgentCount();

    int max_depth = std::min(before_calc_depth, field.getTurn().final - field.getTurn().now);

    std::vector<std::vector<std::vector<double>>> calc_scores(size.x, std::vector<std::vector<double>>(size.y, std::vector<double>(max_depth, -1e15)));

    // calc_score_func(point, depth): この地点にいる状態(タイルを踏んでいる状態)からmax_depth - depth回移動した場合の最大スコア
    auto calc_score_func = procon::makeRecFunc([&](auto&& rec_func, procon::Point point, int depth) -> double{

        if(field.outOfRangeCheck(point).first)
            return -1e16;

        auto& value = calc_scores.at(point.x).at(point.y).at(depth);
        if(value != -1e15)
            return value;

        if(depth >= max_depth)
            return 0.0;

        value = 0.0;

        for(int move_index = 0; move_index < 8; ++move_index){
            auto moved_point = point.getAppliedPosition(move_index);
            if(field.outOfRangeCheck(moved_point).first)
                continue;
            const auto& target_state = field.getState(moved_point);
            double target_score = target_state.equalSide(side) ? 0.0 : target_state.value;
            int warp = target_state.equalSide(!side) ? 2 : 1;
            value = std::max(value, warp * target_score + rec_func(moved_point, depth + warp));
        }
        return value;
    });

    for(int depth = max_depth - 1; depth >= 0; --depth)
        for(int x_index = 0; x_index < size.x; ++x_index)
            for(int y_index = 0; y_index < size.y; ++y_index)
                calc_score_func(procon::Point(x_index, y_index), depth);


    std::vector<int> agent_moves(agent_count);
    for(int mask = 0; mask < (1 << (3 * agent_count)); ++mask){

        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            agent_moves.at(agent_index) = (mask >> (3 * agent_index)) & 7;
        }
    }
}
