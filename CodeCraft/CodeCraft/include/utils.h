// ============================================================
// include/utils.h
// Display, menus, input helpers, grading
// Pure ASCII box - works on Windows CMD and any Linux terminal
// ============================================================
#pragma once
#include <string>
#include <vector>

using namespace std;

// ── ANSI colours ─────────────────────────────────────────────
#define C_RST  "\033[0m"
#define C_BDR  "\033[1;36m"   // border       - bold cyan
#define C_HDR  "\033[1;33m"   // header       - bold yellow
#define C_SEL  "\033[1;92m"   // selected     - bold bright-green
#define C_TXT  "\033[0;97m"   // normal text  - bright white
#define C_DIM  "\033[0;90m"   // dim / hint   - dark grey
#define C_ERR  "\033[1;31m"   // error        - bold red
#define C_SCR  "\033[1;95m"   // score/grade  - bold magenta
#define C_OK   "\033[1;32m"   // success      - bold green
#define C_BNR  "\033[1;96m"   // banner art   - bold bright-cyan

// Box inner width. Margin is computed at runtime from real console width.
static const int BW = 58;

// ── Platform init ─────────────────────────────────────────────
void initConsole();   // enables ANSI + UTF-8 on Windows

// ── Screen ───────────────────────────────────────────────────
void cls();
void waitEnter();     // draws footer then waits for ENTER

// ── Box primitives ───────────────────────────────────────────
void bTop(const string& title = "");
void bDiv();
void bBot();
void bBlank();
void bRow(const string& text, const string& col = C_TXT);

// ── EduRise banner ───────────────────────────────────────────
// Prints the big ASCII-art app name inside the box.
void bBanner();

// ── Arrow-key menu ───────────────────────────────────────────
// showBanner = true -> draws bBanner() at the top of the menu.
// Returns 0-based index of chosen option, or -1 if ESC pressed.
int menuSelect(const string& title,
    const vector<string>& opts,
    const string& sub = "",
    bool                  showBanner = false);

// ── Answer picker (test questions, returns 0-3 or -1 for ESC) ─
int answerPick(const string& question,
    const vector<string>& opts,
    const string& cat,
    int pts, int qNum, int total,
    int scoreNow, int scoreMax);

// ── Text input ───────────────────────────────────────────────
string inputLine(const string& prompt);

// ── Misc helpers ─────────────────────────────────────────────
string cen(const string& s, int w = BW);
string hashPwd(const string& pw);
double calcPct(int got, int maxPts);
int    toGrade(double p);
string gradeStr(int g);
string gradeMsg(int g);

// -- Program entry (called from main) --------------------
void mainMenu();
