#include "takao.h"
#include "ui_takao.h"

Takao::Takao(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Takao),
    manager(std::make_shared<GameManager>())
{
    ui->setupUi(this);

    connect(ui->runButton, &QPushButton::clicked, manager.get(), &GameManager::runFullSimulation);
    connect(ui->csvImportButton, &QPushButton::clicked, this, &Takao::loadCsvField);
    connect(this, &Takao::signalLoadField, manager.get(), &GameManager::loadField);
}

Takao::~Takao()
{
    delete ui;
}

void Takao::loadCsvField(){
    emit signalLoadField(procon::csv::csvImport(QFileDialog::getOpenFileName(this, tr("Load CSV")).toStdString()));
}

void Takao::getFromButtleDataApi(){
    /*
    それぞれのmatchIDについて, 対戦APIからデータを取得する
    取得したデータをcsv形式に変換して, updateFieldに投げる
    */

}

void Takao::updateField(int buttle_index, std::string csv_str){
    /*
    csv_strをField形式に変換する
    buttle_index番目のFieldを更新する
    更新したFieldについて, それをAlgorithmに投げてvector<procon::Movestate>を取得する
    それを行動形式のjsonに変換して, 対戦APIに投げる
    */

}
