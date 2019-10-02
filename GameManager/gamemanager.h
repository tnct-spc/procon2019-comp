#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QTimer>
#include <QTimerEvent>
#include <fstream>
#include <streambuf>
#include "visualizer.h"
#include "gamesimulator.h"

#include "testalgorithm.h"
#include "simplebeamsearch.h"
#include "boarddividealgorithm.h"
#include "newalgorithm.h"

#include "fieldcsvtranslate.h"
#include "movestatetraslate.h"

class GameManager : public QObject
{
    Q_OBJECT

public:
    GameManager();

public slots:
    void loadMatchID(QString Address, QString Token, int MatchID, int team_id, std::vector<int> agent_id, int end_turn);
    void runFullSimulation();
    void resetField();
    void runSimulator();
    bool simulateNextTurn();
    void moveAgents(const std::vector<procon::Point>& inp_vec, std::vector<int> is_delete, bool manual_team);
    void strategy(std::vector<std::vector<bool>> strategy);
    std::vector<int> getAgentIDs(int team_id);
    void strategyApplyMove();
    void reverseField();

    void recieveField();
    void sendMove();

    void setAutoUpdate(bool is_update, double send_interval, double update_interval);

    void importCsvField(std::string path);
    void importJsonField(std::string path);

private:

    void timerEvent(QTimerEvent *event) override;

    void updateField(procon::Field& new_field);
    void setAlgorithms();

    procon::ConnectionSettings setting;

    std::string prev_field_json;
    std::shared_ptr<GameSimulator> game;
    Visualizer visualizer;

    bool auto_update = false;
    int send_timer_id = -1;
    int update_timer_id = -1;

    std::shared_ptr<const procon::Field> field;
    std::vector<std::shared_ptr<AlgorithmWrapper>> algo;

    std::shared_ptr<NewAlgorithm> strategy_algo;
    std::vector<std::vector<bool>> clicked;
    std::vector<procon::MoveState> moves;

    unsigned int now_field = 0;

};

#endif // GAMEMANAGER_H
