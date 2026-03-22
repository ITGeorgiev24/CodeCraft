// ============================================================
// include/quiz.h
// Test generation and interactive runner
//
// Distribution (20 questions from 33):
//   Algebra       5 of 8  x 1 pt  =  5 pts
//   Geometry      5 of 8  x 2 pts = 10 pts
//   Functions     4 of 7  x 2 pts =  8 pts
//   Probability   3 of 4  x 3 pts =  9 pts
//   Number Theory 3 of 6  x 1 pt  =  3 pts
//                          TOTAL  = 35 pts
//
// Unique variants = C(8,5)*C(8,5)*C(7,4)*C(4,3)*C(6,3)
//                 = 56*56*35*4*20 = 8,780,800
// ============================================================
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
