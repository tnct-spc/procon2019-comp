#ifndef ALGOFROMPYTHON_H
#define ALGOFROMPYTHON_H

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <istream>
#include "algorithmwrapper.h"

class AlgoFromPython : public AlgorithmWrapper
{
public:
    AlgoFromPython(const procon::Field& field, bool side);
    std::vector<procon::MoveState> agentAct() override;
private:
    boost::python::object ns;
    std::string script;
};

#endif // ALGOFROMPYTHON_H
