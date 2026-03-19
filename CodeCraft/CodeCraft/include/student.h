#pragma once
#include "question.h"
#include <string>
#include <map>
#include <vector>

struct TestResult {
    std::string username;
    std::string name;
    int  scored = 0;
    int  maxPts = 0;
    double percent = 0.0;
    int  grade = 0;
    std::map<Category, std::pair<int, int>> cats;  // {earned, max}

    std::string toCsv()            const;
    static TestResult fromCsv(const std::string& line);
    void display()                 const;
};