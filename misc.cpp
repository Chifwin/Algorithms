#include "template.cpp"

/*
    BitTrie
        Efficient binary trie. It represents key-value map for three main operations:
            1) XOR all keys with some value in O(1) (operator^=)
            2) Get key-value pair such that key^value is maximum in O(H)
            3) Add key-value pair to map in O(H)
        Examples:
            https://codeforces.com/contest/842/submission/277089370
*/

template<int H, class Info, class bit_type=unsigned long long>
class BitTrie{
    struct Node{
        int sons[2]{};
        Info info;
    };
    vector<Node> t;
    bit_type xorr{};
    inline int new_node(){ t.push_back(Node()); return t.size()-1; }
    inline int to_son(int ind, int s){ return t[ind].sons[s] = (t[ind].sons[s] ? t[ind].sons[s] : new_node()); }
    bool is_empty(int id) { return t[id].info == Info(); }
    inline bool empty_son(int id, int s){ return !t[id].sons[s] || is_empty(t[id].sons[s]); }
    inline void push_clear(int id){
        if (is_empty(id)){
             for(ll i = 0; i < 2; i++) {
                if (t[id].sons[i]) t[t[id].sons[i]].info = Info();
            }
        }
    }
    inline bool get_bit(const bit_type& x, int b){ return (x>>b)&1; }
    inline void set_bit(bit_type& x, int b){ x |= bit_type(1)<<b; }
public:
    BitTrie(size_t qs=0): t(1) { t.reserve(H*qs); }
    BitTrie& operator^=(bit_type x) { xorr ^= x; return *this; }
    void add(bit_type x, Info add){
        for(int id = 0, lev = H; lev >= -1; lev--){
            push_clear(id);
            t[id].info += add;
            if (lev >= 0) id = to_son(id, get_bit(x, lev));
        }
    }
    pair<Info, bit_type> max_xor(bit_type x, int depth=0){
        bit_type ans{};
        x ^= xorr;
        int id = 0;
        if (!is_empty(id)){
            for(int lev = H; lev >= 0; lev--){
                int bit = ((!get_bit(x, lev) && !empty_son(id, 1)) || empty_son(id, 0));
                if (bit) set_bit(ans, lev);
                id = to_son(id, bit);
                if (lev == depth) break;
            }
        }
        return {t[id].info, ans ^ xorr};
    }
    Info find(bit_type x, int depth=0){
        auto [ans, mx_xor] = max_xor(~x, depth);
        if (((x ^ mx_xor) >> depth) == 0) return ans;
        return Info();
    }
    void set_clear(){ t[0].info = Info(); } // doesn't clear keys
};


class DSU{
    vector<int> par;
public:
    DSU(int n): par(n, -1) { }
    int find(int x){ return par[x] < 0 ? x : par[x] = find(par[x]); }
    bool merge(int a, int b){
        if ((a = find(a)) == (b = find(b))) return false;
        if (par[a] < par[b]) swap(a, b);
        return par[par[a] = b] += par[a], true;
    }
    int count(){
        for(int i = 0; i < (int)par.size(); i++) find(i);
        return std::count_if(all(par), [&](ll x){ return x < 0; });
    }
    int size(int x){ return -par[find(x)]; }
};
