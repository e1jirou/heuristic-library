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
    chrono::system_clock::time_point start;

    void begin() {
        start = chrono::system_clock::now();
    }

    double stopwatch() {
        chrono::system_clock::time_point end = chrono::system_clock::now();
        double elapsed = chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        elapsed *= 1e-9; // nanoseconds -> seconds
        return elapsed;
    }

    bool yet(double time_limit) {
        return stopwatch() < time_limit;
    }

    double progress(double time_limit) {
        return stopwatch() / time_limit;
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

