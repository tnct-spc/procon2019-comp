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
    void signalMatchID(QString IP,QString Token,int MatchID,int Port,int team_id, std::vector<int> agent_id, int end_turn);

private slots:
    void loadCsvField();
    void on_Send_clicked();

private:
    Ui::Takao *ui;

    std::shared_ptr<GameManager> manager;
};

#endif // TAKAO_H
