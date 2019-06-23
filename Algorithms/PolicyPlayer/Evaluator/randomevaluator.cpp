#include "randomevaluator.h"

boost::python::numpy::ndarray EvaluatorWrapper::calcInsidePolicy(const procon::Field& field){

    auto shape = boost::python::make_tuple(field.getAgentCount(), 8);
    auto arr = boost::python::numpy::zeros(shape, boost::python::numpy::dtype::get_builtin<float>());
    for(int agent = 0; agent < shape[0]; ++agent)
        for(int move = 0; move < shape[1]; ++move)
            arr[agent][move] = 0.125;
    return arr;
}
