#ifndef CNPYTRANSLATE_H
#define CNPYTRANSLATE_H

#include "cnpy.h"
#include "board.h"

namespace procon::npy{

void cnpyExport(std::string path, const Field& field);

}

#endif // CNPYTRANSLATE_H
