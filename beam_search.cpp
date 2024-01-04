#include <bits/stdc++.h>
using namespace std;

// メモリの再利用を行いつつ集合を管理するクラス
template<class T>
class ObjectPool {
    public:
        // 配列と同じようにアクセスできる
        T& operator[](int i) {
            return data_[i];
        }

        // 配列の長さを変更せずにメモリを確保する
        void reserve(size_t capacity) {
            data_.reserve(capacity);
        }

        // 要素を追加し、追加されたインデックスを返す
        int push(const T& x) {
            if (garbage_.empty()) {
                data_.push_back(x);
                return data_.size() - 1;
            } else {
                int i = garbage_.top();
                garbage_.pop();
                data_[i] = x;
                return i;
            }
        }

        // 要素を（見かけ上）削除する
        void pop(int i) {
            garbage_.push(i);
        }

        // 使用した最大のインデックス(+1)を得る
        // この値より少し大きい値をreserveすることでメモリの再割り当てがなくなる
        size_t size() {
            return data_.size();
        }

    private:
        vector<T> data_;
        stack<int> garbage_;
};

using Hash = unsigned long long;

// 状態遷移を行うために必要な情報
// メモリ使用量をできるだけ小さくしてください
struct Action {
    // TODO

    Action() {
        // TODO
    }
};

// 状態のコストを比較するための構造体
// メモリ使用量をできるだけ小さくしてください
struct Evaluation {
    // TODO

    Evaluation() {
        // TODO
    }

    // 低いほどよい
    bool operator<(const Evaluation& other) const {
        // TODO
    }
};

// 展開するノードの候補を表す構造体
struct Candidate {
    Evaluation evaluation;
    Action action;
    Hash hash;
    int parent;

    Candidate(Evaluation evaluation, Action action, Hash hash, int parent) :
        evaluation(evaluation),
        action(action),
        hash(hash),
        parent(parent) {}

    bool operator<(const Candidate& other) const {
        return evaluation < other.evaluation;
    }
};

// Selectorの設定
struct SelectorConfig {
    size_t beam_width;
    size_t candidates_capacity;
    bool clear_hashes_by_turn;

    SelectorConfig() {}

    SelectorConfig(size_t beam_width, size_t candidates_capacity, bool clear_hashes_by_turn) :
        beam_width(beam_width),
        candidates_capacity(candidates_capacity),
        clear_hashes_by_turn(clear_hashes_by_turn) {}
};

// 実際に展開する候補を選ぶクラス
// 必要に応じて変更してください（ex. ソートを工夫して高速化する）
class Selector {
    public:
        explicit Selector(const SelectorConfig& selector_config) {
            selector_config_ = selector_config;
            candidates_.reserve(selector_config.candidates_capacity);
            hashes_.reserve(selector_config.beam_width);
        }

        // 候補を追加する
        // ターン数最小化型の問題で、candidateによって実行可能解が得られる場合にのみ finished = true とする
        void push(const Candidate& candidate, bool finished) {
            if (finished) {
                finished_candidates_.push_back(candidate);
            } else {
                candidates_.push_back(candidate);
            }
        }

        // ビーム幅の個数だけ、評価がよいものを選ぶ
        // ハッシュ値が一致したものについては、評価がよいほうのみを残す
        // ソートがボトルネックになる場合がある
        const vector<Candidate>& select() {
            sort(candidates_.begin(), candidates_.end());

            size_t left = 0;
            for (size_t right = 0; right < candidates_.size(); ++right) {
                if (!hashes_.insert(candidates_[right].hash).second) {
                    continue;
                }
                candidates_[left++] = candidates_[right];
                if (left >= selector_config_.beam_width) {
                    break;
                }
            }

            candidates_.erase(candidates_.begin() + left, candidates_.end());

            return candidates_;
        }

        // 実行可能解が見つかったか
        bool have_finished() {
            return !finished_candidates_.empty();
        }

