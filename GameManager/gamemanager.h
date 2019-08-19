#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "visualizer.h"
#include "gamesimulator.h"

#include "testalgorithm.h"
#include "simplebeamsearch.h"
#include "boarddividealgorithm.h"

class GameManager : public QObject
{

public:
    GameManager();

public slots:
    void loadField(procon::Field field);
    void loadMatchID(procon::Field field);
    void runFullSimulation();
    void resetField();
    void runSimulator();
    bool simulateNextTurn();
    void moveAgents(const std::vector<procon::Point>& inp_vec, std::vector<int> is_delete, bool manual_team);
    void reverseField();

private:

    void setAlgorithms();

    std::shared_ptr<GameSimulator> game;
    Visualizer visualizer;

    std::shared_ptr<const procon::Field> field;
    std::vector<std::shared_ptr<AlgorithmWrapper>> algo;

    unsigned int now_field = 0;

};

#endif // GAMEMANAGER_H
