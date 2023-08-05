#include <bits/stdc++.h>

using namespace std;

#define ll long long int
#define ull unsigned long long int

struct Input {
    // TODO
};

using Action = int;
using Evaluation = ll;
using Hash = ull;

struct State {
    int depth = 0;

    // TODO

    State(const Input& input) {
        // TODO
    }

    vector<tuple<Action,Evaluation,Hash>> get_actions(Evaluation evaluation, Hash hash) {
        // TODO
    }

    void move_forward(Action action) {
        ++depth;

        // TODO
    }

    void move_backward(Action action) {
        --depth;

        // TODO
    }

    bool finished() {
        // TODO
    }
};

// doubly chained tree

struct Node {
    Action action;
    int parent;
    int child = -1;
    int left; // sibling
    int right = -1; // sibling
    Evaluation evaluation;
    Hash hash;

    Node(Action action, int parent, int left, Evaluation evaluation, Hash hash) :
        action(action),
        parent(parent),
        left(left),
        evaluation(evaluation),
        hash(hash) {}

    void set_right(int v) {
        right = v;
    }

    void set_child(int v) {
        child = v;
    }

    void get_path(vector<Action>& path, vector<Node>& nodes) {
        if (parent == -1) {
            // root
            reverse(path.begin(), path.end());
        } else {
            path.push_back(action);
            nodes[parent].get_path(path, nodes);
        }
    }
};

void remove_node(vector<Node>& nodes, stack<int>& garbage, int v) {
    while (true) {
        assert(nodes[v].child == -1);

        garbage.push(v);
        if (nodes[v].left != -1) {
            nodes[nodes[v].left].right = nodes[v].right;
            if (nodes[v].right != -1) {
                nodes[nodes[v].right].left = nodes[v].left;
            }
            break;
        } else {
            nodes[nodes[v].parent].child = nodes[v].right;
            if (nodes[v].right != -1) {
                nodes[nodes[v].right].left = -1;
                break;
            }
            v = nodes[v].parent;
        }
    }
}

constexpr size_t beam_width = 100; // TODO
constexpr int max_turn = 1000; // TODO
constexpr size_t branches = 4; // TODO
constexpr size_t nodes_size = 10000; // TODO

vector<Action> beam_search(const Input& input) {

    struct Candidate {
        Action action;
        int parent;
        Evaluation evaluation;
        Hash hash;

        Candidate(Action action, int parent, Evaluation evaluation, Hash hash) :
            action(action),
            parent(parent),
            evaluation(evaluation),
            hash(hash) {}
    };

    State state(input);

    vector<Node> nodes;
    nodes.reserve(nodes_size);
    nodes.emplace_back(Node((Action)0, -1, -1, 0, 0));

    stack<int> garbage; // store deleted node indices

    shared_ptr<vector<int>> temp_nodes(new vector<int>);
    temp_nodes->reserve(beam_width);
    temp_nodes->push_back(0);

    shared_ptr<vector<int>> next_nodes(new vector<int>);
    next_nodes->reserve(beam_width);

    vector<Candidate> candidates;
    candidates.reserve(branches * beam_width);

    vector<int> left_memo;
    left_memo.reserve(nodes_size);
    left_memo.push_back(-1);

    vector<int> left_memo_using;
    left_memo_using.reserve(beam_width);

    int root = 0;

    for (int turn = 0; turn < max_turn; ++turn) {
        // go down the direct road
        while (nodes[root].child != -1 && nodes[nodes[root].child].right == -1) {
            root = nodes[root].child;
            state.move_forward(nodes[root].action);
        }
        // Depth First Search
        int v = root;
        while (true) {
            // go to child
            while (nodes[v].child != -1) {
                v = nodes[v].child;
                state.move_forward(nodes[v].action);
            }
            assert(state.depth == turn);

            if (state.finished()) {
                // found the solution
                vector<Action> path;
                path.reserve(turn);
                nodes[v].get_path(path, nodes);
                return path;
            }
            // enumerate children
            for (auto [action, evaluation, hash] : state.get_actions(nodes[v].evaluation, nodes[v].hash)) {
                candidates.emplace_back(Candidate(action, v, evaluation, hash));
            }
            while (v != root && nodes[v].right == -1) {
                // go to parent
                state.move_backward(nodes[v].action);
                v = nodes[v].parent;
            }
            if (v == root) {
                break;
            }
            // go to right sibling
            state.move_backward(nodes[v].action);
            v = nodes[v].right;
            state.move_forward(nodes[v].action);
        }
        // sort candidates
        sort(candidates.begin(), candidates.end(),
            [](const Candidate& a, const Candidate& b) {
                return a.evaluation > b.evaluation;
            }
        );
        if (turn == max_turn - 1) {
            // make the solution
            int v = candidates[0].parent;
            vector<Action> path = {candidates[0].action};
            path.reserve(turn);
            nodes[v].get_path(path, nodes);
            return path;
        }
        // make new nodes
        set<Hash> hashes;
        for (const Candidate& candidate : candidates) {
            if (hashes.count(candidate.hash)) {
                // overlap
                continue;
            }
            int left = left_memo[candidate.parent];
            int v;
            if (garbage.empty()) {
                v = nodes.size();
                nodes.emplace_back(Node(candidate.action, candidate.parent, left, candidate.evaluation, candidate.hash));
                left_memo.push_back(-1);
            } else {
                v = garbage.top();
                garbage.pop();
                nodes[v] = Node(candidate.action, candidate.parent, left, candidate.evaluation, candidate.hash);
            }
            if (left == -1) {
                nodes[candidate.parent].set_child(v);
                left_memo_using.push_back(candidate.parent);
            } else {
                nodes[left].set_right(v);
            }
            left_memo[candidate.parent] = v;
            next_nodes->push_back(v);
            if (next_nodes->size() == beam_width) {
                break;
            }
        }
        for (int v : left_memo_using) {
            left_memo[v] = -1;
        }
        left_memo_using.clear();

        // delete redundant nodes
        for (int v : *temp_nodes) {
            if (nodes[v].child == -1) {
                remove_node(nodes, garbage, v);
            }
        }
        // prepare for next iteration
        swap(temp_nodes, next_nodes);
        next_nodes->clear();
        candidates.clear();
    }
    cerr << "ERROR: unreachable statement" << endl;
    exit(-1);
}
