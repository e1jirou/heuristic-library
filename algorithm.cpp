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

int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // TODO

    return 0;
}
