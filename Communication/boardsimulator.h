#ifndef BOARDSIMULATOR_H
#define BOARDSIMULATOR_H

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <gamesimulator.h>
#include <csvtranslate.h>

class BoardSimulator
{
public:
    BoardSimulator(std::string start_csv_path, std::string save_csv_path);
    boost::python::tuple changeTurn(boost::python::numpy::ndarray input);
private:
    std::string save_csv_path;
    GameSimulator sim;
};

#endif // BOARDSIMULATOR_H
