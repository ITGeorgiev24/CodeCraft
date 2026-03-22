// ============================================================
// quiz.cpp
// ============================================================
#include "quiz.h"
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <ctime>

using namespace std;

// extern so the declaration in quiz.h links correctly
extern const map<Category, int> DIST = {
    { Category::ALGEBRA,       5 },
    { Category::GEOMETRY,      5 },
    { Category::FUNCTIONS,     4 },
    { Category::PROBABILITY,   3 },
    { Category::NUMBER_THEORY, 3 }
};

// -- Random sample (Fisher-Yates partial shuffle) -------------
static vector<Question> sampleFrom(const vector<Question>& pool,
    int count, mt19937& rng) {
    vector<int> idx(pool.size());
    iota(idx.begin(), idx.end(), 0);
    for (int i = 0; i < count && i < (int)idx.size(); ++i) {
        uniform_int_distribution<int> d(i, (int)idx.size() - 1);
        swap(idx[i], idx[d(rng)]);
    }
    vector<Question> out;
    for (int i = 0; i < count && i < (int)pool.size(); ++i)
        out.push_back(pool[idx[i]]);
    return out;
}

// -- Generate a 20-question test from the bank ----------------
vector<Question> generateTest(const vector<Question>& bank) {
    mt19937 rng(static_cast<unsigned>(time(nullptr)));
    vector<Question> test;
    for (auto it = DIST.begin(); it != DIST.end(); ++it) {
        for (auto& q : sampleFrom(byCat(bank, it->first), it->second, rng))
            test.push_back(q);
    }
    shuffle(test.begin(), test.end(), rng);
    return test;
}