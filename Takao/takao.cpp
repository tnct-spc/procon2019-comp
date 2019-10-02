#include "takao.h"
#include "ui_takao.h"
#include <vector>

Takao::Takao(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Takao),
    manager(std::make_shared<GameManager>())
{
    ui->setupUi(this);

    connect(this, &Takao::signalLoadField, manager.get(), &GameManager::loadField);
    connect(this, &Takao::signalMatchID, manager.get(), &GameManager::loadMatchID);
    connect(this, &Takao::signalGetAgentID, manager.get(), &GameManager::getAgentIDs);
    connect(this, &Takao::signalAutoUpdate, manager.get(), &GameManager::setAutoUpdate);

    updateField();
}

Takao::~Takao()
{
    delete ui;
}

void Takao::loadCsvField(){
    emit signalLoadField(procon::csv::csvImport(QFileDialog::getOpenFileName(this, tr("Load CSV")).toStdString()));
}

void Takao::updateField(){
    std::cout << "Datas were sent to GameManager" << std::endl;
    QString Address = ui->Address->text();
    QString Token = ui->Token->text();

    QString MatchID = ui->MatchID->text();
    int Matchid = MatchID.split(" ")[0].toInt();

    QString TeamID = ui->TeamID->text();
    int team_id = TeamID.split(" ")[0].toInt();

    QString AgentID1 = ui->AgentID1->text();
    int agent_id_1 = AgentID1.split(" ")[0].toInt();

    QString AgentID2 = ui->AgentID2->text();
    int agent_id_2 = AgentID2.split(" ")[0].toInt();

    QString AgentID3 = ui->AgentID3->text();
    int agent_id_3 = AgentID3.split(" ")[0].toInt();

    QString AgentID4 = ui->AgentID4->text();
    int agent_id_4 = AgentID4.split(" ")[0].toInt();

    QString AgentID5 = ui->AgentID5->text();
    int agent_id_5 = AgentID5.split(" ")[0].toInt();

    QString AgentID6 = ui->AgentID6->text();
    int agent_id_6 = AgentID6.split(" ")[0].toInt();

    QString AgentID7 = ui->AgentID7->text();
    int agent_id_7 = AgentID7.split(" ")[0].toInt();

    QString AgentID8 = ui->AgentID8->text();
    int agent_id_8 = AgentID8.split(" ")[0].toInt();

    int end_turn = ui->EndTurn->text().split(" ")[0].toInt();
    std::vector<int> agent_id{agent_id_1,agent_id_2,agent_id_3,agent_id_4,agent_id_5,agent_id_6,agent_id_7,agent_id_8};

    emit signalMatchID(Address, Token, Matchid, team_id, agent_id, end_turn);

}

void Takao::on_SendButton_clicked(){
    QString TeamID = ui->TeamID->text();
    int team_id = TeamID.split(" ")[0].toInt();
    auto agent_ids = emit signalGetAgentID(team_id);
    // kuso
    if(agent_ids.size() >= 1)
        ui->AgentID1->setValue(agent_ids.at(0));
    if(agent_ids.size() >= 2)
        ui->AgentID2->setValue(agent_ids.at(1));
    if(agent_ids.size() >= 3)
        ui->AgentID3->setValue(agent_ids.at(2));
    if(agent_ids.size() >= 4)
        ui->AgentID4->setValue(agent_ids.at(3));
    if(agent_ids.size() >= 5)
        ui->AgentID5->setValue(agent_ids.at(4));
    if(agent_ids.size() >= 6)
        ui->AgentID6->setValue(agent_ids.at(5));
    if(agent_ids.size() >= 7)
        ui->AgentID7->setValue(agent_ids.at(6));
    if(agent_ids.size() >= 8)
        ui->AgentID8->setValue(agent_ids.at(7));

    bool is_auto_update = ui->AutoSendBox->isChecked();
    double send_interval = ui->SendInterval->value();
    double update_interval = ui->UpdateInterval->value();
    emit signalAutoUpdate(is_auto_update, send_interval, update_interval);
    updateField();
}
