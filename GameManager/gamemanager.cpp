#include "gamemanager.h"

GameManager::GameManager() :
    visualizer(),
    algo(2)
{
    visualizer.show();
}

void GameManager::runSimulation(){

    game = std::make_shared<GameSimulator>();
    field = game->getFieldPtr();
    visualizer.setFieldPtr(field);

    // 仮実装という事で、algoにランダムウォーク2つを入れて、Visualizerがクリックされる毎に更新を行うようなものを考える
    algo.at(0) = std::make_shared<SimpleBeamSearch>(*field, 0);
    algo.at(1) = std::make_shared<TestAlgorithm>(*field, 1);

    while(!game->isSimulationEnded()){
        game->turnSimulation(algo.at(0), algo.at(1));
        visualizer.update();
        visualizer.repaint();
    }
}
