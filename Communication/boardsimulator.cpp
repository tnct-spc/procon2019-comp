#include "boardsimulator.h"

BoardSimulator::BoardSimulator(std::string start_csv_path, std::string save_csv_path) :
    save_csv_path(save_csv_path),
    sim(GameSimulator(procon::csv::csvImport(start_csv_path)))
{

}

boost::python::tuple BoardSimulator::changeTurn(boost::python::numpy::ndarray input){
    const auto& field = sim.getField();
    std::vector<std::vector<procon::MoveState>> moves(2, std::vector<procon::MoveState>(field.getAgentCount()));
    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < field.getAgentCount(); ++agent_index)
            moves.at(side).at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index),
                                        boost::python::extract<int>(input[side * field.getAgentCount() + agent_index]));
    sim.addAgentAct(0, moves.at(0));
    sim.addAgentAct(1, moves.at(1));
    sim.changeTurn(true);
    procon::csv::csvExport(save_csv_path, field);
    return boost::python::make_tuple(field.getScore(0).getSum() - field.getScore(1).getSum(), sim.isSimulationEnded());
}
