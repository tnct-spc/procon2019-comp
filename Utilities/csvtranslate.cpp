#include "csvtranslate.h"

namespace procon::csv{

Field csvImport(std::string path){
    std::ifstream input_stream(path);
    assert(input_stream.good());
    std::string data;

    auto comma_input = makeRec([&](auto&& comma_input, auto&& head, auto&&... tail) -> void{
        comma_input._f2(head);
        if(sizeof...(tail))
            comma_input(std::forward<decltype(tail)>(tail)...);
    }, [&](auto&& head){
        std::getline(input_stream, data, ',');
        head = std::stoi(data);
    });

    Point size;
    comma_input(size.x, size.y);
    Turn turn(0, 30);
    std::array<Score, 2> scores;
    std::vector<std::vector<FieldState>> states(size.x, std::vector<FieldState>(size.y));
    std::vector<std::array<Point, 2>> agents;


    Field field(size);

    comma_input(turn.now, turn.final);

    for(int side = 0; side < 2; ++side)
        comma_input(scores[side].tile, scores[side].region);

    int agent_count;
    comma_input(agent_count);
    agents.resize(agent_count);

    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            int position;
            comma_input(position);
            agents.at(agent_index)[side] = field.intToPoint(position);
        }

    for(int x_index = 0; x_index < size.x; ++x_index)
        for(int y_index = 0; y_index < size.y; ++y_index){
            FieldState state;
            comma_input(state.tile, state.value);
            states.at(x_index).at(y_index) = state;
        }

    field.setTurn(turn);
    field.setScores(scores);
    field.setStates(states);
    field.setAgents(agents);
    return field;
}

void csvExport(std::string path, const Field& field){

    std::ofstream output_stream;
    output_stream.open(path, std::ios::out | std::ios::app);
    assert(output_stream.good());
    auto comma_output = makeRec([&](auto&& comma_output, auto head, auto... tail) -> void{
        comma_output._f2(head);
        if(sizeof...(tail))
            comma_output(std::forward<decltype(tail)>(tail)...);
    }, [&](auto head){
        output_stream << head << ",";
    });

    auto size = field.getSize();
    comma_output(size.x, size.y, field.getTurn().now, field.getTurn().final);

    auto scores = field.getScores();
    for(int side = 0; side < 2; ++side)
        comma_output(scores[side].tile, scores[side].region);

    int agent_count = field.getAgentCount();
    comma_output(agent_count);

    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index)
            comma_output(field.pointToInt(field.getAgent(side, agent_index)));

    for(int x_index = 0; x_index < size.x; ++x_index)
        for(int y_index = 0; y_index < size.y; ++y_index){
            auto state = field.getState(x_index, y_index);

            if(x_index == size.x - 1 && y_index == size.y - 1)
                output_stream << state.tile << "," << state.value << std::endl;
            else
                comma_output(state.tile, state.value);
        }

    output_stream.close();
}

}
