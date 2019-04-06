#include "generaterandomsimulateddata.h"

GenerateRandomSimulatedData::GenerateRandomSimulatedData()
{

}

void GenerateRandomSimulatedData::run(){

    std::string csv_path = QFileDialog::getOpenFileName().toStdString();
    const procon::Field field = procon::csv::csvImport(csv_path);

    std::string save_dir = QFileDialog::getExistingDirectory().toStdString();

    auto make_filename = [&save_dir](){
        std::string filename;
        do{
            filename = save_dir + '/' + std::to_string(procon::random::call());
        }while(std::ifstream(filename).is_open());
        return filename;
    };

    std::array<int, 3> arr;
    GameSimulator sim;
    for(int count = 0; count < make_count; ++count){
        sim.setField(field);
        for(int side = 0; side < 2; ++side)
            algo[side] = std::make_shared<TestAlgorithm>(field, side);

        for(int index = 0; index < 3; ++index)
            arr[index] = 0;

        int check_turn = procon::random::call(field.getTurn().now, field.getTurn().final - 1);
        while(sim.getField().getTurn().now != check_turn)
            sim.turnSimulation(algo[0], algo[1], false);

        for(int field_count = 0; field_count < playout_count; ++field_count){
            GameSimulator playout(sim.getField());
            while(playout.isSimulationEnded() == false)
                playout.turnSimulation(algo[0], algo[1], false);

            auto f = playout.getField();
            f.calcRegionPoint();

            const auto& score = f.getScores();
            if(score[0].getSum() > score[1].getSum())
                ++arr[0];
            else if(score[0].getSum() < score[1].getSum())
                ++arr[1];
            else
                ++arr[2];
        }
        auto path = make_filename();
        for(int index = 0; index < 3; ++index)
            path += '_' + std::to_string(arr[index]);
        path += ".csv";
        procon::csv::csvExport(path, sim.getField());
    }
}
