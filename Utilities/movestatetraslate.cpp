#include "movestatetraslate.h"

std::vector<procon::MoveState> procon::json::translateToMoveStateData(std::string json_str){

}
std::string procon::json::translateFromMoveStateData(std::vector<procon::MoveState> move_states, std::vector<int> agent_ids){

    nlohmann::json json_data;
    json_data["actions"] = nlohmann::json::array();
    int dx, dy;
    std::string type;

    for(int i = 0; i < move_states.size(); i++){
        std::tie(type, dx, dy) = convertFromMoveState(move_states.at(i));

        nlohmann::json array_data;
        array_data["agentID"] = agent_ids.at(i);
        array_data["type"] = type;
        array_data["dx"] = dx;
        array_data["dy"] = dy;

        json_data["actions"].push_back(array_data);

    }

    return json_data.dump();
}

std::tuple<std::string, int, int> procon::json::convertFromMoveState(procon::MoveState move_state){

    procon::Point point = move_state.getMove();
    bool is_delete = move_state.isDelete();
    int x = point.x;
    int y = point.y;

    std::string type;
    int dx, dy;

    type = "move";
    dx = x;
    dy = y;

    if (is_delete == true) {
        type = "remove";
    }

    // make_tuple
    return std::make_tuple(type, dx, dy);
}
