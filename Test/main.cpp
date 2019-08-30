#define BOOST_PYTHON_STATIC_LIB

#include <QApplication>
#include <QTimer>
#include "randomplayoutforpolicyplayer.h"
#include "com.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Py_Initialize();
    try{
        testServer();
    }
    catch (boost::python::error_already_set) {
        PyErr_Print();
    }
    return a.exec();
}
