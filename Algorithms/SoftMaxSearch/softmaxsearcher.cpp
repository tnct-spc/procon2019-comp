#include "softmaxsearcher.h"

SoftMaxSearcher::SoftMaxSearcher(const procon::Field& field, bool side) :
    AlgorithmWrapper(field, side),
    root(std::make_shared<SoftMaxSearchNode>(0.0, 0.0))
{

}


std::vector<procon::MoveState> SoftMaxSearcher::agentAct(){
}

void SoftMaxSearcher::search(const procon::Field& field){
    for(int _ = 0; _ < search_count; ++_){
        root->search(field, evaluator, temp);
    }
}
