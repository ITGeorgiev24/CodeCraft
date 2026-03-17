#include "utils.h"
#include "account.h"
#include "question.h"
#include "lesson.h"
#include "quiz.h"
#include "statistics.h"
#include "student.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <vector>

using namespace std;

// Creates data/ directory if it does not already exist
static void ensureData() {
#ifdef _WIN32
    system("mkdir data 2>nul");
#else
    system("mkdir -p data 2>/dev/null");
#endif
}

// -- Auth screen: shown before every login/register -----------
static Account authScreen() {
    while (true) {
        int c = menuSelect(
            "ELECTRONIC SCHOOL",
            { "Login", "Register", "Exit" },
            "Mathematics  |  Grade 9  |  PGCPI Burgas 2025/2026"
        );
        if (c == 0) { Account a = screenLogin();    if (!a.empty()) return a; }
        if (c == 1) { Account a = screenRegister(); if (!a.empty()) return a; }
        if (c == 2) {
            cls();
            bTop("GOODBYE");
            bBlank();
            bRow(cen("Study hard and good luck!"), C_OK);
            bBlank();
            bBot();
            exit(0);
        }
    }
}

// -- My Results screen ----------------------------------------
static void myResults(const Account& user) {
    auto list = resultsFor(user.username);
    cls();
    bTop("MY RESULTS  |  " + user.name);

    if (list.empty()) {
        bBlank();
        bRow(cen("No attempts yet -- take a test first!"), C_DIM);
        bBlank();
        waitEnter();
        return;
    }
    for (int i = 0; i < (int)list.size(); ++i) {
        bRow("  Attempt " + to_string(i + 1), C_HDR);
        list[i].display();
    }
    waitEnter();
}

// -- Main menu ------------------------------------------------
static void mainMenu(const Account& user,
    const vector<Question>& bank) {
    while (true) {
        int c = menuSelect(
            "MAIN MENU",
            { "View Lessons",
             "Practice Mode",
             "Take a Test",
             "My Results",
             "Global Statistics",
             "Logout" },
            "Logged in as: " + user.name
        );
        switch (c) {
        case 0: lessonMenu();             break;
        case 1: practiceMode(bank);       break;
        case 2: {
            vector<Question> test = generateTest(bank);
            TestResult result = runTest(test, user);
            cls();
            bTop("SAVING RESULT");
            bBlank();
            if (saveResult(result))
                bRow(cen("Result saved!"), C_OK);
            else
                bRow(cen("Could not save result (check data/ folder)."), C_ERR);
            bBlank();
            waitEnter();
            break;
        }
        case 3: myResults(user);          break;
        case 4: showStats();              break;
        case 5: return;
        }
    }
}

// -- Entry point ----------------------------------------------
int main() {
    ensureData();
    initConsole();

    vector<Question> bank = makeBank();

    while (true) {
        Account user = authScreen();
        mainMenu(user, bank);
    }
}