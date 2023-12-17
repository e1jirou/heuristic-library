#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
#include <atcoder/all>

using namespace std;
using namespace atcoder;

#define ll long long int
#define ull unsigned long long int

using mint = modint998244353;

template<class T>
inline void chmin(T& a, T b) {
    if (a > b) {
        a = b;
    }
}

template<class T>
inline void chmax(T& a, T b) {
    if (a < b) {
        a = b;
    }
}

template <class T>
inline T sum(const vector<T>& data) {
    return accumulate(data.begin(), data.end(), static_cast<T>(0));
}

template <class T>
inline T min(const vector<T>& data) {
    assert(!data.empty());
    return *min_element(data.begin(), data.end());
}

template <class T>
inline T max(const vector<T>& data) {
    assert(!data.empty());
    return *max_element(data.begin(), data.end());
}

template <class T>
inline int argmin(const vector<T>& data) {
    assert(!data.empty());
    return distance(data.begin(), min_element(data.begin(), data.end()));
}

template <class T>
inline int argmax(const vector<T>& data) {
    assert(!data.empty());
    return distance(data.begin(), max_element(data.begin(), data.end()));
}

template <class T>
inline void remove(vector<T>& data, T x) {
    data.erase(remove(data.begin(), data.end(), x), data.end());
}

template <class T>
inline int index(const vector<T>& data, const T x) {
    return distance(data.begin(), find(data.begin(), data.end(), x));
}

template <class T>
inline bool contain(const vector<T>& data, T x) {
    return find(data.begin(), data.end(), x) != data.end();
}

template <class T>
inline int bisect_left(const vector<T>& data, const T x) {
    return distance(data.begin(), lower_bound(data.begin(), data.end(), x));
}

template <class T>
vector<int> argsort(const vector<T>& data) {
    vector<int> ret(data.size());
    iota(ret.begin(), ret.end(), 0);
    sort(ret.begin(), ret.end(), [&](int a, int b){return data[a] < data[b];});
    return ret;
}

inline vector<int> r_permutation(const vector<int>& p) {
    vector<int> ret(p.size());
    for (size_t i = 0; i < p.size(); ++i) {
        ret[p[i]] = i;
    }
    return ret;
}

vector<vector<int>> combinations(int n, int k) {
    if (k == 0) {
        return {{}};
    }
    vector<vector<int>> ret;
    for (int i = k - 1; i < n; i++) {
        vector<vector<int>> sub = combinations(i, k - 1);
        for (vector<int>& comb : sub) {
            comb.push_back(i);
        }
        ret.insert(ret.end(), sub.begin(), sub.end());
    }
    return ret;
}

struct Factorials {
    int n;
    vector<mint> fct;
    vector<mint> rcp;

    Factorials(int n) : n(n), fct(n), rcp(n) {
        fct[0] = 1;
        for (int i = 1; i < n; ++i) {
            fct[i] = i * fct[i - 1];
        }
        rcp[n - 1] = fct[n - 1].inv();
        for (int i = n - 1; i > 0; --i) {
            rcp[i - 1] = i * rcp[i];
        }
    }

    mint comb(int i, int j) {
        assert(0 <= j && j <= i && i < n);
        return fct[i] * rcp[j] * rcp[i - j];
    }

    mint perm(int i, int j) {
        assert(0 <= j && j <= i && i < n);
        return fct[i] * rcp[j];
    }

    mint inv(int i) {
        assert(0 < i && i <= n);
        return fct[i - 1] * rcp[i];
    }
};

vector<ll> divisors(ll n) {
    vector<ll> ret;
    ll d = 1;
    while (d * d < n) {
        if (n % d == 0) {
            ret.push_back(d);
            ret.push_back(n / d);
        }
        ++d;
    }
    if (d * d == n) {
        ret.push_back(d);
    }
    sort(ret.begin(), ret.end());
    return ret;
}

vector<vector<mint>> dot(const vector<vector<mint>>& a, const vector<vector<mint>>& b) {
    assert(a[0].size() == b.size());
    vector<vector<mint>> ret(a.size(), vector<mint>(b[0].size()));
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b[i].size(); ++j) {
            mint tmp = 0;
            for (size_t k = 0; k < b.size(); ++k) {
                tmp += a[i][k] * b[k][j];
            }
            ret[i][j] = tmp;
        }
    }
    return ret;
}

vector<vector<mint>> matrix_power(const vector<vector<mint>>& a, ull exp) {
    assert(a.size() == a[0].size());
    int n = a.size();
    vector<vector<mint>> ret(n, vector<mint>(n, 0));
    for (int i = 0; i < n; ++i) {
        ret[i][i] = 1;
    }
    for (int i = bit_width(exp) - 1; i >= 0; --i) {
        ret = dot(ret, ret);
        if ((exp >> i) & 1) {
            ret = dot(ret, a);
        }
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // TODO

    return 0;
}
