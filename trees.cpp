#include "lazy_segment_tree.cpp"

/*
    HLD:
        Create Heavy-Light decomposition and provide support functions for convenient use of the decomposition
        Also may be used for LCA finding with O(N) memory but bigger constant:
            https://judge.yosupo.jp/submission/198438

    HLDSegTree:
        Specialization of HLD for Lazy Segment Tree
        Support subtree query/update in segment tree time complexity
        Support path query/update with additional O(log(N)) to segment tree time complexity

        Example with all types of queries:
            https://codeforces.com/group/NOwIbqv33y/contest/307219/submission/252684653
    LCA:
        Calculate lca in O(log(N)) time and O(N*log(N)) memory using binary lifting
        https://judge.yosupo.jp/submission/198437
        Array up[][LOG] may be used for custom binary search on path to root
*/

struct HLD{
    int s[MAXN], p[MAXN], tin[MAXN], tout[MAXN], head[MAXN], levs[MAXN], t;
    bool upper(int a, int b){ return tin[a] <= tin[b] && tin[b] < tout[a]; }
    HLD(vector<ll> gr[]){
        auto sizes = [&](auto &&self, int x, int par, int lev) -> void{
            s[x] = 1, p[x] = par, levs[x] = lev;
            if (gr[x].size() && gr[x][0] == par) swap(gr[x][0], gr[x].back());
            for(auto& i : gr[x]){
                if (i == par) continue;
                self(self, i, x, lev+1);
                s[x] += s[i];
                if (s[i] > s[gr[x][0]]) swap(i, gr[x][0]); 
            }
        }; sizes(sizes, 0, 0, 0);
        auto hld = [&](auto &&self, int x) -> void{
            tin[x] = t++;
            for(auto i : gr[x]){
                if (i == p[x]) continue;
                head[i] = (i == gr[x][0] ? head[x] : i);
                self(self, i);
            }
            tout[x] = t;
        }; t = 0; hld(hld, 0);
    }
    int do_path(int a, int b, function<void(int, int)>&& f){
        for(; head[a] != head[b]; b = p[head[b]]){
            if (levs[head[a]] > levs[head[b]]) swap(a, b);
            f(tin[head[b]], tin[b]);
        }
        if (tin[a] > tin[b]) swap(a, b);
        f(tin[a], tin[b]);
        return a;
    }
    void do_subtree(int v, function<void(int, int)>&& f){ f(tin[v], tout[v]-1); }
    int lca(int a, int b){ return do_path(a, b, []([[maybe_unused]]int a,[[maybe_unused]] int b){});}
    int dist(int a, int b){ return levs[a] + levs[b] - 2*levs[lca(a, b)]; }
};

template<class Info, class Tag>
struct HLDSegTree : public HLD{
    LazySegmentTree<Info, Tag> segtree;
    HLDSegTree(vector<ll> gr[], const vector<Info>& init={}): HLD(gr), segtree(0) {
        if (init.size()){
            vector<Info> seg_base(init.size());
            for(int i = 0; i < t; i++) seg_base[tin[i]] = init[i];
            segtree = LazySegmentTree<Info, Tag>(seg_base);
        }else segtree = LazySegmentTree<Info, Tag>(t);
    }
    void vertex_update(int v, const Info& x){ segtree.modify(tin[v], x); }
    void path_update(int a, int b, const Tag& x){ do_path(a, b, [&](int l, int r){ segtree.rangeApply(l, r, x); }); }
    Info path_get(int a, int b){
        Info ans;
        do_path(a, b, [&](int l, int r){ ans = ans + segtree.rangeQuery(l, r); });
        return ans;
    }
    void subtree_update(int a, const Tag& x){ do_subtree(a, [&](int l, int r){ segtree.rangeApply(l, r, x); }); }
    Info subtree_get(int a){
        Info ans;
        do_subtree(a, [&](int l, int r){ ans = ans + segtree.rangeQuery(l, r); });
        return ans;
    }
};

struct Calc_LCA{
    const static int LOG = 20;
    int tin[MAXN], tout[MAXN], levs[MAXN], up[MAXN][LOG], t;
    void calc(const vector<ll> gr[]){ 
        auto dfs = [&](auto &&self, int x, int p, int lev) -> void{
            levs[x] = ++lev, tin[x] = ++t, up[x][0] = p;
            for(int i = 1; i < LOG; i++) up[x][i] = up[up[x][i-1]][i-1];
            for(int i : gr[x]) if (i != p) self(self, i, x, lev);
            tout[x] = ++t;
        }; t = 0; dfs(dfs, 0, 0, 0);
    }
    bool upper(int a, int b){ return tin[a] <= tin[b] && tout[a] >= tout[b]; }
    int lca(int a, int b){
        if (upper(a, b)) return a;
        if (upper(b, a)) return b;
        for(int i = LOG-1; i >= 0; i--) if (!upper(up[a][i], b)) a = up[a][i];
        return up[a][0];
    }
    int dist(int a, int b){ return levs[a] + levs[b] - 2*levs[lca(a, b)]; }
} lca;
