// ============================================================
// statistics.cpp
// ============================================================
#include "../include/statistics.h"
#include "../include/student.h"
#include "../include/utils.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>

// Draws a coloured bar for a percentage value (max bar = 24 chars)
static void barRow(const std::string& label, double p) {
    int filled = static_cast<int>(p / 100.0 * 24.0);
    std::string bar = "[" + std::string(filled, '#')
        + std::string(24 - filled, ' ') + "]";
    std::ostringstream o;
    o << std::fixed << std::setprecision(1) << p;
    std::string row = "  " + label
        + std::string(15 - label.size(), ' ') + bar
        + "  " + o.str() + "%";
    bRow(row, p >= 50.0 ? C_OK : C_ERR);
}

void showStats() {
    auto results = loadResults();
    cls();
    bTop("GLOBAL STATISTICS");

    if (results.empty()) {
        bBlank();
        bRow(cen("No test results yet."), C_DIM, false);
        bRow(cen("Complete a test first!"), C_DIM, false);
        bBlank();
        waitEnter();
        return;
    }

    int total = static_cast<int>(results.size());

    // Best / worst / average
    auto best = *std::max_element(results.begin(), results.end(),
        [](const TestResult& a, const TestResult& b) { return a.percent < b.percent; });
    auto worst = *std::min_element(results.begin(), results.end(),
        [](const TestResult& a, const TestResult& b) { return a.percent < b.percent; });
    double avg = 0;
    for (const auto& r : results) avg += r.percent;
    avg /= total;

    bBlank();
    bRow("  Total attempts : " + std::to_string(total), C_TXT);
    bRow("  Average score  : " + std::to_string(static_cast<int>(avg + 0.5)) + "%"
        + "   Grade: " + std::to_string(toGrade(avg)), C_SCR);
    bDiv();

    bRow("  Best result", C_HDR);
    bRow("    " + best.name + "  " + std::to_string(best.scored)
        + "/" + std::to_string(best.maxPts)
        + "  (" + std::to_string(static_cast<int>(best.percent + 0.5)) + "%)"
        + "  Grade: " + std::to_string(best.grade), C_OK);
    bBlank();
    bRow("  Worst result", C_HDR);
    bRow("    " + worst.name + "  " + std::to_string(worst.scored)
        + "/" + std::to_string(worst.maxPts)
        + "  (" + std::to_string(static_cast<int>(worst.percent + 0.5)) + "%)"
        + "  Grade: " + std::to_string(worst.grade), C_ERR);
    bDiv();

    // Grade distribution
    bRow("  Grade distribution:", C_HDR);
    std::map<int, int> gd;
    for (int g = 2; g <= 6; ++g) gd[g] = 0;
    for (const auto& r : results) gd[r.grade]++;
    for (int g = 6; g >= 2; --g) {
        int cnt = gd[g];
        double p = total ? cnt * 100.0 / total : 0.0;
        barRow("Grade " + std::to_string(g), p);
    }
    bDiv();

    // Category success rates
    bRow("  Category success rates:", C_HDR);
    std::string bestCat = "", worstCat = "";
    double bRate = -1, wRate = 101;
    for (auto cat : allCats()) {
        double e = 0, m = 0;
        for (const auto& r : results) {
            auto it = r.cats.find(cat);
            if (it != r.cats.end()) { e += it->second.first; m += it->second.second; }
        }
        double rate = m ? e * 100.0 / m : 0;
        barRow(catName(cat), rate);
        if (rate > bRate) { bRate = rate; bestCat = catName(cat); }
        if (rate < wRate) { wRate = rate; worstCat = catName(cat); }
    }
    bDiv();
    bRow("  Strongest : " + bestCat, C_OK);
    bRow("  Weakest   : " + worstCat, C_ERR);
    bDiv();

    // All attempts table
    bRow("  All attempts:", C_HDR);
    bRow("  " + std::string(20, ' ') + "Score   %    Grade", C_DIM);
    for (const auto& r : results) {
        std::string n = r.name.size() > 18 ? r.name.substr(0, 18) : r.name;
        std::ostringstream row;
        row << "  " << std::left << std::setw(20) << n
            << std::setw(8)
            << (std::to_string(r.scored) + "/" + std::to_string(r.maxPts))
            << std::setw(6)
            << (std::to_string(static_cast<int>(r.percent + 0.5)) + "%")
            << r.grade;
        bRow(row.str(), r.grade >= 4 ? C_OK : C_ERR);
    }
    bBlank();
    waitEnter();
}
