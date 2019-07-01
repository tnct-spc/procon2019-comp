#ifndef TREAP_H
#define TREAP_H

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <tuple>
#include <cassert>

namespace procon::treap{
template <typename T, typename U = int>
struct Node{

    using np = Node<T, U>*;

    static np nil;

    T val;
    U lazy;
    std::uint32_t pri;

    int size;
    T sum;

    np l = nil;
    np r = nil;

    Node(T v, U OU = U()) : val(v), lazy(OU), pri(rndpri()), size(1), sum(v), l(nil), r(nil){}
    Node(T v, U OU, uint32_t p) : val(v), lazy(OU), pri(p), size(1), sum(v), l(nil), r(nil){}

    static uint32_t rndpri() {
        static uint32_t x = 123456789, y = 362436069, z = 521288629, w = time(0);
        uint32_t t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return std::max<uint32_t>(1, w & 0x3FFFFFFF);
    }
};


template <typename T, typename U = int>
class Treap{

    using nt = Node<T, U>;
    using np = nt*;
    using F = std::function<T(T, T)>;
    using G = std::function<T(T, U, int)>;
    using H = std::function<U(U, U)>;

public:

    np root;
    bool is_list;
    F f;
    G g;
    H h;
    T OT;
    U OU;

    Treap(bool is_list, F f, G g, H h, T OT, U OU) : root(nt::nil), is_list(is_list), f(f), g(g), h(h), OT(OT), OU(OU){}

    Treap(T val, bool is_list, F f, G g, H h, T OT, U OU) : root(new nt(val)), is_list(is_list), f(f), g(g), h(h), OT(OT), OU(OU){}

    // 配列で初期化する
    Treap(std::vector<T> v, bool is_list, F f, G g, H h, T OT, U OU) : root(nt::nil), is_list(is_list), f(f), g(g), h(h), OT(OT), OU(OU){
        for(auto& xx : v)
            root = _merge(root, new nt(xx, OU));
    }

    static Treap make(bool is_list, F f = [](T x, T){return x;}, T OT = T(), G g = [](auto x, auto, auto){return x;}, H h = [](auto x, auto){return x;}, U OU = U()){
        assert(nt::nil != nullptr);
        return Treap(is_list, f, g, h, OT, OU);
    }

    static Treap make(T val, bool is_list, F f = [](auto x, auto){return x;}, T OT = T(), G g = [](auto x, auto, auto){return x;}, H h = [](auto x, auto){return x;}, U OU = U()){
        assert(nt::nil != nullptr);
        return Treap(val, is_list, f, g, h, OT, OU);
    }

    static Treap make(std::vector<T> val, bool is_list, F f = [](auto x, auto){return x;}, T OT = T(), G g = [](auto x, auto, auto){return x;}, H h = [](auto x, auto){return x;}, U OU = U()){
        assert(nt::nil != nullptr);
        return Treap(val, is_list, f, g, h, OT, OU);
    }

    ~Treap(){
        clear();
        if(root != nt::nil)
            delete root;
    }

    int _size(np x){return x == nt::nil ? 0 : x->size;}
    T _sum(np x){return x == nt::nil ? OT : x->sum;}

    np _update(np x){

        if(x == nt::nil)
            return x;

        if(is_list){
            _push(x);
            _push(x->l);
            _push(x->r);
        }

        x->sum = f(f(_sum(x->l), x->val), _sum(x->r));
        x->size = _size(x->l) + _size(x->r) + 1;
        return x;
    }

    void _push(np x){
        if(x->lazy == OU)
            return ;

        x->sum = g(x->sum, x->lazy, x->size);
        x->val = g(x->val, x->lazy, 1);

        if(x->l != nt::nil)
            x->l->lazy = h(x->l->lazy, x->lazy);
        if(x->r != nt::nil)
            x->r->lazy = h(x->r->lazy, x->lazy);

        x->lazy = OU;

    }

    np _merge(np l, np r){
        if(l == nt::nil || r ==nt::nil)
            return l == nt::nil ? r : l;

        if(l->pri > r->pri){
            l->r = _merge(l->r, r);
            return _update(l);
        }else{
            r->l = _merge(l, r->l);
            return _update(r);
        }
    }

    std::pair<np,np> _split(np x, int k){
        if(x == nt::nil)
            return make_pair(nt::nil, nt::nil);

        assert(0 <= k && k <= _size(x));

        if(k <= _size(x->l)){
            std::pair<np, np> s = _split(x->l, k);
            x->l = s.second;
            return make_pair(s.first, _update(x));

        }else{
            std::pair<np, np> s = _split(x->r, k - _size(x->l) - 1);
            x->r = s.first;
            return make_pair(_update(x), s.second);
        }
    }

    np _insert(np x, int k, T val){
        np l, r;
        tie(l, r) = _split(x, k);
        return _merge(_merge(l, new nt(val, OU)), r);
    }

    np _erase(np x, int k){
        np l, r, m;
        tie(l, r) = _split(x, k);
        tie(m, r) = _split(r, 1);
        if(m != nt::nil)
            delete m;
        return _merge(l, r);
    }

    void _set(np x, int k, T val){
        _update(x);

        if(k < _size(x->l))
            _set(x->l, k, val);
        else if(_size(x->l) == k)
            x->val = val;
        else
            _set(x->r, k - _size(x->l) - 1, val);

        _update(x);
    }

