#include "field.h"

namespace procon{

procon::Field::Field(Point size) :
    size(size),
    turn(0, 30),
    states(size.x, std::vector<FieldState>(size.y, FieldState(0))),
    agents(2)
{
}

procon::Field::Field(int size_x, int size_y) :
    size(size_x, size_y),
    turn(0, 30),
    states(size.x, std::vector<FieldState>(size.y, FieldState(0))),
    agents(2)
{
}

void Field::setTile(Point p, int value){

    assert(0 <= p.x && p.x < size.x && 0 <= p.y && p.y < size.y);
    assert(0 <= value && value <= 2);
    auto& state = states[p.x][p.y];
    assert(state.isEmpty() ^ value == false);
    if(state.isEmpty() && value){
        scores.at(value - 1).tile += state.value;
    }else if(!state.isEmpty() && !value){
        scores.at(state.getDecrementedSide()).tile -= state.value;
    }
    states[p.x][p.y].tile = value;
}

const FieldState& Field::getState(Point p) const{
    assert(0 <= p.x && p.x < size.x && 0 <= p.y && p.y < size.y);
    return states[p.x][p.y];
}

void Field::setAgent(bool side, int agent_index, Point agent_data){
    assert(0 <= agent_index && agent_index < getAgentCount());
    agents.at(agent_index)[side] = agent_data;
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

        field.setTileSide(*it, mask);
        field.setTileSide(inversed_point, !mask);
    }

    return field;
}

}
