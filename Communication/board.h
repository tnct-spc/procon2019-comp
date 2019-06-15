#ifndef BOARD_H
#define BOARD_H

#pragma push_macro("slots")
#undef slots
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#pragma pop_macro("slots")
#include "gamesimulator.h"
#include "csvtranslate.h"

namespace procon{
namespace communication{

namespace bp = boost::python;
namespace np = boost::python::numpy;

struct Board{
    Board(std::string path);
    Board(const GameSimulator& sim);
    static np::ndarray getDataFromField(const procon::Field& field);
    np::ndarray getData();
    np::ndarray getCenterData();
    bp::tuple getTurn();
    bp::tuple getScore();
    int getAgentCount();
    bool isEnded();
    np::ndarray getValidMoves(bool side);
    Board copyBoard(){return Board(sim);}
    Board reverseBoard(){auto rev = field.getSideReversedField(); return Board(GameSimulator(rev));}
    void addAgentAct(bool side, np::ndarray arr);
    const procon::Field& getField() const{return field;}

    GameSimulator sim;
    std::bitset<2> act_flag;
    const procon::Field& field;
};

}
}

#endif // BOARD_H
