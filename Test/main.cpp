#include <QApplication>
#include <QTimer>
#include "generaterandomsimulateddata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GenerateRandomSimulatedData test;
    QTimer::singleShot(0, &test, &GenerateRandomSimulatedData::run);
    return a.exec();
}
