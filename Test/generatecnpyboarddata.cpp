#include "generatecnpyboarddata.h"

GenerateCnpyBoardData::GenerateCnpyBoardData()
{
}

void GenerateCnpyBoardData::run(){

    namespace bp = boost::python;
    namespace np = boost::python::numpy;

    std::string csv_path = "/home/shibh308/workspace/MCTS-RL-test/field.csv";
    std::string save_dir = "/home/shibh308/workspace/tmp";

    /*
    std::string csv_path = QFileDialog::getOpenFileName().toStdString();
    std::string save_dir = QFileDialog::getExistingDirectory().toStdString();
    */

    for(int _ = 0; _ < playout_count; ++_, [&]{
        procon::communication::Board board(csv_path);
        std::vector<std::pair<procon::communication::Board, std::vector<np::ndarray>>> ret_data;
        auto& field = board.field;
        while(board.isEnded() == false){
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
            ret_data.emplace_back(board, std::move(policy));
            board.addAgentAct(0, bp::extract<np::ndarray>(ret_0[0]));
            board.addAgentAct(1, bp::extract<np::ndarray>(ret_1[0]));
        }
        const auto& score = field.getScores();
        int result = 0;
        if(score[0].getSum() > score[1].getSum())
            result = 1;
        else if(score[0].getSum() < score[1].getSum())
            result = -1;

        for(auto& ret : ret_data){
            auto [board, policy] = ret;
            for(auto& pol : policy)
                pol[pol.get_shape()[0] - 1] *= result;

            auto field_ndarr = board.getData();
            auto field_ndarr_rev = board.reverseBoard().getData();

            auto shape_tuple = field_ndarr.get_shape();
            std::vector<std::vector<std::vector<std::vector<float>>>> field_vector(2, std::vector<std::vector<std::vector<float>>>(shape_tuple[0], std::vector<std::vector<float>>(shape_tuple[1], std::vector<float>(shape_tuple[2]))));
            for(int x = shape_tuple[0]; x < shape_tuple[0]; ++x)
                for(int y = shape_tuple[1]; y < shape_tuple[1]; ++y)
                    for(int z = shape_tuple[2]; z < shape_tuple[2]; ++z){
                        field_vector[0][x][y][z] = bp::extract<float>(field_ndarr[x][y][z]);
                        field_vector[1][x][y][z] = bp::extract<float>(field_ndarr_rev[x][y][z]);
                    }

            int arr_size = policy[0].get_shape()[0];
            std::vector<std::vector<float>> policy_vector(2, std::vector<float>(arr_size));
            for(int side = 0; side < 2; ++side)
                for(int index = 0; index < arr_size; ++index)
                    policy_vector[side][index] = bp::extract<float>(policy[side][index]);

            for(int side = 0; side < 2; ++side){
                std::string save_filename = save_dir + "/" + std::to_string(procon::random::call()) + ".npz";
                cnpy::npz_save(save_filename, "input", &field_vector[side][0][0][0], std::vector<size_t>{static_cast<size_t>(shape_tuple[0]), static_cast<size_t>(shape_tuple[1]), static_cast<size_t>(shape_tuple[2])}, "w");
                cnpy::npz_save(save_filename, "output", policy_vector[side], "a");
            }
        }
    }());
}
