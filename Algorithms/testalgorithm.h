#ifndef TESTALGORITHM_H
#define TESTALGORITHM_H

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
    // conflict_falgが0なら得点によって動的に決める、1ならコンフリクトさせにいく、2ならコンフリクトさせない
    std::vector<procon::MoveState> testMakeConflict(int conflict_flag = 0);
};

#endif // TESTALGORITHM_H
