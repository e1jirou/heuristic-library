#include <bits/stdc++.h>

using namespace std;

// vector<int> edge3x3(int v) {
//     vector<int> ret;
//     if (v >= 3) {
//         ret.push_back(v - 3);
//     }
//     if (v % 3 >= 1) {
//         ret.push_back(v - 1);
//     }
//     if (v % 3 <= 1) {
//         ret.push_back(v + 1);
//     }
//     if (v <= 5) {
//         ret.push_back(v + 3);
//     }
//     return ret;
// }

// bitset<512> make_connection_judge() {
//     /*
//     This function aids in making a conservative assessment of
//     whether a vertex within a grid is an articulation point,
//     considering only the eight surrounding vertices.

//     0 1 2
//     3 4 5
//     6 7 8
//     */
//     bitset<512> is_connected;
//     for (uint32_t i = 1; i < 512; ++i) {
//         const bitset<9> mask(i);
//         // Depth First Search
//         int root = countr_zero(i);
//         bitset<9> visited(0);
//         visited[root] = true;
//         stack<int> todo;
//         todo.push(root);
//         while (!todo.empty()) {
//             int u = todo.top();
//             todo.pop();
//             for (int v : edge3x3(u)) {
//                 if (mask[v] && !visited[v]) {
//                     visited[v] = true;
//                     todo.push(v);
//                 }
//             }
//         }
//         is_connected[i] = (mask == visited);
//     }
//     return is_connected;
// }

// const bitset<512> is_connected = make_connection_judge();

const bitset<512> is_connected("11111111110111011111101111010001110011111100110110001011000000011111111111011101100000001101000111001111110011010000000000000001111111110000000010000000000000000000000000000000000000000000000011111111110111011000000011010001000000000000000000000000000000011111111111011101100000000000000011001111110011011000101100000001111111111101110100000000000000001100111111001101000000000000000111111111000000001000000000000000110011110000000010001011000000011111111111011101100000001101000111001111110011011000101111011110");
