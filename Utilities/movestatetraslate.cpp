#include "movestatetraslate.h"

std::vector<procon::MoveState> procon::json::translateToMoveStateData(std::string json_str){

}
std::string procon::json::translateFromMoveStateData(std::vector<procon::MoveState>){


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
