#ifndef BUTTLEAPI_H
#define BUTTLEAPI_H

#include "field.h"
#include "csvtranslate.h"
#include "movestatetraslate.h"

namespace procon{

class ButtleAPI
{
public:
    ButtleAPI(std::string host, int port, std::string token);
    std::vector<std::pair<int,int>> getMatches();
    procon::Field getField(int match_id);
    void setMove(int match_id, std::vector<procon::MoveState> moves, std::vector<int> agent_ids);
};

}

#endif // BUTTLEAPI_H
