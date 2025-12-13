#include "template.cpp"

/* draft */

template<int siz>
struct SQueue{
    int q[siz], st = 0, end = 0;
    bool empty(){ return st >= end; }
    void pop(){ st++; }
    int front(){ return q[st]; }
    void push(int x){ q[end++] = x; }
    void clear(){ st = end = 0;}
};

struct Dinic {
    struct FlowEdge {
        int v, u;
        ll cap, flow = 0;
        FlowEdge(int v, int u, ll cap) : v(v), u(u), cap(cap) {}
        FlowEdge(){}
    };
    FlowEdge edges[MAXN*3];
    vector<int> adj[MAXN];
    int n, m = 0;
    int s, t;
    int level[MAXN], ptr[MAXN];
    SQueue<MAXN> q;
    void init(int _n, int _s, int _t) {
        n = _n, s = _s, t = _t;
        m = 0;
        fi(n) adj[i].clear();
    }
    void add_edge(int v, int u, ll cap) {
        edges[m] = FlowEdge(v, u, cap);
        edges[m+1] = FlowEdge(u, v, 0);
        adj[v].push_back(m);
        adj[u].push_back(m + 1);
        m += 2;
    }
    bool bfs() {
        while (!q.empty()) {
            int v = q.front();
            if (v == t) break;
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap == edges[id].flow) continue;
                if (level[edges[id].u] >= level[s]) continue;
                level[edges[id].u] = level[v] + 1;
                q.push(edges[id].u);
            }
        }
        return level[t] >= level[s];
    }
    ll dfs(int v, ll pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;
        for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].u;
            if (level[v] + 1 != level[u]) continue;
            ll tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0) continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }
    ll flow() {
        ll f = 0;
        while (1) {
            level[s] += n+5;
            q.push(s);
            if (!bfs()) break;
            fill(ptr, ptr+n, 0);
            while (ll pushed = dfs(s, INF)) f += pushed;
        }
        return f;
    }
} flow;
