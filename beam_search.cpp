#include <bits/stdc++.h>

using namespace std;

#define ll long long int
#define ull unsigned long long int

using Action = int; // TODO
using Evaluation = ll; // TODO
using Hash = ull; // TODO

struct State {
    int depth = 0;

    // TODO

    State() {
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

constexpr size_t beam_width = 100; // TODO
constexpr int max_turn = 1000; // TODO
constexpr size_t branches = 4; // TODO
constexpr size_t nodes_size = 10000; // TODO

pair<int,vector<Action>> beam_search_on_forest(const vector<pair<State,Node>>& roots) {

    struct Candidate {
        Action action;
        int parent;
        Evaluation evaluation;
        Hash hash;
        int tree_id;

        Candidate(Action action, int parent, Evaluation evaluation, Hash hash, int tree_id) :
            action(action),
            parent(parent),
            evaluation(evaluation),
            hash(hash),
            tree_id(tree_id) {}
    };

    struct Tree {
        State state;
        int id;
        int root;
        bool valid = true;

        Tree(const State& state, int id) :
            state(state),
            id(id),
            root(id) {}

        pair<int,vector<Action>> dfs(vector<Node>& nodes, int turn, vector<Candidate>& candidates) {
            if (!valid) {
                return {-1, {}};
            }
            // go down the direct road
            while (nodes[root].child != -1 && nodes[nodes[root].child].right == -1) {
                root = nodes[root].child;
                state.move_forward(nodes[root].action);
            }
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
                    return {id, path};
                }
                // enumerate children
                for (auto [action, evaluation, hash] : state.get_actions(nodes[v].evaluation, nodes[v].hash)) {
                    candidates.emplace_back(Candidate(action, v, evaluation, hash, id));
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
            return {-1, {}};
        }
    };

    vector<Tree> forest;
    forest.reserve(roots.size());

    vector<Node> nodes;
    nodes.reserve(nodes_size);

    stack<int> garbage; // store deleted node indices

    shared_ptr<vector<int>> temp_nodes(new vector<int>);
    temp_nodes->reserve(beam_width);

    shared_ptr<vector<int>> next_nodes(new vector<int>);
    next_nodes->reserve(beam_width);

    vector<Candidate> candidates;
    candidates.reserve(branches * beam_width);

    vector<int> left_memo;
    left_memo.reserve(nodes_size);

    vector<int> left_memo_using;
    left_memo_using.reserve(beam_width);

    unordered_set<Hash> hashes;

    for (size_t id = 0; id < roots.size(); ++id) {
        forest.emplace_back(Tree(roots[id].first, id));
        nodes.emplace_back(roots[id].second);
        temp_nodes->push_back(id);
        left_memo.push_back(-1);
    }
    for (int turn = 0; turn < max_turn; ++turn) {
        // depth first search on each tree
        pair<int,vector<Action>> ret = {-1, {}};
        for (Tree& tree : forest) {
            pair<int,vector<Action>> path = tree.dfs(nodes, turn, candidates);
            if (path.first != -1 && (ret.first == -1 || path.second.size() < ret.second.size())) {
                ret = path;
            }
        }
        if (ret.first != -1) {
            // cerr << nodes.size() << endl;
            return ret;
        }
        // sort candidates
        std::sort(candidates.begin(), candidates.end(),
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
            // cerr << nodes.size() << endl;
            return {candidates[0].tree_id, path};
        }
        // make new nodes
        for (const Candidate& candidate : candidates) {
            if (!hashes.insert(candidate.hash).second) {
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
        hashes.clear();

        for (int v : left_memo_using) {
            left_memo[v] = -1;
        }
        left_memo_using.clear();

        // delete redundant nodes
        for (int v : *temp_nodes) {
            if (nodes[v].child != -1) {
                continue;
            }
            // delete v
            while (true) {
                assert(nodes[v].child == -1);
                if (nodes[v].parent == -1) {
                    // delete root
                    assert(v < (int)forest.size());
                    forest[v].valid = false;
                    break;
                }
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
        // prepare for next iteration
        swap(temp_nodes, next_nodes);
        next_nodes->clear();
        candidates.clear();
    }
    cerr << "ERROR: unreachable statement" << endl;
    std::exit(-1);
}
