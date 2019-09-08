#include "gamemanager.h"

GameManager::GameManager() :
    game(std::make_shared<GameSimulator>()),
    visualizer(game->getFieldPtr()),
    field(game->getFieldPtr()),
    algo(2)
{

    connect(&visualizer, &Visualizer::signalResetField, this, &GameManager::resetField);
    connect(&visualizer, &Visualizer::signalRunSimulator, this, &GameManager::runSimulator);
    connect(&visualizer, &Visualizer::signalRunFullSimulation, this, &GameManager::runFullSimulation);
    connect(&visualizer, &Visualizer::signalReverseField, this, &GameManager::reverseField);
    connect(&visualizer, &Visualizer::signalSimulateNextTurn, this, &GameManager::simulateNextTurn);
    connect(&visualizer, &Visualizer::signalMoveAgents, this, &GameManager::moveAgents);
    connect(&visualizer, &Visualizer::signalStrategy, this, &GameManager::strategy);

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
    visualizer.resetAgentAct();
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

void GameManager::loadMatchID(QString IP,QString Token,int MatchID,int Port){

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

void GameManager::moveAgents(const std::vector<procon::Point>& move, std::vector<int> is_delete, bool manual_team){
    //is_deleteは自軍タイル除去時にのみ使う物 基本的に使わなさそう
    std::cout << "turn : " << field->getTurn().now+1 << std::endl << std::endl;
    std::vector<procon::MoveState> agent_act (std::vector<procon::MoveState>(field->getAgentCount()));
    for(int agent = 0; agent < field->getAgentCount(); ++agent){

        procon::Point origin_pos = field->getAgent(manual_team, agent);
        procon::Point pos = move.at(agent);
        procon::Point new_pos = pos;
        new_pos.x -= origin_pos.x;
        new_pos.y -= origin_pos.y;
        //is_deleteなら強制的に削除
        agent_act[agent].move_index = field->getAgent(manual_team,agent).getMoveIndex(move[agent]);
        agent_act[agent].is_delete = is_delete[agent];
    }

    auto auto_move = algo[!manual_team]->agentAct();
    game->addAgentAct(!manual_team, auto_move);
    game->addAgentAct(manual_team,agent_act);

    game->changeTurn(true);
    now_field = field->getTurn().now;
    visualizer.update();

}

void GameManager::strategy(std::vector<std::vector<bool>> strategy){
    clicked = strategy;
}
