#ifndef GENERATECNPYBOARDDATA_H
#define GENERATECNPYBOARDDATA_H

#include <QCoreApplication>
#include <QFileDialog>
#include "cnpy.h"

#include "com_utils.h"
#include "cnpytranslate.h"

class GenerateCnpyBoardData : public QObject
{
public:
    GenerateCnpyBoardData();
    void run();

private:
    const int playout_count = 1e5;
    const int agent_move_bound = 8;
    const double temp = 0.1;
};

#endif // GENERATECNPYBOARDDATA_H
