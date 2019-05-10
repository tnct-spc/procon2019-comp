#ifndef COM_UTILS_H
#define COM_UTILS_H

#include <numeric>
#include "board.h"

namespace procon{
namespace communication{

    boost::python::numpy::ndarray simpleFastGreedy(const Board& board, bool side, int agent_move_bound, double temperature);

}
}

#endif // COM_UTILS_H
