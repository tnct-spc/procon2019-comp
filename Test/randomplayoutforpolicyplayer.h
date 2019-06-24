#ifndef RANDOMPLAYOUTFORPOLICYPLAYER_H
#define RANDOMPLAYOUTFORPOLICYPLAYER_H

#include <QFileDialog>
#include "gamesimulator.h"
#include "csvtranslate.h"
#include "FieldConverter/fieldconverterwrapper.h"
#include "PolicyPlayer/policyplayer.h"
#include "PolicyPlayer/selectors.h"
#include "PolicyPlayer/Searcher/simplesearcher.h"
#include "PolicyPlayer/Evaluator/randomevaluator.h"

class RandomPlayoutForPolicyPlayer : public QObject
{
public:
    RandomPlayoutForPolicyPlayer();
    template <typename T>
    void run();

private:
    std::array<std::shared_ptr<PolicyPlayer>, 2> players;
    std::array<std::shared_ptr<SearcherWrapper>, 2> searchers;
    std::array<std::shared_ptr<EvaluatorWrapper>, 2> evaluators;
};

#endif // RANDOMPLAYOUTFORPOLICYPLAYER_H
