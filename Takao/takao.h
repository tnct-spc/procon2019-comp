#ifndef TAKAO_H
#define TAKAO_H

#include <QMainWindow>
#include <QFileDialog>
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
    void signalMatchID(QString Address, QString Token, int MatchID, int team_id, std::vector<int> agent_id, int end_turn);
    std::vector<int> signalGetAgentID(QString Address, QString Token, int MatchID, int team_id);
    void signalAutoUpdate(bool update_flag, double send_interval, double update_interval);
    void signalImportJsonField(std::string path);
    void signalImportCsvField(std::string path);


private slots:
    void on_SendButton_clicked();
    void on_GetMatchDataButton_clicked();
    void on_ImportJSONButton_clicked();
    void on_ImportCSVButton_clicked();

private:
    void updateField(std::vector<int> agent_ids);
    Ui::Takao *ui;
    std::shared_ptr<GameManager> manager;
};

#endif // TAKAO_H
