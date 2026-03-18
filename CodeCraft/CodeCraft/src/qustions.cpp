// ============================================================
// question.cpp  --  Category helpers and full 33-question bank
// ============================================================
#include "question.h"

using namespace std;

string catName(Category c) {
    switch (c) {
    case Category::ALGEBRA:       return "Algebra";
    case Category::GEOMETRY:      return "Geometry";
    case Category::FUNCTIONS:     return "Functions";
    case Category::PROBABILITY:   return "Probability";
    case Category::NUMBER_THEORY: return "Number Theory";
    default:                      return "Unknown";
    }
}

vector<Category> allCats() {
    return { Category::ALGEBRA, Category::GEOMETRY, Category::FUNCTIONS,
             Category::PROBABILITY, Category::NUMBER_THEORY };
}

vector<Question> makeBank() {
    return {
        // -- ALGEBRA  (8 questions, 1 pt each) ------------------
        {1,  "Solve:  2x + 4 = 10",
             {"x = 2","x = 3","x = 4","x = 5"},              1, Category::ALGEBRA, 1},
        {2,  "Simplify:  3(x+2) - 2(x-1)",
             {"x+4","x+6","x+8","2x+4"},                     2, Category::ALGEBRA, 1},
        {3,  "What is  x^2  when  x = -5?",
             {"-25","-10","10","25"},                         3, Category::ALGEBRA, 1},
        {4,  "Solve:  x / 3 = 7",
             {"x=3","x=10","x=21","x=4"},                    2, Category::ALGEBRA, 1},
        {5,  "Expanded form of  (a + b)^2?",
             {"a^2+b^2","a^2+ab+b^2","a^2+2ab+b^2","2a+2b"}, 2, Category::ALGEBRA, 1},
        {6,  "Solve:  5x - 15 = 0",
             {"x=5","x=15","x=3","x=10"},                    2, Category::ALGEBRA, 1},
        {7,  "GCF of 12 and 18?",
             {"3","4","6","9"},                               2, Category::ALGEBRA, 1},
        {8,  "Simplify:  x^2 * x^3",
             {"2x^5","x^5","x^6","x"},                       1, Category::ALGEBRA, 1},

             // -- GEOMETRY  (8 questions, 2 pts each) ----------------
             {9,  "Area of a circle with radius 5?  (pi = 3.14)",
                  {"15.7","31.4","78.5","25"},                     2, Category::GEOMETRY, 2},
             {10, "Area of a triangle: base=6, height=4?",
                  {"24","20","10","12"},                           3, Category::GEOMETRY, 2},
             {11, "Right triangle legs a=3, b=4. Hypotenuse?",
                  {"3.5","5","6","7"},                             1, Category::GEOMETRY, 2},
             {12, "Perimeter of a rectangle: length=5, width=3?",
                  {"8","15","16","30"},                            2, Category::GEOMETRY, 2},
             {13, "Volume of a cube with side = 3?",
                  {"6","9","18","27"},                             3, Category::GEOMETRY, 2},
             {14, "Sum of interior angles in any triangle?",
                  {"90 deg","180 deg","270 deg","360 deg"},        1, Category::GEOMETRY, 2},
             {15, "Area of a square with side = 7?",
                  {"14","21","28","49"},                           3, Category::GEOMETRY, 2},
             {16, "Two supplementary angles sum to?",
                  {"45 deg","90 deg","180 deg","360 deg"},         2, Category::GEOMETRY, 2},

                  // -- FUNCTIONS  (7 questions, 2 pts each) ---------------
                  {17, "If f(x) = 2x + 1,  what is f(3)?",
                       {"5","7","9","3"},                               1, Category::FUNCTIONS, 2},
                  {18, "Slope of the line  y = 3x - 2?",
                       {"-2","2","3","-3"},                             2, Category::FUNCTIONS, 2},
                  {19, "Where does  y = x - 5  cross the x-axis?",
                       {"(0,5)","(5,0)","(-5,0)","(0,-5)"},             1, Category::FUNCTIONS, 2},
                  {20, "If f(x) = x^2,  what is f(-3)?",
                       {"-9","-6","6","9"},                             3, Category::FUNCTIONS, 2},
                  {21, "What type of function is  y = x^2?",
                       {"Linear","Quadratic","Cubic","Exponential"},    1, Category::FUNCTIONS, 2},
                  {22, "Domain of  f(x) = 1/x?",
                       {"All reals","All reals except 0","x>0","x>=0"}, 1, Category::FUNCTIONS, 2},
                  {23, "If f(x) = |x|,  what is f(-7)?",
                       {"-49","-7","0","7"},                            3, Category::FUNCTIONS, 2},

                       // -- PROBABILITY  (4 questions, 3 pts each) -------------
{24, "Probability of rolling a 6 on a fair die?",
     {"1/2","1/3","1/4","1/6"},                       3, Category::PROBABILITY, 3},
{25, "P(A)=0.3, P(B)=0.4, P(A and B)=0.1.  Find P(A or B).",
     {"0.5","0.6","0.7","0.8"},                       1, Category::PROBABILITY, 3},
{26, "Probability of drawing an Ace from 52 cards?",
     {"1/52","1/26","1/13","1/4"},                    2, Category::PROBABILITY, 3},
{27, "Bag: 3 red, 7 blue balls. P(red)?",
     {"0.3","0.7","3/7","7/3"},                       0, Category::PROBABILITY, 3},

     // -- NUMBER THEORY  (6 questions, 1 pt each) ------------
     {28, "Is 17 a prime number?",
          {"No, composite","Yes, prime","Neither","Only if 17>10"},
                                                           1, Category::NUMBER_THEORY, 1},
     {29, "LCM of 4 and 6?",
          {"2","8","12","24"},                             2, Category::NUMBER_THEORY, 1},
     {30, "How many positive divisors does 12 have?",
          {"3","4","5","6"},                               3, Category::NUMBER_THEORY, 1},
     {31, "What is  5!  (5 factorial)?",
          {"25","60","120","720"},                         2, Category::NUMBER_THEORY, 1},
     {32, "Is 144 a perfect square?",
          {"No","Yes","Only approx","Cannot determine"},   1, Category::NUMBER_THEORY, 1},
     {33, "Remainder when 17 is divided by 5?",
          {"1","2","3","4"},                               1, Category::NUMBER_THEORY, 1},
    };
}

vector<Question> byCat(const vector<Question>& bank, Category c) {
    vector<Question> out;
    for (const auto& q : bank)
        if (q.cat == c) out.push_back(q);
    return out;
}
