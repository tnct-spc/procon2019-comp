#ifndef FIELDCSVTRANSLATE_H
#define FIELDCSVTRANSLATE_H

#include "field.h"
#include "csvtranslate.h"
#include <iostream>// pathに存在しているファイルを読み込む
#include <fstream>
#include <string>
#include <stdio.h>


namespace procon::json{
    std::string translateToFieldCsv(std::string json_str);
    std::string translateFromFieldCsv(std::string field_csv_str);

    procon::Field importJsonToField(std::string path);
}

#endif // FIELDCSVTRANSLATE_H
