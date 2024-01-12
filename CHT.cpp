#include "template.cpp"

/*
    Dynamic CHT on indices.
    Usage: https://codeforces.com/contest/319/submission/241323520
*/

template<ll _inf, ll left_border, ll right_border, bool is_max=false>
class LiChaoTree{
    struct Line{
        ll k, b;
        int sons[2]; // {left, right}
        Line(ll _k=0, ll _b=_inf): k(_k), b(_b), sons{-1, -1} {}
        ll operator()(ll x){ return k*x + b; }
    };
    vector<Line> lines;
    int new_line(){ lines.push_back(Line()); return lines.size()-1; }
    int son(int root, int s){ return lines[root].sons[s] = (lines[root].sons[s] == -1 ? new_line() : lines[root].sons[s]); }
    void swap_lines(Line& a, Line& b){ // necessary as left/righ must stay same
        swap(a.b, b.b), swap(a.k, b.k);
    }
    void add_line(int cur, ll l, ll r, Line newl){
        ll m = (l + r) / 2;
        Line &curl = lines[cur];
        bool lef = newl(l) < curl(l);
        bool mid = newl(m) < curl(m);
        if (mid) swap_lines(newl, curl);
        if (r - l <= 1) return;
        if (lef != mid) add_line(son(cur, 0), l, m, newl);
        else            add_line(son(cur, 1), m, r, newl);
    }
    ll get(int cur, ll x, ll l, ll r){
        ll cur_value = lines[cur](x);
        ll m = (l + r) / 2;
        if (r - l <= 1) return cur_value;
        if (x < m) return min(cur_value, get(son(cur, 0), x, l, m));
        else       return min(cur_value, get(son(cur, 1), x, m, r));
    }
public:
    LiChaoTree(){ lines.resize(1);}
    void add_line(ll k, ll b){
        add_line(0, left_border, right_border, (is_max ? Line(-k, -b) : Line(k, b)));
    }
    ll get(ll x){
        return get(0, x, left_border, right_border) * (is_max ? -1 : 1);
    }
};