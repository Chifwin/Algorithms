#include "template.cpp"

/*
    AhoCorasick:
        Base for Aho-Corasick.
        Can be ~10% faster with better vector/map container from /misc.
        Example:
            https://vjudge.net/solution/61227035
            MapArray: https://vjudge.net/solution/61253308  921ms 273'600kB
            std::map: https://vjudge.net/solution/61253180 1499ms 272'900kB
    
    Manacher:
        O(n) pre-calc and O(1) palindrome check

    StringHash:
        Rolling hash for strings

    prefix_function
        Example:
            https://codeforces.com/contest/2205/submission/364940721 n=(8000^2)/2 in 200ms
    
    SuffixArray: (existing impl, here for the sake of completeness)
        Computes Suffix array and Longest common prefix of suffixes in O(n*log(n))
        Example:
            https://vjudge.net/solution/61252382
*/

class AhoCorasick{
    struct Node{
        int sufflink = -1;
        map<char, int> next;
    };
public:
    vector<Node> t;
    AhoCorasick(ll n){ t.emplace_back(); t.reserve(n); }
    int add(string s, int root=0){
        for(char c : s){
            if (!t[root].next.count(c)){
                t[root].next[c] = t.size();
                t.emplace_back();
            }
            root = t[root].next[c];
        }
        return root;
    }
    void build(){
        queue<int> q;
        q.push(0);
        while(q.size()){
            int v = q.front(); q.pop();
            for(auto [c, nex] : t[v].next){
                int link = t[v].sufflink;
                while(link != -1 && !t[link].next.count(c)) link = t[link].sufflink;
                t[nex].sufflink = (link == -1 ? 0 : t[link].next[c]);
                q.push(nex);
            }
        }
    }
    void print(int v=0, string s=""){
        dbg(v, s, t[v].sufflink);
        for(auto [c, nex] : t[v].next) print(nex, s + (char)c);
    }
    int next(int v, char c){
        while(!t[v].next[c]){
            if (!v) return v;
            v = t[v].sufflink;
        }
        return t[v].next[c];
    }
};

class Manacher{
    vector<int> d1;
public:
    template<class T=string, class TT=char>
    Manacher(const T& x, TT d){
        T s(x.size()*2, d);
        for(size_t i = 0; i < x.size(); i++) s[2*i] = x[i];
        d1.assign(s.size(), 0);
        int l = 0, r = -1;
        for(int i = 0; i < (int)s.size(); i++){
            int& k = d1[i];
            k = (i > r ? 1 : min(d1[l+r-i], r-i+1));
            while(i+k < sz(s) && i-k >= 0 && s[i+k] == s[i-k]) k++;
            if (i+k-1 > r) l = i-k+1, r = i+k-1;
        }
    }
    int get(int pos, bool odl=true){ return (odl ? d1[pos*2]+1 : d1[pos*2+1])/2; }
    bool is_pal(int l, int r){ int len = r - l + 1; return get((l+r)>>1, len&1)*2 >= len;}
};

template<class T>
vector<int> prefix_function(const T& s) {
    vector<int> pi(s.size());
    for (size_t i = 1; i < s.size(); i++) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j]) j = pi[j-1];
        pi[i] = j + (s[i] == s[j]);
    }
    return pi;
}

template<int mod=1000'000'007, int k=7877>
class StringHash{
    vector<int> prefs;
public:
    StringHash(const string& s=""){
        prefs.resize(s.size());
        for(int i = (int)s.size()-1; i >= 0; i--) prefs[i] = (s[i] + (i == (int)s.size()-1 ? 0 : (long long)k*prefs[i+1]))%mod;
    }
    int get(int l, int r){
        return (prefs[l] - (r == (int)prefs.size()-1 ? 0 : prefs[r+1]*binpow(k, r-l+1, mod)%mod) + mod)%mod;
    }
    int get(){ return get(0, (int)prefs.size()-1); }
};

array<vector<int>, 2> SuffixArray(string& s) { // or basic_string<int>
    int n = sz(s) + 1, k = 0, a, b, lim = *max_element(all(s))+1;
    vector<int> x(all(s)), y(n), ws(max(n, lim)), sa(n), lcp(n);
    x.push_back(0), iota(all(sa), 0);
    for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p) {
        p = j, iota(all(y), n - j);
        fi(n) if (sa[i] >= j) y[p++] = sa[i] - j;
        fill(all(ws), 0);
        fi(n) ws[x[i]]++;
        fio(lim) ws[i] += ws[i - 1];
        for (int i = n; i--;) sa[--ws[x[y[i]]]] = y[i];
        swap(x, y), p = 1, x[sa[0]] = 0;
        fio(n) a = sa[i - 1], b = sa[i], x[b] = (y[a] == y[b] && y[a + j] == y[b + j]) ? p - 1 : p++;
    }
    for (int i = 0, j; i < n - 1; lcp[x[i++]] = k)
        for (k && k--, j = sa[x[i] - 1]; s[i + k] == s[j + k]; k++);
    sa.erase(sa.begin()); lcp.erase(lcp.begin());
    return {sa, lcp};
}
