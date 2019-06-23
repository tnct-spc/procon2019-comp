#include "takao.h"
#include <QApplication>
#pragma push_macro("slots")
#undef slots
#include <boost/python.hpp>
#pragma pop_macro("slots")
#include <boost/python/numpy.hpp>

int main(int argc, char *argv[])
{
    Py_Initialize();
    boost::python::numpy::initialize();

    QApplication a(argc, argv);
    Takao w;
    w.show();

    return a.exec();
}
