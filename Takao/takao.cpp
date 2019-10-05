#include "takao.h"
#include "ui_takao.h"

#include "com.h"

Takao::Takao(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Takao),
    manager(std::make_shared<GameManager>())
{
    ui->setupUi(this);

    connect(this, &Takao::signalMatchID, manager.get(), &GameManager::loadMatchID);
    connect(this, &Takao::signalGetAgentID, manager.get(), &GameManager::getAgentIDs);
    connect(this, &Takao::signalAutoUpdate, manager.get(), &GameManager::setAutoUpdate);
    connect(this, &Takao::signalImportCsvField, manager.get(), &GameManager::importCsvField);
    connect(this, &Takao::signalImportJsonField, manager.get(), &GameManager::importJsonField);

    updateField(std::vector<int>());
}

Takao::~Takao()
{
    delete ui;
}

void Takao::updateField(std::vector<int> agent_ids){
    std::cout << "Datas were sent to GameManager" << std::endl;
    QString Address = ui->Address->text();
    QString Token = ui->Token->text();

    QString MatchID = ui->MatchID->text();
    int Matchid = MatchID.split(" ")[0].toInt();

    QString TeamID = ui->TeamID->text();
    int team_id = TeamID.split(" ")[0].toInt();

    int end_turn = ui->EndTurn->text().split(" ")[0].toInt();

    emit signalMatchID(Address, Token, Matchid, team_id, agent_ids, end_turn);

}

void Takao::on_SendButton_clicked(){
    QString Address = ui->Address->text();
    QString Token = ui->Token->text();

    QString MatchID = ui->MatchID->text();
    int Matchid = MatchID.split(" ")[0].toInt();

    QString TeamID = ui->TeamID->text();
    int team_id = TeamID.split(" ")[0].toInt();

    auto agent_ids = emit signalGetAgentID(Address, Token, Matchid, team_id);

    bool is_auto_update = ui->AutoSendBox->isChecked();
    double send_interval = ui->SendInterval->value();
    double update_interval = ui->UpdateInterval->value();
    emit signalAutoUpdate(is_auto_update, send_interval, update_interval);
    updateField(agent_ids);
}

void Takao::on_GetMatchDataButton_clicked(){
    std::cout << "-------get match data-------" << std::endl;
    std::cout << Com::getMatches() << std::endl;
}

void Takao::on_ImportCSVButton_clicked(){
    emit signalImportCsvField(QFileDialog::getOpenFileName(this, tr("Load CSV")).toStdString());
}

void Takao::on_ImportJSONButton_clicked(){
    emit signalImportJsonField(QFileDialog::getOpenFileName(this, tr("Load JSON")).toStdString());
}
