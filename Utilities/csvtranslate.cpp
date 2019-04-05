#include "csvtranslate.h"

namespace procon::csv{

Field csvImport(std::string path){
    std::ifstream input_stream;
    assert(input_stream.bad() == false);
}

void csvExport(std::string path, const Field& field){

    std::ofstream output_stream;
    output_stream.open(path, std::ios::out | std::ios::app);
    assert(output_stream.bad() == false);
    auto comma_output = makeRec([&](auto&& comma_output, auto head, auto... tail) -> void{

        output_stream << head << ",";
        if(sizeof...(tail))
            comma_output(std::forward<decltype(tail)>(tail)...);
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
            comma_output(state.tile, state.value);
        }

    output_stream.close();
}

}
