#include "lastyearalgorithm.h"

LastYearAlgorithm::LastYearAlgorithm(const procon::Field& field, bool side) :
    AlgorithmWrapper(field, side)
{
    size_sum = field.getSize().x * field.getSize().y;
    agent_count = field.getAgentCount();

}

std::vector<procon::MoveState> LastYearAlgorithm::agentAct(){

    std::vector<std::vector<std::pair<int ,std::pair<int,int>>>> poses(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        poses.at(agent_index) = calcSingleAgent(agent_index);

    std::pair<std::pair<int,int> , std::pair<int,int>> ans;
    int max_adv = -1e9;

    std::vector<std::map<std::pair<int,int> , std::vector<std::vector<std::vector<int>>>>> agent_distributions(agent_count);

    auto calc_distribution = [=](std::map<std::pair<int,int> , std::vector<std::vector<std::vector<int>>>>& agent_distribution, std::map<std::pair<int,int>, MapElement> route_map){
        for(auto element : route_map){
            agent_distribution[element.first] = std::vector<std::vector<std::vector<int>>>(params.maxdepth_max+1, std::vector<std::vector<int>>(12, std::vector<int>(12, 0)));
            std::vector<std::list<std::pair<int, int>>> poses = element.second.routes.second;

            for(int route_index = 0; route_index < poses.size(); route_index++){
                int depth = 0;
                for(auto pos : poses.at(route_index)){
                    agent_distribution[element.first].at(depth).at(pos.first).at(pos.second)++;
                    depth++;
                }
            }
        }
    };

    if(params.fix_conflict)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index)
            calc_distribution(agent_distributions.at(agent_index), route_map_agent0);

    auto calc_pena = [&](std::pair<int,int> pos_agent0, std::pair<int,int> pos_agent1){
        std::vector<std::vector<std::vector<std::vector<int>>>> agent_distribution(agent_count);
        for(int agent_index = 0; agent_index < agent_count; ++agent_index)
            agent_distribution.at(agent_index) = agent_distributions.at(agent_index)[pos_agent0];
        double pena = 0.0;
        int min_depth = (*std::min_element(agent_distribution.begin(), agent_distribution.end(), [](const std::vector<std::vector<std::vector<int>>>& c){return c.size();})).size();
        for(int depth = 0; depth < min_depth; depth++){
            for(int x = 0; x < field.getSize().x; x++)
                for(int y = 0; y < field.getSize().y; y++){
                    double pena_value = 1.0 / (depth + 1);
                    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
                        pena_value *= agent_distribution.at(agent_index).at(depth).at(x).at(y);
                    pena += pena_value;
                }
        }
        return pena;
    };

    // ここの更新パートをどうにかする
    for(auto& pos0 : poses_0)
        for(auto& pos1 : poses_1){

            int pena = params.fix_conflict ? calc_pena(pos0.second, pos1.second) * params.pena_ratio : 1;

            int adv = (pos0.first + pos1.first);
            if(pena)
                adv /= pena;

            if(pos0.second != pos1.second && adv >= max_adv){
                max_adv = adv;
                ans = std::make_pair(pos0.second, pos1.second);
            }
        }

    std::pair<int,int> pos_0 = ans.first;
    std::pair<int,int> pos_1 = ans.second;


    // ここに答えが入る
    std::vector<procon::Point> after_positions(agent_count);

    std::vector<procon::MoveState> ret_moves(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index){
        ret_moves.at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index), field.getAgent(side, agent_index).getMoveIndex(after_positions.at(agent_index)));
    }

    return ret_moves;
}

void LastYearAlgorithm::setParams(LastYearAlgorithm::Parameters& param){
    params = param;
}

