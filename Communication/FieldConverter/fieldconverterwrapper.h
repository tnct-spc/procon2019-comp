#ifndef FIELDCONVERTERWRAPPER_H
#define FIELDCONVERTERWRAPPER_H

#include <field.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

class FieldConverterWrapper
{
public:
    FieldConverterWrapper();
    virtual boost::python::numpy::ndarray operator()(const procon::Field& field) = 0;
    // 相互変換ができる必要はない
    int ndim;
};

#endif // FIELDCONVERTERWRAPPER_H
