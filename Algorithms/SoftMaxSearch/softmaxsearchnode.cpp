#include "softmaxsearchnode.h"

template<> procon::treap::Node<double, bool>* procon::treap::Node<double, bool>::nil = new procon::treap::Node<double, bool>(0, 0.0, false);

SoftMaxSearchNode::SoftMaxSearchNode(double value_a, double value_b) :
    value_a(value_a),
    value_b(value_b),
    treap_a(procon::treap::Treap<double, bool>::make(false, [](auto x, auto y){return x + y;})),
    treap_b(procon::treap::Treap<double, bool>::make(false, [](auto x, auto y){return x + y;}))
{

}

void SoftMaxSearchNode::search(const procon::Field& field, EvaluaterObject& evaluator, double temp){

    if(move_a.empty()){
        assert(move_b.empty());
        expand(field, evaluator, temp);
        return ;
    }
    assert(!move_b.empty());

    // ここでTreapを使うと, たのしい！
    double rand_val_a = treap_a.get(0, treap_a.size()) * procon::random::call() / std::numeric_limits<unsigned long>::max();
    int target_move_a_idx = treap_a.func_upperbound(rand_val_a);
    int target_move_a = move_a.at(target_move_a_idx);

    double rand_val_b = treap_b.get(0, treap_b.size()) * procon::random::call() / std::numeric_limits<unsigned long>::max();
    int target_move_b_idx = treap_b.func_upperbound(rand_val_b);
    int target_move_b = move_b.at(target_move_b_idx);

    auto& node = childs[hash_encoder(target_move_a, target_move_b)];

    double before_value_a = node->value_a;
    double before_value_b = node->value_b;

    double before_per_a = before_value_a / treap_a.get(0, treap_a.size());
    double before_per_b = before_value_b / treap_b.get(0, treap_b.size());

    node->search(field, evaluator, temp);

    double after_value_a = node->value_a;
    double after_value_b = node->value_b;

    treap_a.set(target_move_a_idx, after_value_a);
    treap_b.set(target_move_b_idx, after_value_b);

    double after_per_a = after_value_a / treap_a.get(0, treap_a.size());
    double after_per_b = after_value_b / treap_b.get(0, treap_b.size());

    value_a -= before_value_a * before_per_a;
    value_b -= before_value_b * before_per_b;

    value_a *= (1.0 - after_per_a) / (1.0 - before_per_a);
    value_b *= (1.0 - after_per_b) / (1.0 - before_per_b);

    value_a += after_value_a * after_per_a;
    value_b += after_value_b * after_per_b;
}

void SoftMaxSearchNode::expand(const procon::Field& field, EvaluaterObject& evaluator, double temp){
    // 最小盤面での動作を仮定するため, 枝狩りを行わない
    int agent_count = field.getAgentCount();
    for(int move = 0; move < (1 << (agent_count * 3)); ++move){
        int move_flag = move;
        std::vector<int> agent_moves(agent_count);
        for(int agent = 0; agent < agent_count; move_flag >>= 3, ++agent)
            agent_moves.at(agent) = move_flag & 7;
    }
    /*
    やること:
     全移動パターンをそれぞれのエージェントで列挙
     直積全部をEvaluatorにappend, まとめてcalcで確率計算
     treapへの要素追加
    */
}
