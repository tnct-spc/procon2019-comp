#ifndef FIELDCSVTRANSLATE_H
#define FIELDCSVTRANSLATE_H

#include "field.h"

namespace procon::json{
    std::string translateToFieldCsv(std::string json_str);
    std::string translateFromFieldCsv(std::string field_csv_str);
}

#endif // FIELDCSVTRANSLATE_H
