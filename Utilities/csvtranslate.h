#ifndef CSVTRANSLATE_H
#define CSVTRANSLATE_H

#include <fstream>
#include "field.h"

namespace procon::csv{
    Field csvImport(std::string path);
    void csvExport(std::string path, const Field& field);
}

#endif // CSVTRANSLATE_H
