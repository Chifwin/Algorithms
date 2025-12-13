#include "template.cpp"

/*
    BitTrie
        Efficient binary trie. It represents key-value map for three main operations:
            1) XOR all keys with some value in O(1) (operator^=)
            2) Get key-value pair such that key^value is maximum in O(H)
            3) Add key-value pair to map in O(H)
        Examples:
            https://codeforces.com/contest/842/submission/277089370
    DSU
        Just short fast DSU
    Vec
        std::array with std::vector interface (mostly)
    Assotiative containers over arrays, faster for small collections than std::map. Useful for Aho-Corasick
        MapVector
            std::vector wrapper with basic std::map interface
        MapArray<V, size>
            C-array wrapper with basic std::map<int, V> interface, when 0 <= keys < size
    Comb:
        Factorials and inverse factorials for O(1) C(n, k)
    Interval_Map:
        std::map wrapper with operations:
            1) assign(l, r, val): set 'val' on array [l, r)
                At most one O(log) operation + amortized O(1) // it was insanely hard
            2) get value in point l in O(log)
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
             for(int i = 0; i < 2; i++) {
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
        return std::count_if(all(par), [&](int x){ return x < 0; });
    }
    int size(int x){ return -par[find(x)]; }
};

template<class T, int len>
class Vec{
    array<T, len> ar;
    int s = 0;
public:
	Vec(initializer_list<T> il) { copy(il.begin(), il.end(), ar.begin()); s = il.size(); }
	Vec() {}
    void push_back(const T& x){ ar[s++] = x; }
    void pop_back(){ s--; }
    template<typename... Args> void emplace_back(Args... args){ ar[s++] = T(args...); }
    void clear(){ s = 0; }
    T* begin(){ return ar.begin(); }
    T* end(){ return ar.begin()+s; }
    const T* begin() const { return ar.begin(); }
    const T* end() const { return ar.begin()+s; }
    int size() const { return s; }
	bool empty() const { return !s; }
    T& operator[](int i){ return ar[i]; }
    T operator[](int i)const{ return ar[i]; }
    void reserve(int x){ assert(x <= len); }
    friend operator<(const Vec<T, len>& a, const Vec<T, len>& b){
        for(ll i = 0; i < min(a.size(), b.size()); i++) if (a[i] != b[i]) return a[i] < b[i];
        return a.size() < b.size();
    }
};

template<class K, class V>
class MapVector{
    vector<pair<K, V>> ar;
    int find(K x){
        for(int i = 0; i < (int)ar.size(); i++) if (ar[i].f == x) return i;
        return -1;
    }
public:
    V& operator[](K x){
        int i = find(x);
        if (i != -1) return ar[i].s;
        ar.emplace_back(x, V());
        return ar.back().s;
    }
    int count(int x){ return find(x) != -1; }
    auto begin(){ return ar.begin(); }
    auto end(){ return ar.end(); }
    auto begin() const { return ar.cbegin(); }
    auto end() const { return ar.cend(); }
    void erase(K x){
        int i = find(x);
        if (i != -1) {
            swap(ar[i], ar.back());
            ar.pop_back();
        }
    }
    int size(){ return ar.size(); }
};

template<class V, int N>
class MapArray{
    V ar[N];
    struct Iter{
        int ind;
        const MapArray &par;
        Iter operator++(){ do ind++; while(ind < N && par.ar[ind] == V()); return *this; }
        pair<int, V> operator*(){ assert(ind >= 0 && ind < N && par.ar[ind] != V()); return {ind, par.ar[ind]}; }
        friend bool operator!=(Iter l, Iter r) {return l.ind != r.ind; }
    };
public:
    MapArray(){ fill(ar, ar+N, V()); }
    V& operator[](int x){ return ar[x]; }
    int count(int x){ return ar[x] != V(); }
    const Iter begin() const { return ++Iter{-1, *this}; }
    const Iter end() const { return Iter{N, *this}; }
    void erase(int x){ ar[x] = V(); }
    int size(){ return count_if(ar, ar+N, [&](V v){ return v != V(); }); }
};

template <class Node, int K=20>
class Sparse{
    static_assert(MAXN < (1<<K));
    Node vals[K][MAXN];
    int log2_floor(int i) {
        return i ? __builtin_clz(1) - __builtin_clz(i) : -1;
    }
public:
    template<class T>
    Sparse(const T ar, int n){
        for(int i = 0; i < n; i++) vals[0][i] = Node(ar[i], i);
        for(int i = 1; i < K; i++){
            for(int j = 0; j + (1ll<<i) <= n; j++){
                vals[i][j] = vals[i-1][j] + vals[i-1][j + (1ll<<(i-1))];
            }
        }
    }
    Node get(int l, int r){
        int i = log2_floor(r-l+1);
        return vals[i][l] + vals[i][r-(1ll<<i)+1];
    }
};

template<int maxn, int mod>
struct Comb{
    int fac[maxn]{1}, invfac[maxn];
    Comb(){
        for(long long i = 1; i < maxn; i++) fac[i] = fac[i-1] * i % mod;
        invfac[maxn-1] = invmod(fac[maxn-1], mod);
        for(long long i = maxn-1; i > 0; i--) invfac[i-1] = invfac[i] * i % mod;
    }
    inline long long C(int n, int k){
        if (n < 0 || k > n || k < 0) return 0;
        assert(n < maxn && k < maxn);
        return (long long)fac[n] * invfac[k] % mod * invfac[n-k] % mod;
    }
	inline long long sab_non_empty(int n, int bins){ return C(n-1, bins-1);}
	inline long long sab(int n, int bins){ return C(n+bins-1, bins-1);}
}; Comb<MAXN, MOD> comb;

template<typename K, typename V>
class Interval_Map {
	V m_valBegin;
	std::map<K, V> m_map;
public:
	Interval_Map(V const& val) : m_valBegin(val) {}
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) { // [keyBegin, keyEnd)
	    if (!(keyBegin < keyEnd)) return;
        auto it = m_map.lower_bound(keyEnd);
        if (it == m_map.end() || keyEnd < it->first){
            // Add cut piece by right border
            if (it == m_map.begin()){
                if (!(val == m_valBegin)) it = m_map.emplace_hint(it, keyEnd, m_valBegin);
            }else{
                auto pit = std::prev(it);
                if (!(val == pit->second)) {
                    if (!(pit->first < keyBegin)){
                        // Change start of the previous interval in amortized O(1) without reallocation
                        auto node = m_map.extract(pit);
                        node.key() = keyEnd;
                        it = m_map.insert(it, std::move(node));
                    }else it = m_map.emplace_hint(it, keyEnd, pit->second);
                }
            }
        }
        while(it != m_map.begin() && !((--it)->first < keyBegin)) it = m_map.erase(it);
        if (it != m_map.end()){
            if (it->first < keyBegin) it++;
            if (it != m_map.end() && it->second == val) it = m_map.erase(it); 
            if (it == m_map.begin()){
                if (!(val == m_valBegin)) m_map.emplace_hint(it, keyBegin, val);
            }else if (!(val == prev(it)->second)) m_map.emplace_hint(it, keyBegin, val);
        }
	}
	V const& operator[]( K const& key ) const {
		auto it = m_map.upper_bound(key);
		if(it == m_map.begin()) return m_valBegin;
        return (--it)->second;
	}
};