        // 実行可能解に到達する「候補」を返す
        vector<Candidate> get_finished_candidates() {
            return finished_candidates_;
        }

        // 次のイテレーションに向けて準備する
        void clear() {
            candidates_.clear();
            
            if (selector_config_.clear_hashes_by_turn) {
                hashes_.clear();
            }
        }

    private:
        SelectorConfig selector_config_;
        vector<Candidate> candidates_;
        unordered_set<Hash> hashes_;
        vector<Candidate> finished_candidates_;
};

// 深さ優先探索に沿って更新する情報をまとめたクラス
class State {
    public:
        explicit State() {}

        explicit State(/* const Input& input */) {
            // TODO
        }

        // 次の状態候補を全てselectorに追加する
        // 引数
        //   evaluation: 今の評価
        //   hash      : 今のハッシュ値
        //   parent    : 今のノードID（次のノードにとって親となる）
        void expand(const Evaluation& evaluation, Hash hash, int parent, Selector& selector) {
            // TODO
        }

        // actionを実行して次の状態に遷移する
        void move_forward(Action action) {
            // TODO
        }

        // actionを実行する前の状態に遷移する
        // 今の状態は、親からactionを実行して遷移した状態である
        void move_backward(Action action) {
            // TODO
        }

    private:
        // TODO
};

// 探索木（二重連鎖木）のノード
struct Node {
    Action action;
    Evaluation evaluation;
    Hash hash;
    int parent, child, left, right;

    // 根のコンストラクタ
    Node(Action action, const Evaluation& evaluation, Hash hash) :
        action(action),
        evaluation(evaluation),
        hash(hash),
        parent(-1),
        child(-1),
        left(-1),
        right(-1) {}

    // 通常のコンストラクタ
    Node(const Candidate& candidate, int right) :
        action(candidate.action),
        evaluation(candidate.evaluation),
        hash(candidate.hash),
        parent(candidate.parent),
        child(-1),
        left(-1),
        right(right) {}
};

// 二重連鎖木に対する操作をまとめたクラス
class Tree {
    public:
        explicit Tree(const State& state, size_t nodes_capacity, const Node& root) {
            state_ = state;
            nodes_.reserve(nodes_capacity);
            root_ = nodes_.push(root);
        }

        // 状態を更新しながら深さ優先探索を行い、次のノードの候補を全てselectorに追加する
        void dfs(Selector& selector) {
            update_root();

            int v = root_;
            while (true) {
                v = move_to_leaf(v);
                state_.expand(nodes_[v].evaluation, nodes_[v].hash, v, selector);
                v = move_to_ancestor(v);
                if (v == root_) {
                    break;
                }
                v = move_to_right(v);
            }
        }