    void _add(np x, int l, int r, U val){
        assert(is_list);
        _update(x);

        if(x == nt::nil)
            return ;
        l = std::max(l, 0);
        r = std::min(r, _size(x));

        int sl = _size(x->l);

        if(l >= r)
            return ;

        if (l == 0 && r == _size(x)){
            x->lazy = h(x->lazy, val);
        }
        else{
            if(l <= sl && sl < r)
                x->val = g(x->val, val, 1);

            _add(x->l, l, r, val);
            _add(x->r, l - sl - 1, r - sl - 1, val);
        }

        _update(x);
    }

    np _getnode(np x, int k){

        _update(x);

        assert(0 <= k && k < _size(x));

        if(k < _size(x->l))
            return _getnode(x->l, k);
        else if(_size(x->l) == k)
            return x;
        else
            return _getnode(x->r, k - _size(x->l) - 1);
    }

    T _get(np x, int k){
        return _getnode(x, k)->val;
    }

    T _rangesum(np x, int l, int r){
        _update(x);

        l = std::max(l, 0);
        r = std::min(r, _size(x));
        if(l >= r)
            return OT;
        if(l == 0 && r == _size(x))
            return _sum(x);

        int sl = _size(x->l);
        T ret = (l <= sl && sl < r ? x->val : OT);
        ret = f(_rangesum(x->l, l, r), ret);
        ret = f(ret, _rangesum(x->r, l - sl - 1, r - sl - 1));
        return ret;
    }

    int _lowerbound(np x, T val){
        _update(x);

        if(x == nt::nil)
            return 0;
        if(val <= x->val)
            return _lowerbound(x->l, val);
        else
            return _lowerbound(x->r,val) + _size(x->l) + 1;
    }

    int _upperbound(np x, T val){
        _update(x);

        if(x == nt::nil)
            return 0;
        if(val < x->val)
            return _upperbound(x->l, val);
        else
            return _upperbound(x->r,val) + _size(x->l) + 1;
    }

    int _func_lowerbound(np x, T val, T _left_sum){
        if(x == nt::nil)
            return 0;

        _update(x->l);
        _update(x->r);

        T left_sum = f(_left_sum, x->l->sum);
        if(val <= left_sum)
            return _func_lowerbound(x->l, val, _left_sum);
        left_sum = f(x->l->sum, x->val);
        if(val <= left_sum)
            return _size(x->l);
        return _func_lowerbound(x->r, val, f(left_sum, x->r->val)) + _size(x->l) + 1;
    }

    int _func_upperbound(np x, T val, T _left_sum){
        if(x == nt::nil)
            return 0;

        _update(x->l);
        _update(x->r);

        T left_sum = f(_left_sum, x->l->sum);
        if(val < left_sum)
            return _func_upperbound(x->l, val, _left_sum);
        left_sum = f(x->l->sum, x->val);
        if(val < left_sum)
            return _size(x->l);
        return _func_upperbound(x->r, val, f(left_sum, x->r->val)) + _size(x->l) + 1;
    }

    np _insert(np x, T val){
        return _insert(x, _lowerbound(x, val), val);
    }

    void _clear(np x){
        if(x->l != nt::nil){
            _clear(x->l);
            delete(x->l);
            x->l = nt::nil;
        }
        if(x->r != nt::nil){
            _clear(x->r);
            delete(x->r);
            x->r = nt::nil;
        }
    }

    void push_front(T val){
        root = _merge(new nt(val, OU), root);
    }

    void push_back(T val){
        root = _merge(root, new nt(val, OU));
    }

    void pop_front(){
        root = _split(root, 1).second;
    }

    void pop_back(){
        root = _split(root, _size(root) - 1).first;
    }

    // [0, k)と[k, size)に分割して, [k, size)側を返す
    Treap split_left(int k){
        np p;
        tie(root, p) = _split(root, k);
        return decltype(this)(f, g, h, p);
    }

    // [0, k)と[k, size)に分割して, [0, k)側を返す
    Treap split_right(int k){
        np p;
        tie(p, root) = _split(root, k);
        return decltype(this)(f, g, h, p);
    }

    // rootを含めたサイズの出力
    int size(){
        return (root == nt::nil ? 0 : root->size);
    }

    // サイズが0かどうかの判定
    bool empty(){
        return root == nt::nil;
    }

    // k番目への代入
    void set(int k, T val){
        return _set(root, k, val);
    }

    // k番目への演算
    void add(int k, U val){
        assert(is_list);
        return _add(root, k, k + 1, val);
    }

    // [l, r)への区間演算
    void add(int l, int r, U val){
        assert(is_list);
        return _add(root, l, r, val);
    }

    // k番目の値取得
    T get(int k){
        return _get(root, k);
    }

    // [l, r)の区間取得
    T get(int l, int r){
        return _rangesum(root, l, r);
    }

    // k番目への挿入
    void insert(int k, T val){
        assert(is_list);
        root = _insert(root, k, val);
    }

    // 適切な位置への挿入
    void insert(T val){
        root = _insert(root, val);
    }

    // val <= get(k) となるような最小のk
    int lowerbound(T val){
        return _lowerbound(root, val);
    }

    // val < get(k) となるような最小のk
    int upperbound(T val){
        return _upperbound(root, val);
    }

    // val <= sum(0, k) となるような最小のk
    int func_lowerbound(T val){
        _update(root);
        return _func_lowerbound(root, val, OT);
    }

    // val < sum(0, k) となるような最小のk
    int func_upperbound(T val){
        _update(root);
        return _func_upperbound(root, val, OT);
    }

    // k番目の要素削除
    void erase(int k){
        root = _erase(root, k);
    }

    // 要素の全削除
    void clear(){
        if(root != nt::nil){
            _clear(root);
            delete(root);
            root = nt::nil;
        }
    }
};
}

#endif // TREAP_H
