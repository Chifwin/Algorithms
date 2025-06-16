#include "template.cpp"

/*
    Eulerian_Path:
        Finds Eulerian path/cycle in both directed and undirected graphs in O(N+M)
        Return desired {sequence of vertices, sequence of edges}
        Edges are numerated from 0 in order of addition
        Undirected graph: 
            https://codeforces.com/contest/1981/submission/264060798
            https://judge.yosupo.jp/submission/212822
        Directed graph: 
            https://judge.yosupo.jp/submission/212823
    
    matching_bipartite:
        Find maximal matching in bipartite graph.

        Hint: NEVER user Kuhn

        Is realization of Hopcroft-Karp/Dinitz algorithm as described here:
            https://codeforces.com/blog/entry/118098
        Doesn't require division on two parts, so edges could be undirected
        In case memory is important - need only edges from left to right part and
            vertices of left part have lower indices than right.

        Example:
            https://judge.yosupo.jp/submission/201755
            https://codeforces.com/contest/2052/submission/297016240

    Two_SAT:
        Template for solving 2SAT. Variables are from 1 to n. In clause positive value means true value, negative - false.
        SCC by Tarjan algorithm.
*/

template<bool directed=false>
class Eulerian_Path{
    vector<vector<array<int, 2>>> gr;
    int m;
public:
    Eulerian_Path(int n): gr(n), m(0) {}
    void add_edge(int a, int b){
        gr[a].push_back({b, m++});
        if constexpr (!directed) gr[b].push_back({a, m-1});
    }
    optional<array<vector<int>, 2>> get_res(){
        int start = -1, end = -1;
        if constexpr (directed){
            vector<int> indeg(gr.size(), 0);
            for(auto& ed : gr) for (auto [v, _] : ed) indeg[v]++;
            for(size_t i = 0; i < gr.size(); i++){
                if ((int)gr[i].size()+1 == indeg[i]){
                    if (start != -1) return {};
                    start = i;
                }else if ((int)gr[i].size() == indeg[i]+1){
                    if (end != -1) return {};
                    end = i;
                }else if ((int)gr[i].size() != indeg[i]) return {};
            }
        } else{
            for(size_t i = 0; i < gr.size(); i++){
                if (gr[i].size() % 2){
                    if (start == -1) start = i;
                    else if (end == -1) end = i;
                    else return {};
                }
            }
        }
        if (start != -1){
            if (end == -1) return {};
            add_edge(start, end);
        } else {
            start = 0;
            for(size_t i = 0; i < gr.size(); i++) if (gr[i].size()) start = i;
        }
        vector<char> active_edge;
        if constexpr (!directed) active_edge.assign(m, 1);
        vector<array<int, 2>> st{{start, -1}};
        vector<int> rese, resv, it(gr.size(), 0);
        st.reserve(m+1), rese.reserve(m), resv.reserve(m+1);
        while(st.size()){
            int v = st.back()[0];
            if constexpr (!directed) 
                while(it[v] < (int)gr[v].size() && !active_edge[gr[v][it[v]][1]]) it[v]++;
            if (it[v] < (int)gr[v].size()){
                if constexpr (!directed) active_edge[gr[v][it[v]][1]] = 0;
                st.push_back(gr[v][it[v]++]);
            }else {
                if (st.back()[1] != -1) rese.push_back(st.back()[1]);
                st.pop_back(), resv.push_back(v);
            }
        }
        if (m != (int)rese.size()) return {};
        if constexpr (directed) reverse(resv.begin(), resv.end()), reverse(rese.begin(), rese.end());
        if (end != -1){ // path
            resv.pop_back();
            for(size_t i = 1; i < rese.size(); i++){
                if (rese[i-1] == m-1){
                    rotate(resv.begin(), resv.begin()+i, resv.end());
                    rotate(rese.begin(), rese.begin()+i, rese.end());
                    rese.pop_back();
                    break;
                }
            }
        }
        return {{resv, rese}};
    }
};

vector<pair<int,int>> matching_bipartite(vll gr[], int n){
    vector<char> is_l(n, -1);
    vector<int> q(n), dist(n), match(n, -1), ptr(n);
    for(int i = 0; i < n; i++){
        if (is_l[i] != -1) continue;
        int l = 0, r = 0;
        q[r++] = i, is_l[i] = 1;
        while(l < r){
            int x = q[l++];
            for(auto v : gr[x]) if (is_l[v] == -1) is_l[v] = !is_l[x], q[r++] = v;
        }
    }
    for(int i = 0; i < n; i++) if (is_l[i]) match[i] = 0;
    auto kuhn = [&](auto && self, int x) -> bool {
        for(int &i = ptr[x]; i < int(gr[x].size()); i++){
            int &u = match[gr[x][i]];
            if (u == -1 || (dist[u] == dist[x] + 1 && self(self, u))) return (match[u = x] = true);
        }
        return false;
    };
    while(1){
        fill(dist.begin(), dist.end(), n);
        int l = 0, r = 0, flag = false, x, u;
        for(int i = 0; i < n; i++) if (is_l[i] && !match[i]) dist[i] = 0, q[r++] = i;
        while(l < r){
            for(auto v : gr[x = q[l++]]){
                flag |= ((u = match[v]) == -1);
                if (u != -1 && dist[x] + 1 < dist[u]) dist[u] = dist[x]+1, q[r++] = u;
            }
        }
        if (!flag) break;
        fill(ptr.begin(), ptr.end(), 0);
        for(int i = 0; i < n; i++) if (is_l[i] && !match[i]) kuhn(kuhn, i);
    }
    vector<pair<int, int>> ans;
    ans.reserve(n/2);
    for(int i = 0; i < n; i++) if (!is_l[i] && match[i] != -1) ans.emplace_back(match[i], i);
    return ans;
}

class Two_SAT{
    vector<vector<int>> gr;
    int code(ll v){ return v > 0 ? 2*v-2 : -2*v-1; }
public:
    Two_SAT(ll n) { gr.resize(2*n); }
    bool has_ans(vll& vals){
        int timer = 0, n = gr.size()/2;
        vals.assign(n, -1);
        vector<int> tout(2*n, 0), st, comps(2*n, 0);
        auto dfs = [&](auto &&self, int x) -> int {
            int low = tout[x] = ++timer, v;
            st.push_back(x);
            for(int v : gr[x]) if (!comps[v]) low = min(low, tout[v] ? tout[v] : self(self, v));
            if (low == tout[x]) do{
                    v = st.back(); st.pop_back();
                    comps[v] = low;
                    if (vals[v/2] == -1) vals[v/2] = !(v&1);
                } while(v != x);
            return tout[x] = low;
        };
        for(int i = 0; i < 2*n; i++) if (!comps[i]) dfs(dfs, i);
        for(int i = 0; i < n; i++) if (comps[2*i] == comps[2*i+1]) return false;
        return true;
    }
    void add_impl(ll a, ll b) /* a -> b */ { gr[code(a)].pb(code(b)); }
    void add_or(ll a, ll b) /* a | b */ { add_impl(-a, b), add_impl(-b, a); }
    void add_xor(ll a, ll b) /* a ^ b */ { add_or(a, b), add_or(-a, -b); }
    void set_val(ll v) /* vals[abs(v)] = (v > 0) */ { add_or(v, v); }
};
