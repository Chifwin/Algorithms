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

template<class Info, class Tag, ll type=0>
class LazySegmentTree {
    // type == 0 -> only lazy propagation; type == 1 -> implicit; type == 2 -> persistent
    template<ll typ, bool dummy> struct InfoTag_t{
        Info info;
        Tag tag;
        ll l=-1, r=-1;
    };
    template<bool dummy> struct InfoTag_t<0, dummy>{
        Info info;
        Tag tag;
    };
    using InfoTag = InfoTag_t<type, true>;
    ll n;
    vector<InfoTag> tree;
    inline ll new_node(InfoTag node=InfoTag()){ tree.push_back(node); return tree.size()-1; }
    inline ll lson(ll v, [[maybe_unused]] bool create=true){
        if constexpr(type != 0){
            if (tree[v].l == -1 && create) tree[v].l = new_node();
            return tree[v].l; 
        }
        return v << 1;
    }
    inline ll rson(ll v, [[maybe_unused]] bool create=true){
        if constexpr(type != 0){
            if (tree[v].r == -1 && create) tree[v].r = new_node();
            return tree[v].r; 
        }
        return v << 1 | 1;
    }
    void pull(ll id) {
        ll l = lson(id, false), r = rson(id, false);
        tree[id].info = (l == -1 ? Info() : tree[l].info) + (r == -1 ? Info() : tree[r].info);
    }
    void apply(ll id, const Tag &v, ll len) {
        tree[id].info.merge(v, len);
        tree[id].tag += v;
    }
    void push(ll id, ll l, ll r) {
        ll m = (l + r) >> 1;
        Tag t = tree[id].tag;
        if constexpr (type != 0) if (t.empty()) return;
        apply(lson(id), t, m - l + 1);
        apply(rson(id), t, r - m);
        tree[id].tag = Tag();
    }
    ll modify(ll id, ll l, ll r, ll p, const Info &v) {
        ll new_root = id;
        if (r == l) {
            if constexpr(type == 2) new_root = new_node();
            tree[new_root].info = v;
            return new_root;
        }
        ll m = (l + r) >> 1;
        push(id, l, r);
        if constexpr(type == 2) new_root = new_node(tree[id]);
        if (p <= m){
            ll ne = modify(lson(id), l, m, p, v);
            if constexpr(type == 2) tree[new_root].l = ne;
        } else {
            ll ne = modify(rson(id), m + 1, r, p, v);
            if constexpr(type == 2) tree[new_root].r = ne;
        }
        pull(new_root);
        return new_root;
    }
    Info rangeQuery(ll id, ll l, ll r, ll ql, ll qr) {
        if (qr < l || r < ql || id == -1) return Info();
        if (ql <= l && r <= qr) return tree[id].info;
        ll m = (l + r) >> 1;
        push(id, l, r);
        return rangeQuery(lson(id, false), l, m, ql, qr) + rangeQuery(rson(id, false), m + 1, r, ql, qr);
    }
    ll rangeApply(ll id, ll l, ll r, ll ql, ll qr, const Tag &v) {
        ll new_root = id;
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
        function<void(ll, ll, ll)> build = [&](ll id, ll l, ll r){
            if (r == l) {
                tree[id].info = init[l];
                return;
            }
            ll m = (l + r) >> 1;
            build(lson(id), l, m);
            build(rson(id), m + 1, r);
            pull(id);
        };
        build(1, 0, n - 1);
    }
    ll modify(ll p, const Info &v, ll ver=1) {
        return modify((type == 2 ? ver : 1), 0, n - 1, p, v);
    }
    ll rangeApply(ll ql, ll qr, const Tag &v, ll ver=1) {
        return rangeApply((type == 2 ? ver : 1), 0, n - 1, ql, qr, v);
    }
    Info rangeQuery(ll ql, ll qr, ll ver=1) {
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
