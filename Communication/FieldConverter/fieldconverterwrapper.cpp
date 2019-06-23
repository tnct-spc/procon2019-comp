#include "fieldconverterwrapper.h"

FieldConverterWrapper::FieldConverterWrapper(const procon::Field&) :
    data(boost::python::numpy::zeros(boost::python::tuple(), boost::python::numpy::dtype::get_builtin<float>())),
    ndim(0)
{
}
