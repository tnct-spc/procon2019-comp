#include "takao.h"
#include "ui_takao.h"

Takao::Takao(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Takao),
    manager(std::make_shared<GameManager>())
{
    ui->setupUi(this);

}

Takao::~Takao()
{
    delete ui;
}
