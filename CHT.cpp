#include "template.cpp"

/*
    LiChaoTree:
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
    void swap_lines(Line& a, Line& b){ // necessary as left/right must stay same
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

template<bool is_max=false>
class CHT{
    ll fix_max(ll x){ return x=(is_max ? -x : x); }
    vector<pll> hull;
    ld intersection(pll a, pll b){ return 1.l * (b.s - a.s) / (a.f - b.f); }
    bool can_delete(pll a, pll b, pll c){ return intersection(a, c) < intersection(a, b); }
    ll get_val(pll a, ll x){ return a.f * x + a.s; }
public:
    ll get(ll x){
        ll l = -1, r = hull.size() - 1;
        while(l+1 < r){
            ll m = (l + r) / 2;
            ((get_val(hull[m], x) < get_val(hull[m+1], x)) ? r : l) = m;
        }
        return fix_max(get_val(hull[r], x));
    }
    void add_line(ll k, ll b){
        pll x{fix_max(k), fix_max(b)};
        if (!empty() && k == hull.back().f) {
            if (b > hull.back().s) hull.pop_back();
            else return;
        }
        while(hull.size() >= 2 && can_delete(hull[hull.size()-2], hull[hull.size()-1], x)) hull.pop_back();
        hull.pb(x);
    }
    int size(){ return hull.size(); }
    void add(pll x){ add_line(fix_max(x.f), fix_max(x.s)); }
    bool empty() { return hull.empty(); }
    void clear() { return hull.clear(); }
    const pll operator[](int ind){ return hull[ind]; }
};

template<bool is_max=false>
struct CHT_dynamic{
    using CHT = CHT<is_max>;
    vector<CHT> lines;
    CHT merge(CHT a, CHT b) {
        CHT res;
        int j = 0;
        for (int i = 0; i < sz(a); i++) {
            while (j < sz(b) && a[i].f < b[j].f) res.add(b[j++]);
            res.add(a[i]);
        }
        while (j < sz(b)) res.add(b[j++]);
        return res;
    }
    void add_line(ll k, ll b) {
        CHT cur;
        cur.add_line(k, b);
        for (CHT &ex : lines) {
            if (ex.empty()) {
                swap(ex, cur);
                break;
            }
            cur = merge(cur, ex);
            ex.clear();
        }
        if (!cur.empty()) lines.push_back(cur);
    }
    ll get(ll x) {
        ll ret = (is_max ? LLONG_MIN : LLONG_MAX);
        for (CHT &cur : lines) if (!cur.empty()) {
            if (is_max) maxeq(ret, cur.get(x));
            else mineq(ret, cur.get(x));
        }
        return ret;
    }
};
