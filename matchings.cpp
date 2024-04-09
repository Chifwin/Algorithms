#include "template.cpp"

/*
    Find maximal matching in bipartite graph.
    Is realization of Hopcroft-Karp/Dinitz algorithm as described here:
        https://codeforces.com/blog/entry/118098
    Doesn't require division on two parts, so edges could be undirected
    In case memory is important - need only edges from left to right part and
        vertices of left part have lower indices than right.
    https://judge.yosupo.jp/submission/201755
*/

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
    function<bool(int)> kuhn = [&](int x) -> bool {
        for(int &i = ptr[x]; i < int(gr[x].size()); i++){
            if (is_l[gr[x][i]]) continue;
            int &u = match[gr[x][i]];
            if (u == -1 || (dist[u] == dist[x] + 1 && kuhn(u))) return (match[u = x] = true);
        }
        return false;
    };
    while(1){
        fill(dist.begin(), dist.end(), n);
        int l = 0, r = 0, flag = false, x, u;
        for(int i = 0; i < n; i++) if (is_l[i] && !match[i]) dist[i] = 0, q[r++] = i;
        while(l < r){
            if (is_l[x = q[l++]]) for(auto v : gr[x]){
                if (is_l[v]) continue;
                flag |= ((u = match[v]) == -1);
                if (u != -1 && dist[x] + 1 < dist[u]) dist[u] = dist[x]+1, q[r++] = u;
            }
        }
        if (!flag) break;
        fill(ptr.begin(), ptr.end(), 0);
        for(int i = 0; i < n; i++) if (is_l[i] && !match[i]) kuhn(i);
    }
    vector<pair<int, int>> ans;
    for(int i = 0; i < n; i++) if (!is_l[i] && match[i] != -1) ans.emplace_back(match[i], i);
    return ans;
}
