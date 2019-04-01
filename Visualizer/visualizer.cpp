#include "visualizer.h"
#include "ui_visualizer.h"

Visualizer::Visualizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Visualizer)
{
    ui->setupUi(this);
}

Visualizer::~Visualizer()
{
    delete ui;
}
