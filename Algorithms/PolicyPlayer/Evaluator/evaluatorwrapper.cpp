#include "evaluatorwrapper.h"

EvaluatorWrapper::EvaluatorWrapper()
{

}

boost::python::numpy::ndarray EvaluatorWrapper::calcPolicy(const procon::Field& field) const{
    auto result = calcInsidePolicy(field);
    auto shape = result.get_shape();
    for(int agent = 0; agent < shape[0]; ++agent){
        float sum = 0;
        for(int move = 0; move < shape[1]; ++move)
            sum += boost::python::extract<float>(result[agent][move]);
        assert(sum != 0.0);
        for(int move = 0; move < shape[1]; ++move)
            result[agent][move] = boost::python::extract<float>(result[agent][move]) / sum;
    }
    return result;
}
