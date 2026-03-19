#include "student.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// ?? CSV: username|name|scored|max|pct|grade|A_e|A_m|G_e|G_m|...
std::string TestResult::toCsv() const {
    std::ostringstream o;
    o << username << "|" << name << "|" << scored << "|" << maxPts
        << "|" << std::fixed << std::setprecision(2) << percent
        << "|" << grade << "|";
    for (auto c : allCats()) {
        auto it = cats.find(c);
        if (it != cats.end())
            o << it->second.first << "|" << it->second.second << "|";
        else
            o << "0|0|";
    }
    return o.str();
}

TestResult TestResult::fromCsv(const std::string& line) {
    TestResult r;
    std::istringstream ss(line);
    auto tok = [&] { std::string t; std::getline(ss, t, '|'); return t; };
    r.username = tok(); r.name = tok();
    r.scored = std::stoi(tok()); r.maxPts = std::stoi(tok());
    r.percent = std::stod(tok()); r.grade = std::stoi(tok());
    for (auto c : allCats()) {
        int e = std::stoi(tok()), m = std::stoi(tok());
        r.cats[c] = { e, m };
    }
    return r;
}

// ── Display ──────────────────────────────────────────────────
void TestResult::display() const {
    bDiv();
    bRow("  Student  : " + name, C_TXT);
    bRow("  Score    : " + std::to_string(scored) + " / " + std::to_string(maxPts), C_SCR);
    bRow("  Grade    : " + gradeStr(grade), C_SCR);
    bDiv();
    bRow("  Category breakdown:", C_DIM);
    for (auto c : allCats()) {
        auto it = cats.find(c);
        if (it == cats.end()) continue;
        int e = it->second.first, m = it->second.second;
        double p = pct(e, m);
        std::string bar = "[";
        int filled = static_cast<int>(p / 5.0);  // 0-20 chars
        bar += std::string(filled, '#') + std::string(20 - filled, ' ') + "]";
        std::string line = "  " + catName(c)
            + std::string(14 - catName(c).size(), ' ')
            + bar + "  " + std::to_string(e) + "/" + std::to_string(m);
        bRow(line, p >= 50 ? C_OK : C_ERR);
    }
    bBlank();
    bRow("  " + gradeMsg(grade), C_DIM);
}

// ── File I/O ─────────────────────────────────────────────────
bool saveResult(const TestResult& r) {
    std::ofstream f(RES_FILE, std::ios::app);
    f << r.toCsv() << "\n";
    return f.good();
}

std::vector<TestResult> loadResults() {
    std::vector<TestResult> v;
    std::ifstream f(RES_FILE);
    std::string line;
    while (std::getline(f, line))
        if (!line.empty()) try { v.push_back(TestResult::fromCsv(line)); }
    catch (...) {}
    return v;
}

std::vector<TestResult> resultsFor(const std::string& username) {
    std::vector<TestResult> v;
    for (const auto& r : loadResults())
        if (r.username == username) v.push_back(r);
    return v;
}
