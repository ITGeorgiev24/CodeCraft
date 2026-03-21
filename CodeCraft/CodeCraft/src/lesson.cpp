// ============================================================
// lesson.cpp  --  All five lessons rendered inside the box
// ============================================================
#include "lesson.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// -- Helpers --------------------------------------------------
static void sec(const string& t) {
    bDiv();
    bRow("  " + t, C_HDR);
    bDiv();
}
static void ln(const string& t, const string& col = C_TXT) {
    bRow("  " + t, col);
}
static void ex(const string& problem, const string& solution) {
    bRow("  " + problem, C_TXT);
    bRow("    =>  " + solution, C_SEL);
    bBlank();
}

// -- ALGEBRA --------------------------------------------------
static void lessonAlgebra() {
    cls();
    bTop("LESSON  |  ALGEBRA");

    sec("Variables and Expressions");
    ln("Variable: a symbol (e.g. x, y) for an unknown value.");
    ln("Expression: combines variables, numbers, operations.");
    bBlank();
    ln("Examples:  3x + 2       x^2 - 5x + 6       2(a + b)");
    bBlank();

    sec("Solving Linear Equations   ax + b = c");
    ln("1. Move constants to the right (sign changes).");
    ln("2. Divide both sides by the coefficient of x.");
    bBlank();
    ex("Solve:  2x + 4 = 10", "2x = 6   =>   x = 3");
    ex("Solve:  3x - 9 = 0", "3x = 9   =>   x = 3");

    sec("Key Identities");
    ln("(a + b)^2  =  a^2 + 2ab + b^2");
    ln("(a - b)^2  =  a^2 - 2ab + b^2");
    ln("(a + b)(a - b)  =  a^2 - b^2");
    bBlank();
    ex("Expand: (x + 3)^2", "x^2 + 6x + 9");

    sec("Exercises");
    ln("[1]  Solve:  5x - 15 = 0", C_DIM);
    ln("[2]  Simplify:  3(x+2) - 2(x-1)", C_DIM);
    ln("[3]  Find GCF of 24 and 36", C_DIM);
    bBlank();

    sec("Homework");
    ln("A.  Solve:  4(x - 2) + 3 = 15");
    ln("B.  Expand:  (2a - 3b)^2");
    ln("C.  Prove that (a+b)^2 - (a-b)^2 = 4ab");
    bBlank();
    waitEnter();
}

// -- GEOMETRY -------------------------------------------------
static void lessonGeometry() {
    cls();
    bTop("LESSON  |  GEOMETRY");

    sec("Area Formulas");
    ln("Square      :  A = side^2");
    ln("Rectangle   :  A = length * width");
    ln("Triangle    :  A = (base * height) / 2");
    ln("Circle      :  A = pi * r^2     (pi = 3.14159)");
    bBlank();
    ex("Triangle  base=6, height=4", "A = (6*4)/2 = 12");
    ex("Circle  r=5", "A = 3.14 * 25 = 78.5");

    sec("Perimeter and Circumference");
    ln("Rectangle   :  P = 2 * (length + width)");
    ln("Circle      :  C = 2 * pi * r");
    bBlank();

    sec("Pythagorean Theorem");
    ln("For a RIGHT triangle:   a^2 + b^2 = c^2");
    ln("c is the hypotenuse (opposite the right angle).");
    bBlank();
    ex("Legs a=3, b=4. Find c.", "c = sqrt(9+16) = sqrt(25) = 5");

    sec("Volume");
    ln("Cube       :  V = side^3");
    ln("Cylinder   :  V = pi * r^2 * height");
    bBlank();

    sec("Exercises");
    ln("[1]  Area of rectangle  length=8, width=5", C_DIM);
    ln("[2]  Hypotenuse when a=5, b=12", C_DIM);
    ln("[3]  Circumference of circle  r=7", C_DIM);
    bBlank();
    waitEnter();
}

