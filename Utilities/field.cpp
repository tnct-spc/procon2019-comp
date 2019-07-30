#include "field.h"

namespace procon{

procon::Field::Field(Point size) :
    size(size),
    turn(0, 30),
    states(size.x, std::vector<FieldState>(size.y, FieldState(0))),
    agents(2),
    regions(size.x, std::vector<std::bitset<2>>(size.y))
{
    setAbsSum();
}

procon::Field::Field(int size_x, int size_y) :
    size(size_x, size_y),
    turn(0, 30),
    states(size.x, std::vector<FieldState>(size.y, FieldState(0))),
    agents(2),
    regions(size.x, std::vector<std::bitset<2>>(size.y))
{
    setAbsSum();
}

void Field::setTile(Point p, int value){

    assert(0 <= p.x && p.x < size.x && 0 <= p.y && p.y < size.y);
    assert(0 <= value && value <= 2);
    auto& state = states[p.x][p.y];
    assert(state.isEmpty() ^ value == false);
    if(state.isEmpty() && value){
        setIsRegion(p, value - 1, false);
        scores.at(value - 1).tile += state.value;
    }else if(!state.isEmpty() && !value){
        scores.at(state.getDecrementedSide()).tile -= state.value;
    }
    states[p.x][p.y].tile = value;
}

void Field::setIsRegion(Point p, bool side, bool is_region){
    assert(outOfRangeCheck(p).first == false);
    bool bef = regions.at(p.x).at(p.y)[side];
    if(bef && !is_region){
        // reset
        scores.at(side).region -= std::abs(getState(p).value);
        regions.at(p.x).at(p.y)[side] = false;
    }else if(!bef && is_region){
        // set
        scores.at(side).region += std::abs(getState(p).value);
        regions.at(p.x).at(p.y)[side] = true;
    }
}

const FieldState& Field::getState(Point p) const{
    assert(0 <= p.x && p.x < size.x && 0 <= p.y && p.y < size.y);
    return states[p.x][p.y];
}

void Field::setAgent(bool side, int agent_index, Point agent_data){
    assert(0 <= agent_index && agent_index < getAgentCount());
    if(getState(agent_data).isEmpty())
        setTileSide(agent_data, side);
    agents.at(agent_index)[side] = agent_data;
}

std::vector<std::vector<std::bitset<2>>> Field::reCalcRegion(){

    std::vector<std::vector<std::bitset<2>>> ret(size.x, std::vector<std::bitset<2>>(size.y));

    for(int side = 0; side < 2; ++side){
        std::queue<Point> point_que;
        std::bitset<400> visited_flag;

        auto is_wall = [this, side](const Point& p){return getState(p).equalSide(side);};

        auto add_to_que = [this, is_wall, &point_que, &visited_flag](const Point& p){
            if(is_wall(p) == false && visited_flag[pointToInt(p)] == false){
                visited_flag.set(pointToInt(p));
                point_que.emplace(p);
            }
        };

        for(int x_index = 0; x_index < size.x; ++x_index){
            add_to_que(Point(x_index, 0));
            add_to_que(Point(x_index, size.y - 1));
        }
        for(int y_index = 1; y_index < size.y - 1; ++y_index){
            add_to_que(Point(0, y_index));
            add_to_que(Point(size.x - 1, y_index));
        }

        while(!point_que.empty()){
            auto point = point_que.front();
            point_que.pop();
            for(int move_index = 0; move_index < 4; ++move_index){
                auto [out_of_range, moved_pos] = outOfRangeCheck(point.getAppliedPosition(move_index));
                if(out_of_range)
                    continue;
                add_to_que(moved_pos);
            }
        }
        for(int x_index = 0; x_index < size.x; ++x_index)
            for(int y_index = 0; y_index < size.y; ++y_index){
                Point point(x_index, y_index);
                if(visited_flag[pointToInt(point)] == false && is_wall(point) == false)
                    ret.at(x_index).at(y_index).set(side);
            }
    }
    return ret;
}

void Field::calcRegionPoint(){

    for(int side = 0; side < 2; ++side)
        scores.at(side).region = 0;

    regions = reCalcRegion();
    for(int x_index = 0; x_index < size.x; ++x_index)
        for(int y_index = 0; y_index < size.y; ++y_index)
            for(int side = 0; side < 2; ++side)
                if(regions.at(x_index).at(y_index)[side])
                    scores.at(side).region += std::abs(getState(x_index, y_index).value);
}

void Field::applyTileChange(std::vector<std::vector<procon::Point>>& put_tiles, std::vector<std::vector<procon::Point>>& remove_tiles){
    assert(put_tiles.size() == 2);
    assert(remove_tiles.size() == 2);
    for(int side = 0; side < 2; ++side){
        auto& points = put_tiles.at(side);
        auto get_team_tile_num = [&side, this](auto point){
            int tile_cnt = 0;
            for(int move = 0; move < 8; ++move){
                auto moved_point = point.getAppliedPosition(move);
                if(outOfRangeCheck(moved_point).first)
                    continue;
                tile_cnt += (getState(moved_point).getDecrementedSide() == side);
            }
            return tile_cnt;
        };
        std::queue<procon::Point> p_que;
        for(auto& point : points){
            if(get_team_tile_num(point) >= 2)
                for(int move = 0; move < 4; ++move){
                    auto moved_point = point.getAppliedPosition(move);
                    if(!outOfRangeCheck(moved_point).first && getState(moved_point).getDecrementedSide() != side)
                        p_que.emplace(moved_point);
                }
            // apply
            setTileSide(point, side);
        }
        // BFS labeling
    }
    for(int side = 0; side < 2; ++side)
        for(auto& point : remove_tiles.at(side)){
            bool all_tile = true;
            bool all_tile_or_region = true;
            bool any_region = false;
            for(int move = 0; move < 4; ++move){
                auto moved_point = point.getAppliedPosition(move);
                if(outOfRangeCheck(moved_point).first == true){
                    all_tile = false;
                    all_tile_or_region = false;
                }else{
                    bool tile_flag = (getState(moved_point).getDecrementedSide() == side);
                    bool region_flag = regions.at(moved_point.x).at(moved_point.y)[side];
                    all_tile &= tile_flag;
                    all_tile_or_region &= (tile_flag | region_flag);
                    any_region |= region_flag;
                }
            }
            if(all_tile || (any_region && all_tile_or_region)){
                // set region
                setIsRegion(point, side, true);
            }else if(any_region){
                // delete 8 kinbou
            }
            // apply
            setTileEmpty(point);
        }
}

MoveState Field::makeMoveState(bool side, const Point &p, int move_index) const{
    auto [out_of_range, moved_pos] = outOfRangeCheck(p.getAppliedPosition(move_index));
    if(out_of_range)
        return MoveState();
    return MoveState(move_index, getState(moved_pos).equalSide(!side) ? true : false);
}

std::pair<bool, Point> Field::outOfRangeCheck(Point p) const{
    bool out_of_range = false;
    if(p.x < 0){
        out_of_range = true;
        ++p.x;
    }
    if(p.y < 0){
        out_of_range = true;
        ++p.y;
    }
    if(size.x <= p.x){
        out_of_range = true;
        --p.x;
    }
    if(size.y <= p.y){
        out_of_range = true;
        --p.y;
    }
    return std::make_pair(out_of_range, p);
}

std::bitset<2> Field::getIsRegion(Point p) const{

    assert(0 <= p.x && p.x < size.x && 0 <= p.y && p.y < size.y);
    return regions[p.x][p.y];
}

void Field::setNowTurn(int now_turn){
    assert(0 <= now_turn && now_turn <= turn.final);
    turn.now = now_turn;
}

void Field::setFinalTurn(int final_turn){
    assert(0 <= final_turn && turn.now <= final_turn);
    turn.final = final_turn;
}

void Field::incrementTurn(){
    assert(turn.now < turn.final);
    ++turn.now;
}

void Field::setAbsSum(){
    score_abs_sum = 0;
    for(int x_index = 0; x_index < size.x; ++x_index)
        for(int y_index = 0; y_index < size.y; ++y_index)
            score_abs_sum += std::abs(states.at(x_index).at(y_index).value);
}

std::vector<std::vector<int>> Field::getValidMoves(bool side) const{
    int agent_count = getAgentCount();
    std::vector<std::vector<int>> moves(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        for(int move_index = 0; move_index < 8; ++move_index){
            auto moved_point = getAgent(side, agent_index).getAppliedPosition(move_index);
            if(outOfRangeCheck(moved_point).first == false)
                moves.at(agent_index).emplace_back(move_index);
        }
    return moves;
}

Field Field::generateRandomField(Point size, size_t agent_count, int min_value, int max_value, double minus_per){

    assert(min_value <= max_value);
    assert(0 <= max_value);
    // [first, second]
    std::pair<int, int> plus_range(std::max(min_value, 0), max_value);
    std::pair<int, int> minus_range(std::min(min_value, 0), 0);
    bool minus_flag = (minus_range.first != minus_range.second);

    if(static_cast<bool>(size) == false){
        size.x = random::call(10, 20);
        size.y = random::call(10, 20);
    }
    if(agent_count == 0)
        agent_count = random::call(2, 8);

    Field field(size);
    field.turn.final = random::call(30, 60);
    field.agents.resize(agent_count);

    bool is_x_symmetry = random::call(2);

    int random_x_size = is_x_symmetry ? (size.x + 1) / 2 : size.x;
    int random_y_size = is_x_symmetry ? size.y : (size.y + 1) / 2;

    int point_sum;
    do{
        point_sum = 0;
        for(int x_index = 0; x_index < random_x_size; ++x_index)
            for(int y_index = 0; y_index < random_y_size; ++y_index){

                int value;
                if(minus_flag && (1.0 * random::call() / std::numeric_limits<unsigned long>::max()) <= minus_per)
                    value = random::call(minus_range.first, minus_range.second);
                else
                    value = random::call(plus_range.first, plus_range.second);
                field.states[x_index][y_index].value = value;

                if(is_x_symmetry)
                    field.states[size.x - x_index - 1][y_index].value = value;
                else
                    field.states[x_index][size.y - y_index - 1].value = value;

                point_sum += 2 * value;
            }
    }while(point_sum < 0);

    if(is_x_symmetry && size.x % 2 == 1)
        --random_x_size;
    if(!is_x_symmetry && size.y % 2 == 1)
        --random_y_size;

    std::set<Point> agent_values;
    while(agent_values.size() != agent_count)
        agent_values.emplace(random::call(random_x_size), random::call(random_y_size));

    auto it = agent_values.begin();
    auto is_first_side_left = random::call();

    for(int index = 0; it != agent_values.end(); ++it, ++index){

        bool mask = (is_first_side_left >> index) & 1;

        Point inversed_point;
        inversed_point.x = is_x_symmetry ? size.x - it->x - 1 : it->x;
        inversed_point.y = is_x_symmetry ? it->y : size.y - it->y - 1;

        field.scores[mask].tile += field.getState(*it).tile;
        field.scores[!mask].tile += field.getState(inversed_point).tile;
        field.setAgent(mask, index, *it);
        field.setAgent(!mask, index, inversed_point);
    }

    field.setAbsSum();
    return field;
}

Field Field::getSideReversedField() const{
    Field field = *this;

    for(int agent_index = 0; agent_index < getAgentCount(); ++agent_index)
        std::swap(field.agents.at(agent_index).at(0), field.agents.at(agent_index).at(1));
    std::swap(field.scores.at(0), field.scores.at(1));
    for(int x_index = 0; x_index < size.x; ++x_index)
        for(int y_index = 0; y_index < size.y; ++y_index){
            auto& state = field.states.at(x_index).at(y_index);
            if(state.tile)
                state.tile = (state.tile == 1 ? 2 : 1);
        }

    return field;
}

}
