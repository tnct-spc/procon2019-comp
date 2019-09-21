#ifndef FIELDCSVTRANSLATE_H
#define FIELDCSVTRANSLATE_H

#include "field.h"
#include "csvtranslate.h"
#include <nlohmann/json.hpp>
#include <iostream>// pathに存在しているファイルを読み込む
#include <fstream>
#include <string>
#include <stdio.h>


namespace procon::json{
    std::string translateToFieldCsv(std::string json_str, int team_id, std::vector<int>& agent_ids, int end_turn);
    std::string translateFromFieldCsv(std::string field_csv_str);

    procon::Field importJsonToField(std::string path, int team_id, int end_turn);
}

#endif // FIELDCSVTRANSLATE_H
