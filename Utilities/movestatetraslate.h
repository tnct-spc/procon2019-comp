#ifndef MOVESTATETRASLATE_H
#define MOVESTATETRASLATE_H

#include "field.h"
#include "nlohmann/json.hpp"

namespace procon::json{
    std::vector<procon::MoveState> translateToMoveStateData(std::string json_str);
    std::string translateFromMoveStateData(std::vector<procon::MoveState> move_states);
    std::tuple<std::string, int, int> convertFromMoveState(procon::MoveState move_state);
};

#endif // MOVESTATETRASLATE_H
