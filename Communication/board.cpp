#include "board.h"


namespace procon{
namespace communication{

namespace bp = boost::python;
namespace np = boost::python::numpy;

Board::Board(std::string path) :
    sim(procon::csv::csvImport(path)),
    field(sim.getField())
{
}

Board::Board(const GameSimulator& sim) :
    sim(sim),
    field(sim.getField())
{
}

np::ndarray Board::getDataFromField(const procon::Field field){

    auto size = field.getSize();
    auto agent_count = field.getAgentCount();

    auto shape = bp::make_tuple(4, size.x, size.y);
    auto data = np::zeros(shape, np::dtype::get_builtin<int>());

    data[0][0][0] = 1;
    for(int x_index = 0; x_index < size.x; ++x_index)
        for(int y_index = 0; y_index < size.y; ++y_index){
            auto tile = field.getState(x_index, y_index).getDecrementedSide();
            if(tile != -1)
                data[tile][x_index][y_index] = 1;
        }

    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            auto agent_pos = field.getAgent(side, agent_index);
            data[2 + side][agent_pos.x][agent_pos.y] = 1;
        }

    return data;
}

np::ndarray Board::getData(){
    return getDataFromField(field);
}

np::ndarray Board::getCenterDataFromField(const procon::Field field){

    /*
    全エージェント分の {生の得点, {味方のタイル, 敵のタイル, 空のタイル}, 自分, 敵}
    各sideの{タイル点, 領域点, 総和}
    残りターン数
    */

    auto agent_count = field.getAgentCount();

    int dim_size = 6 * (2 * agent_count) + 7;
    auto shape = bp::make_tuple(dim_size, 39, 39);
    auto data = np::zeros(shape, np::dtype::get_builtin<int>());

    auto set_data = [&](int dim_index, int side, Point origin_agent_pos){
        Point pos_dif(origin_agent_pos.x - 19, origin_agent_pos.y - 19);
        for(int x_index = 0; x_index < 39; ++x_index)
            for(int y_index = 0; y_index < 39; ++y_index){
                Point pos(x_index + pos_dif.x, y_index + pos_dif.y);
                for(int dim = 1; dim < 6; ++dim)
                    data[dim_index * 6 + dim][x_index][y_index] = -1;
                if(field.outOfRangeCheck(pos).first){
                    data[dim_index * 6][x_index][y_index] = -50;
                }else{
                    auto state = field.getState(pos);
                    data[dim_index * 6][x_index][y_index] = state.value;
                    data[dim_index * 6 + 1 + (state.isEmpty() ? 2 : (state.getDecrementedSide() != side))][x_index][y_index] = 1;
                }
            }
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            auto agent_pos = field.getAgent(side, agent_index) - pos_dif;
            data[dim_index * 6 + 4][agent_pos.x][agent_pos.y] = agent_index;
        }
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            auto agent_pos = field.getAgent(!side, agent_index) - pos_dif;
            data[dim_index * 6 + 5][agent_pos.x][agent_pos.y] = agent_index;
        }
    };
    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index)
            set_data(side * agent_count + agent_index, side, field.getAgent(side, agent_index));


    auto scores = field.getScores();
    std::vector<int> values{scores[0].tile, scores[0].region, scores[0].getSum(),
                            scores[1].tile, scores[1].region, scores[1].getSum(),
                            field.getTurn().getRemainTurn()};
    for(int x_index = 0; x_index < 39; ++x_index)
        for(int y_index = 0; y_index < 39; ++y_index)
            for(int dim = 0; dim < 7; ++dim)
                data[6 * (2 * agent_count) + dim][x_index][y_index] = values[dim];

    return data;
}

np::ndarray Board::getCenterData(){
    return getCenterDataFromField(field);
}

bp::tuple Board::getTurn(){
    return bp::make_tuple(field.getTurn().now, field.getTurn().final, field.getTurn().getRemainTurn());
}

bp::tuple Board::getScore(){
    auto scores = field.getScores();
    return bp::make_tuple(bp::make_tuple(scores.at(0).tile, scores.at(0).region, scores.at(0).getSum()),
                          bp::make_tuple(scores.at(1).tile, scores.at(1).region, scores.at(1).getSum()));
}

np::ndarray Board::getValidMoves(bool side){
    int agent_count = field.getAgentCount();
    auto shape = bp::make_tuple(agent_count, 8);
    auto data = np::zeros(shape, np::dtype::get_builtin<bool>());

    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        for(int move = 0; move < 8; ++move)
            data[agent_index][move] = !field.outOfRangeCheck(field.getAgent(side, agent_index).getAppliedPosition(move)).first;

    return data;
}

int Board::getAgentCount(){
    return field.getAgentCount();
}

bool Board::isEnded(){
    return sim.isSimulationEnded();
}

void Board::addAgentAct(bool side, np::ndarray arr){
    int agent_count = field.getAgentCount();
    assert(arr.shape(0) == agent_count);
    std::vector<procon::MoveState> moves(agent_count);
    int* data = reinterpret_cast<int*>(arr.get_data());
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        moves.at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index), data[agent_index]);

    sim.addAgentAct(side, moves);

    act_flag.set(side);
    if(act_flag.all()){
        sim.changeTurn(true);
        field = sim.getField();
        act_flag.reset();
    }
}

}
}
