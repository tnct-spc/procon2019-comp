#include "gamesimulator.h"

template <typename... Args>
GameSimulator::GameSimulator(Args... args)
{
    field = procon::Field::generateRandomField(std::forward<Args>(args)...);
    acts.resize(2, std::vector<procon::MoveState>(field.getAgentCount()));
}

bool GameSimulator::addAgentAct(bool side, const std::vector<procon::MoveState>& moves){
    acts_flag.set(side);

    acts[side] = moves;

    if(acts_flag.all()){
        changeTurn();
        return true;
    }else{
        return false;
    }
}

void GameSimulator::changeTurn(){
    // 移動先 => 移動元 に逆辺を張り、トポロジカルソートをする
    // 移動が失敗する、もしくは移動しないなら移動元の頂点の入次数は減らない

    int agent_count = field.getAgentCount();
    std::vector<procon::Point> compressed_points;

    std::vector<std::vector<procon::Point>> before_positions(2, std::vector<procon::Point>(agent_count));
    std::vector<std::vector<procon::Point>> moved_positions(2, std::vector<procon::Point>(agent_count));

    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            auto before_position = field.getAgent(side, agent_index);
            auto moved_position = before_position;

            if(acts.at(side).at(agent_index).isDelete() == false){
                moved_position = moved_position.getAppliedPosition(acts.at(side).at(agent_index).move_index);
                moved_position = field.outOfRangeCheck(moved_position).second;
            }

            before_positions.at(side).at(agent_index) = before_position;
            moved_positions.at(side).at(agent_index) = moved_position;

            compressed_points.emplace_back(before_position);
            compressed_points.emplace_back(moved_position);
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
            candidate_points.at(get_index(moved_positions.at(side).at(agent_index))).emplace_back(side, agent_index);
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
        // TODO: ここで領域計算のdiffを取るか、最後に領域計算のやり直しをする
        // TODO: タイル削除への対応がされていない！！！！！！！！
        field.setAgent(side, agent_index, moved_positions.at(side).at(agent_index));
        field.addTileScore(side, field.getState(moved_positions.at(side).at(agent_index)).tile);
        int before_index = get_index(before_positions.at(side).at(agent_index));
        if(--in_count.at(before_index) == 0)
            ts_que.emplace(before_index);
    }

    acts_flag.reset();
    field.incrementTurn();
}
