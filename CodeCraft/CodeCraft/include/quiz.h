#pragma once
#include "question.h"
#include "student.h"
#include "account.h"
#include <vector>
#include <map>

using namespace std;

// Defined (extern) in quiz.cpp
extern const map<Category, int> DIST;

vector<Question> generateTest(const vector<Question>& bank);
TestResult       runTest(const vector<Question>& test, const Account& user);
void             practiceMode(const vector<Question>& bank);
