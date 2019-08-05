#ifndef BUTTLEAPI_H
#define BUTTLEAPI_H

#include "field.h"

namespace procon{

class ButtleAPI
{
public:
    ButtleAPI(std::string host, int port, std::string token);
    std::vector<std::pair<int,int>> getMatches();
    procon::Field getField(int match_id);
    void setMove(int match_id, std::vector<procon::MoveState> moves);
};

}

#endif // BUTTLEAPI_H
