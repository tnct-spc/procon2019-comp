#ifndef CSVTRANSLATE_H
#define CSVTRANSLATE_H

#include <fstream>
#include <sstream>
#include "field.h"

namespace procon::csv{
    Field csvDecode(std::string csv_string);
    std::string csvEncode(const Field& field);
    Field csvImport(std::string path);
    void csvExport(std::string path, const Field& field);
}

#endif // CSVTRANSLATE_H
