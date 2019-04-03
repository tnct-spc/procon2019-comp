#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "gamesimulator.h"

#include "testalgorithm.h"

class GameManager
{

public:
    GameManager();

    std::vector<std::shared_ptr<AlgorithmWrapper>> algo;

private:
    GameSimulator game;
    const procon::Field& field;
};

#endif // GAMEMANAGER_H
