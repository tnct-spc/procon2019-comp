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

void GameManager::agentAct(const int turn, const int agent, const std::tuple<int, int, int> tuple_val){
    //field->ifCreateArea(0, 0);
    int type, x_inp, y_inp;
    std::tie(type, x_inp, y_inp) = tuple_val;

    procon::Point agent_pos = field->getAgent(turn, agent);
    procon::Point grid_size = field->getSize();
    int x_pos = agent_pos.x + x_inp;
    int y_pos = agent_pos.y + y_inp;

    if(
        type == 0 ||
        x_pos < 0 || x_pos >= grid_size.x ||
        y_pos < 0 || y_pos >= grid_size.y ||
        (type == 1 && field->getState(x_pos, y_pos).tile == (turn==1 ? 1 : 2)) ||
        (type == 2 && field->getState(x_pos, y_pos).tile == 0)
        ){
        act_stack.at(turn).at(agent) = std::make_tuple(1, agent_pos.x, agent_pos.y);
        return ;
    }
    act_stack.at(turn).at(agent) = std::make_tuple(type, x_pos, y_pos);
}

void GameManager::moveAgents(const std::vector<std::vector<procon::Point>>& move, std::vector<std::vector<int>> is_delete){
    //is_deleteは自軍タイル除去時にのみ使う物 基本的に使わなさそう

    std::cout << "turn : " << field->getTurn().now+1 << std::endl << std::endl;

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < field->getAgentCount(); ++agent){

            procon::Point origin_pos = field->getAgent(side, agent);

            procon::Point pos = move.at(side).at(agent);

            procon::Point new_pos = pos;

            new_pos.x -= origin_pos.x;
            new_pos.y -= origin_pos.y;



            //is_deleteなら強制的に削除
            agentAct(side, agent,  std::make_tuple( ( is_delete.at(side).at(agent) || (field->getState(pos.x, pos.y).tile == (side == 0 ? 2 : 1)) ? 2 : 1 ), new_pos.x, new_pos.y) );

        }

    GameSimulator sim;
    sim.changeTurn(true);

    now_field = field->getTurn().now;

    visualizer.update();
}

