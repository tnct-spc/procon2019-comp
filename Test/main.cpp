#include <QApplication>
#include <QTimer>
#include "generatecnpyboarddata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Py_Initialize();
    boost::python::numpy::initialize();
    GenerateCnpyBoardData test;
    QTimer::singleShot(0, &test, &GenerateCnpyBoardData::run);
    return a.exec();
}
