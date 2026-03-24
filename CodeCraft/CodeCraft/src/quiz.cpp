#include "../include/quiz.h"
#include "../include/utils.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <ctime>

using namespace std;

// extern - matches the declaration in quiz.h
extern const map<Category, int> DIST = {
    { Category::ALGEBRA,       5 },
    { Category::GEOMETRY,      5 },
    { Category::FUNCTIONS,     4 },
    { Category::PROBABILITY,   3 },
    { Category::NUMBER_THEORY, 3 }
};

// ── Fisher-Yates partial shuffle sample ──────────────────────
static vector<Question> sampleFrom(const vector<Question>& pool,
    int count, mt19937& rng) {
    vector<int> idx(pool.size());
    iota(idx.begin(), idx.end(), 0);

    for (int i = 0; i < count && i < (int)idx.size(); ++i) {
        uniform_int_distribution<int> d(i, (int)idx.size() - 1);
        swap(idx[i], idx[d(rng)]);
    }

    vector<Question> out;
    out.reserve(count);
    for (int i = 0; i < count && i < (int)pool.size(); ++i)
        out.push_back(pool[idx[i]]);
    return out;
}

// ── Generate a 20-question test ──────────────────────────────
vector<Question> generateTest(const vector<Question>& bank) {
    mt19937 rng(static_cast<unsigned>(time(nullptr)));
    vector<Question> test;
    test.reserve(20);

    for (auto it = DIST.begin(); it != DIST.end(); ++it) {
        auto pool = byCat(bank, it->first);
        for (auto& q : sampleFrom(pool, it->second, rng))
            test.push_back(q);
    }
    shuffle(test.begin(), test.end(), rng);
    return test;
}

// ── Interactive test runner ───────────────────────────────────
TestResult runTest(const vector<Question>& test, const Account& user) {
    map<Category, pair<int, int>> cats;
    for (auto c : allCats()) cats[c] = { 0, 0 };

    int  scored = 0;
    int  maxPts = 0;
    int  total = (int)test.size();
    bool aborted = false;

    for (int i = 0; i < total; ++i) {
        const Question& q = test[i];
        cats[q.cat].second += q.pts;
        maxPts += q.pts;

        int ans = answerPick(q.text, q.opts, catName(q.cat),
            q.pts, i + 1, total, scored, maxPts);

        // ESC during test - abort immediately
        if (ans == -1) {
            aborted = true;
            break;
        }

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
            bRow(cen("Wrong.  Correct answer: "
                + string(1, L[q.correct]) + ")  "
                + q.opts[q.correct]), C_ERR);
        }
        bBlank();
        bRow(cen("Running score: " + to_string(scored)
            + " / " + to_string(maxPts)), C_SCR);
        bBlank();
        waitEnter();
    }

    if (aborted) {
        cls();
        bTop("EduRise  |  Test Aborted");
        bBlank();
        bRow(cen("Test was cancelled.  No result was saved."), C_ERR);
        bBlank();
        waitEnter();
        return TestResult{};
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
    bTop("EduRise  |  Test Result");
    bBlank();
    r.display();
    waitEnter();
    return r;
}

// ── Practice mode ────────────────────────────────────────────
void practiceMode(const vector<Question>& bank) {
    vector<string> names;
    for (auto c : allCats()) names.push_back(catName(c));
    names.push_back("Back");

    int choice = menuSelect("EduRise  |  Practice",
        names,
        "Choose a category - no grading");

    if (choice == -1 || choice == (int)names.size() - 1) return;

    auto cats = allCats();
    auto pool = byCat(bank, cats[choice]);
    int  total = (int)pool.size();

    for (int i = 0; i < total; ++i) {
        const Question& q = pool[i];

        int ans = answerPick(q.text, q.opts, catName(q.cat),
            q.pts, i + 1, total, 0, 0);

        if (ans == -1) return;  // ESC - back to main menu

        cls();
        bTop("EduRise  |  Practice  --  " + catName(q.cat));
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

    cls();
    bTop("EduRise  |  Practice Complete");
    bBlank();
    bRow(cen("You finished all questions in this category!"), C_OK);
    bBlank();
    waitEnter();
}
