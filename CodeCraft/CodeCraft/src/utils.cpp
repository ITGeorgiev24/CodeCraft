// ============================================================
// src/utils.cpp
// ============================================================

// Must be first - stops <windows.h> defining min/max macros
#define NOMINMAX

// All standard includes before platform-specific ones
#include "../include/utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cstdlib>

// Platform-specific includes AFTER standard library
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

// using namespace after ALL includes
using namespace std;

// ── Key codes ────────────────────────────────────────────────
#define KEY_UP    1001
#define KEY_DOWN  1002
#define KEY_ENTER 1003
#define KEY_ESC   1004

// ── Console width ────────────────────────────────────────────
static int consoleWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
        return w.ws_col;
#endif
    return 80;
}

// ── Raw key reading ──────────────────────────────────────────
static int getKey() {
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 224) {
        c = _getch();
        if (c == 72) return KEY_UP;
        if (c == 80) return KEY_DOWN;
        return 0;
    }
    if (c == 13) return KEY_ENTER;
    if (c == 27) return KEY_ESC;
    return c;
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(unsigned)(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int c = getchar();
    if (c == 27) {
        int c2 = getchar();
        if (c2 == '[') {
            int c3 = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            if (c3 == 'A') return KEY_UP;
            if (c3 == 'B') return KEY_DOWN;
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return KEY_ESC;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if (c == 10) return KEY_ENTER;
    return c;
#endif
}

// ── Internal helpers ─────────────────────────────────────────

// Left margin to centre the box in the terminal
static int margin() {
    int m = (consoleWidth() - (BW + 2)) / 2;
    return (m > 0) ? m : 0;
}

// Repeat a character n times
static string rep(char c, int n) {
    return (n > 0) ? string(n, c) : string();
}

// ── Platform init ────────────────────────────────────────────
void initConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(h, &mode);
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

// ── Screen ───────────────────────────────────────────────────
void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitEnter() {
    cout << "\n";
    bRow(cen("Press ENTER to continue"), C_DIM);
    bBot();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ── Text centering ───────────────────────────────────────────
string cen(const string& s, int w) {
    int len = (int)s.size();
    if (len >= w) return s;
    int left = (w - len) / 2;
    int right = w - len - left;
    return string(left, ' ') + s + string(right, ' ');
}

// ── Box primitives ───────────────────────────────────────────
void bTop(const string& title) {
    int m = margin();
    if (title.empty()) {
        cout << rep(' ', m)
            << C_BDR "+" << rep('-', BW) << "+" C_RST "\n";
    }
    else {
        string t = " " + title + " ";
        int dashes = BW - (int)t.size();
        int dl = max(0, dashes / 2);
        int dr = max(0, dashes - dl);
        cout << rep(' ', m)
            << C_BDR "+" << rep('-', dl)
            << C_HDR << t
            << C_BDR << rep('-', dr) << "+" C_RST "\n";
    }
}

void bDiv() {
    cout << rep(' ', margin())
        << C_BDR "+" << rep('-', BW) << "+" C_RST "\n";
}

void bBot() { bDiv(); }

void bBlank() {
    cout << rep(' ', margin())
        << C_BDR "|" C_RST
        << rep(' ', BW)
        << C_BDR "|" C_RST "\n";
}

void bRow(const string& text, const string& col) {
    int    len = (int)text.size();
    int    padding = (len < BW) ? (BW - len) : 0;
    string content = (len > BW) ? text.substr(0, BW) : text;
    cout << rep(' ', margin())
        << C_BDR "|" C_RST
        << col << content << rep(' ', padding) << C_RST
        << C_BDR "|" C_RST "\n";
}

// ── EduRise ASCII-art banner ─────────────────────────────────
void bBanner() {
    static const char* ART[] = {
        "  ___     _       ___  _           ",
        " | __| __| |_  _ | _ \\(_)___ ___   ",
        " | _| / _` | || ||   /| (_-</ -_)  ",
        " |___|\\__,_|\\_,_||_|_\\|_/__/\\___|  "
    };
    bBlank();
    for (int i = 0; i < 4; ++i)
        bRow(cen(string(ART[i])), C_BNR);
    bBlank();
    bRow(cen("Learn  .  Practice  .  Succeed"), C_DIM);
    bBlank();
}

// ── Arrow-key menu ───────────────────────────────────────────
int menuSelect(const string& title,
    const vector<string>& opts,
    const string& sub,
    bool showBanner) {
    int sel = 0;
    int n = (int)opts.size();

    while (true) {
        cls();
        bTop(title);
        if (showBanner) {
            bBanner();
            bDiv();
        }
        if (!sub.empty()) {
            bRow(cen(sub), C_DIM);
            bDiv();
        }
        bBlank();
        for (int i = 0; i < n; ++i) {
            if (i == sel)
                // Selected - centred with >> << in bright green
                bRow(cen(">>  " + opts[i] + "  <<"), C_SEL);
            else
                // Unselected - same centre position, no markers
                bRow(cen("    " + opts[i] + "    "), C_TXT);
        }
        bBlank();
        bDiv();
        bRow(cen("[ UP/DOWN ] Move   [ ENTER ] Select   [ ESC ] Back"), C_DIM);
        bBot();

        int k = getKey();
        if (k == KEY_UP)    sel = (sel - 1 + n) % n;
        if (k == KEY_DOWN)  sel = (sel + 1) % n;
        if (k == KEY_ENTER) return sel;
        if (k == KEY_ESC)   return -1;
    }
}

// ── Answer picker ────────────────────────────────────────────
int answerPick(const string& question,
    const vector<string>& opts,
    const string& cat,
    int pts, int qNum, int total,
    int scoreNow, int scoreMax) {
    int sel = 0;
    const string LETTERS = "ABCD";

    while (true) {
        cls();
        bTop("Question " + to_string(qNum) + " of " + to_string(total));
        bRow(cen(cat + "  |  " + to_string(pts)
            + (pts > 1 ? " pts" : " pt")), C_DIM);
        bDiv();

        // Word-wrap the question at BW-4 chars per line
        string q = question;
        int    lineW = BW - 4;
        while (!q.empty()) {
            int take = min((int)q.size(), lineW);
            if (take < (int)q.size() && q[take] != ' ') {
                int sp = (int)q.rfind(' ', (size_t)take);
                if (sp > 0) take = sp;
            }
            bRow("  " + q.substr(0, (size_t)take), C_TXT);
            size_t skip = (size_t)take;
            if (skip < q.size() && q[skip] == ' ') ++skip;
            q = q.substr(skip);
        }

        bDiv();
        bBlank();
        for (int i = 0; i < 4; ++i) {
            string optText = string(1, LETTERS[i]) + ")  " + opts[i];
            if (i == sel)
                bRow(cen(">>  " + optText + "  <<"), C_SEL);
            else
                bRow(cen("    " + optText + "    "), C_TXT);
        }
        bBlank();
        bDiv();
        bRow(" Score: " + to_string(scoreNow) + "/" + to_string(scoreMax)
            + "   [ UP/DOWN ] Move   [ ENTER ] Confirm   [ ESC ] Menu",
            C_DIM);
        bBot();

        int k = getKey();
        if (k == KEY_UP)    sel = (sel - 1 + 4) % 4;
        if (k == KEY_DOWN)  sel = (sel + 1) % 4;
        if (k == KEY_ENTER) return sel;
        if (k == KEY_ESC)   return -1;
    }
}

// ── Text input ───────────────────────────────────────────────
string inputLine(const string& prompt) {
    int m = margin();
    cout << rep(' ', m)
        << C_BDR "| " C_TXT
        << left << setw(16) << prompt
        << C_SEL " >> " C_RST;
    string s;
    getline(cin, s);
    return s;
}

// ── Password hash (djb2) ─────────────────────────────────────
string hashPwd(const string& pw) {
    unsigned long long h = 5381ULL;
    for (unsigned char c : pw) h = ((h << 5) + h) + c;
    ostringstream o;
    o << hex << setfill('0') << setw(16) << h;
    return o.str();
}

// ── Grading ──────────────────────────────────────────────────
double calcPct(int got, int maxPts) {
    return maxPts ? (got * 100.0 / maxPts) : 0.0;
}

int toGrade(double p) {
    if (p >= 85) return 6;
    if (p >= 65) return 5;
    if (p >= 50) return 4;
    if (p >= 30) return 3;
    return 2;
}

string gradeStr(int g) {
    switch (g) {
    case 6:  return "6  -  Excellent   (Otlichen)";
    case 5:  return "5  -  Very Good   (Mnogo Dobar)";
    case 4:  return "4  -  Good        (Dobar)";
    case 3:  return "3  -  Average     (Sreden)";
    default: return "2  -  Poor        (Slab)";
    }
}

string gradeMsg(int g) {
    switch (g) {
    case 6:  return "Outstanding! Excellent mastery.";
    case 5:  return "Very good! Strong understanding.";
    case 4:  return "Good - review the questions you missed.";
    case 3:  return "Passed, but significant revision needed.";
    default: return "Did not pass. Study the lessons and retry.";
    }
}
