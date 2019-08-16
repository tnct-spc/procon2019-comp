#include "buttleapi.h"

procon::ButtleAPI::ButtleAPI(std::string host, int port, std::string token){

}

std::vector<std::pair<int,int>> procon::ButtleAPI::getMatches() {
    //Communicationにある同名のAPIを叩いてstd::stringを取得する

}

procon::Field procon::ButtleAPI::getField(int match_id){
    //CommunicationにあるgetMatchStatusという関数を使ってstd::stringを取得する
    procon::Field field = csv::csvDecode(std::csv_str);
}

void procon::ButtleAPI::setMove(int match_id, std::vector<procon::MoveState> moves, std::vector<int> agent_ids){

    std::string json_str = json::translateFromMoveStateData(moves, agent_ids);
    //cominicationのsendActionという関数を使ってAPIにstringを送信
}
