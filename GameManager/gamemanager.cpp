#include "gamemanager.h"

GameManager::GameManager() :
    game(),
    field(game.getField())
{
    // 仮実装という事で、algoにランダムウォーク2つを入れて、Visualizerがクリックされる毎に更新を行うようなものを考える
    algo.at(0) = std::make_shared<TestAlgorithm>(field, 0);
    algo.at(1) = std::make_shared<TestAlgorithm>(field, 1);
}
