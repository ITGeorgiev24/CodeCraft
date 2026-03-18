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
