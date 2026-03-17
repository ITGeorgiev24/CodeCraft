#include "utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cstdlib>

// ?? Platform-specific key reading ????????????????????????????
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
static int getKey() {
    int c = _getch();
    if (c == 0 || c == 224) {      // special-key prefix
        c = _getch();
        if (c == 72) return 1001;  // UP
        if (c == 80) return 1002;  // DOWN
        return 0;
    }
    if (c == 13) return 1003;      // ENTER
    if (c == 27) return 1004;      // ESC
    return c;
}
#else
#include <termios.h>
#include <unistd.h>
static int getKey() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~static_cast<unsigned int>(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int c = getchar();
    if (c == 27) {
        int c2 = getchar();
        if (c2 == '[') {
            int c3 = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            if (c3 == 'A') return 1001;
            if (c3 == 'B') return 1002;
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return 1004;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if (c == 10) return 1003;
    return c;
}
#endif

#define KEY_UP    1001
#define KEY_DOWN  1002
#define KEY_ENTER 1003

// ?? Platform init ????????????????????????????????????????????
void initConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);   // UTF-8
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(h, &mode);
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

// ?? Screen ???????????????????????????????????????????????????
void cls() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitEnter() {
    std::cout << "\n";
    bRow(cen("Press  ENTER  to continue"), C_DIM);
    bBot();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ?? Helpers ??????????????????????????????????????????????????
static std::string rep(const std::string& s, int n) {
    std::string r;
    for (int i = 0; i < n; ++i) r += s;
    return r;
}

std::string cen(const std::string& s, int w) {
    int len = static_cast<int>(s.size());
    if (len >= w) return s;
    int l = (w - len) / 2;
    int r = w - len - l;
    return std::string(l, ' ') + s + std::string(r, ' ');
}

// ?? Box primitives ???????????????????????????????????????????
void bTop(const std::string& title) {
    std::cout << std::string(ML, ' ')
        << C_BDR "?" << rep("?", BW) << "?" C_RST "\n";
    if (!title.empty()) {
        bRow(cen(title), C_HDR);
        bDiv();
    }
}

void bDiv() {
    std::cout << std::string(ML, ' ')
        << C_BDR "?" << rep("?", BW) << "?" C_RST "\n";
}

void bBot() {
    std::cout << std::string(ML, ' ')
        << C_BDR "?" << rep("?", BW) << "?" C_RST "\n";
}

void bBlank() {
    std::cout << std::string(ML, ' ')
        << C_BDR "?" C_RST
        << std::string(BW, ' ')
        << C_BDR "?" C_RST "\n";
}

void bRow(const std::string& text, const std::string& col, bool centre) {
    std::string content = centre ? cen(text) : text;
    int len = static_cast<int>(content.size());
    int padding = (len < BW) ? (BW - len) : 0;
    if (len > BW) content = content.substr(0, BW);

    std::cout << std::string(ML, ' ')
        << C_BDR "?" C_RST
        << col << content << std::string(padding, ' ') << C_RST
        << C_BDR "?" C_RST "\n";
}

// ?? Arrow-key menu ???????????????????????????????????????????
int menuSelect(const std::string& title,
    const std::vector<std::string>& opts,
    const std::string& sub) {
    int sel = 0;
    int n = static_cast<int>(opts.size());

    while (true) {
        cls();
        bTop(title);
        if (!sub.empty()) {
            bRow(cen(sub), C_DIM);
            bDiv();
        }
        bBlank();
        for (int i = 0; i < n; ++i) {
            std::string row = (i == sel ? "  \xE2\x80\xBA " : "    ") + opts[i];
            bRow(row, i == sel ? C_SEL : C_TXT);
        }
        bBlank();
        bDiv();
        bRow(cen("\xe2\x86\x91 \xe2\x86\x93  Navigate      ENTER  Select"), C_DIM);
        bBot();

        int k = getKey();
        if (k == KEY_UP)    sel = (sel - 1 + n) % n;
        if (k == KEY_DOWN)  sel = (sel + 1) % n;
        if (k == KEY_ENTER) return sel;
    }
}

// ?? Arrow-key answer picker ??????????????????????????????????
int answerPick(const std::string& question,
    const std::vector<std::string>& opts,
    const std::string& cat,
    int pts, int qNum, int total,
    int scoreNow, int scoreMax) {
    int sel = 0;
    const std::string letters = "ABCD";

    while (true) {
        cls();
        // Title row
        std::string qTitle = "Question " + std::to_string(qNum)
            + " / " + std::to_string(total);
        bTop(qTitle);
        bRow(cen(cat + "  \xC2\xB7  " + std::to_string(pts)
            + (pts > 1 ? " pts" : " pt")), C_DIM);
        bDiv();

        // Word-wrap the question text into BW-4 columns
        std::string q = question;
        int lineW = BW - 4;
        while (!q.empty()) {
            int take = std::min(static_cast<int>(q.size()), lineW);
            if (take < static_cast<int>(q.size()) && q[take] != ' ') {
                int sp = static_cast<int>(q.rfind(' ', static_cast<size_t>(take)));
                if (sp > 0) take = sp;
            }
            bRow("  " + q.substr(0, static_cast<size_t>(take)), C_TXT);
            size_t skip = static_cast<size_t>(take);
            if (skip < q.size() && q[skip] == ' ') ++skip;
            q = q.substr(skip);
        }

        bDiv();
        bBlank();
        for (int i = 0; i < 4; ++i) {
            std::string row = (i == sel ? "  \xE2\x80\xBA " : "    ")
                + std::string(1, letters[i]) + ")  " + opts[i];
            bRow(row, i == sel ? C_SEL : C_TXT);
        }
        bBlank();
        bDiv();
        std::string scoreInfo = "  Score: "
            + std::to_string(scoreNow) + "/" + std::to_string(scoreMax);
        bRow(scoreInfo + std::string(BW - scoreInfo.size() - 20, ' ')
            + "\xe2\x86\x91\xe2\x86\x93 Move  ENTER Confirm", C_DIM);
        bBot();

        int k = getKey();
        if (k == KEY_UP)    sel = (sel - 1 + 4) % 4;
        if (k == KEY_DOWN)  sel = (sel + 1) % 4;
        if (k == KEY_ENTER) return sel;
    }
}

// ?? Text input ???????????????????????????????????????????????
std::string inputLine(const std::string& prompt) {
    std::cout << std::string(ML, ' ')
        << C_BDR "  " C_TXT << prompt
        << C_SEL "  \xE2\x80\xBA  " C_RST;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

// ?? Password hash (djb2) ?????????????????????????????????????
std::string hashPwd(const std::string& pw) {
    unsigned long long h = 5381ULL;
    for (unsigned char c : pw) h = ((h << 5) + h) + c;
    std::ostringstream o;
    o << std::hex << std::setfill('0') << std::setw(16) << h;
    return o.str();
}

// ?? Grading ??????????????????????????????????????????????????
double pct(int got, int max) {
    return max ? (got * 100.0 / max) : 0.0;
}

int toGrade(double p) {
    if (p >= 85) return 6;
    if (p >= 65) return 5;
    if (p >= 50) return 4;
    if (p >= 30) return 3;
    return 2;
}

std::string gradeStr(int g) {
    switch (g) {
    case 6: return "6  -  Excellent   (Otlichen)";
    case 5: return "5  -  Very Good   (Mnogo Dobar)";
    case 4: return "4  -  Good        (Dobar)";
    case 3: return "3  -  Average     (Sreden)";
    default:return "2  -  Poor        (Slab)";
    }
}

std::string gradeMsg(int g) {
    switch (g) {
    case 6: return "Outstanding! Excellent mastery.";
    case 5: return "Very good! Strong understanding.";
    case 4: return "Good – review the questions you missed.";
    case 3: return "Passed, but significant revision needed.";
    default:return "Did not pass. Study the lessons and retry.";
    }
}