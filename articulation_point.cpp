#include <bits/stdc++.h>

using namespace std;

// constexpr bitset<512> make_connection_judge() {
//     /*
//     This function aids in making a conservative assessment of
//     whether a vertex within a grid is an articulation point,
//     considering only the eight surrounding vertices.

//     0 1 2
//     3 4 5
//     6 7 8
//     */
//     vector<vector<int>> edges(9);
//     for (int v = 0; v < 9; ++v) {
//         if (v / 3 > 0) {
//             edges[v].push_back(v - 3);
//         }
//         if (v % 3 > 0) {
//             edges[v].push_back(v - 1);
//         }
//         if (v % 3 < 3 - 1) {
//             edges[v].push_back(v + 1);
//         }
//         if (v / 3 < 3 - 1) {
//             edges[v].push_back(v + 3);
//         }
//     }
//     bitset<512> is_connected(0);

//     for (uint32_t mask = 1; mask < 512; ++mask) {
//         // Depth First Search
//         int root = countr_zero(mask);
//         uint32_t visited = (1u << root);
//         uint32_t todo = (1u << root);
//         while (todo) {
//             int u = countr_zero(todo);
//             todo ^= (1u << u);
//             for (int v : edges[u]) {
//                 if ((mask & (1u << v)) && (visited & (1u << v)) == 0) {
//                     visited |= (1u << v);
//                     todo |= (1u << v);
//                 }
//             }
//         }
//         is_connected[mask] = (visited == mask);
//     }
//     return is_connected;
// }

// const bitset<512> is_connected = make_connection_judge();

const bitset<512> is_connected_pattern("11111111110111011111101111010001110011111100110110001011000000011111111111011101100000001101000111001111110011010000000000000001111111110000000010000000000000000000000000000000000000000000000011111111110111011000000011010001000000000000000000000000000000011111111111011101100000000000000011001111110011011000101100000001111111111101110100000000000000001100111111001101000000000000000111111111000000001000000000000000110011110000000010001011000000011111111111011101100000001101000111001111110011011000101111011110");

// strict solver
int low_link_for_undirected_grpah(int u, int i, int parent, vector<int>& ord, vector<int>& low, vector<bool>& is_articulation_point, const vector<vector<int>>& edges) {
    assert(ord[u] == -1);
    ord[u] = i++;
    int num_children = 0;
    for (int v : edges[u]) {
        if (ord[v] == -1) {
            // forward edge
            ++num_children;
            i = low_link_for_undirected_grpah(v, i, u, ord, low, is_articulation_point, edges);
            low[u] = min(low[u], low[v]);
            if (ord[u] && ord[u] <= low[v]) {
                is_articulation_point[u] = true;
            }
        } else {
            // backward edge
            if (v != parent && ord[v] < ord[u] - 1) {
                low[u] = min(low[u], ord[v]);
            }
        }
    }
    if (ord[u] == 0 && num_children >= 2) {
        // root
        is_articulation_point[u] = true;
    }
    return i;
}
