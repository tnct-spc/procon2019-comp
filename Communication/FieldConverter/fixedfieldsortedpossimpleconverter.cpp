#include "fixedfieldsortedpossimpleconverter.h"

FixedFieldSortedPosSimpleConverter::FixedFieldSortedPosSimpleConverter(const procon::Field& field) :
    FieldConverterWrapper(field)
{
    /*
	盤面固定, ターン数もあらかじめ確定している事を前提にしている
    (7, x, y)
	0チャンネル目:   味方エージェントをそれぞれ位置の辞書順に[1,agent_count] / agent_count, 他は0埋め
	1チャンネル目:   敵エージェントをそれぞれ位置の辞書順に[1, agent_count] / agent_count, 他は0埋め
    2-5チャンネル目: それぞれの(タイル点, 領域点) Fieldのabs_sumで割って正規化
	6チャンネル目:   現在ターン数 / 合計ターン数
	*/

    ndim = 7;
    auto shape = boost::python::make_tuple(7, field.getSize().x, field.getSize().y);
    data = boost::python::numpy::zeros(shape, boost::python::numpy::dtype::get_builtin<float>());
    int agent_count = field.getAgentCount();
    for(int side = 0; side < 2; ++side)
        for(int agent_index = 0; agent_index < agent_count; ++agent_index){
            auto pos = field.getAgent(side, agent_index);
            data[side][pos.x][pos.y] = agent_index + 1;
        }
    const auto& scores = field.getScores();
    float sum = field.getAbsSum();
    std::vector<float> values = {scores.at(0).tile / sum, scores.at(0).region / sum,
                                 scores.at(1).tile / sum, scores.at(1).region / sum,
                                 static_cast<float>(field.getTurn().now) / field.getTurn().final};
    const auto& size = field.getSize();
    for(int dim_idx = 0; dim_idx < values.size(); ++dim_idx)
        for(int x_index = 0; x_index < size.x; ++x_index)
            for(int y_index = 0; y_index < size.y; ++y_index)
                data[dim_idx + 2][x_index][y_index] = values.at(dim_idx);
}
