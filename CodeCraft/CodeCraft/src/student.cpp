#include "../include/student.h"
#include "../include/utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

string TestResult::toCsv() const {
    ostringstream o;
    o << username << "|" << name << "|"
        << scored << "|" << maxPts << "|"
        << fixed << setprecision(2) << percent << "|"
        << grade << "|";
    for (auto c : allCats()) {
        auto it = cats.find(c);
        if (it != cats.end())
            o << it->second.first << "|" << it->second.second << "|";
        else
            o << "0|0|";
    }
    return o.str();
}

TestResult TestResult::fromCsv(const string& line) {
    TestResult r;
    istringstream ss(line);
    auto tok = [&]() { string t; getline(ss, t, '|'); return t; };
    r.username = tok();
    r.name = tok();
    r.scored = stoi(tok());
    r.maxPts = stoi(tok());
    r.percent = stod(tok());
    r.grade = stoi(tok());
    for (auto c : allCats()) {
        int e = stoi(tok());
        int m = stoi(tok());
        r.cats[c] = { e, m };
    }
    return r;
}

void TestResult::display() const {
    bDiv();
    bRow("  Student  : " + name, C_TXT);
    bRow("  Score    : " + to_string(scored) + " / " + to_string(maxPts), C_SCR);
    bRow("  Grade    : " + gradeStr(grade), C_SCR);
    bDiv();
    bRow("  Category breakdown:", C_DIM);
    for (auto c : allCats()) {
        auto it = cats.find(c);
        if (it == cats.end()) continue;
        int    e = it->second.first;
        int    m = it->second.second;
        double p = calcPct(e, m);
        int    bar = min(20, (int)(p / 5.0));
        string barStr = "[" + string(bar, '#') + string(20 - bar, ' ') + "]";
        string label = catName(c);
        string row = "  " + label
            + string(14 - (int)label.size(), ' ')
            + barStr + "  " + to_string(e) + "/" + to_string(m);
        bRow(row, p >= 50.0 ? C_OK : C_ERR);
    }
    bBlank();
    bRow("  " + gradeMsg(grade), C_DIM);
}

bool saveResult(const TestResult& r) {
    ofstream f(RES_FILE, ios::app);
    f << r.toCsv() << "\n";
    return f.good();
}

vector<TestResult> loadResults() {
    vector<TestResult> v;
    ifstream f(RES_FILE);
    string line;
    while (getline(f, line)) {
        if (!line.empty()) {
            try {
                v.push_back(TestResult::fromCsv(line));
            }
            catch (...) {}
        }
    }
    return v;
}

vector<TestResult> resultsFor(const string& username) {
    vector<TestResult> v;
    for (const auto& r : loadResults())
        if (r.username == username) v.push_back(r);
    return v;
}
