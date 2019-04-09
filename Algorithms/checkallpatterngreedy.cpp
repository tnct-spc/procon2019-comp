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

        if(depth >= max_depth)
            return 0.0;

        auto& value = calc_scores.at(point.x).at(point.y).at(depth);
        if(value != -1e15)
            return value;

        value = 0.0;

        for(int move_index = 0; move_index < 8; ++move_index){
            auto moved_point = point.getAppliedPosition(move_index);
            if(field.outOfRangeCheck(moved_point).first)
                continue;
            const auto& target_state = field.getState(moved_point);
            double target_score = target_state.equalSide(side) ? 0.0 : target_state.value;
            int warp = target_state.equalSide(!side) ? std::min(2, max_depth - depth) : 1;
            value = std::max(value, warp * target_score + rec_func(moved_point, depth + warp));
        }
        return value;
    });

    double max_score = -1e18;
    int max_mask = 0;

    for(int mask = 0; mask < (1 << (3 * agent_count)); ++mask){

        double score_sum = 0.0;

        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            int move_index = (mask >> (3 * agent_index)) & 7;
            auto after_move = field.getAgent(side, agent_index).getAppliedPosition(move_index);

            if(field.outOfRangeCheck(after_move).first)
                continue;
            else{
                const auto& state = field.getState(after_move);
                double score = state.tile * (state.equalSide(!side) ? 2 : 1);
                score += calc_score_func(after_move, state.equalSide(!side) + 1);
                score_sum += score;
            }
        }
        if(max_score <= score_sum){
            max_score = score_sum;
            max_mask = mask;
        }
    }

    std::vector<procon::MoveState> ret_moves(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        ret_moves.at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index),  (max_mask >> (3 * agent_index)) & 7);

    return ret_moves;
}
