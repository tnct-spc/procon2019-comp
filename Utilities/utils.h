#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <array>
#include <limits>
#include <cassert>

namespace procon{

constexpr std::array<int, 9> dx = {-1, 1, 0, 0, -1, 1, -1, 1, 0};
constexpr std::array<int, 9> dy = {0, 0, 1, -1, 1, -1, -1, 1, 0};

struct MoveState;

struct Point
{
    constexpr Point() : x(0), y(0){}
    constexpr Point(int x, int y) : x(x), y(y){}

    Point& operator+=(const Point& p);
    Point& operator-=(const Point& p);

    friend Point operator+(const Point& p, const Point& q){return Point(p) += q;}
    friend Point operator-(const Point& p, const Point& q){return Point(p) -= q;}

    friend bool operator==(const Point& p, const Point& q);
    friend bool operator!=(const Point& p, const Point& q){return !(p == q);}

    friend bool operator<(const Point& p, const Point& q);
    friend bool operator<=(const Point& p, const Point& q){return p == q || p < q;}
    friend bool operator>(const Point& p, const Point& q){return !(p <= q);}
    friend bool operator>=(const Point& p, const Point& q){return !(p < q);}

    Point getAppliedPosition(int move_index) const;
    Point getAppliedPosition(const MoveState& m) const;

    int getMoveIndex(const Point& moved_position) const;

    explicit operator bool() const{return x || y;}

    int x, y;
};

constexpr Point getMove(int move_index){return Point(dx[move_index], dy[move_index]);}

struct MoveState{

    constexpr MoveState(int move_index = 8, bool is_delete = false) : move_index(move_index), is_delete(is_delete){}

    int move_index;
    bool is_delete;

    constexpr bool isDelete(){return is_delete;}
    constexpr Point getMove(){return procon::getMove(move_index);}
};

struct FieldState{
    constexpr FieldState(int value = 0, int tile = 0) : tile(tile), value(value){}
    constexpr int getDecrementedSide() const{return tile - 1;}
    constexpr bool equalSide(bool side) const{return tile == side + 1;}
    constexpr bool isEmpty() const{return !tile;}
    int tile, value;
};

struct Score{
    constexpr Score(int tile = 0, int region = 0) : tile(tile), region(region){}
    constexpr int getSum() const{return tile + region;}
    int tile, region;
};

struct Turn{
    constexpr Turn(int now, int final) : now(now), final(final){}
    // 0-indexed, [0, final)
    constexpr int getRemainTurn() const{return final - now;}
    int now, final;
};

struct ConnectionSettings{
    ConnectionSettings(int match_id = 1, std::string address = "127.0.0.1:8888", std::string token = "procon30_example_token", int team_id = 1, std::vector<int> agent_id = std::vector<int>{8, 6, 9, 120}, int end_turn = 40) :
        match_id(match_id),
        team_id(team_id),
        end_turn(end_turn),
        agent_id(agent_id),
        address(address),
        token(token)
    {}
    int match_id, team_id, end_turn;
    std::vector<int> agent_id;
    std::string address, token;
};

namespace random{

    unsigned long call(unsigned long mod = std::numeric_limits<unsigned long>::max());
    // 閉区間での設定
    long call(long min_value, long max_value);
}

template<typename flow>
struct Dinic{
    const flow INF;
    struct Edge{
        Edge(int to, flow cap, int rev, bool is_rev) : to(to), cap(cap), rev(rev), is_rev(is_rev){}
        int to;
        flow cap;
        int rev;
        bool is_rev;
    };
    int point_count;
    std::vector<std::vector<Edge>> graph;
    std::vector<int> min_cost, iter;

    Dinic(int point_count) : INF(std::numeric_limits<flow>::max()), point_count(point_count), graph(point_count){};
    void addEdge(int from, int to, flow cap);
    bool bfs(int s_index, int t_index);
    flow dfs(int point_index, const int t_index, flow f);
    flow calcMaxFlow(int s_index, int t_index);
};

template <typename F, typename F2>
struct FixPointForEach{
    const F _f;
    const F2 _rec;

    FixPointForEach(F&& f, F2&& rec) : _f(std::forward<F>(f)), _rec(std::forward<F2>(rec)){}

    template<typename Type>
    decltype(auto) operator()(Type&& arg) const{return _f(std::forward<Type>(arg));}
    template<typename... Types>
    decltype(auto) operator()(Types&&... args) const{return _rec(*this, std::forward<Types>(args)...);}
};

template <typename F>
decltype(auto) makeForEachFunc(F&& f){

    auto rec = [&](auto&& f, auto&& head, auto&&... tail) -> void{
        f._f(head);
        if(sizeof...(tail))
            f(std::forward<decltype(tail)>(tail)...);
    };

    return FixPointForEach<F, decltype(rec)>(std::forward<F>(f), std::forward<decltype(rec)>(rec));

}

}

#endif // UTILS_H
