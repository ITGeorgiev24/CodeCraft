#include "../include/statistics.h"
#include "../include/student.h"
#include "../include/utils.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;

// Draws a percentage bar inside the box
static void barRow(const string& label, double p) {
    int filled = min(24, (int)(p / 100.0 * 24.0));
    string bar = "[" + string(filled, '#')
        + string(24 - filled, ' ') + "]";
    ostringstream pctStr;
    pctStr << fixed << setprecision(1) << p;

    string row = "  " + label
        + string(max(0, 15 - (int)label.size()), ' ')
        + bar + "  " + pctStr.str() + "%";
    bRow(row, p >= 50.0 ? C_OK : C_ERR);
}

void showStats() {
    auto results = loadResults();
    cls();
    bTop("EduRise  |  Global Statistics");

    if (results.empty()) {
        bBlank();
        bRow(cen("No test results recorded yet."), C_DIM);
        bRow(cen("Complete a test to see stats!"), C_DIM);
        bBlank();
        waitEnter();
        return;
    }

    int total = (int)results.size();

    // Best, worst, average
    auto best = *max_element(results.begin(), results.end(),
        [](const TestResult& a, const TestResult& b) {
            return a.percent < b.percent; });
    auto worst = *min_element(results.begin(), results.end(),
        [](const TestResult& a, const TestResult& b) {
            return a.percent < b.percent; });

    double avg = 0.0;
    for (const auto& r : results) avg += r.percent;
    avg /= total;

    // ── Summary ──────────────────────────────────────────────
    bBlank();
    bRow("  Total attempts  : " + to_string(total), C_TXT);
    bRow("  Average score   : " + to_string((int)(avg + 0.5))
        + "%   =>  Grade " + to_string(toGrade(avg)), C_SCR);
    bDiv();

    bRow("  Best result", C_HDR);
    bRow("    " + best.name
        + "  --  " + to_string(best.scored) + "/" + to_string(best.maxPts)
        + "  (" + to_string((int)(best.percent + 0.5)) + "%)"
        + "  Grade: " + to_string(best.grade), C_OK);
    bBlank();

    bRow("  Worst result", C_HDR);
    bRow("    " + worst.name
        + "  --  " + to_string(worst.scored) + "/" + to_string(worst.maxPts)
        + "  (" + to_string((int)(worst.percent + 0.5)) + "%)"
        + "  Grade: " + to_string(worst.grade), C_ERR);
    bDiv();

    // ── Grade distribution ───────────────────────────────────
    bRow("  Grade distribution:", C_HDR);
    map<int, int> gd;
    for (int g = 2; g <= 6; ++g) gd[g] = 0;
    for (const auto& r : results) gd[r.grade]++;
    for (int g = 6; g >= 2; --g) {
        double p = total ? gd[g] * 100.0 / total : 0.0;
        barRow("Grade " + to_string(g), p);
    }
    bDiv();

    // ── Category success rates ───────────────────────────────
    bRow("  Category success rates:", C_HDR);
    string bestCat = "";
    string worstCat = "";
    double bRate = -1.0;
    double wRate = 101.0;

    for (auto cat : allCats()) {
        double earned = 0, maxed = 0;
        for (const auto& r : results) {
            auto it = r.cats.find(cat);
            if (it != r.cats.end()) {
                earned += it->second.first;
                maxed += it->second.second;
            }
        }
        double rate = maxed ? earned * 100.0 / maxed : 0.0;
        barRow(catName(cat), rate);
        if (rate > bRate) { bRate = rate; bestCat = catName(cat); }
        if (rate < wRate) { wRate = rate; worstCat = catName(cat); }
    }
    bDiv();
    bRow("  Strongest category : " + bestCat, C_OK);
    bRow("  Weakest  category  : " + worstCat, C_ERR);
    bDiv();

    // ── All attempts table ───────────────────────────────────
    bRow("  All attempts:", C_HDR);
    bRow("  " + string(20, ' ') + "Score    %     Grade", C_DIM);
    bDiv();

    for (const auto& r : results) {
        string nm = (r.name.size() > 18) ? r.name.substr(0, 18) : r.name;
        ostringstream row;
        row << "  " << left << setw(20) << nm
            << setw(9)
            << (to_string(r.scored) + "/" + to_string(r.maxPts))
            << setw(7)
            << (to_string((int)(r.percent + 0.5)) + "%")
            << r.grade;
        bRow(row.str(), r.grade >= 4 ? C_OK : C_ERR);
    }

    bBlank();
    waitEnter();
}
