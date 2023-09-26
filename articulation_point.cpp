#include <bits/stdc++.h>

using namespace std;

template <int n>
constexpr array<uint32_t, (1u << (n * n)) / 32> make_connection_judge() {
    /*
    This function aids in making a conservative assessment of
    whether a vertex within a grid is an articulation point,
    considering only the eight surrounding vertices.

    0 1 2
    3 4 5
    6 7 8
    */
    vector<vector<int>> edges(n * n);
    for (int v = 0; v < n * n; ++v) {
        if (v / n > 0) {
            edges[v].push_back(v - n);
        }
        if (v % n > 0) {
            edges[v].push_back(v - 1);
        }
        if (v % n < n - 1) {
            edges[v].push_back(v + 1);
        }
        if (v / n < n - 1) {
            edges[v].push_back(v + n);
        }
    }
    array<uint32_t, (1u << (n * n)) / 32> is_connected;
    fill(is_connected.begin(), is_connected.end(), 0);

    for (uint32_t mask = 1; mask < (1u << (n * n)); ++mask) {
        // Depth First Search
        int root = countr_zero(mask);
        uint32_t visited = (1u << root);
        uint32_t todo = (1u << root);
        while (todo) {
            int u = countr_zero(todo);
            todo ^= (1u << u);
            for (int v : edges[u]) {
                if ((mask & (1u << v)) && (visited & (1u << v)) == 0) {
                    visited |= (1u << v);
                    todo |= (1u << v);
                }
            }
        }
        if (mask == visited) {
            is_connected[mask / 32] |= (1u << (mask % 32));
        }
    }
    return is_connected;
}

constexpr array<uint32_t, (1u << (3 * 3)) / 32> is_connected = make_connection_judge<3>();
