#include <QApplication>
#include "generaterandomsimulateddata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GenerateRandomSimulatedData test;
    test.run();
    return 0;
}
