#ifndef COMMUNICATON_H
#define COMMUNICATON_H

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "gamesimulator.h"
#include "csvtranslate.h"

// 流石にかなり冗長になるため
namespace bp = boost::python;
namespace np = boost::python::numpy;

namespace procon{
namespace communication{

class Board{
public:
    Board(std::string path);
    np::ndarray getData();
    bp::tuple getScore();
    void addAgentAct(bool side, np::ndarray arr);

private:
    GameSimulator sim;
    std::bitset<2> act_flag;
    const procon::Field& field;
};

}
}

#endif // COMMUNICATON_H
