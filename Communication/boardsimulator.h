#ifndef BOARDSIMULATOR_H
#define BOARDSIMULATOR_H

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <gamesimulator.h>
#include <csvtranslate.h>

class BoardSimulator
{
public:
    BoardSimulator(std::string start_csv_str);
    boost::python::tuple changeTurn(boost::python::numpy::ndarray input);
private:
    GameSimulator sim;
};

#endif // BOARDSIMULATOR_H
