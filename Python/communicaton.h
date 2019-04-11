#ifndef COMMUNICATON_H
#define COMMUNICATON_H

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "field.h"

// 流石にかなり冗長になるため
namespace bp = boost::python;
namespace np = boost::python::numpy;

namespace procon{
namespace communication{

class Board{
public:
    Board(const procon::Field& field) : field(field){}
    np::ndarray getData();
    bp::tuple getScore();
private:
    const procon::Field& field;
};

}
}

#endif // COMMUNICATON_H
