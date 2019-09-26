#ifndef TAKAO_H
#define TAKAO_H

#include <QMainWindow>
#include "gamemanager.h"

namespace Ui {
class Takao;
}

class Takao : public QMainWindow
{
    Q_OBJECT

public:
    explicit Takao(QWidget *parent = nullptr);
    ~Takao();

signals:
    void signalLoadField(procon::Field field);
    void signalGameData(QString IP,QString Token,int MatchID,int Port,int team_id,int agent_id_1,int agent_id_2,int agent_id_3,int agent_id_4,int agent_id_5,int agent_id_6,int agent_id_7,int agent_id_8);

private slots:
    void loadCsvField();
    void on_SendButton_clicked();

private:
    Ui::Takao *ui;
    std::shared_ptr<GameManager> manager;
};

#endif // TAKAO_H
