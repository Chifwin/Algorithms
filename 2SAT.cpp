#include "template.cpp"

/*
    Template for solving 2SAT. Variables are from 1 to n. In clause positive value means true value, negative - false.
    SCC by Tarjan algorithm.
*/

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