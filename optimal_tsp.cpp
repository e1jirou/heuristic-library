#include <bits/stdc++.h>
using namespace std;

vector<int> tsp_solver(vector<vector<int>> cost_matrix) {
    // calculate the TSP optimal solution
    // start from n and end to n
    int n = cost_matrix.size() - 1;
    int s = n;
    int inf = 1 << 30;

    // dp
    vector<vector<int>> dp(n, vector<int>(1 << n,inf));
    for (int i = 0; i < n; ++i) {
        dp[i][1 << i] = cost_matrix[s][i];
    }
    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int i = 0; i < n; ++i) {
            if (((mask >> i) & 1) == 0) {
                continue;
            }
            for (int j = 0; j < n; ++j) {
                if ((mask >> j) & 1) {
                    continue;
                }
                dp[j][mask | (1 << j)] = min(dp[j][mask | (1 << j)], dp[i][mask] + cost_matrix[i][j]);
            }
        }
    }
    // calculate the optimal value
    int last = 0;
    int cost = inf;
    for (int i = 0; i < n; ++i) {
        if (dp[i].back() + cost_matrix[i][s] < cost) {
            last = i;
            cost = dp[i].back() + cost_matrix[i][s];
        }
    }
    // restore the path
    vector<int> ret = {last};
    int mask = (1 << n) - 1;
    mask ^= 1 << last;
    while (mask) {
        int j = ret.back();
        for (int i = 0; i < n; ++i) {
            if (((mask >> i) & 1) && dp[i][mask] + cost_matrix[i][j] == dp[j][mask ^ (1 << j)]) {
                ret.push_back(i);
                mask ^= 1 << i;
                break;
            }
        }
        assert(ret.back() != j);
    }
    ret.push_back(s);
    reverse(ret.begin(), ret.end());
    return ret;
}
