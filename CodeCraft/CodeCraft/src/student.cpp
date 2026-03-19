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
