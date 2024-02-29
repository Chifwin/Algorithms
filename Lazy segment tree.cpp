#include "template.cpp"

/*
    Segment tree with lazy propagation, can be also implicit and persistent version.
    Info store information in node, Tag it's update.
    Both must have identity element as result of empty constructor.
    Examples: 
        Lazy propagation: https://codeforces.com/contest/258/submission/248993052
        Implicit:         https://codeforces.com/contest/12/submission/248999849
*/

template<class Info, class Tag, int type=0>
class LazySegmentTree {
    // type == 0 -> only lazy propagation; type == 1 -> implicit; type == 2 -> persistent
    template<int typ, bool dummy=true> struct InfoTag_t{
        Info info;
        Tag tag;
        int l=-1, r=-1;
    };
    template<bool dummy> struct InfoTag_t<0, dummy>{
        Info info;
        Tag tag;
    };
    using InfoTag = InfoTag_t<type, true>;
    int n;
    vector<InfoTag> tree;
    inline int new_node(InfoTag node=InfoTag()){ tree.push_back(node); return tree.size()-1; }
    inline int lson(int v){
        if constexpr(type != 0){
            if (tree[v].l == -1) tree[v].l = new_node();
            return tree[v].l; 
        }
        return v << 1;
    }
    inline int rson(int v){
        if constexpr(type != 0){
            if (tree[v].r == -1) tree[v].r = new_node();
            return tree[v].r; 
        }
        return v << 1 | 1;
    }
    void pull(int id) {
        int l = lson(id), r = rson(id);
        tree[id].info = (l == -1 ? Info() : tree[l].info) + (r == -1 ? Info() : tree[r].info);
    }
    void apply(int id, const Tag &v, int len) {
        tree[id].info.merge(v, len);
        tree[id].tag += v;
    }
    void push(int id, int l, int r) {
        int m = (l + r) >> 1;
        Tag t = tree[id].tag;
        apply(lson(id), t, m - l + 1);
        apply(rson(id), t, r - m);
        tree[id].tag = Tag();
    }
    int modify(int id, int l, int r, int p, const Info &v) {
        int new_root = id;
        if (r == l) {
            if constexpr(type == 2) new_root = new_node();
            tree[new_root].info = v;
            return new_root;
        }
        int m = (l + r) >> 1;
        push(id, l, r);
        if constexpr(type == 2) new_root = new_node(tree[id]);
        if (p <= m){
            int ne = modify(lson(id), l, m, p, v);
            if constexpr(type == 2) tree[new_root].l = ne;
        } else {
            int ne = modify(rson(id), m + 1, r, p, v);
            if constexpr(type == 2) tree[new_root].r = ne;
        }
        pull(new_root);
        return new_root;
    }
    Info rangeQuery(int id, int l, int r, int ql, int qr) {
        if (qr < l || r < ql || id == -1) return Info();
        if (ql <= l && r <= qr) return tree[id].info;
        int m = (l + r) >> 1;
        push(id, l, r);
        return rangeQuery(lson(id), l, m, ql, qr) + rangeQuery(rson(id), m + 1, r, ql, qr);
    }
    int rangeApply(int id, int l, int r, int ql, int qr, const Tag &v) {
        int new_root = id;
        if (qr < l || r < ql) return id;
        if constexpr(type == 2) new_root = new_node(tree[id]);
        if (ql <= l && r <= qr) {
            apply(new_root, v, r - l + 1);
            return new_root;
        }
        [[maybe_unused]] int m = (l + r) >> 1, ne = -1;
        push(id, l, r);
        ne = rangeApply(lson(id), l, m, ql, qr, v);
        if constexpr(type == 2) tree[new_root].l = ne;
        ne = rangeApply(rson(id), m + 1, r, ql, qr, v);
        if constexpr(type == 2) tree[new_root].r = ne;
        pull(new_root);
        return new_root;
    }
public:
    LazySegmentTree(int _n, [[maybe_unused]] ll q=0): n(_n) {
        if constexpr(type == 0) tree.resize(4*n);
        else { tree.resize(2); tree.reserve(4*q+10); }
    }
    template <typename T>
    LazySegmentTree(const vector<T>& init, ll q=0) : LazySegmentTree((int)init.size(), q) {
        function<void(int, int, int)> build = [&](int id, int l, int r){
            if (r == l) {
                tree[id].info = init[l];
                return;
            }
            int m = (l + r) >> 1;
            build(lson(id), l, m);
            build(rson(id), m + 1, r);
            pull(id);
        };
        build(1, 0, n - 1);
    }
    int modify(int p, const Info &v, int ver=1) {
        return modify((type == 2 ? ver : 1), 0, n - 1, p, v);
    }
    int rangeApply(int ql, int qr, const Tag &v, int ver=1) {
        return rangeApply((type == 2 ? ver : 1), 0, n - 1, ql, qr, v);
    }
    Info rangeQuery(int ql, int qr, int ver=1) {
        return rangeQuery((type == 2 ? ver : 1), 0, n - 1, ql, qr);
    }
    /*
    struct Tag{ Tag() {} };
    struct Info{
        Info() {}
        void merge(Tag r, [[maybe_unused]] int len){}
    };
    Info operator+(Info l, Info r){ rt l; }
    Tag& operator+=(Tag &l, Tag r) { rt l; }
    */
};
