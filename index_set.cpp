#include <bits/stdc++.h>

using namespace std;

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

        int choice(mt19937& engine) const {
            assert(!data_.empty());
            return data_[engine() % data_.size()];
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
