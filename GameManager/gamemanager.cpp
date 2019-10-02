#include "gamemanager.h"
#include "com.h"

GameManager::GameManager() :
    game(std::make_shared<GameSimulator>()),
    setting(),
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
    connect(&visualizer, &Visualizer::signalSendMove, this, &GameManager::strategyApplyMove);

    connect(&visualizer, &Visualizer::signalGetField, this, &GameManager::recieveField);
    connect(&visualizer, &Visualizer::signalSetMove, this, &GameManager::sendMove);

    setAlgorithms();

    visualizer.setFieldPtr(field);
    visualizer.show();
}

void GameManager::setAlgorithms(){
    // 仮実装という事で、algoにランダムウォーク2つを入れて、Visualizerがクリックされる毎に更新を行うようなものを考える
    algo.at(0) = std::make_shared<SimpleBeamSearch>(*field, 0);
    algo.at(1) = std::make_shared<SimpleBeamSearch>(*field, 1);
    strategy_algo = std::make_shared<NewAlgorithm>(*field, 0);
    clicked = std::vector<std::vector<bool>>(field->getSize().x, std::vector<bool>(field->getSize().y, false));
    moves = strategy_algo->agentAct(clicked);
}

void GameManager::runFullSimulation(){
    resetField();
    visualizer.resetAgentAct();
    runSimulator();
}

void GameManager::loadMatchID(QString Address, QString Token, int MatchID, int team_id, std::vector<int> agent_id, int end_turn){
    setting = procon::ConnectionSettings(MatchID, Address.toStdString(), Token.toStdString(), team_id, agent_id, end_turn);
    Com::setData(setting.address, setting.token);
    recieveField();
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
void GameManager::timerEvent(QTimerEvent *event){

    if(event->timerId() == update_timer_id)
        recieveField();
    if(event->timerId() == send_timer_id)
        sendMove();
}

void GameManager::updateField(procon::Field& new_field){
    std::cout << "field updated" << std::endl;
    game = std::make_shared<GameSimulator>(new_field);
    field = game->getFieldPtr();

    // visualizer.resetStrategy(false);
    setAlgorithms();

    visualizer.setFieldPtr(field);
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
    moves = strategy_algo->agentAct(clicked);
    visualizer.setMoves(moves);
}

void GameManager::strategyApplyMove(){
    if(game->isSimulationEnded())
        return ;
    if(moves.empty())
        strategy(std::vector<std::vector<bool>>(field->getSize().x, std::vector<bool>(field->getSize().y, false)));
    game->addAgentAct(0, moves);
    game->addAgentAct(1, algo.at(1)->agentAct());
    game->changeTurn(true);
    moves.clear();
    now_field = field->getTurn().now;
    visualizer.update();
}

std::vector<int> GameManager::getAgentIDs(int team_id){
    std::string ret_field = Com::getMatchStatus(setting.match_id);
    std::cout << "-------set agent_id-------" << std::endl;
    std::cout << ret_field << std::endl;
    if(ret_field == "error")
        return std::vector<int>();
    return procon::json::getAgentIds(ret_field, team_id);
}

void GameManager::recieveField(){
    std::string ret_field = Com::getMatchStatus(setting.match_id);
    std::cout << "-------recieve field-------" << std::endl;
    std::cout << ret_field << std::endl;
    if(ret_field == "error")
        return ;
    std::string field_csv = procon::json::translateToFieldCsv(ret_field, setting.team_id, setting.agent_id, setting.end_turn);
    if(field_csv == "team_error"){
        std::cout << "team id error" << std::endl;
        return ;
    }
    if(field_csv == "agent_error"){
        std::cout << "agent id error" << std::endl;
        return ;
    }
    if(prev_field_json == ret_field){
        std::cout << "passed" << std::endl;
        return ;
    }
    prev_field_json = ret_field;
    procon::Field new_field = procon::csv::csvDecode(field_csv);

    updateField(new_field);

    /*
    game = std::make_shared<GameSimulator>(new_field);
    field = game->getFieldPtr();

    visualizer.resetStrategy(false);
    setAlgorithms();

    std::cout << "field updated" << std::endl;

    visualizer.setFieldPtr(field);
    visualizer.update();
    visualizer.repaint();
    */
}

void GameManager::sendMove(){
    if(setting.agent_id.empty()){
        std::cout << "send move error : agent_id is not defined" << std::endl;
        return ;
    }
    std::string action_json_str = procon::json::translateFromMoveStateData(moves, setting.agent_id);
    std::string result = Com::sendAction(setting.match_id, action_json_str);
    std::cout << "-------send move-------" << std::endl;
    std::cout << result << std::endl;
}

void GameManager::setAutoUpdate(bool is_update, double send_interval, double update_interval){
    auto_update = is_update;
    std::cout << "set auto_update: " << (auto_update ? "true" : "false") << std::endl;
    if(send_timer_id != -1)
        killTimer(send_timer_id);
    if(update_timer_id != -1)
        killTimer(update_timer_id);
    if(auto_update){
        send_timer_id = startTimer(send_interval * 1000);
        update_timer_id = startTimer(update_interval * 1000);
    }
}

void GameManager::importCsvField(std::string path){
    std::cout << "import csv field" << std::endl;
    std::ifstream stream(path);
    std::string field_csv((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    std::cout << field_csv << std::endl;
    procon::Field new_field = procon::csv::csvDecode(field_csv);

    updateField(new_field);
}

void GameManager::importJsonField(std::string path){
    std::cout << "import json field" << std::endl;
    std::ifstream stream(path);
    std::string field_json((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    setting.agent_id = procon::json::getAgentIds(field_json, setting.team_id);
    std::cout << "agent_id read" << std::endl;
    for(auto& x : setting.agent_id)
        std::cout << x << " ";
    std::cout << std::endl;
    std::cout << field_json << std::endl;
    std::string field_csv = procon::json::translateToFieldCsv(field_json, setting.team_id, setting.agent_id, setting.end_turn);
    if(field_csv == "team_error"){
        std::cout << "team id error" << std::endl;
        return ;
    }
    if(field_csv == "agent_error"){
        std::cout << "agent id error" << std::endl;
        return ;
    }
    std::cout << field_csv << std::endl;
    procon::Field new_field = procon::csv::csvDecode(field_csv);

    updateField(new_field);
}