        // 根からノードvまでのパスを取得する
        vector<Action> get_path(int v) {
            // cerr << nodes_.size() << endl;

            vector<Action> path;
            while (nodes_[v].parent != -1) {
                path.push_back(nodes_[v].action);
                v = nodes_[v].parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // 新しいノードを追加する
        int add_leaf(const Candidate& candidate) {
            int parent = candidate.parent;
            int sibling = nodes_[parent].child;
            int v = nodes_.push(Node(candidate, sibling));

            nodes_[parent].child = v;

            if (sibling != -1) {
                nodes_[sibling].left = v;
            }
            return v;
        }

        // ノードvに子がいなかった場合、vと不要な先祖を削除する
        void remove_if_leaf(int v) {
            if (nodes_[v].child == -1) {
                remove_leaf(v);
            }
        }

        // 最も評価がよいノードを返す
        int get_best_leaf(const vector<int>& last_nodes) {
            assert(!last_nodes.empty());
            int ret = last_nodes[0];
            for (int v : last_nodes) {
                if (nodes_[v].evaluation < nodes_[ret].evaluation) {
                    ret = v;
                }
            }
            return ret;
        }

    private:
        State state_;
        ObjectPool<Node> nodes_;
        int root_;

        // 根から一本道の部分は往復しないようにする
        void update_root() {
            int child = nodes_[root_].child;
            while (child != -1 && nodes_[child].right == -1) {
                root_ = child;
                state_.move_forward(nodes_[child].action);
                child = nodes_[child].child;
            }
        }

        // ノードvの子孫で、最も左にある葉に移動する
        int move_to_leaf(int v) {
            int child = nodes_[v].child;
            while (child != -1) {
                v = child;
                state_.move_forward(nodes_[child].action);
                child = nodes_[child].child;
            }
            return v;
        }

        // ノードvの先祖で、右への分岐があるところまで移動する
        int move_to_ancestor(int v) {
            while (v != root_ && nodes_[v].right == -1) {
                state_.move_backward(nodes_[v].action);
                v = nodes_[v].parent;
            }
            return v;
        }

        // ノードvの右のノードに移動する
        int move_to_right(int v) {
            state_.move_backward(nodes_[v].action);
            v = nodes_[v].right;
            state_.move_forward(nodes_[v].action);
            return v;
        }

        // 不要になった葉を再帰的に削除する
        void remove_leaf(int v) {
            while (true) {
                int left = nodes_[v].left;
                int right = nodes_[v].right;
                if (left == -1) {
                    int parent = nodes_[v].parent;

                    if (parent == -1) {
                        cerr << "ERROR: root is removed" << endl;
                        exit(-1);
                    }
                    nodes_.pop(v);
                    nodes_[parent].child = right;
                    if (right != -1) {
                        nodes_[right].left = -1;
                        return;
                    }
                    v = parent;
                } else {
                    nodes_.pop(v);
                    nodes_[left].right = right;
                    if (right != -1) {
                        nodes_[right].left = left;
                    }
                    return;
                }
            }
        }
};

// ビームサーチの設定
struct BeamSearchConfig {
    int max_turn;
    size_t beam_width;
    size_t nodes_capacity;

    BeamSearchConfig(int max_turn, size_t beam_width, size_t nodes_capacity) :
        max_turn(max_turn),
        beam_width(beam_width),
        nodes_capacity(nodes_capacity) {}
};

// ビームサーチを行う関数
vector<Action> beam_search(const BeamSearchConfig& beam_search_config, const SelectorConfig& selector_config, State state, Node root) {
    Tree tree(state, beam_search_config.nodes_capacity, root);

    vector<int> curr_nodes;
    curr_nodes.reserve(beam_search_config.beam_width);
    // 本来は curr_nodes = {state.root_} とすべきだが, 省略しても問題ない

    vector<int> next_nodes;
    next_nodes.reserve(beam_search_config.beam_width);

    Selector selector(selector_config);

    for (int turn = 0; turn < beam_search_config.max_turn; ++turn) {
        tree.dfs(selector);

        if (selector.have_finished()) {
            // ターン数最小化型の問題で実行可能解が見つかったとき
            Candidate candidate = selector.get_finished_candidates()[0];
            vector<Action> ret = tree.get_path(candidate.parent);
            ret.push_back(candidate.action);
            return ret;
        }
        // 新しいノードを追加する
        for (const Candidate& candidate : selector.select()) {
            next_nodes.push_back(tree.add_leaf(candidate));
        }
        if (next_nodes.empty()) {
            // 新しいノードがないとき
            cerr << "ERROR: Failed to find any valid solution" << endl;
            return {};
        }
        // 不要なノードを削除する
        for (int v : curr_nodes) {
            tree.remove_if_leaf(v);
        }
        // ダブルバッファリングで配列を使い回す
        swap(curr_nodes, next_nodes);
        next_nodes.clear();

        selector.clear();
    }
    // ターン数固定型の問題で全ターンが終了したとき
    int best_leaf = tree.get_best_leaf(curr_nodes);
    return tree.get_path(best_leaf);
}
