#ifndef EVALUATORWRAPPER_H
#define EVALUATORWRAPPER_H

#include "field.h"
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

class EvaluatorWrapper
{
public:
    EvaluatorWrapper();
    boost::python::numpy::ndarray calcPolicy(const procon::Field& field) const;

protected:
    virtual boost::python::numpy::ndarray calcInsidePolicy(const procon::Field& field) const = 0;
};

#endif // EVALUATORWRAPPER_H
