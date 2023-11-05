#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

// #define NDEBUG

#include <bits/stdc++.h>
#include <atcoder/all>

using namespace std;
using namespace atcoder;

#define ll long long int
#define ull unsigned long long int

class Xorshift {
    public:
        Xorshift(uint32_t seed): x_(seed) {
            assert(seed);
        }

        uint32_t randrange(uint32_t stop) {
            // [0, stop)
            assert(stop > 0);
            next();
            return x_ % stop;
        }

        uint32_t randrange(uint32_t start, uint32_t stop) {
            // [start, stop)
            assert(start < stop);
            next();
            return start + x_ % (stop - start);
        }

        uint32_t randint(uint32_t a, uint32_t b) {
            // [a, b]
            assert(a <= b);
            return randrange(a, b + 1);
        }

        double random() {
            // [0.0, 1.0]
            next();
            return static_cast<double>(x_) / static_cast<double>(UINT32_MAX);
        }

        double uniform(double a, double b) {
            // [a, b] or [b, a]
            return a + (b - a) * random();
        }

    private:
        void next() {
            x_ ^= x_ << 13;
            x_ ^= x_ >> 7;
            x_ ^= x_ << 17;
        }

        uint32_t x_;
};

class IndexSet {
    public:
        IndexSet(int n) : n_(n), positions_(n, -1) {}

        void insert(int x) {
            assert(0 <= x && x < n_);
            assert(!contains(x));
            positions_[x] = data_.size();
            data_.push_back(x);
        }

        void erase(int x) {
            assert(0 <= x && x < n_);
            assert(contains(x));
            int i = positions_[x];
            int y = data_.back();
            data_[i] = y;
            data_.pop_back();
            positions_[y] = i;
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

        vector<int> get_data() const {
            return data_;
        }

        size_t size() const {
            return data_.size();
        }

    private:
        int n_;
        vector<int> data_;
        vector<int> positions_;
};

class Timer {
    public:
        Timer() {
            begin();
            elapsed_time_ = 0.0;
        }

        void begin() {
            start_time_ = chrono::system_clock::now();
        }

        double get_time() {
            chrono::system_clock::time_point end_time = chrono::system_clock::now();
            elapsed_time_ = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time_).count();
            elapsed_time_ *= 1e-9; // nanoseconds -> seconds
            return elapsed_time_;
        }

        double get_last_time() const {
            return elapsed_time_;
        }

        bool yet(double time_limit) {
            return get_time() < time_limit;
        }

        double progress(double time_limit) {
            return get_time() / time_limit;
        }

    private:
        chrono::system_clock::time_point start_time_;
        double elapsed_time_;
};

constexpr size_t sa_time_steps = (1 << 13);
constexpr size_t sa_random_steps = (1 << 12);

class SimulatedAnnealing {
    public:
        SimulatedAnnealing(double t0, double t1, double time_limit): time_limit_(time_limit) {
            assert(t0 >= t1);
            for (size_t i = 0; i < sa_time_steps; ++i) {
                double progress = static_cast<double>(i) / static_cast<double>(sa_time_steps);
                temperatures_[i] = pow(t0, 1.0 - progress) * pow(t1, progress);
            }
            for (size_t i = 0; i < sa_random_steps; ++i) {
                log2_random_[i] = log2(static_cast<double>(i + 1) / static_cast<double>(sa_random_steps));
            }
        }

        bool move(double profit, const Timer& timer, Xorshift& engine) {
            if (profit >= 0.0) {
                return true;
            } else {
                double t = get_temperature(timer);
                return profit > t * log2_random_[engine.randrange(sa_random_steps)];
            }
        }

    private:
        double get_temperature(const Timer& timer) {
            // size_t i = timer.get_time() * (static_cast<double>(sa_time_steps) / time_limit_);
            size_t i = static_cast<size_t>(timer.get_last_time() * (static_cast<double>(sa_time_steps) / time_limit_));
            i = max(0ul, min(i, sa_time_steps - 1ul));
            return temperatures_[i];
        }

        double time_limit_;
        array<double,sa_time_steps> temperatures_;
        array<double,sa_random_steps> log2_random_;
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

    Solver(const Input& input): input(input) {}

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
    if (cases <= 0) {
        return;
    }
    cerr << "cases: " << cases << endl;

    vector<ll> scores(cases);
    vector<double> times(cases);

#ifndef ONLINE_JUDGE
    #pragma omp parallel for
#endif
    for (int seed = 0; seed < cases; ++seed) {
        string filename = to_string(seed);
        filename = "in/" + string(4 - filename.size(), '0') + filename + ".txt";
 
        Input input;
        input.input(filename);
 
        Solver solver(input);
        solver.solve();

        times[seed] = solver.timer.get_time();
        scores[seed] = solver.score();

        double random_time = Xorshift(seed + 1).uniform(0.0, 0.01);
        Timer timer;
        while (timer.yet(random_time)) {}

        cerr << filename << " " << scores[seed] << " " << times[seed] << " sec" << endl;
    }
    cerr << "Average Score: " << accumulate(scores.begin(), scores.end(), 0) / cases << endl;
    cerr << "Max Time: " << *max_element(times.begin(), times.end()) << " sec" << endl;
    cerr << "Average Time: " << accumulate(times.begin(), times.end(), 0.0) / cases << " sec" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

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
