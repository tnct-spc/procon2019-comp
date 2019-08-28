#include "buttleapi.h"
#include "com.h"
#include "csvtranslate.h"


procon::ButtleAPI::ButtleAPI(std::string host, int port, std::string token){
    Com::setData(host,port,token);
    Com::checkConnection();
}

std::vector<std::pair<int,int>> procon::ButtleAPI::getMatches() {
    std::string getmatches;
    getmatches = com::getMaches();

}

procon::Field procon::ButtleAPI::getField(int match_id){
    std::string matchStatus;
    matchStatus = Com::getMatchStatus(match_id);
    procon::Field field =
}

void procon::ButtleAPI::setMove(int match_id, std::vector<procon::MoveState> moves, std::vector<int> agent_ids){

    std::string json_str = json::translateFromMoveStateData(moves, agent_ids);
    //cominicationのsendActionという関数を使ってAPIにstringを送信
}
