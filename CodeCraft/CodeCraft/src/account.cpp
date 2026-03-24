#include "../include/account.h"
#include "../include/utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>
#include <conio.h>

using namespace std;

// ── ESC-aware centered input ──────────────────────────────────
string inputLineEsc(const string& label) {
    string input;

    while (true) {
        cls();
        bTop("EduRise");
        bBlank();

        bRow(cen("Fill in the fields below (ESC to go back)"), C_DIM);
        bBlank();

        string display = label + ": " + input;
        bRow(cen(display));

        bBlank();
        bDiv();
        bRow(cen("Press ENTER to confirm"), C_DIM);
        bBot();

        char ch = _getch();

        if (ch == 27) return "__ESC__";
        else if (ch == '\r') return input;
        else if (ch == '\b') {
            if (!input.empty()) input.pop_back();
        }
        else {
            input += ch;
        }
    }
}

// ── Serialisation ────────────────────────────────────────────
string Account::toCsv() const {
    return username + "|" + pwHash + "|" + name;
}

Account Account::fromCsv(const string& line) {
    Account a;
    istringstream ss(line);
    getline(ss, a.username, '|');
    getline(ss, a.pwHash, '|');
    getline(ss, a.name, '|');
    return a;
}

// ── File I/O ─────────────────────────────────────────────────
vector<Account> loadAccounts() {
    vector<Account> v;
    ifstream f(ACC_FILE);
    string line;
    while (getline(f, line))
        if (!line.empty())
            v.push_back(Account::fromCsv(line));
    return v;
}

bool saveAccounts(const vector<Account>& v) {
    ofstream f(ACC_FILE, ios::trunc);
    for (const auto& a : v) f << a.toCsv() << "\n";
    return f.good();
}

bool userExists(const string& u) {
    for (const auto& a : loadAccounts())
        if (a.username == u) return true;
    return false;
}

// ── Register / Login ─────────────────────────────────────────
Account doRegister(const string& u, const string& pw, const string& name) {
    if ((int)u.size() < 3) return {};
    if ((int)pw.size() < 4) return {};
    if (name.empty()) return {};
    if (userExists(u)) return {};

    Account a{ u, hashPwd(pw), name };
    auto v = loadAccounts();
    v.push_back(a);
    saveAccounts(v);
    return a;
}

Account doLogin(const string& u, const string& pw) {
    for (const auto& a : loadAccounts())
        if (a.username == u && a.pwHash == hashPwd(pw)) return a;
    return {};
}

// ── Interactive screens ──────────────────────────────────────
Account screenRegister() {
    while (true) {
        cls();
        bTop("EduRise  |  Register");
        bBlank();
        bRow("  Create a new account to track your progress.", C_DIM);
        bBlank();
        bRow("  Username  :  minimum 3 characters", C_DIM);
        bRow("  Password  :  minimum 4 characters", C_DIM);
        bRow("  Full name :  your real name", C_DIM);
        bBlank();
        bDiv();
        bRow(cen("Fill in the fields below (ESC to go back)"), C_DIM);
        bDiv();

        string u = inputLineEsc("Username");
        if (u == "__ESC__") return {};

        string p = inputLineEsc("Password");
        if (p == "__ESC__") return {};

        string n = inputLineEsc("Full name");
        if (n == "__ESC__") return {};

        transform(u.begin(), u.end(), u.begin(),
            [](unsigned char c) { return (char)tolower(c); });

        Account acc = doRegister(u, p, n);

        cls();
        bTop("EduRise  |  Register");
        bBlank();

        if (!acc.empty()) {
            bRow(cen("Account created!  Welcome, " + acc.name + "!"), C_OK);
            bBlank();
            waitEnter();
            return acc;
        }

        string err = userExists(u)
            ? "Username '" + u + "' is already taken."
            : "Input too short. Please check the requirements.";

        bRow(cen(err), C_ERR);
        bBlank();
        bDiv();
        bRow(cen("Press ENTER to try again (ESC to go back)"), C_DIM);
        bBot();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

Account screenLogin() {
    int attempts = 0;

    while (attempts < 5) {
        cls();
        bTop("EduRise  |  Login");
        bBlank();
        bRow("  Enter your username and password.", C_DIM);
        bBlank();
        bDiv();
        bRow(cen("Fill in the fields below (ESC to go back)"), C_DIM);
        bDiv();

        string u = inputLineEsc("Username");
        if (u == "__ESC__") return {};

        string p = inputLineEsc("Password");
        if (p == "__ESC__") return {};

        transform(u.begin(), u.end(), u.begin(),
            [](unsigned char c) { return (char)tolower(c); });

        Account acc = doLogin(u, p);

        cls();
        bTop("EduRise  |  Login");
        bBlank();

        if (!acc.empty()) {
            bRow(cen("Welcome back, " + acc.name + "!"), C_OK);
            bBlank();
            waitEnter();
            return acc;
        }

        ++attempts;
        bRow(cen("Invalid username or password."), C_ERR);
        bBlank();
        bDiv();
        bRow(cen("Press ENTER to try again (ESC to go back)"), C_DIM);
        bBot();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return {};
}