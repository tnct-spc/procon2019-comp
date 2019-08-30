#include "buttleapi.h"
#include "com.h"
#include "nlohmann/json.hpp"

//namespace
using std::string;
using nlohmann::json;
using procon::Field;
using procon::ButtleAPI;
using std::vector;
using std::pair;

ButtleAPI::ButtleAPI(std::string host, int port, std::string token){
    Com::setData(host,port,token);
    Com::checkConnection();
}

std::vector<std::pair<int,int>> procon::ButtleAPI::getMatches(){
    string getData;
    getData = {"hoge"};

    nlohmann::json js;
    getData >> js;

    vector<pair<int,int>> ret;
    for (short i = 0; ; i++) {
        if (js["id"] == nullptr) break;
        pair<int,int> temp;
        temp.first = js["id"];
        temp.second = js["turn"];
        ret.push_back(temp);
    }
}

Field procon::ButtleAPI::getField(int match_id){
    string matchStatus;
    matchStatus = Com::getMatchStatus(match_id);
    Field field = csvDecode(csv_string);
    return field;
}

void procon::ButtleAPI::setMove(int match_id, std::vector<procon::MoveState> moves, std::vector<int> agent_ids){

    string json_str = json::translateFromMoveStateData(moves, agent_ids);
    Com::sendAction(match_id, json_str);
}
