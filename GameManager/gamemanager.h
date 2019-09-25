#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

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
    void loadField(procon::Field field);
    void loadMatchID(QString IP, QString Token, int MatchID, int Port, int team_id, std::vector<int> agent_id, int end_turn);
    void runFullSimulation();
    void resetField();
    void runSimulator();
    bool simulateNextTurn();
    void moveAgents(const std::vector<procon::Point>& inp_vec, std::vector<int> is_delete, bool manual_team);
    void strategy(std::vector<std::vector<bool>> strategy);
    void strategyApplyMove();
    void reverseField();

    void recieveField();
    void sendMove();

private:

    void setAlgorithms();

    procon::ConnectionSettings setting;

    std::shared_ptr<GameSimulator> game;
    Visualizer visualizer;

    std::shared_ptr<const procon::Field> field;
    std::vector<std::shared_ptr<AlgorithmWrapper>> algo;

    std::shared_ptr<NewAlgorithm> strategy_algo;
    std::vector<std::vector<bool>> clicked;
    std::vector<procon::MoveState> moves;

    unsigned int now_field = 0;

};

#endif // GAMEMANAGER_H
