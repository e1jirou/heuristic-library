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
inline void remove_all(vector<T>& data, T x) {
    data.erase(remove(data.begin(), data.end(), x), data.end());
}

template <class T>
inline int get_index(const vector<T>& data, const T x) {
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

class Factorials {
    public:
        explicit Factorials(int n) : n_(n), fct_(n), rcp_(n) {
            fct_[0] = 1;
            for (int i = 1; i < n_; ++i) {
                fct_[i] = i * fct_[i - 1];
            }
            rcp_[n_ - 1] = fct_[n_ - 1].inv();
            for (int i = n_ - 1; i > 0; --i) {
                rcp_[i - 1] = i * rcp_[i];
            }
        }

        mint fact(int i) const {
            assert(0 <= i && i < n_);
            return fct_[i];
        }

        mint fact_inv(int i) const {
            assert(0 <= i && i < n_);
            return rcp_[i];
        }

        mint comb(int i, int j) const {
            assert(0 <= j && j <= i && i < n_);
            return fct_[i] * rcp_[j] * rcp_[i - j];
        }

        mint perm(int i, int j) const {
            assert(0 <= j && j <= i && i < n_);
            return fct_[i] * rcp_[j];
        }

        mint inv(int i) const {
            assert(0 < i && i <= n_);
            return fct_[i - 1] * rcp_[i];
        }

    private:
        int n_;
        vector<mint> fct_;
        vector<mint> rcp_;
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

template <class S, S (*op)(S, S), S (*e)()>
class SlidingWindowAggregation {
    public:
        explicit SlidingWindowAggregation() {
            former_.push_back(e());
            latter_.push_back(e());
        }

        size_t size() const {
            return former_.size() + latter_.size() - 2;
        }

        S get() const {
            return op(former_.back(), latter_.back());
        }

        void push(S x) {
            data_.push_back(x);
            latter_.push_back(op(latter_.back(), x));
        }

        void pop() {
            if (former_.size() == 1) {
                assert(latter_.size() >= 2);
                reconstruct();
            } else {
                former_.pop_back();
            }
        }

    private:
        vector<S> data_;
        vector<S> former_;
        vector<S> latter_;

        void reconstruct() {
            for (int i = data_.size() - 1; i > 0; --i) {
                former_.push_back(op(former_.back(), data_[i]));
            }
            data_.clear();
            latter_.erase(latter_.begin() + 1, latter_.end());
        }
};

template <class S, S (*rooting)(S), S (*op)(S, S), S (*e)()>
class Rerooting {
    public:
        explicit Rerooting(int n) {
            n_ = n;
            edges_.resize(n_);
        }

        void add_edge(int u, int v) {
            assert(0 <= u && u < n_);
            assert(0 <= v && v < n_);
            edges_[u].push_back(v);
            edges_[v].push_back(u);
        }

        vector<S> rerooting(int root=0) {
            dfs(root);
            bottom_up();
            return top_down();
        }

    private:
        int n_;
        vector<vector<int>> edges_;
        vector<vector<int>> children_;
        vector<int> order_;
        vector<vector<S>> forward_;
        vector<vector<S>> backward_;

        void dfs(int root) {
            vector<bool> visited(n_, false);
            visited[root] = true;

            order_ = {root};
            order_.reserve(n_);
            size_t head = 0;

            children_ = vector<vector<int>>(n_);

            while (head < order_.size()) {
                int u = order_[head++];
                children_[u].reserve(edges_[u].size());
                for (int v : edges_[u]) {
                    if (visited[v]) {
                        continue;
                    }
                    visited[v] = true;
                    children_[u].push_back(v);
                    order_.push_back(v);
                    
                }
            }
            assert(static_cast<int>(head) == n_);
        }

        void bottom_up() {
            vector<S> dp(n_, e());
            forward_ = vector<vector<S>>(n_);
            backward_ = vector<vector<S>>(n_);

            for (int i = n_ - 1; i >= 0; --i) {
                int v = order_[i];
                const vector<int>& children_v = children_[v];

                vector<S>& forward_v = forward_[v];
                forward_v.resize(children_v.size() + 1, e());
                for (size_t j = 0; j < children_v.size(); ++j) {
                    forward_v[j + 1] = op(forward_v[j], dp[children_v[j]]);
                }
                vector<S>& backward_v = backward_[v];
                backward_v.resize(children_v.size() + 1, e());
                for (size_t j = children_v.size(); j > 0; --j) {
                    backward_v[j - 1] = op(dp[children_v[j - 1]], backward_v[j]);
                }
                assert(forward_v.back() == backward_v.front());
                dp[v] = rooting(backward_v[0]);
            }
        }

        vector<S> top_down() const {
            vector<S> ret(n_, e());
            vector<S> dp(n_, e());
            for (int v : order_) {
                const vector<S>& forward_v = forward_[v];
                const vector<S>& backward_v = backward_[v];
                const vector<int>& children_v = children_[v];
                S dp_v = dp[v];
                ret[v] = rooting(op(dp_v, backward_v[0]));
                for (size_t i = 0; i < children_v.size(); ++i) {
                    dp[children_v[i]] = rooting(op(dp_v, op(forward_v[i], backward_v[i + 1])));
                }
            }
            return ret;
        }
};

class LowestCommonAncestor {
    public:
        explicit LowestCommonAncestor(int n, const vector<vector<int>>& edges, int root) {
            assert(static_cast<int>(edges.size()) == n);
            assert(0 <= root && root < n);

            n_ = n;
            vector<int> parents = dfs(edges, root);
            doubling(parents);
        }

        int get_depth(int v) const {
            assert(0 <= v && v < n_);
            return depths_[v];
        }

        int lca(int u, int v) const {
            if (depths_[u] > depths_[v]) {
                swap(u, v);
            }
            uint32_t d = depths_[v] - depths_[u];
            while (d) {
                int i = countr_zero(d);
                v = table_[i][v];
                d ^= (1u << i);
            }
            if (u == v) {
                return v;
            }
            for (int i = bit_width(static_cast<uint32_t>(depths_[v])) - 1; i >= 0; --i) {
                if (table_[i][u] != table_[i][v]) {
                    u = table_[i][u];
                    v = table_[i][v];
                }
            }
            return table_[0][v];
        }

    private:
        int n_;
        vector<int> depths_;
        vector<vector<int>> table_;

        vector<int> dfs(const vector<vector<int>>& edges, int root) {
            depths_ = vector<int>(n_, -1);
            depths_[root] = 0;

            vector<int> parents(n_, -1);

            stack<int> todo;
            todo.push(root);

            while (!todo.empty()) {
                int u = todo.top();
                todo.pop();
                for (int v : edges[u]) {
                    if (depths_[v] == -1) {
                        depths_[v] = depths_[u] + 1;
                        parents[v] = u;
                        todo.push(v);
                    }
                }
            }
            return parents;
        }

        void doubling(const vector<int>& parents) {
            uint32_t max_depth = *max_element(depths_.begin(), depths_.end());
            table_ = vector<vector<int>>(bit_width(max_depth), vector<int>(n_, -1));
            table_[0] = parents;

            for (size_t i = 1; i < table_.size(); ++i) {
                const vector<int>& fr = table_[i - 1];
                vector<int>& to = table_[i];
                for (int v = 0; v < n_; ++v) {
                    if (fr[v] != -1) {
                        to[v] = fr[fr[v]];
                    }
                }
            }
        }
};

int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    cout << fixed << setprecision(10);

    // TODO

    return 0;
}
