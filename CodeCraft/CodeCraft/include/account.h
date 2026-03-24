#pragma once
#include <string>
#include <vector>

using namespace std;

static const string ACC_FILE = "data/accounts.txt";

struct Account {
    string username;
    string pwHash;
    string name;

    bool   empty()  const { return username.empty(); }
    string toCsv()  const;
    static Account fromCsv(const string& line);
};

vector<Account> loadAccounts();
bool            saveAccounts(const vector<Account>& v);
bool            userExists(const string& u);

Account doRegister(const string& u, const string& pw, const string& name);
Account doLogin(const string& u, const string& pw);

Account screenRegister();
Account screenLogin();