// -- FUNCTIONS ------------------------------------------------
static void lessonFunctions() {
    cls();
    bTop("LESSON  |  FUNCTIONS");

    sec("What Is a Function?");
    ln("Maps each input x to exactly one output f(x).");
    ln("Domain : all valid inputs     Range : all possible outputs");
    bBlank();

    sec("Linear Functions   y = mx + b");
    ln("m = slope         b = y-intercept");
    ln("Positive m: line rises.    Negative m: line falls.");
    bBlank();
    ex("f(x) = 2x + 1,  find f(3)", "f(3) = 2*3+1 = 7");
    ex("Slope of  y = 3x - 2", "m = 3");

    sec("Quadratic Functions   y = ax^2 + bx + c");
    ln("Graph: parabola.   a > 0 opens up,   a < 0 opens down.");
    ln("Vertex x-coord:  Xv = -b / (2a)");
    bBlank();
    ex("f(x) = x^2,  find f(-3)", "f(-3) = (-3)^2 = 9");

    sec("Domain Restrictions");
    ln("f(x) = 1/x     : all reals EXCEPT 0");
    ln("f(x) = sqrt(x) : x >= 0");
    ln("f(x) = |x|     : all reals");
    bBlank();

    sec("Exercises");
    ln("[1]  If f(x) = 4x - 3,  find f(5)", C_DIM);
    ln("[2]  y-intercept of  y = -2x + 7?", C_DIM);
    ln("[3]  Domain of  f(x) = sqrt(x - 4)?", C_DIM);
    bBlank();
    waitEnter();
}


// -- PROBABILITY ----------------------------------------------
static void lessonProbability() {
    cls();
    bTop("LESSON  |  PROBABILITY");

    sec("Basic Probability");
    ln("P(event) = favourable outcomes / total outcomes");
    ln("Range:  0 (impossible)  to  1 (certain)");
    bBlank();
    ex("P(rolling a 6) on fair die", "1/6  ~  0.167");
    ex("P(Ace) from 52 cards", "4/52 = 1/13  ~  0.077");

    sec("Combined Events");
    ln("P(A or B)   =  P(A) + P(B) - P(A and B)");
    ln("P(A and B)  =  P(A) * P(B)   [if independent]");
    ln("P(not A)    =  1 - P(A)");
    bBlank();
    ex("P(A)=0.3, P(B)=0.4, P(A&B)=0.1  =>  P(A or B)",
        "0.3 + 0.4 - 0.1 = 0.6");

    sec("Exercises");
    ln("[1]  Bag: 3 red, 5 green, 2 blue.  P(green)?", C_DIM);
    ln("[2]  Flip two coins.  P(both heads)?", C_DIM);
    ln("[3]  P(not drawing a king from 52 cards)?", C_DIM);
    bBlank();
    waitEnter();
}

// -- NUMBER THEORY --------------------------------------------
static void lessonNumberTheory() {
    cls();
    bTop("LESSON  |  NUMBER THEORY");

    sec("Prime Numbers");
    ln("A prime has exactly 2 divisors: 1 and itself.");
    ln("Primes:  2  3  5  7  11  13  17  19  23  29  31 ...");
    ln("Note: 1 is neither prime nor composite.");
    bBlank();

    sec("GCF and LCM");
    ln("GCF: largest divisor shared by all numbers.");
    ln("LCM: smallest multiple shared by all numbers.");
    ln("LCM(a, b)  =  (a * b) / GCF(a, b)");
    bBlank();
    ex("GCF(12, 18)   [12=2*2*3,  18=2*3*3]", "GCF = 2*3 = 6");
    ex("LCM(4, 6)     [GCF=2]", "(4*6)/2 = 12");

    sec("Divisibility Rules");
    ln("Div by 2 : last digit is even");
    ln("Div by 3 : digit sum is divisible by 3");
    ln("Div by 5 : last digit is 0 or 5");
    bBlank();

    sec("Factorials");
    ln("n! = n * (n-1) * ... * 2 * 1     and     0! = 1");
    ln("Examples:  3! = 6     5! = 120     6! = 720");
    bBlank();

    sec("Exercises");
    ln("[1]  List all primes less than 30", C_DIM);
    ln("[2]  Find GCF(24, 36)", C_DIM);
    ln("[3]  Calculate 7!", C_DIM);
    bBlank();
    waitEnter();
}

// -- Lesson menu ----------------------------------------------
void lessonMenu() {
    vector<string> opts = {
        "Algebra", "Geometry", "Functions",
        "Probability", "Number Theory", "Back"
    };
    while (true) {
        int c = menuSelect("LESSONS", opts, "Select a topic to study");
        switch (c) {
        case 0: lessonAlgebra();      break;
        case 1: lessonGeometry();     break;
        case 2: lessonFunctions();    break;
        case 3: lessonProbability();  break;
        case 4: lessonNumberTheory(); break;
        case 5: return;
        }
    }
}