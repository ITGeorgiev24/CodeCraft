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
    vector<string> opts;
    int            correct;
    Category       cat;
    int            pts;

    bool check(int answerIdx) const { return answerIdx == correct; }
};

vector<Question> makeBank();
vector<Question> byCat(const vector<Question>& bank, Category c);
