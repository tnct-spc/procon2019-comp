#include "gamemanager.h"

GameManager::GameManager() :
    game(std::make_shared<GameSimulator>()),
    visualizer(),
    field(game->getFieldPtr()),
    algo(2)
{

    connect(&visualizer, &Visualizer::signalResetField, this, &GameManager::resetField);
    connect(&visualizer, &Visualizer::signalRunSimulator, this, &GameManager::runSimulator);
    connect(&visualizer, &Visualizer::signalRunFullSimulation, this, &GameManager::runFullSimulation);
    connect(&visualizer, &Visualizer::signalReverseField, this, &GameManager::reverseField);
    connect(&visualizer, &Visualizer::signalSimulateNextTurn, this, &GameManager::simulateNextTurn);
    connect(&visualizer, &Visualizer::signalMoveAgents, this, &GameManager::moveAgents);

    setAlgorithms();

    visualizer.setFieldPtr(field);
    visualizer.show();
}

void GameManager::setAlgorithms(){
    // 仮実装という事で、algoにランダムウォーク2つを入れて、Visualizerがクリックされる毎に更新を行うようなものを考える
    algo.at(0) = std::make_shared<SimpleBeamSearch>(*field, 0);
    algo.at(1) = std::make_shared<TestAlgorithm>(*field, 1);
}

void GameManager::runFullSimulation(){
    resetField();
    runSimulator();
}

void GameManager::loadField(procon::Field field){

    game = std::make_shared<GameSimulator>(field);
    auto field_ptr = game->getFieldPtr();

    setAlgorithms();

    visualizer.setFieldPtr(field_ptr);
    visualizer.update();
    visualizer.repaint();
}

void GameManager::resetField(){

    game = std::make_shared<GameSimulator>();
    field = game->getFieldPtr();

    setAlgorithms();

    visualizer.setFieldPtr(field);
    visualizer.update();
    visualizer.repaint();
}

void GameManager::reverseField(){
    game->reverseField();
    field = game->getFieldPtr();
    setAlgorithms();
    visualizer.setFieldPtr(field);
    visualizer.update();
    visualizer.repaint();
}

void GameManager::runSimulator(){
    while(simulateNextTurn());
}

bool GameManager::simulateNextTurn(){
    if(game->isSimulationEnded())
        return false;
    game->turnSimulation(algo.at(0), algo.at(1), true);
    visualizer.update();
    visualizer.repaint();
    return true;
}

void GameManager::moveAgents(const std::vector<std::vector<procon::Point>>& inp_vec, std::vector<std::vector<int>> is_delete){
    //is_deleteは自軍タイル除去時にのみ使う物 基本的に使わなさそう

    std::cout << "turn : " << field->getTurn().now+1 << std::endl << std::endl;

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < field->getAgentCount(); ++agent){

            std::pair<int,int> origin_pos = field->getAgent(side, agent);

            std::pair<int,int> pos = move.at(side).at(agent);

            std::pair<int,int> new_pos = pos;

            new_pos.first -= origin_pos.first;
            new_pos.second -= origin_pos.second;



            //is_deleteなら強制的に削除
            agentAct(side, agent,  std::make_tuple( ( is_delete.at(side).at(agent) || (field->getState(pos.first, pos.second).first == (side == 0 ? 2 : 1)) ? 2 : 1 ), new_pos.first, new_pos.second ) );

        }

    GameSimulator sim;
    Visualizer vis;
    sim.changeTurn(true);

    now_field = field->getTurn().now;

    visualizer.update();

    if(field->getTurn().now == field->getTurn().final){
        vis.auto_mode = false;
    }else
        nextMoveForManualMode();
}

