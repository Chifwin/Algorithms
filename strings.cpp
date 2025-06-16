#include "template.cpp"

/*
    AhoCorasick:
        Base for Aho-Corasick.
        Can be ~10% faster with better vector/map container from /misc.
        Example:
            https://vjudge.net/solution/61227035
    
    Manacher:
        O(n) pre-calc and O(1) palindrome check

    StringHash:
        Rolling hash for strings
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
        dbg(v, s, t[v].sufflink)
        for(auto [c, nex] : t[v].next) print(nex, s + (char)c);
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
