#include "template.cpp"

/*
    Segment tree with lazy propagation, can be also implicit and persistent version.
    Info store information in node, Tag it's update.
    Both must have identity element as result of empty constructor.
    Second constructor argument is for implicit and persistent versions to reserve 
        nodes, should be around 20*number_of_queries.
    Examples: 
        Lazy propagation: https://codeforces.com/contest/258/submission/248993052
                          https://codeforces.com/contest/718/submission/249037011
        Implicit:         https://codeforces.com/contest/12/submission/248999849
        Persistent:       https://codeforces.com/contest/500/submission/249040072
*/

template<class Info, class Tag, int type=0>
class LazySegmentTree {
    // type == 0 -> only lazy propagation; type == 1 -> implicit; type == 2 -> persistent
    template<int typ, bool dummy> struct InfoTag_t{
        Info info;
        Tag tag;
        int l=-1, r=-1;
    };
    template<bool dummy> struct InfoTag_t<0, dummy>{
        Info info;
        Tag tag;
    };
    using InfoTag = InfoTag_t<type, true>;
    ll n;
    vector<InfoTag> tree;
    inline int new_node(InfoTag node=InfoTag()){ tree.push_back(node); return tree.size()-1; }
    inline int lson(int v, [[maybe_unused]] bool create=true){
        if constexpr(type != 0){
            if (tree[v].l == -1 && create) tree[v].l = new_node();
            return tree[v].l; 
        }
        return v << 1;
    }
    inline int rson(int v, [[maybe_unused]] bool create=true){
        if constexpr(type != 0){
            if (tree[v].r == -1 && create) tree[v].r = new_node();
            return tree[v].r; 
        }
        return v << 1 | 1;
    }
    void pull(int id) {
        int l = lson(id, false), r = rson(id, false);
        tree[id].info = (l == -1 ? Info() : tree[l].info) + (r == -1 ? Info() : tree[r].info);
    }
    void apply(int id, const Tag &v, ll len) {
        tree[id].info.merge(v, len);
        tree[id].tag += v;
    }
    void push(int id, ll l, ll r) {
        ll m = (l + r) >> 1;
        Tag t = tree[id].tag;
        if constexpr (type != 0) if (t.empty()) return;
        apply(lson(id), t, m - l + 1);
        apply(rson(id), t, r - m);
        tree[id].tag = Tag();
    }
    int modify(int id, ll l, ll r, ll p, const Info &v) {
        int new_root = id;
        if (r == l) {
            if constexpr(type == 2) new_root = new_node();
            tree[new_root].info = v;
            return new_root;
        }
        ll m = (l + r) >> 1;
        push(id, l, r);
        if constexpr(type == 2) new_root = new_node(tree[id]);
        if (p <= m){
            [[maybe_unused]] int ne = modify(lson(id), l, m, p, v);
            if constexpr(type == 2) tree[new_root].l = ne;
        } else {
            [[maybe_unused]] int ne = modify(rson(id), m + 1, r, p, v);
            if constexpr(type == 2) tree[new_root].r = ne;
        }
        pull(new_root);
        return new_root;
    }
    Info rangeQuery(int id, ll l, ll r, ll ql, ll qr) {
        if (qr < l || r < ql || id == -1) return Info();
        if (ql <= l && r <= qr) return tree[id].info;
        ll m = (l + r) >> 1;
        push(id, l, r);
        return rangeQuery(lson(id, false), l, m, ql, qr) + rangeQuery(rson(id, false), m + 1, r, ql, qr);
    }
    int rangeApply(int id, ll l, ll r, ll ql, ll qr, const Tag &v) {
        int new_root = id;
        if (qr < l || r < ql) return id;
        if constexpr(type == 2) new_root = new_node(tree[id]);
        if (ql <= l && r <= qr) {
            apply(new_root, v, r - l + 1);
            return new_root;
        }
        [[maybe_unused]] ll m = (l + r) >> 1, ne = -1;
        push(id, l, r);
        ne = rangeApply(lson(id), l, m, ql, qr, v);
        if constexpr(type == 2) tree[new_root].l = ne;
        ne = rangeApply(rson(id), m + 1, r, ql, qr, v);
        if constexpr(type == 2) tree[new_root].r = ne;
        pull(new_root);
        return new_root;
    }
public:
    LazySegmentTree(ll _n, [[maybe_unused]] ll q=0): n(_n) {
        if constexpr(type == 0) tree.resize(4*n);
        else { tree.resize(2); tree.reserve(q+10); }
    }
    template <typename T>
    LazySegmentTree(const vector<T>& init, ll q=0) : LazySegmentTree((ll)init.size(), q) {
        auto build = [&](auto && self, int id, ll l, ll r){
            if (r == l) {
                tree[id].info = init[l];
                return;
            }
            ll m = (l + r) >> 1;
            self(self, lson(id), l, m);
            self(self, rson(id), m + 1, r);
            pull(id);
        };
        build(build, 1, 0, n - 1);
    }
    int modify(ll p, const Info &v, int ver=1) {
        return modify((type == 2 ? ver : 1), 0, n - 1, p, v);
    }
    int rangeApply(ll ql, ll qr, const Tag &v, int ver=1) {
        return rangeApply((type == 2 ? ver : 1), 0, n - 1, ql, qr, v);
    }
    Info rangeQuery(ll ql, ll qr, int ver=1) {
        return rangeQuery((type == 2 ? ver : 1), 0, n - 1, ql, qr);
    }
    /*
    struct Tag{
        Tag() {}
        bool empty(){ return false; }
    };
    struct Info{
        Info() {}
        void merge(Tag r, [[maybe_unused]] ll len){}
    };
    Info operator+(Info l, Info r){ return l; }
    Tag& operator+=(Tag &l, Tag r) { return l; }
    */
};


/*
    Adapter for Bottom-Up Segment Tree from Atcoder library:
    https://github.com/atcoder/ac-library/blob/master/atcoder/lazysegtree.hpp
    Note that Info::merge(Tag, len) get always incorrect len.
    Tested on PTZ Winter 2025 task (I3)
*/

template<class Info, class Tag>
class FastLazySegmentTree{
    lazy_segtree<Info, [](Info a, Info b){return a + b;}, [](){return Info();}, Tag, [](Tag a, Info b){ b.merge(a, -1); return b;}, [](Tag a, Tag b){return a += b;}, [](){return Tag();}> seg;
public:
    FastLazySegmentTree(ll _n): seg(_n) {}
    template <typename T>
    FastLazySegmentTree(const vector<T>& init) : seg(vector<Info>(init.begin(), init.end())) {}
    void modify(ll p, const Info& v){ seg.set(p, v); }
    void rangeApply(ll ql, ll qr, const Tag& v){ seg.apply(ql, qr+1, v); }
    Info rangeQuery(ll ql, ll qr){ return seg.prod(ql, qr+1); }
};