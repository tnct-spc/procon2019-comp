#include "cnpytranslate.h"

namespace procon::npy{

void cnpyExport(std::string path, const Field& field){

    auto data = communication::Board::getDataFromField(field);
    std::vector<size_t> shape{9, static_cast<size_t>(field.getSize().x), static_cast<size_t>(field.getSize().y)};
    std::vector<int> size{static_cast<int>(shape.at(1) * shape.at(2)), static_cast<int>(shape.at(2)), 1};
    std::vector<double> data_vec(shape.at(0) * shape.at(1) * shape.at(2));
    double* data_ptr = reinterpret_cast<double*>(data.get_data());
    for(int dim = 0; dim < 4; ++dim)
        for(size_t x = 0; x < shape.at(1); ++x)
            for(size_t y = 0; y < shape.at(2); ++y){
                int index = size.at(0) * dim + size.at(1) * x + size.at(2) * y;
                data_vec.at(index) = data_ptr[index];
            }

    auto set_full = [&](int dim, double val){
        for(size_t x = 0; x < shape.at(1); ++x)
            for(size_t y = 0; y < shape.at(2); ++y){
                int index = size.at(0) * dim + size.at(1) * x + size.at(2) * y;
                data_vec.at(index) = val;
            }
    };

    const auto& scores = field.getScores();
    set_full(4, scores.at(0).tile);
    set_full(5, scores.at(0).region);
    set_full(6, scores.at(1).tile);
    set_full(7, scores.at(1).region);
    set_full(8, field.getTurn().getRemainTurn());

    cnpy::npy_save(path, &data_vec[0], shape, "w");
}

}
