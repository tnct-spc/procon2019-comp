#include "visualizer.h"
#include "simplevisualizer.h"
#include <QApplication>

#include "field.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Visualizer w;
    SimpleVisualizer x;
    w.show();
    x.show();

    return a.exec();
}
