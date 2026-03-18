// ============================================================
// account.cpp
// ============================================================
#include "../include/account.h"
#include "../include/utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// -- Serialisation --------------------------------------------
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

// -- File I/O -------------------------------------------------
vector<Account> loadAccounts() {
    vector<Account> v;
    ifstream f(ACC_FILE);
    string line;
    while (getline(f, line))
        if (!line.empty()) v.push_back(Account::fromCsv(line));
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

// -- Register / Login -----------------------------------------
Account doRegister(const string& u, const string& pw, const string& name) {
    if (u.size() < 3 || pw.size() < 4 || name.empty()) return {};
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

// -- Interactive screens --------------------------------------
Account screenRegister() {
    while (true) {
        cls();
        bTop("REGISTER");
        bBlank();
        bRow("  Create a new account to track your results.", C_DIM);
        bBlank();
        bRow("  Username  :  min. 3 characters", C_DIM);
        bRow("  Password  :  min. 4 characters", C_DIM);
        bRow("  Full name :  your real name", C_DIM);
        bBlank();
        bDiv();
        bRow(cen("Fill in the fields below"), C_DIM);
        bDiv();
        cout << "\n";

        string u = inputLine("Username ");
        string p = inputLine("Password ");
        string n = inputLine("Full name");

        transform(u.begin(), u.end(), u.begin(),
            [](unsigned char c) { return tolower(c); });

        Account acc = doRegister(u, p, n);

        cls();
        bTop("REGISTER");
        bBlank();
        if (!acc.empty()) {
            bRow(cen("Account created!  Welcome, " + acc.name + "!"), C_OK);
            bBlank();
            waitEnter();
            return acc;
        }
        string err = userExists(u)
            ? "Username '" + u + "' is already taken."
            : "Input too short. Check the requirements.";
        bRow(cen(err), C_ERR);
        bBlank();
        bDiv();
        bRow(cen("Press ENTER to try again"), C_DIM);
        bBot();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

