#include "template.cpp"


template<class Info, class Tag>
class Treap{
    struct Node{
        Info val, sub;
        Tag upd;
        int l = -1, r = -1;
        int prior, size = 1;
        char rev = 0;
        Node(Info val): val(val), sub(val), prior(rand()) {}
    };
    vector<Node> t;
    int recalc(int v){
        if (v == -1) return v;
        Node& cur = t[v];
        cur.size = 1, cur.sub = Info();
        if (cur.l != -1) cur.size += t[cur.l].size, cur.sub = t[cur.l].sub;
        cur.sub = cur.sub + cur.val;
        if (cur.r != -1) cur.size += t[cur.r].size, cur.sub = cur.sub + t[cur.r].sub;
        return v;
    }
    void apply_upd(int v, Tag tag){
        if (v == -1) return;
        t[v].upd += tag;
        t[v].val.merge(tag, 1);
        t[v].sub.merge(tag, t[v].size);
    }
    void apply_rev(int v){
        t[v].rev ^= 1;
        t[v].sub.rev();
        t[v].val.rev();
    }
    void push(int v){
        if (v == -1) return;
        if (t[v].rev){
            t[v].rev = false;
            swap(t[v].l, t[v].r);
            if (t[v].l != -1) apply_rev(t[v].l);
            if (t[v].r != -1) apply_rev(t[v].r);
        }
        apply_upd(t[v].l, t[v].upd);
        apply_upd(t[v].r, t[v].upd);
        t[v].upd = Tag();
        recalc(v);
    }
    template<typename F>
    int apply(ll v, ll l, ll r, F func){
        auto [T, R] = split(v, r);
        auto [L, M] = split(T, l-1);
        func(M);
        return merge(L, merge(M, R));
    }
    template<typename F>
    pair<int, int> split_pred(int p, F func) {
        if (p == -1) return {-1, -1};
        push(p);
        if (func(t[p])) {
            auto a = split_pred(t[p].r, func);
            t[p].r = a.f;
            return {recalc(p), recalc(a.s)};
        }
        auto a = split_pred(t[p].l, func);
        t[p].l = a.s;
        return {recalc(a.f), recalc(p)};
    }
public:
    Treap(ll n) { t.reserve(n); }
    int merge(int l, int r) {
        if (l == -1) return r;
        if (r == -1) return l;
        push(l), push(r);
        if (t[l].prior > t[r].prior) {
            t[l].r = merge(t[l].r, r);
            return recalc(l);
        }
        t[r].l = merge(l, t[r].l);
        return recalc(r);
    }
    pair<int, int> split(int p, int x) { // {x+1 nodes, rem}
        return split_pred(p, [&](Node& cur){
            int sz_l = (cur.l == -1 ? 0 : t[cur.l].size);
            if (sz_l <= x){
                x -= sz_l + 1;
                return true;
            }
            return false;
        });
    }
    // pair<int, int> bin(int p, int x){ // example split {nodes with val.x <= x, rem}
    //     return split_pred(p, [&](Node& cur){
    //         return (cur.val.x <= x);
    //     });
    // }
    int new_node(Info val){
        t.emplace_back(val);
        return t.size()-1;
    }
    void new_nodel(int& v, Info val){ v = merge(v, new_node(Val)); }
    Info get(int& v, int l, int r){
        Info res;
        v = apply(v, l, r, [&](int root){ res = t[root].sub; });
        return res;
    }
    void rev(int& v, int l, int r){
        v = apply(v, l, r, [&](int root){ apply_rev(root); });
    }
    void update(int& v, int l, int r, Tag t){
        v = apply(v, l, r, [&](int root){ apply_upd(root, t); });
    }
    void print(int root){
        if (root == -1) return;
        Node &v = t[root];
        dbg(root, v.size, v.l, v.r, v.rev, v.val.to_string(), v.sub.to_string());
        print(v.l);
        print(v.r);
    }
};

struct Tag{
    Tag() {}
};
struct Info{
    Info() {}
    void merge(Tag tag, ll len){}
    void rev(){}
    string to_string(){
        stringstream cout;
        return "[" +  cout.str() + "]";
    }
};
Info operator+(Info l, Info r){ return l; }
Tag& operator+=(Tag &l, Tag r) { return l; }