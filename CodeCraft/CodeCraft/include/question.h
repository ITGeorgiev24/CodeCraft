// ============================================================
// include/question.h
// Question struct, Category enum, 33-question bank loader
// ============================================================
#pragma once
#include <string>
#include <vector>

using namespace std;

enum class Category {
    ALGEBRA,
    GEOMETRY,
    FUNCTIONS,
    PROBABILITY,
    NUMBER_THEORY
};

string           catName(Category c);
vector<Category> allCats();

struct Question {
    int            id;
    string         text;
    vector<string> opts;    // exactly 4 choices
    int            correct; // 0-3  (A-D)
    Category       cat;
    int            pts;

    bool check(int answerIdx) const { return answerIdx == correct; }
};

vector<Question> makeBank();
vector<Question> byCat(const vector<Question>& bank, Category c);
