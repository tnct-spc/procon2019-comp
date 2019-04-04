#include "takao.h"
#include "ui_takao.h"

Takao::Takao(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Takao),
    manager(std::make_shared<GameManager>())
{
    ui->setupUi(this);

    connect(ui->runButton, &QPushButton::clicked, manager.get(), &GameManager::runFullSimulation);
}

Takao::~Takao()
{
    delete ui;
}
