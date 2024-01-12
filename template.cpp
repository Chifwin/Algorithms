#include <bits/stdc++.h>

// #pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#define ll long long
#define ull unsigned long long
#define ld long double
#define vll vector<ll>
#define pll pair<ll, ll>
#define plll pair<pll, ll>
#define lpll pair<ll, pll>
#define FOR(i, start, stop) for(ll i = (start); i < (stop); i++)
#define fi(n)  FOR(i, 0, n)
#define fj(n)  FOR(j, 0, n)
#define fk(n)  FOR(k, 0, n)
#define fio(n) FOR(i, 1, n)
#define fjo(n) FOR(j, 1, n)
#define fko(n) FOR(k, 1, n)
#define fiv(n) for (ll i = n-1; i >= 0; i--)
#define fjv(n) for (ll j = n-1; j >= 0; j--)
#define fkv(n) for (ll k = n-1; k >= 0; k--)
#define f first
#define s second
#define pb push_back
#define rt return
#define cn continue;
#define br break;
#define sz(x) ((ll)x.size())
#define all(queries) begin(queries),end(queries)
#define rall(queries) (queries).rbegin(),(queries).rend()
#define YESNO(flag) cout<<(flag ? "YES" : "NO") <<'\n';
#define dbg(...) cerr << "\033[33m" << (#__VA_ARGS__) << ": "; dbg_vals(__VA_ARGS__); cerr << "\033[0m\n";
#define IOS ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

using namespace std;

template<class T> istream& operator>>(istream& in, vector<T>& x){for(T& i : x) in >> i; return in; }
template<class T, class TT> istream& operator>>(istream& in, pair<T, TT>& x){ in >> x.f >> x.s; return in; }
template<class T, class TT> ostream& operator<<(ostream& out, const pair<T, TT>& x){ out << x.f << ' ' << x.s; return out; }
template <typename Iter, typename=decltype(declval<Iter>().begin()), typename=enable_if_t<!is_convertible_v<Iter, string>>> ostream& operator<<(ostream& out, const Iter& x){ for (const auto& i : x) out << i << ' '; return out;}

template<typename T> void dbg_vals(T x){ cerr<< "\033[31m" << x << "\033[0m"; }
template<typename T, typename... Args> void dbg_vals(T x, Args... args){ dbg_vals(x); cerr << " | "; dbg_vals(args...); }

struct custom_hash { static uint64_t splitmix64(uint64_t x) {/*http://xorshift.di.unimi.it/splitmix64.c*/ x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9; x = (x ^ (x >> 27)) * 0x94d049bb133111eb; return x ^ (x >> 31);} size_t operator()(uint64_t x) const { static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();return splitmix64(x + FIXED_RANDOM);}};
template<class T> using ll_umap = unordered_map<ll, T, custom_hash>;
using ll_uset = unordered_set<ll, custom_hash>;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
template<class T, class comp=less<T>> using ordered_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, comp, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
// find_by_order(k) - iterator on k's elements
// order_of_key(x) - number of elements, strictly less than x

template<class T> using min_pq = priority_queue<T, vector<T>, greater<T>>;

const ll MAXN = 2123456;
const ll MOD = 1000'000'007;
const ld EPS = 1e-10;
const ll INF = (ll)(1ll<<60) + (ll)((1ll<<31)-1);
const ld PI = atanl(1)*4;

ll rll(ll r=LLONG_MAX, ll l=0){ /*random long long*/ static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count()); return uniform_int_distribution<ll>(l, r)(rng); }
array<ll, 3> extgcd(ll a, ll b) { if (!b) return {a, 1, 0}; auto [d, y, x] = extgcd(b, a % b); return {d, x, y - a/b * x}; } // {gcd, x, y} such that a*x + b*y = gcd(a, b)
ll gcd(ll x, ll y){ return __gcd(x, y); }
ll binpow(ll x, ll p, ll mod){ if (p < 0) return 0; x %= mod; ll ans = 1; while(p){ if (p&1) ans = (ans*x)%mod; x = (x*x)%mod; p >>= 1; } return ans; }
ll invmod(ll x, ll mod=MOD){ return binpow(x, mod-2, mod); }
ll sign(ll x){ if (x < 0) rt -1; rt !!x; }
void FREOPEN(string s){ freopen(string(s + ".in").c_str(), "r", stdin); freopen(string(s + ".out").c_str(), "w", stdout); }
template<class T, class TT> void mineq(T& a, const TT& b){ if (b < a) a = b;}
template<class T, class TT> void maxeq(T& a, const TT& b){ if (b > a) a = b;}
const pll dxy[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
const pll dxy8[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

ll n, m, k, q;
ll ar[MAXN];

void solve(){
}

int main(){
    IOS;
    cout << fixed << setprecision(15);
    ll _ = 1;
    cin >> _; 
    while(_--) {
        // static ll CASE = 1; cout << "Case #" << CASE++ << ": ";
        solve();
    }
    return 0;
}