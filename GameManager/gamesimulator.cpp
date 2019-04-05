#include "gamesimulator.h"

template <typename... Args>
GameSimulator::GameSimulator(Args... args) :
    field(std::make_shared<procon::Field>(procon::Field::generateRandomField(std::forward<Args>(args)...))),
    acts(2, std::vector<procon::MoveState>(field->getAgentCount()))
{
}

GameSimulator::GameSimulator() :
    field(std::make_shared<procon::Field>(procon::Field::generateRandomField())),
    acts(2, std::vector<procon::MoveState>(field->getAgentCount()))
{
}

void GameSimulator::addAgentAct(bool side, const std::vector<procon::MoveState>& moves){

    assert(moves.size() == static_cast<size_t>(field->getAgentCount()));
    acts_flag.set(side);
    acts[side] = moves;
}

void GameSimulator::changeTurn(){

    assert(acts_flag.all());
    // 移動先 => 移動元 に逆辺を張り、トポロジカルソートをする
    // 移動が失敗する、もしくは移動しないなら移動元の頂点の入次数は減らない

    int agent_count = field->getAgentCount();
    std::vector<procon::Point> compressed_points;

    std::vector<std::vector<procon::Point>> before_positions(2, std::vector<procon::Point>(agent_count));
    std::vector<std::vector<procon::Point>> after_positions(2, std::vector<procon::Point>(agent_count));

    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            auto before_position = field->getAgent(side, agent_index);
            auto after_position = before_position;

            after_position = after_position.getAppliedPosition(acts.at(side).at(agent_index).move_index);
            bool is_out_of_range;
            std::tie(is_out_of_range, after_position) = field->outOfRangeCheck(after_position);
            if(is_out_of_range)
                acts.at(side).at(agent_index).is_delete = false;

            before_positions.at(side).at(agent_index) = before_position;
            after_positions.at(side).at(agent_index) = after_position;

            compressed_points.emplace_back(before_position);
            compressed_points.emplace_back(after_position);
        }

    // 座標圧縮
    std::sort(compressed_points.begin(), compressed_points.end());
    compressed_points.erase(std::unique(compressed_points.begin(), compressed_points.end()), compressed_points.end());

    auto get_index = [compressed_points](procon::Point& p){
        auto it = std::lower_bound(compressed_points.begin(), compressed_points.end(), p);
        assert(it != compressed_points.end());
        return std::distance(compressed_points.begin(), it);
    };

    int points_size = compressed_points.size();
    // 各点に対応する移動候補
    std::vector<std::vector<std::pair<bool, int>>> candidate_points(points_size);
    // 逆辺の入次数
    std::vector<int> in_count(points_size, 0);

    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            candidate_points.at(get_index(after_positions.at(side).at(agent_index))).emplace_back(side, agent_index);
            ++in_count.at(get_index(before_positions.at(side).at(agent_index)));
        }

    // トポロジカルソート用のque
    std::queue<int> ts_que;
    for(int index = 0; index < points_size; ++index)
        if(in_count.at(index) == 0)
            ts_que.emplace(index);

    while(!ts_que.empty()){
        int elm = ts_que.front();
        ts_que.pop();
        if(candidate_points.at(elm).size() != 1)
            continue;
        auto [side, agent_index] = candidate_points.at(elm).front();
        const auto& position = after_positions.at(side).at(agent_index);
        const auto& after_state = field->getState(position);

        if(acts.at(side).at(agent_index).isDelete()){
            if(after_state.isEmpty())
                field->setTileEmpty(position);
        }else{
            // 敵タイルでないなら
            if(after_state.equalSide(!side) == false){
                field->setAgent(side, agent_index, position);
                int before_index = get_index(before_positions.at(side).at(agent_index));
                if(--in_count.at(before_index) == 0)
                    ts_que.emplace(before_index);
            }
        }
    }

    acts_flag.reset();
    field->incrementTurn();
}

void GameSimulator::turnSimulation(std::shared_ptr<AlgorithmWrapper> algo_1, std::shared_ptr<AlgorithmWrapper> algo_2){
    auto move_1 = algo_1->agentAct();
    auto move_2 = algo_2->agentAct();

    addAgentAct(0, move_1);
    addAgentAct(1, move_2);
    changeTurn();
}

template <typename... Args>
procon::Field GameSimulator::runSimulation(std::shared_ptr<AlgorithmWrapper> algo_1, std::shared_ptr<AlgorithmWrapper> algo_2, Args... args){
    assert(algo_1->getSide() == false && algo_2->getSide() == true);
    static GameSimulator sim(std::forward<Args>(args)...);
    while(!sim.isSimulationEnded()){
        auto move_1 = algo_1->agentAct();
        auto move_2 = algo_2->agentAct();
        sim.addAgentAct(0, move_1);
        sim.addAgentAct(1, move_2);

        sim.changeTurn();
    }
    return sim.getField();
}

procon::Field GameSimulator::runSimulation(std::shared_ptr<AlgorithmWrapper> algo_1, std::shared_ptr<AlgorithmWrapper> algo_2){
    return runSimulation(algo_1, algo_2, procon::Point(0, 0));
}
