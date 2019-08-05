#include "csvtranslate.h"

namespace procon::csv{

Field csvDecode(std::string csv_string){
    std::stringstream input_stream(csv_string);
    std::string data;

    auto comma_input = makeForEachFunc([&](auto&& head){
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
            int ignore_0, ignore_1;
            comma_input(state.tile, state.value, ignore_0, ignore_1);
            states.at(x_index).at(y_index) = state;
        }

    field.setTurn(turn);
    field.setScores(scores);
    field.setStates(states);
    field.setAgents(agents);
    field.calcRegionPoint();
    return field;
}

Field csvImport(std::string path){
    std::fstream input_stream(path);
    assert(input_stream.good());
    std::istreambuf_iterator<char> it(input_stream);
    std::istreambuf_iterator<char> last;
    std::string str(it, last);
    input_stream.close();
    return csvDecode(str);
}

std::string csvEncode(const Field& field){

    std::stringstream output_stream;
    auto comma_output = makeForEachFunc([&](auto&& head){output_stream << head << ",";});

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
            auto is_region = field.getIsRegion(x_index, y_index);

            if(x_index == size.x - 1 && y_index == size.y - 1)
                output_stream << state.tile << "," << state.value << "," << is_region[0] << "," << is_region[1] << std::endl;
            else
                comma_output(state.tile, state.value, is_region[0], is_region[1]);
        }

    return output_stream.str();
}

void csvExport(std::string path, const Field& field){
    std::string res = csvEncode(field);
    std::ofstream output_stream;
    output_stream.open(path, std::ios::out | std::ios::app);
    assert(output_stream.good());
    std::string str = csvEncode(field);
    output_stream << str;
    output_stream.close();
}

}
