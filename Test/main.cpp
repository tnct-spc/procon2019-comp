#include <QApplication>
#include <QTimer>
#include "randomplayoutforpolicyplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Py_Initialize();
    boost::python::numpy::initialize();
    RandomPlayoutForPolicyPlayer test;
    QTimer::singleShot(0, &test, &RandomPlayoutForPolicyPlayer::run);
    testServer();
    return a.exec();
}