std::vector<std::pair<int, std::pair<int,int>>> LastYearAlgorithm::calcSingleAgent(int agent){

    // [0,maxdepth]
    int maxdepth = std::min(params.maxdepth_max, field.getTurn().final - field.getTurn().now);

    const procon::Point& agent_pos = field.getAgent(side, agent);

    std::vector<std::vector<Edge>> edges = calcDijkStra(getPosValue(agent_pos), maxdepth, agent);

    std::map<std::pair<int,int>, MapElement> route_map;

    std::priority_queue<std::pair<double, std::list<std::pair<int,int>>>> que;

    for(int pos = 0; pos < size_sum; ++pos)
        for(int depth = 1; depth <= maxdepth; ++depth){
            double score;
            std::list<std::pair<int,int>> route;

            std::tie(score, route) = getRoute(edges, pos, depth);
            if(route.empty())
                continue;
            que.emplace((score / depth) * params.route_length_weight.at(depth - 1), std::move(route));
        }

    int bound = que.size() * params.bound_val;

    for(int index = 0; !que.empty(); ++index){
        double average;
        std::list<std::pair<int,int>> route;

        std::tie(average, route) = que.top();
        que.pop();

        std::pair<int,int> back = *std::next(route.begin());

        MapElement element(back);

        if(route_map.find(back) == route_map.end())
            route_map[back] = MapElement(back);
        route_map[back].addRoute(average, std::move(route));

        if(index >= bound && route_map.size() > 2)
            break;
    }

    std::vector<std::pair<int, std::pair<int,int>>> anses;

    for(auto& map_element : route_map){

        map_element.second.setPutCount(field.getSize());

        const std::pair<int,int>& target_pos = map_element.second.pos;
        const std::vector<std::list<std::pair<int,int>>> routes = map_element.second.routes.second;
        const std::vector<std::vector<int>>& putcounts = map_element.second.put_count;

        std::vector<std::vector<std::vector<int>>> color(3, std::vector<std::vector<int>>(field.getSize().first, std::vector<int>(field.getSize().second, 255)));

        color.at(0).at(target_pos.first).at(target_pos.second) = 128;
        color.at(1).at(agent_pos.first).at(agent_pos.second) = 128;
        color.at(2).at(agent_pos.first).at(agent_pos.second) = 128;

        int put_count_sum = 0.0;
        std::for_each(putcounts.begin(), putcounts.end(), [&put_count_sum](const std::vector<int>& v){put_count_sum += std::accumulate(v.begin(), v.end(), 0);});

        for(int point = 0; point < size_sum; ++point){
            std::pair<int,int> point_pair = getPosPair(point);

            if(point_pair == target_pos)
                continue;
            color.at(1).at(point_pair.first).at(point_pair.second) -= putcounts.at(point_pair.first).at(point_pair.second) * 512 / put_count_sum;
            color.at(2).at(point_pair.first).at(point_pair.second) -= putcounts.at(point_pair.first).at(point_pair.second) * 512 / put_count_sum;
        }

        anses.emplace_back(routes.size(), target_pos);

    }

    if(agent){
        route_map_agent1 = route_map;
    }else{
        route_map_agent0 = route_map;
    }
    std::sort(anses.begin(), anses.end(), std::greater<std::pair<int, std::pair<int,int>>>());

    return anses;

}

std::pair<double, std::list<std::pair<int,int>>> LastYearAlgorithm::getRoute(std::vector<std::vector<Edge>>& input, int target_pos, int depth){

    auto get_list = [&](int pos, int dep){

        std::list<std::pair<int, int>> pos_list;

        if(input.at(pos).at(dep).depth != dep)
            return pos_list;

        while(dep > 0){
            pos_list.emplace_back(getPosPair(pos));

            int bef_pos = pos;
            pos = input.at(pos).at(dep).prev.first;
            dep = input.at(bef_pos).at(dep).prev.second;
        }

        pos_list.emplace_back(getPosPair(pos));

        pos_list.reverse();

        return pos_list;
    };

    std::list<std::pair<int,int>> routes;
    routes = get_list(target_pos, depth);

    return std::make_pair(input.at(target_pos).at(depth).sum, std::move(routes));
}

