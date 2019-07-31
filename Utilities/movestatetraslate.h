#ifndef MOVESTATETRASLATE_H
#define MOVESTATETRASLATE_H

#include "field.h"

namespace procon::json{
    std::vector<procon::MoveState> translateToMoveStateData(std::string json_str);
    std::string std::tuple<std::string, int, int> convertFromMoveState(procon::MoveState);translateFromMoveStateData(std::vector<procon::MoveState>);
    std::tuple<std::string, int, int> convertFromMoveState(procon::MoveState);
};

#endif // MOVESTATETRASLATE_H
