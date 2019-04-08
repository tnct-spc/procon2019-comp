#ifndef TESTALGORITHM_H
#define TESTALGORITHM_H

#include <iostream>
#include <map>
#include "algorithmwrapper.h"
#include "simplebeamsearch.h"

class TestAlgorithm : public AlgorithmWrapper
{
    // テスト用 諸々の動作確認をしたい時に使うアルゴリズム

    using AlgorithmWrapper::AlgorithmWrapper;
public:
    std::vector<procon::MoveState> agentAct() override;

private:
    // SimpleBeamSearchベース、コンフリクトを発生させられる場合に優先的にそれをします
    std::vector<procon::MoveState> testMakeConflict();
};

#endif // TESTALGORITHM_H
