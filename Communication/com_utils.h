#ifndef COM_UTILS_H
#define COM_UTILS_H

#include <numeric>
#include "board.h"

namespace procon{
namespace communication{

    boost::python::numpy::ndarray simpleFastGreedy(const Board& board, bool side);

}
}

#endif // COM_UTILS_H
