#ifndef FIELDCONVERTERWRAPPER_H
#define FIELDCONVERTERWRAPPER_H

#include "field.h"
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

class FieldConverterWrapper
{
public:
    FieldConverterWrapper(const procon::Field& field);
    // 相互変換ができる必要はない
    boost::python::numpy::ndarray data;
    int ndim;
};

#endif // FIELDCONVERTERWRAPPER_H
