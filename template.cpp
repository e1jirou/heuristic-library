#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
// #include <atcoder/all>

using namespace std;
// using namespace atcoder;

#define ll long long int
#define ull unsigned long long int

class Xorshift {
    public:
        Xorshift(uint32_t seed): x(seed) {
            assert(seed);
        }

        uint32_t randrange(uint32_t end) {
            next();
            return x % end;
        }

        uint32_t randrange(uint32_t begin, uint32_t end) {
            next();
            return begin + x % (end - begin);
        }

        double uniform() {
            next();
            return static_cast<double>(x) / static_cast<double>(UINT32_MAX);
        }

    private:
        uint32_t next() {
            x ^= x << 13;
            x ^= x >> 7;
            x ^= x << 17;
        }

        uint32_t x;
};

class IndexSet {
    public:
        IndexSet(int n) : n_(n), positions_(n, -1) {}

        void add(int x) {
            assert(0 <= x && x < n_);
            assert(!contains(x));
            positions_[x] = data_.size();
            data_.push_back(x);
        }

        void remove(int x) {
            assert(0 <= x && x < n_);
            assert(contains(x));
            int pos = positions_[x];
            int y = data_.back();
            data_[pos] = y;
            data_.pop_back();
            positions_[y] = pos;
            positions_[x] = -1;
        }

        bool contains(int x) const {
            assert(0 <= x && x < n_);
            return positions_[x] != -1;
        }

        int choice(Xorshift& engine) const {
            assert(!data_.empty());
            return data_[engine.randrange(data_.size())];
        }

        vector<int> all_data() const {
            return data_;
        }

        int size() const {
            return data_.size();
        }

    private:
        int n_;
        vector<int> data_;
        vector<int> positions_;
};

class Timer {
    public:
        void begin() {
            start_time_ = chrono::system_clock::now();
        }

        double get_time() const {
            chrono::system_clock::time_point end_time = chrono::system_clock::now();
            double elapsed_time = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time_).count();
            elapsed_time *= 1e-9; // nanoseconds -> seconds
            return elapsed_time;
        }

        bool yet(double time_limit) const {
            return get_time() < time_limit;
        }

        double progress(double time_limit) const {
            return get_time() / time_limit;
        }

        bool annealing_scheduler(double profit, Xorshift& engine, double time_limit, double t0, double t1) const {
            assert(0.0 <= t1 && t1 <= t0);
            if (profit >= 0.0) {
                return true;
            } else {
                double ratio = progress(time_limit);
                double t = pow(t0, 1.0 - ratio) * pow(t1, ratio);
                return engine.uniform() < pow(2.0, profit/t);
            }
        }

    private:
        chrono::system_clock::time_point start_time_;
};

constexpr double time_limit = 1.95; // TODO

struct Input {
    // TODO

    void input() {
        // TODO
    }

    void input(const string& filename) {
        ifstream in(filename);
        assert(in);

        // TODO
    }
};

struct Solver {
    Timer timer;
    const Input input;

    Solver(const Input& input): input(input) {
        timer.begin();
    }

    void solve() {
        // TODO
    }

    void print() const {
        // TODO
    }

    ll score() const {
        // TODO
    }
};

void multi_test(int cases) {
    cerr << "cases: " << cases << endl;

    ll sum_scores = 0;
    double sum_time = 0.0;
    double max_time = 0.0;
    int max_time_seed = -1;

    for (int seed = 0; seed < cases; ++seed) {
        string filename = "in/";
        filename += '0' + seed / 1000;
        filename += '0' + (seed / 100) % 10;
        filename += '0' + (seed / 10) % 10;
        filename += '0' + seed % 10;
        filename += ".txt";
 
        Input input;
        input.input(filename);
 
        Solver solver(input);
        solver.solve();

        double elapsed_time = solver.timer.get_time();

        cerr << filename << " " << solver.score() << " " << elapsed_time << " sec" << endl;
        sum_scores += solver.score();

        sum_time += elapsed_time;
        if (elapsed_time > max_time) {
            max_time = elapsed_time;
            max_time_seed = seed;
        }
    }
    cerr << "Average Score: " << sum_scores / max(1, cases) << endl;
    cerr << "Max Time: " << max_time << " sec (seed = " << max_time_seed << ")" << endl;
    cerr << "Average Time: " << sum_time / max(1, cases) << " sec" << endl;
}

int main() {
    Input input;
    input.input();

    Solver solver(input);
    solver.solve();
    solver.print();

#ifndef ONLINE_JUDGE
    cerr << "Score = " << solver.score() << ", " << solver.timer.get_time() << " sec" << endl;

    int cases = 0;
    multi_test(cases);
#endif

    return 0;
}
