#include "generatecnpyboarddata.h"

GenerateCnpyBoardData::GenerateCnpyBoardData(bool use_center) :
    use_center(use_center)
{
}

void GenerateCnpyBoardData::run(){

    namespace bp = boost::python;
    namespace np = boost::python::numpy;

    std::string csv_path = QFileDialog::getOpenFileName().toStdString();
    std::string save_dir = QFileDialog::getExistingDirectory().toStdString();

    for(int _ = 0; _ < playout_count; ++_, [&]{

        std::string save_filename = save_dir + "/" + std::to_string(procon::random::call()) + ".npz";
        std::vector<std::vector<float>> field_vectors(2);
        std::vector<std::vector<float>> policy_vectors(2);

        procon::communication::Board board(csv_path);
        std::vector<std::pair<procon::communication::Board, std::vector<np::ndarray>>> ret_data;
        auto& field = board.field;
        std::vector<int> shape;

        auto field_random_init = [&](){
            const auto& size = field.getSize();
            for(int x_index = 0; x_index < size.x; ++x_index)
                for(int y_index = 0; y_index < size.y; ++y_index){
                    int tile_state = procon::random::call(3);
                    if(field.getState(x_index, y_index).isEmpty() == false)
                        field.setTileEmpty(x_index, y_index);
                    if((tile_state & 2) == false)
                        field.setTileSide(x_index, y_index, tile_state & 1);
                }

            std::set<procon::Point> agent_exists_point;
            int agent_count = field.getAgentCount();
            for(int side = 0; side < 2; ++side)
                for(int agent_index = 0; agent_index < agent_count; ++agent_index){
                    procon::Point agent_point;
                    do{
                        agent_point.x = procon::random::call(size.x);
                        agent_point.y = procon::random::call(size.y);
                    }while(agent_exists_point.find(agent_point) != agent_exists_point.end());
                    agent_exists_point.emplace(agent_point);
                    if(field.getState(agent_point).isEmpty() == false)
                        field.setTileEmpty(agent_point);
                    field.setAgent(side, agent_index, agent_point);
                }
            auto turn = field.getTurn();
            turn.now = procon::random::call(turn.final);
            field.setTurn(turn);
            field.calcRegionPoint();
            board.sim.setField(field);
            ret_data.clear();
        };

        auto agent_greedy_move = [&](){
            auto ret_0 = procon::communication::simpleFastGreedy(field, 0, agent_move_bound, temp);
            auto ret_1 = procon::communication::simpleFastGreedy(field, 1, agent_move_bound, temp);
            std::vector<np::ndarray> policy{
                bp::extract<np::ndarray>(ret_0[1]),
                bp::extract<np::ndarray>(ret_1[1])
            };
            auto shape = policy[0].get_shape();
            int arr_size = 1;
            for(int dim = 0; dim < policy[0].get_nd(); ++dim)
                arr_size *= shape[dim];
            policy[0] = policy[0].reshape(bp::make_tuple(arr_size));
            policy[1] = policy[1].reshape(bp::make_tuple(arr_size));
            for(int index = 0; index < 2; ++index){
                auto new_policy = np::zeros(bp::make_tuple(arr_size + 1), np::dtype::get_builtin<float>());
                for(int ptr_index = 0; ptr_index < arr_size; ++ptr_index)
                    new_policy[ptr_index] = static_cast<float>(bp::extract<float>(policy[index][ptr_index]));
                new_policy[arr_size] = (index == 0 ? 1 : -1);
                policy[index] = new_policy;
            }

            ret_data.emplace_back(board, policy);

            board.addAgentAct(0, bp::extract<np::ndarray>(ret_0[0]));
            board.addAgentAct(1, bp::extract<np::ndarray>(ret_1[0]));
        };

        auto agent_random_move = [&](){
            int agent_count = field.getAgentCount();
            np::ndarray moves_ndarr_0 = np::zeros(bp::make_tuple(agent_count), np::dtype::get_builtin<int>());
            np::ndarray moves_ndarr_1 = np::zeros(bp::make_tuple(agent_count), np::dtype::get_builtin<int>());
            for(int agent_index = 0; agent_index < agent_count; ++agent_index){
                moves_ndarr_0[agent_index] = procon::random::call(8);
                moves_ndarr_1[agent_index] = procon::random::call(8);
            }
            board.addAgentAct(0, moves_ndarr_0);
            board.addAgentAct(1, moves_ndarr_1);
        };

        if(procon::random::call(16) == false)
            field_random_init();

        while(board.isEnded() == false){
            if(procon::random::call(8))
                agent_greedy_move();
            else
                agent_random_move();
        }
        const auto& score = field.getScores();
        int result = 0;
        if(score[0].getSum() > score[1].getSum())
            result = 1;
        else if(score[0].getSum() < score[1].getSum())
            result = -1;

        int data_size = 0;
        for(auto& ret : ret_data){
            if(procon::random::call(16))
                continue;
            ++data_size;
            auto& [now_board, policy] = ret;
            const auto& now_field = now_board.getField();
            for(auto& pol : policy)
                pol[pol.get_shape()[0] - 1] *= result;

            // TODO: procon::npy::cnpyExportと同じ形式を使う(cnpyExport自体はちゃんとした形式で出してくれないので, どうにかする)

            auto get_make_npy_full_data = [this](auto&& _field){
                if(use_center)
                    return procon::communication::makeNpyFullCenterData(_field);
                else
                    return procon::communication::makeNpyFullData(_field);
            };
            auto [now_shape, ret_field] = get_make_npy_full_data(now_field);
            shape = std::move(now_shape);
            auto ret_field_rev = get_make_npy_full_data(now_field.getSideReversedField()).second;

            int arr_size = policy[0].get_shape()[0];
            std::vector<std::vector<float>> field_vector{ret_field, ret_field_rev};
            std::vector<std::vector<float>> policy_vector(2, std::vector<float>(arr_size));

            for(int side = 0; side < 2; ++side){

                for(int index = 0; index < arr_size; ++index)
                    policy_vector[side][index] = bp::extract<float>(policy[side][index]);

                for(auto& elm : field_vector[side])
                    field_vectors[side].emplace_back(elm);
                for(auto& elm : policy_vector[side])
                    policy_vectors[side].emplace_back(elm);
            }
        }
        if(data_size == 0)
            return ;
        cnpy::npz_save(save_filename, "data_length", std::vector<int>{data_size}, "w");
        cnpy::npz_save(save_filename, "input_shape", shape, "a");
        cnpy::npz_save(save_filename, "output_shape", std::vector<int>{static_cast<int>(policy_vectors[0].size() / data_size)}, "a");
        for(int side = 0; side < 2; ++side){
            cnpy::npz_save(save_filename, "input_" + std::to_string(side), field_vectors[side], "a");
            cnpy::npz_save(save_filename, "output_" + std::to_string(side), policy_vectors[side], "a");
        }
    }());
}
