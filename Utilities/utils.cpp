#include "utils.h"

namespace procon{

Point& Point::operator+=(const Point& p){
    x += p.x;
    y += p.y;
    return *this;
}

Point& Point::operator-=(const Point& p){
    x -= p.x;
    y -= p.y;
    return *this;
}

Point Point::getAppliedPosition(const int move_index) const{
    return *this + getMove(move_index);
}

Point Point::getAppliedPosition(const MoveState &m) const{
    return getAppliedPosition(m.move_index);
}


bool operator==(const Point& p, const Point& q){
    return p.x == q.x && p.y == q.y;
}

bool operator<(const Point& p, const Point& q){
    return p.x < q.x || (p.x == q.x && p.y < q.y);
}

int Point::getMoveIndex(const Point &moved_position) const{
    assert(std::abs(x - moved_position.x) <= 1 && std::abs(y - moved_position.y) <= 1);
    // ループを回しているのがよくないので、気が向いたらO(1)で求められるようにする
    for(int move_index = 0; move_index < 9; ++move_index){
        if(*this + getMove(move_index) == moved_position)
            return move_index;
    }
}

template<typename flow>
void Dinic<flow>::addEdge(int from, int to, flow cap){
    assert(0 <= from && from < point_count);
    assert(0 <= to && to < point_count);
    graph.at(from).emplace_back(to, cap, graph.at(to).size(), false);
    graph.at(to).emplace_back(from, 0, graph.at(from).size() - 1, true);
}

template<typename flow>
bool Dinic<flow>::bfs(int s_index, int t_index){
    min_cost.assign(graph.size(), -1);
    std::queue<int> point_que;
    min_cost.at(s_index) = 0;
    point_que.emplace(s_index);
    while(!point_que.empty() && min_cost.at(t_index) == -1) {
        int point = point_que.front();
        point_que.pop();
        for(auto& edge : graph.at(point))
            if(edge.cap > 0 && min_cost.at(edge.to) == -1){
                min_cost.at(edge.to) = min_cost.at(point) + 1;
                point_que.emplace(edge.to);
            }
    }
    return min_cost.at(t_index) != -1;
}

template<typename flow>
flow Dinic<flow>::dfs(int point_index, const int t_index, flow f) {
    if(point_index == t_index) return f;
    for(int &i = iter.at(point_index); i < graph.at(point_index).size(); i++){
        Edge &edge = graph.at(point_index).at(i);
        if(edge.cap > 0 && min_cost.at(point_index) < min_cost[edge.to]){
            flow dfs_ret = dfs(edge.to, t_index, std::min(f, edge.cap));
            if(dfs_ret > 0){
                edge.cap -= dfs_ret;
                graph.at(edge.to).at(edge.rev).cap += dfs_ret;
                return dfs_ret;
            }
        }
    }
    return 0;
}

template<typename flow>
flow Dinic<flow>::calcMaxFlow(int s_index, int t_index){
    flow max_flow = 0;
    while(bfs(s_index, t_index)){
        iter.assign(graph.size(), 0);
        flow f = 0;
        while((f = dfs(s_index, t_index, INF)) > 0)
            max_flow += f;
    }
    return max_flow;
}

namespace random{

    bool once_called = false;
    unsigned long x = 123456789, y = 362436069, z = 521288629, w;

    unsigned long call(unsigned long mod){
        if(!once_called){
            w = time(nullptr);
            once_called = true;
            call();
        }
        unsigned long t = (x ^ (x << 11));
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        // 剰余をとっている都合上正確な分布にはならないが、64bitの範囲で均等に取られているので誤差の範囲内
        return w > mod ? w % mod : w;
    }
    long call(long min_value, long max_value){
        assert(min_value <= max_value);
        unsigned long length = max_value - min_value + 1;
        return min_value + call(length);
    }
}

}
