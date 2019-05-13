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

np::ndarray Board::getData(){

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

bp::tuple Board::getTurn(){
    return bp::make_tuple(field.getTurn().now, field.getTurn().final, field.getTurn().getRemainTurn());
}

bp::tuple Board::getScore(){
    auto scores = field.getScores();
    return bp::make_tuple(bp::make_tuple(scores.at(0).tile, scores.at(0).region), bp::make_tuple(scores.at(0).tile, scores.at(0).region));
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
        act_flag.reset();
    }
}

}
}