// [0,maxval]
std::vector<std::vector<LastYearAlgorithm::Edge>> LastYearAlgorithm::calcDijkStra(int start_pos, int maxval, bool agent){

    std::bitset<144> field_states;
    for(int x_pos = 0; x_pos < field.getSize().first; ++x_pos)
        for(int y_pos = 0; y_pos < field.getSize().second; ++y_pos){

            int index = getPosValue(std::make_pair(x_pos, y_pos));

            field_states[index] = (field.getState(x_pos,y_pos).first != side + 1);

        }

    std::vector<std::vector<Edge>> dp_vector(size_sum);

    for(int start_index = 0; start_index < size_sum; ++start_index)
        for(int end_index = 0; end_index <= maxval; ++end_index)
            dp_vector.at(start_index).emplace_back(start_index, field_states);

    dp_vector.at(start_pos).at(0).depth = 0;

    for(int depth = 0; depth < maxval; ++depth)
        for(int point = 0; point < size_sum; ++point)
            if(dp_vector.at(point).at(depth).depth == depth)
                for(int direction = 0; direction < 8; ++direction)
                    if(!outOfRange(point, direction)){

                        int end_index = getRotatePos(point, direction);

                        std::pair<int, int> end_pos = getPosPair(end_index);
                        std::pair<int, int> end_pos_state = field.getState(end_pos.first, end_pos.second);


                        int length = ((end_pos_state.first != (side ? 1 : 2)) ? 1 : 2);
                        double value = (end_pos_state.first == side + 1 ? 0 : end_pos_state.second * length);

                        if(!depth){

                            if(end_pos == field.getAgent(side ^ 1, 0) || end_pos == field.getAgent(side ^ 1, 1))
                                value *= params.conflict_atk_per;

                            if(end_pos_state.first == (side ? 1 : 2) &&
                            ((std::abs(end_pos.first - field.getAgent(side ^ 1, 0).first) <= 1 &&
                            std::abs(end_pos.second - field.getAgent(side ^ 1, 0).second) <= 1) ||
                            (std::abs(end_pos.first - field.getAgent(side ^ 1, 1).first) <= 1 &&
                            std::abs(end_pos.second - field.getAgent(side ^ 1, 1).second) <= 1)))
                                value *= params.conflict_def_per;

                            if(end_pos == field.getAgent(side ^ 1, agent ^ 1))
                                value *= params.conflict_ally_per;
                        }


                        value *= params.point_depth_weight.at(depth);

                        if(length == 2 && depth + 1 == maxval){
                            length = 1;
                            value /= 2;
                        }
                        else if(depth + length > maxval)
                            continue;

                        Edge e = Edge::make(dp_vector.at(point).at(depth), end_index, value, length, end_pos_state.first == side + 1);

                        dp_vector.at(end_index).at(depth + length) = std::max(dp_vector.at(end_index).at(depth + length), e);
                    }

    return dp_vector;
}

std::pair<int, int> LastYearAlgorithm::getPosPair(int x){
    return std::make_pair(x / field.getSize().second, x % field.getSize().second);
}

int LastYearAlgorithm::getPosValue(std::pair<int, int> pos){
    return field.getSize().second * pos.first + pos.second;
}

int LastYearAlgorithm::getRotatePos(int pos, int rotate){
    return pos + field.getSize().second * dx.at(rotate) + dy.at(rotate);
}

bool LastYearAlgorithm::outOfRange(int pos, int rotate){

    std::pair<int, int> field_size = field.getSize();
    std::pair<int,int> pos_pair = getPosPair(pos);
    pos_pair.first += dx.at(rotate);
    pos_pair.second += dy.at(rotate);

    return pos_pair.first < 0 || pos_pair.second < 0 || pos_pair.first >= field_size.first || pos_pair.second >= field_size.second;
}

const std::vector<int> LastYearAlgorithm::dx({1, 1, 0, -1, -1, -1, 0, 1, 0});
const std::vector<int> LastYearAlgorithm::dy({0, -1, -1, -1, 0, 1, 1, 1, 0});
