// ============================================================
// src/mainMenu.cpp
// Single entry point called by main().
// Handles everything after program start:
//   - data folder creation
//   - console init
//   - auth screen (Login / Register / Exit)
//   - main menu loop (Lessons, Practice, Test, Results, Stats)
// ============================================================
#include "../include/utils.h"
#include "../include/account.h"
#include "../include/question.h"
#include "../include/lesson.h"
#include "../include/quiz.h"
#include "../include/statistics.h"
#include "../include/student.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

// -- Creates data/ folder if it does not already exist --------
static void ensureData() {
#ifdef _WIN32
    system("mkdir data 2>nul");
#else
    system("mkdir -p data 2>/dev/null");
#endif
}

// -- My Results sub-screen ------------------------------------
static void myResults(const Account& user) {
    auto list = resultsFor(user.username);
    cls();
    bTop("EduRise  |  My Results  --  " + user.name);

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

// -- Auth screen ----------------------------------------------
// Shows the EduRise banner + Login / Register / Exit.
// Returns the logged-in Account once credentials are accepted.
static Account authScreen() {
    while (true) {
        int c = menuSelect(
            "EduRise",
            { "Login", "Register", "Exit" },
            "Mathematics  |  Grade 9",
            true    // show the big EduRise ASCII-art banner
        );

        if (c == -1 || c == 2) {   // ESC or Exit
            cls();
            bTop("EduRise  |  Goodbye");
            bBlank();
            bRow(cen("Study hard and good luck!"), C_OK);
            bBlank();
            bBot();
            exit(0);
        }
        if (c == 0) {
            Account a = screenLogin();
            if (!a.empty()) return a;
        }
        if (c == 1) {
            Account a = screenRegister();
            if (!a.empty()) return a;
        }
    }
}

// -- Main menu loop -------------------------------------------
static void runMenu(const Account& user, const vector<Question>& bank) {
    while (true) {
        int c = menuSelect(
            "EduRise  |  Main Menu",
            {
                "View Lessons",
                "Practice Mode",
                "Take a Test",
                "My Results",
                "Global Statistics",
                "Logout"
            },
            "Logged in as: " + user.name,
            true    // show the EduRise ASCII-art banner
        );

        if (c == -1 || c == 5) return;   // ESC or Logout

        switch (c) {
        case 0:
            lessonMenu();
            break;

        case 1:
            practiceMode(bank);
            break;

        case 2: {
            vector<Question> test = generateTest(bank);
            TestResult result = runTest(test, user);
            if (!result.empty()) {
                cls();
                bTop("EduRise  |  Saving Result");
                bBlank();
                if (saveResult(result))
                    bRow(cen("Result saved successfully!"), C_OK);
                else
                    bRow(cen("Could not save result (check data/ folder)."), C_ERR);
                bBlank();
                waitEnter();
            }
            break;
        }

        case 3:
            myResults(user);
            break;

        case 4:
            showStats();
            break;
        }
    }
}

// -- mainMenu() -----------------------------------------------
// This is the function called directly from main().
// It sets everything up and runs the full program loop.
void mainMenu() {
    ensureData();
    initConsole();

    vector<Question> bank = makeBank();

    // Outer loop: auth -> menu -> back to auth on logout
    while (true) {
        Account user = authScreen();
        runMenu(user, bank);
    }
}
