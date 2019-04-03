#include "visualizer.h"
#include <QApplication>

#include "field.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto f = procon::Field::generateRandomField();
    Visualizer w(f);
    w.show();

    return a.exec();
}
