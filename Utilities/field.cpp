#include "field.h"

namespace procon{

procon::Field::Field(Point size) :
    size(size),
    points(size.x, std::vector<int>(size.y, 0)),
    agents(2)
{
}

procon::Field::Field(int size_x, int size_y) :
    size(size_x, size_y),
    points(size.x, std::vector<int>(size.y, 0)),
    agents(2)
{
}

void Field::setState(Point p, int value){
    assert(0 <= p.x && p.x < size.x && 0 <= p.y && p.y < size.y);
    assert(value <= 0 && value <= 2);
    auto shift_count = pointToInt(p) * 2;
    data.set(shift_count, value & 1);
    data.set(shift_count + 1, value & 2);
}

constexpr int Field::getState(Point p) const{
    assert(0 <= p.x && p.x < size.x && 0 <= p.y && p.y < size.y);
    auto shift_count = pointToInt(p) * 2;
    return (data[shift_count + 1] << 1) + data[shift_count];
}

void Field::setAgent(bool team, int agent_index, Point agent_data){
    assert(0 <= agent_index && agent_index < getAgentCount());
    agents.at(agent_index)[team] = agent_data;
}

Field Field::generateRandomField(Point size, int agent_count, int min_value, int max_value){

    if(static_cast<bool>(size) == false){
        size.x = 10 + random::call(11);
        size.y = 10 + random::call(11);
    }
    if(agent_count == 0)
        agent_count = 2 + random::call(7);

    Field field(size);
    field.agents.resize(agent_count);

    int random_length = max_value - min_value + 1;

    bool is_x_symmetry = random::call(2);

    int random_x_size = is_x_symmetry ? (size.x + 1) / 2 : size.x;
    int random_y_size = is_x_symmetry ? size.y : (size.y + 1) / 2;

    for(int x_index = 0; x_index < random_x_size; ++x_index)
        for(int y_index = 0; y_index < random_y_size; ++y_index){
            field.points[x_index][y_index] = min_value + random::call(random_length);
            if(is_x_symmetry)
                field.points[size.x - x_index - 1][y_index] = min_value + random::call(random_length);
            else
                field.points[x_index][size.y - y_index - 1] = min_value + random::call(random_length);
        }

    if(is_x_symmetry && size.x % 2 == 1)
        --random_x_size;
    if(!is_x_symmetry && size.y % 2 == 1)
        --random_y_size;

    std::set<Point> agent_points;
    while(agent_points.size() != agent_count)
        agent_points.emplace(random::call(random_x_size), random::call(random_y_size));

    auto it = agent_points.begin();
    auto is_first_team_left = random::call();

    for(int index = 0; it != agent_points.end(); ++it, ++index){

        bool mask = (is_first_team_left >> index) & 1;

        Point inverse_point;
        inverse_point.x = is_x_symmetry ? size.x - it->x - 1 : it->x;
        inverse_point.y = is_x_symmetry ? it->y : size.y - it->y - 1;

        field.setAgent(mask, index, *it);
        field.setAgent(!mask, index, inverse_point);
    }

    return field;
}

}
