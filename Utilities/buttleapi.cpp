#include "buttleapi.h"
#include "com.h"
#include "nlohmann/json.hpp"

//namespace
using std::string;
using nlohmann::json;
using procon::Field;

procon::ButtleAPI::ButtleAPI(std::string host, int port, std::string token){
    Com::setData(host,port,token);
    Com::checkConnection();
}

std::vector<std::pair<int,int>> procon::ButtleAPI::getMatches(){
    std::string getData;
    getData = {"hoge"};

    nlohmann::json js;
    getData = js;
}

procon::Field procon::ButtleAPI::getField(int match_id){
    std::string matchStatus;
    matchStatus = Com::getMatchStatus(match_id);
    procon::Field field = csvDecode(csv_string);
}

void procon::ButtleAPI::setMove(int match_id, std::vector<procon::MoveState> moves, std::vector<int> agent_ids){

    std::string json_str = json::translateFromMoveStateData(moves, agent_ids);
    Com::sendAction(id,arg);
}
