#include "algofrompython.h"

AlgoFromPython::AlgoFromPython(const procon::Field& field, bool side) :
    AlgorithmWrapper(field, side)
{
    ns = boost::python::import("__main__").attr("__dict__");
    std::ifstream ifs("input.py");
    script((std::istreambuf_iterator<char>(), std::istreambuf_iterator<char>()));
}

std::vector<procon::MoveState> AlgoFromPython::agentAct(){
    boost::python::exec(script.c_str(), ns);
}
