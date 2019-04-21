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
    void runFullSimulation();
    void resetField();
    void runSimulator();
    bool simulateNextTurn();
    void reverseField();

private:

    void setAlgorithms();

    std::shared_ptr<GameSimulator> game;
    Visualizer visualizer;

    std::shared_ptr<const procon::Field> field;
    std::vector<std::shared_ptr<AlgorithmWrapper>> algo;

};

#endif // GAMEMANAGER_H
