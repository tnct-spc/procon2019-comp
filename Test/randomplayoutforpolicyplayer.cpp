#include "randomplayoutforpolicyplayer.h"

#include <iostream>

RandomPlayoutForPolicyPlayer::RandomPlayoutForPolicyPlayer()
{

}

void RandomPlayoutForPolicyPlayer::run(){

    namespace bp = boost::python;
    namespace np = boost::python::numpy;

    std::string csv_path = QFileDialog::getOpenFileName().toStdString();

    std::vector<std::pair<np::ndarray, np::ndarray>> datas;

    // value head なしでの学習とする
    // 盤面, ターン数は固定である事を前提にする
    // MCTSによって性能が底上げされるので, どちらが勝ったかの情報は不要っぽい(？)

    while([&]{

        auto origin_field = procon::csv::csvImport(csv_path);

        int mask = origin_field.getTurn().final * 2;
        std::set<std::pair<int,bool>> save_cond;
        for(int cnt = 0; cnt < 3; ++cnt){
            int rand_value = procon::random::call(mask);
            save_cond.emplace(rand_value / 2, rand_value % 2);
        }

        /*
        auto field_random_init = [&](){
            const auto& size = origin_field.getSize();
            for(int x_index = 0; x_index < size.x; ++x_index)
                for(int y_index = 0; y_index < size.y; ++y_index){
                    int tile_state = procon::random::call(3);
                    if(origin_field.getState(x_index, y_index).isEmpty() == false)
                        origin_field.setTileEmpty(x_index, y_index);
                    if((tile_state & 2) == false)
                        origin_field.setTileSide(x_index, y_index, tile_state & 1);
                }

            std::set<procon::Point> agent_exists_point;
            int agent_count = origin_field.getAgentCount();
            for(int side = 0; side < 2; ++side)
                for(int agent_index = 0; agent_index < agent_count; ++agent_index){
                    procon::Point agent_point;
                    do{
                        agent_point.x = procon::random::call(size.x);
                        agent_point.y = procon::random::call(size.y);
                    }while(agent_exists_point.find(agent_point) != agent_exists_point.end());
                    agent_exists_point.emplace(agent_point);
                    if(origin_field.getState(agent_point).isEmpty() == false)
                        origin_field.setTileEmpty(agent_point);
                    origin_field.setAgent(side, agent_index, agent_point);
                }
            auto turn = origin_field.getTurn();
            turn.now = procon::random::call(turn.final);
            origin_field.setTurn(turn);
            origin_field.calcRegionPoint();
        };
        */

        auto sim = GameSimulator(origin_field);
        const auto& field = sim.getField();
        std::vector<std::pair<np::ndarray, np::ndarray>> ret_data;

        for(int side = 0; side < 2; ++side){
            players.at(side) = std::make_shared<PolicyPlayer>(field, static_cast<bool>(side));
            evaluators.at(side) = std::make_shared<RandomEvaluator>();
            searchers.at(side) = std::make_shared<SimpleSearcher>(evaluators.at(side));

            players.at(side)->setSelector(procon::selector::argmax_selector);
            players.at(side)->setSearcher(searchers.at(side));
        }


        auto agent_move = [&](){

            auto move_0 = players.at(0)->agentAct();
            auto move_1 = players.at(1)->agentAct();

            auto policy_0 = players.at(0)->getLastSearchResult();
            auto policy_1 = players.at(1)->getLastSearchResult();

            auto translated_field_0 = FixedFieldSortedPosSimpleConverter(field).data;
            auto translated_field_1 = FixedFieldSortedPosSimpleConverter(field.getSideReversedField()).data;

            if(save_cond.find(std::make_pair(field.getTurn().now, 0)) != save_cond.end())
                ret_data.emplace_back(translated_field_0, policy_0);
            if(save_cond.find(std::make_pair(field.getTurn().now, 1)) != save_cond.end())
                ret_data.emplace_back(translated_field_1, policy_1);

            sim.addAgentAct(0, move_0);
            sim.addAgentAct(1, move_1);
            sim.changeTurn(true);
        };

        /*
        if(procon::random::call(16) == false)
            field_random_init();
        */

        while(field.getTurn().now != field.getTurn().final)
            agent_move();

        for(auto& data : ret_data)
            datas.emplace_back(std::move(data));

        std::cout << datas.size() << std::endl;
        if(datas.size() >= 1e7){
            // ここに適切な処理を入れる
            return false;
        }
        return true;
    }());
}
