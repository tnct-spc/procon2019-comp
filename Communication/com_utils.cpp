#include "com_utils.h"

namespace procon{
namespace communication{

namespace bp = boost::python;
namespace np = boost::python::numpy;

bp::tuple simpleFastGreedy(const Field& field, bool side, int agent_move_bound, double temperature){

    assert(temperature >= 0.001);

    int agent_count = field.getAgentCount();
    std::vector<std::vector<std::pair<int, double>>> valid_moves(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        for(int move_index = 0; move_index < 8; ++move_index){
            auto after_point = field.getAgent(side, agent_index).getAppliedPosition(move_index);
            if(field.outOfRangeCheck(after_point).first == false){
                auto state = field.getState(after_point);
                valid_moves.at(agent_index).emplace_back(move_index, state.tile == 0 ? 0 : state.value);
            }
        }
    for(auto& moves : valid_moves){
        sort(moves.begin(), moves.end(), [](const auto& x, const auto& y){return x.second > y.second;});
        if(static_cast<int>(moves.size()) > agent_move_bound)
            moves.resize(agent_move_bound);
    }

    std::vector<std::pair<long double, int>> moves;

    for(int loop_value = 0, max_flag = false; max_flag == false; ++loop_value){
        int value = loop_value;
        max_flag = true;
        std::set<procon::Point> used_points;

        int moves_bitset = 0;
        int point_sum = 0;

        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            int size = valid_moves.at(agent_index).size();
            max_flag &= (value % size == size - 1);

            int move_index = valid_moves.at(agent_index).at(value % size).first;
            auto point = field.getAgent(side, agent_index).getAppliedPosition(move_index);
            auto state = field.getState(point);

            moves_bitset |= move_index << (3 * agent_index);

            if(state.getDecrementedSide() != side && used_points.find(point) == used_points.end())
                point_sum += state.value;
            used_points.emplace(point);

            value /= size;
        }
        moves.emplace_back((point_sum > 0 ? 1 : -1) * std::pow(1.0l * std::abs(point_sum), 1.0 / temperature), moves_bitset);
    }
    // (point, moves)
    sort(moves.begin(), moves.end(), std::greater<>());
    assert(moves.at(0).first >= 0);
    int index_pos = 0;
    int index_nonpos = moves.size();
    while(index_nonpos - index_pos > 1){
        int mid = (index_pos + index_nonpos) >> 1;
        (moves.at(mid).first >= 0 ? index_pos : index_nonpos) = mid;
    }
    moves.resize(index_nonpos);

    long double sum = std::accumulate(moves.begin(), moves.end(), 0.0l, [](const auto& x, const auto& y){return x + y.first;});
    if(sum == 0){
        for(auto& move : moves)
            move.first = 1;
        sum = moves.size();
    }

    np::ndarray policy = np::zeros(bp::make_tuple(agent_count, 8), np::dtype::get_builtin<float>());
    for(auto& move : moves){
        double value = move.first / sum;
        int moves_bitset = move.second;
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            int move_index = (moves_bitset >> (3 * agent_index)) & 7;
            policy[agent_index][move_index] += value;
        }
    }

    std::vector<long double> cum_sum(1, 0.0);
    for(auto& move : moves)
        cum_sum.emplace_back(cum_sum.back() + move.first / sum);
    long double rand_value = 1.0l * procon::random::call() / std::numeric_limits<unsigned long>::max();
    int target_index = std::distance(cum_sum.begin(), std::upper_bound(cum_sum.begin(), cum_sum.end(), rand_value)) - 1;
    int moves_bitset = moves.at(target_index).second;

    np::ndarray moves_ndarr = np::zeros(bp::make_tuple(agent_count), np::dtype::get_builtin<int>());
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        moves_ndarr[agent_index] = (moves_bitset >> (3 * agent_index)) & 7;
    return bp::make_tuple(moves_ndarr, policy);
}

bp::tuple greedyFromBoard(const Board& board, bool side, int agent_move_bound, double temperature){return simpleFastGreedy(board.getField(), side, agent_move_bound, temperature);}

}
}
