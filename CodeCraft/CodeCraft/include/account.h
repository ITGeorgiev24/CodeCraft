// ============================================================
// account.h  --  User accounts  (register / login)
// Passwords stored as djb2 hex hashes, never plain-text.
// ============================================================
#pragma once
#include <string>
#include <vector>

using namespace std;

struct Account {
    string username;    // unique, lowercase
    string pwHash;      // djb2 hex
    string name;        // display name

    bool   empty() const { return username.empty(); }
    string toCsv() const;
    static Account fromCsv(const string& line);
};