#ifndef GENERATERANDOMSIMULATEDDATA_H
#define GENERATERANDOMSIMULATEDDATA_H

#include <QFileDialog>
#include "csvtranslate.h"
#include "gamesimulator.h"

class GenerateRandomSimulatedData : public QObject
{
public:
    GenerateRandomSimulatedData();
    void run();

private:
    const int make_count = 1e3;
    const int playout_count = 100;

    std::array<std::shared_ptr<AlgorithmWrapper>, 2> algo;
};

#endif // GENERATERANDOMSIMULATEDDATA_H
