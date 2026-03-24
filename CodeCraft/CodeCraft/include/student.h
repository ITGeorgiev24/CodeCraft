#pragma once
#include "question.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

static const string RES_FILE = "data/results.txt";

struct TestResult {
    string   username;
    string   name;
    int      scored = 0;
    int      maxPts = 0;
    double   percent = 0.0;
    int      grade = 0;
    map<Category, pair<int, int>> cats;  // {earned, max} per category

    bool   empty()  const { return username.empty(); }
    string toCsv()  const;
    static TestResult fromCsv(const string& line);
    void   display() const;
};

bool               saveResult(const TestResult& r);
vector<TestResult> loadResults();
vector<TestResult> resultsFor(const string& username);
