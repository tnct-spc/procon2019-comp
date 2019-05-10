#ifndef UTILS_H
#define UTILS_H

#include <numeric>
#include "board.h"

namespace bp = boost::python;
namespace np = boost::python::numpy;

namespace procon{
namespace communication{

    np::ndarray simpleFastGreedy(const Board& board, bool side);

}
}

#endif // UTILS_H
