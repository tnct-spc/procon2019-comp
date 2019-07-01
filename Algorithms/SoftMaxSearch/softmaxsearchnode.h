#ifndef SOFTMAXSEARCHNODE_H
#define SOFTMAXSEARCHNODE_H

#include "evaluaterobject.h"
#include "treap.h"
#include <memory>

struct SoftMaxSearchNode
{
    SoftMaxSearchNode(double value_a, double value_b);
    void search(const procon::Field& field, EvaluaterObject& evaluator, double temp);
    void expand(const procon::Field& field, EvaluaterObject& evaluator, double temp);

    double value_a, value_b;
    std::vector<int> move_a, move_b;
    procon::treap::Treap<double, bool> treap_a, treap_b;

    std::unordered_map<long, std::shared_ptr<SoftMaxSearchNode>> childs;

    static long hash_encoder(long move_1, long move_2){
        return move_1 | (move_2 << 24);
    }
    static std::pair<long,long> hash_decoder(long moves){
        return std::make_pair(moves & ((1L << 32) - 1), moves >> 32);
    }
};

#endif // SOFTMAXSEARCHNODE_H
