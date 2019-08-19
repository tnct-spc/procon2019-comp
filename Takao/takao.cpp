#include "takao.h"
#include "ui_takao.h"

Takao::Takao(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Takao),
    manager(std::make_shared<GameManager>())
{
    ui->setupUi(this);

    connect(this, &Takao::signalLoadField, manager.get(), &GameManager::loadField);
    connect(this, &Takao::signalMatchID, manager.get(), &GameManager::loadMatchID);
}

Takao::~Takao()
{
    delete ui;
}

void Takao::loadCsvField(){
    emit signalLoadField(procon::csv::csvImport(QFileDialog::getOpenFileName(this, tr("Load CSV")).toStdString()));
}

void Takao::loadMatchID(){
    emit signalMatchID(procon::csv::MatchID(QLineEdit))
}
