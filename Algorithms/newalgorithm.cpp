#include "newalgorithm.h"

std::vector<procon::MoveState> NewAlgorithm::agentAct(){
    // do not use
    abort();
    return std::vector<procon::MoveState>();
}

std::vector<procon::MoveState> NewAlgorithm::agentAct(std::vector<std::vector<bool>>& select_flag){

    int agent_count = field.getAgentCount();

    std::set<procon::Point> select_points;
    for(int x_index = 0; x_index < field.getSize().x; ++x_index)
        for(int y_index = 0; y_index < field.getSize().y; ++y_index)
            if(select_flag.at(x_index).at(y_index)){
                procon::Point select_point(x_index, y_index);
                for(int agent_index = 0; agent_index < agent_count; ++agent_index){
                    auto agent_point = field.getAgent(side, agent_index);
                    auto point_diff = select_point - agent_point;
                    if(std::abs(point_diff.x) <= 1 && std::abs(point_diff.y) <= 1){
                        select_points.emplace(select_point);
                        continue;
                    }
                }
            }

    struct BsState{
        double value;
        procon::Point pos;
        std::set<procon::Point> used;
        BsState make(procon::Point nex_pos, double nex_value){
            auto nex_used = used;
            nex_used.emplace(nex_pos);
            return BsState{value * 1.3 + nex_value, nex_pos, nex_used};
        }
    };

    auto calc = [&](int agent_index, int move_index){
        std::priority_queue<BsState, std::vector<BsState>, std::function<bool(BsState&, BsState&)>> state_que([](auto& a, auto& b){return a.value > b.value;});
        { // init
            auto start_pos = field.getAgent(side, agent_index).getAppliedPosition(move_index);
            if(field.outOfRangeCheck(start_pos).first)
                return 0.0;
            if(select_points.find(start_pos) != select_points.end())
                return 1e9;
            std::set<procon::Point> first_set;
            first_set.emplace(start_pos);
            state_que.emplace(BsState{static_cast<double>(field.getState(start_pos).equalSide(side) ? 0 : field.getState(start_pos).value), start_pos, first_set});
        }

        for(int depth = 0; depth < max_depth; ++depth){
            decltype(state_que) nex_que([](auto& a, auto& b){return a.value > b.value;});
            while(!state_que.empty()){
                auto state = state_que.top();
                state_que.pop();
                for(int move_index = 0; move_index < 9; ++move_index){
                    auto nex_pos = state.pos.getAppliedPosition(move_index);
                    if(field.outOfRangeCheck(nex_pos).first)
                        continue;
                    auto nex_state = field.getState(nex_pos);
                    bool add_flag = (state.used.find(nex_pos) == state.used.end()) && !nex_state.equalSide(side);
                    nex_que.emplace(state.make(nex_pos, add_flag ? nex_state.value : 0));
                    if(static_cast<int>(nex_que.size()) > max_depth)
                        nex_que.pop();
                }
            }
            state_que = std::move(nex_que);
            assert(!state_que.empty());
        }
        while(state_que.size() > 1)
            state_que.pop();
        return state_que.top().value;
    };

    std::vector<procon::Point> use_point_vec;
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        for(int move_index = 0; move_index < 9; ++move_index){
            auto nex_pos = field.getAgent(side, agent_index).getAppliedPosition(move_index);
            if(field.outOfRangeCheck(nex_pos).first == false)
                use_point_vec.emplace_back(nex_pos);
        }

    sort(use_point_vec.begin(), use_point_vec.end());
    use_point_vec.erase(unique(use_point_vec.begin(), use_point_vec.end()), use_point_vec.end());

    int matrix_size = std::max(agent_count, static_cast<int>(use_point_vec.size()));
    std::vector<std::vector<double>> eval_values(matrix_size, std::vector<double>(matrix_size, 0.0));

    auto get_index = [&use_point_vec](procon::Point p){return std::distance(use_point_vec.begin(), std::lower_bound(use_point_vec.begin(), use_point_vec.end(), p));};

    for(int agent_index = 0; agent_index < agent_count; ++agent_index){
        for(int move_index = 0; move_index < 9; ++move_index){
            auto nex_pos = field.getAgent(side, agent_index).getAppliedPosition(move_index);
            if(field.outOfRangeCheck(nex_pos).first == false){
                int point_index = get_index(nex_pos);
                eval_values.at(agent_index).at(point_index) = calc(agent_index, move_index) + 1e-3;
            }
        }
    }

    auto hungarian = [&](std::vector<std::vector<double>> a){
        int n = a.size();
        int p, q;
        std::vector<double> fx(n, 1e17), fy(n, 0);
        std::vector<int> x(n, -1), y(n, -1);

        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
                fx.at(i) = std::max(fx.at(i), a.at(i).at(j));

        for(int i = 0; i < n;){
            std::vector<int> t(n, -1), s(n + 1, i);
            for(p = 0, q = 0; p <= q && x.at(i) < 0; ++p){
                for(int k = s.at(p), j = 0; j < n && x.at(i) < 0; ++j)
                    if(fx.at(k) + fy.at(j) == a.at(k).at(j) && t.at(j) < 0){
                        s.at(++q) = y.at(j);
                        t.at(j) = k;
                        if(s.at(q) < 0){
                            for(p = j; p >= 0; j = p){
                                y.at(j) = t.at(j);
                                k = t.at(j);
                                p = x.at(k);
                                x.at(k) = j;
                            }
                        }
                    }
            }
            if(x.at(i) < 0){
                double d = 1e17;
                for(int k = 0; k <= q; ++k)
                    for(int j = 0; j < n; ++j)
                        if(t.at(j) < 0)
                            d = std::min(d, fx.at(s.at(k)) + fy.at(j) - a.at(s.at(k)).at(j));
                for(int j = 0; j < n; ++j)
                    fy.at(j) += (t.at(j) < 0 ? 0 : d);
                for(int k = 0; k <= q; ++k)
                    fx.at(s.at(k)) -= d;
            }
            else
                ++i;
        }
        return x;
    };

    auto ret = hungarian(eval_values);
    std::vector<procon::MoveState> ret_state(agent_count);
    for(int agent_index = 0; agent_index < agent_count; ++agent_index)
        ret_state.at(agent_index) = field.makeMoveState(side, field.getAgent(side, agent_index), field.getAgent(side, agent_index).getMoveIndex(use_point_vec.at(ret.at(agent_index))));

    return ret_state;
}
