#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "visualizer.h"
#include "gamesimulator.h"

#include "testalgorithm.h"

class GameManager
{

public:
    GameManager();

private:
    GameSimulator game;
    Visualizer visualizer;

    std::shared_ptr<const procon::Field> field;
    std::vector<std::shared_ptr<AlgorithmWrapper>> algo;
};

#endif // GAMEMANAGER_H
