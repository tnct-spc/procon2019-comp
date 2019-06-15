#ifndef COM_UTILS_H
#define COM_UTILS_H

#include <numeric>
#include "board.h"

namespace procon{
namespace communication{

    bp::tuple simpleFastGreedy(const procon::Field& field, bool side, int agent_move_bound, double temperature);
    bp::tuple greedyFromBoard(const Board& board, bool side, int agent_move_bound, double temperature);

    std::pair<std::vector<int>, std::vector<float>> makeNpyFullData(const Field& field);
    std::pair<std::vector<int>, std::vector<float>> makeNpyFullCenterData(const Field& field);

}
}

#endif // COM_UTILS_H
