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
