#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
// #include <atcoder/all>

using namespace std;
// using namespace atcoder;

#define ll long long int
#define ull unsigned long long int

uniform_real_distribution<> uniform(0.0, 1.0);

struct Timer {
    chrono::system_clock::time_point start_time_;

    void begin() {
        start_time_ = chrono::system_clock::now();
    }

    double get_time() {
        chrono::system_clock::time_point end_time = chrono::system_clock::now();
        double elapsed_time = chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time_).count();
        elapsed_time *= 1e-9; // nanoseconds -> seconds
        return elapsed_time;
    }

    bool yet(double time_limit) {
        return get_time() < time_limit;
    }

    double progress(double time_limit) {
        return get_time() / time_limit;
    }

    bool annealing_scheduler(double profit, mt19937& engine, double time_limit, double t0, double t1) {
        assert(0.0 <= t1 && t1 <= t0);
        if (profit >= 0.0) {
            return true;
        } else {
            double ratio = progress(time_limit);
            double t = pow(t0, 1.0 - ratio) * pow(t1, ratio);
            return uniform(engine) < pow(2.0, profit/t);
        }
    }
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

    void print() {
        // TODO
    }

    ll score() {
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
    int cases = 0;
    multi_test(cases);
#endif

    return 0;
}
