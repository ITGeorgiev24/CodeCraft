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

// -- Interactive test runner ----------------------------------
TestResult runTest(const vector<Question>& test, const Account& user) {
    map<Category, pair<int, int>> cats;
    for (auto c : allCats()) cats[c] = { 0, 0 };

    int scored = 0, maxPts = 0;
    int total = (int)test.size();

    for (int i = 0; i < total; ++i) {
        const Question& q = test[i];
        cats[q.cat].second += q.pts;
        maxPts += q.pts;

        int ans = answerPick(q.text, q.opts, catName(q.cat),
            q.pts, i + 1, total, scored, maxPts);

        cls();
        bTop("Question " + to_string(i + 1) + " of " + to_string(total));
        bBlank();
        if (q.check(ans)) {
            cats[q.cat].first += q.pts;
            scored += q.pts;
            bRow(cen("CORRECT!   +" + to_string(q.pts) + " pts"), C_OK);
        }
        else {
            const string L = "ABCD";
            bRow(cen("Wrong.  Correct: " + string(1, L[q.correct])
                + ")  " + q.opts[q.correct]), C_ERR);
        }
        bBlank();
        bRow(cen("Running score: " + to_string(scored)
            + " / " + to_string(maxPts)), C_SCR);
        bBlank();
        waitEnter();
    }

    double p = calcPct(scored, maxPts);

    TestResult r;
    r.username = user.username;
    r.name = user.name;
    r.scored = scored;
    r.maxPts = maxPts;
    r.percent = p;
    r.grade = toGrade(p);
    r.cats = cats;

    cls();
    bTop("TEST RESULT");
    bBlank();
    r.display();
    waitEnter();
    return r;
}

// -- Practice mode --------------------------------------------
void practiceMode(const vector<Question>& bank) {
    vector<string> names;
    for (auto c : allCats()) names.push_back(catName(c));
    names.push_back("Back");

    int choice = menuSelect("PRACTICE MODE", names,
        "Choose a category - no grading");
    if (choice == (int)names.size() - 1) return;

    auto cats = allCats();
    auto pool = byCat(bank, cats[choice]);
    int  total = (int)pool.size();

    for (int i = 0; i < total; ++i) {
        const Question& q = pool[i];
        int ans = answerPick(q.text, q.opts, catName(q.cat),
            q.pts, i + 1, total, 0, 0);
        cls();
        bTop("Practice  --  " + catName(q.cat));
        bBlank();
        const string L = "ABCD";
        if (q.check(ans))
            bRow(cen("Correct!"), C_OK);
        else
            bRow(cen("Correct answer: " + string(1, L[q.correct])
                + ")  " + q.opts[q.correct]), C_ERR);
        bBlank();
        waitEnter();
    }
}

