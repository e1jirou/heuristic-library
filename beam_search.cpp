#include <bits/stdc++.h>

using namespace std;

#define ll long long int
#define ull unsigned long long int

using Action = ull; // TODO

struct Input {
    // TODO
};

struct State {
    // TODO

    State(const Input& input) {
        // TODO
    }

    vector<pair<Action,ll>> get_actions() {
        // TODO
    }

    ll evaluate() {
        // TODO
    }

    void forward(Action a) {
        // TODO
    }

    void backward(Action a) {
        // TODO
    }

    bool finished() {
        // TODO
    }
};

struct Node {
    shared_ptr<Node> parent;
    Action parent_action;
    ll evaluation;
    int depth;
    bool expanded;
    int child_index;
    vector<pair<Action,weak_ptr<Node>>> children;

    Node(shared_ptr<Node> parent, Action parent_action, State& state): parent(parent), parent_action(parent_action) {
        state.forward(parent_action);
        evaluation = state.evaluate();
        state.backward(parent_action);

        depth = (parent == nullptr) ? 0 : (parent->depth + 1);
        expanded = false;
        child_index = 0;
    }

    void add_child(Action action, weak_ptr<Node> child) {
        children.push_back({action, child});
    }

    void get_path(vector<Action>& path) {
        if (parent == nullptr) {
            // root
            reverse(path.begin(), path.end());
        } else {
            path.push_back(parent_action);
            parent->get_path(path);
        }
    }
};

bool operator<(shared_ptr<Node> a, shared_ptr<Node> b) {
    return -a->evaluation < -b->evaluation;
}

constexpr int beam_width = 100; // TODO
constexpr int max_turn = 1000; // TODO

vector<Action> beam_search(const Input& input) {
    State state(input);

    shared_ptr<Node> root(new Node(nullptr, 0, state));
    priority_queue<shared_ptr<Node>> temp_nodes; // If you want to ensure diversity, it might be better to use multiset<priority_queue<shared_ptr<Node>>>
    temp_nodes.push(root);

    for (int turn = 0; turn < max_turn; ++turn) {
        priority_queue<shared_ptr<Node>> next_nodes;

        // Depth First Search
        shared_ptr<Node> node = root;
        while (true) {
            if (node->depth == turn && !node->expanded) {
                if (state.finished()) {
                    // found the solution
                    vector<Action> path;
                    node->get_path(path);
                    return path;
                }
                // expand leaf
                node->expanded = true;
                // add next nodes
                for (auto [action, evaluation] : state.get_actions()) {
                    if ((int)next_nodes.size() == beam_width) {
                        if (evaluation < next_nodes.top()->evaluation) {
                            // do not add the node
                            continue;
                        } else {
                            // pop the worst node
                            next_nodes.pop();
                        }
                    }
                    // push the new node
                    next_nodes.push(shared_ptr<Node>(new Node(node, action, state)));
                }
            } else if (node->depth < turn) {
                while (node->child_index < (int)node->children.size() && node->children[node->child_index].second.expired()) {
                    // erase the deleted child
                    node->children.erase(node->children.begin() + node->child_index);
                }
                if (node->child_index < (int)node->children.size()) {
                    // move to the child
                    state.forward(node->children[node->child_index].first);
                    node = node->children[node->child_index++].second.lock();
                    continue;
                }
            } else {
                node->child_index = 0;
                if (node->parent == nullptr) {
                    // root
                    break;
                } else {
                    // move to the parent
                    state.backward(node->parent_action);
                    node = node->parent;
                }
            }
        }
        temp_nodes = next_nodes;
    }
}
