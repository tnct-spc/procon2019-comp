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
    void signalMatchID(QString Address, QString Token, int MatchID, int team_id, std::vector<int> agent_id, int end_turn);
    std::vector<int> signalGetAgentID(int team_id);


private slots:
    void loadCsvField();
    void on_SendButton_clicked();

private:
    void updateField();
    Ui::Takao *ui;
    std::shared_ptr<GameManager> manager;
};

#endif // TAKAO_H
