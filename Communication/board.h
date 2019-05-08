#ifndef BOARD_H
#define BOARD_H

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
    Board(const GameSimulator& sim);
    np::ndarray getData();
    bp::tuple getTurn();
    bp::tuple getScore();
    int getAgentCount();
    bool isEnded();
    np::ndarray getValidMoves(bool side);
    Board copyBoard(){return Board(sim);}
    Board reverseBoard(){auto rev = field.getSideReversedField(); return Board(GameSimulator(rev));}
    void addAgentAct(bool side, np::ndarray arr);

private:
    GameSimulator sim;
    std::bitset<2> act_flag;
    const procon::Field& field;
};

}
}

#endif // BOARD_H
