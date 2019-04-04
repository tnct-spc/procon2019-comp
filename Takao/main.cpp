#include "takao.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Takao w;
    w.show();

    return a.exec();
}
