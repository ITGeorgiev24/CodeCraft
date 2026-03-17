// ============================================================
// utils.h  –  display, arrow-key menus, input, helpers
// ============================================================
#pragma once
#include <string>
#include <vector>

// ── ANSI colours ─────────────────────────────────────────────
#define C_RST  "\033[0m"
#define C_BDR  "\033[1;36m"   // border       – bold cyan
#define C_HDR  "\033[1;33m"   // header text  – bold yellow
#define C_SEL  "\033[1;92m"   // selected row – bold bright-green
#define C_TXT  "\033[0;97m"   // body text    – bright white
#define C_DIM  "\033[0;90m"   // hint / dim   – dark grey
#define C_ERR  "\033[1;31m"   // error        – bold red
#define C_SCR  "\033[1;95m"   // score/grade  – bold magenta
#define C_OK   "\033[1;32m"   // success      – bold green

// ── Layout ───────────────────────────────────────────────────
// Box 60 wide (2 borders + 58 inner); margin = (80-60)/2 = 10.
static constexpr int BW = 58;
static constexpr int ML = 10;

// ── Platform init ────────────────────────────────────────────
void initConsole();

// ── Screen ───────────────────────────────────────────────────
void cls();
void waitEnter();

// ── Box primitives ───────────────────────────────────────────
void bTop(const std::string& title = "");
void bDiv();
void bBot();
void bBlank();
void bRow(const std::string& text,
    const std::string& col = C_TXT,
    bool               centre = false);

// ── Arrow-key menus ──────────────────────────────────────────
int menuSelect(const std::string& title,
    const std::vector<std::string>& opts,
    const std::string& sub = "");

int answerPick(const std::string& question,
    const std::vector<std::string>& opts,
    const std::string& cat,
    int pts, int qNum, int total,
    int scoreNow, int scoreMax);

// ── Input ────────────────────────────────────────────────────
std::string inputLine(const std::string& prompt);

// ── Misc helpers ─────────────────────────────────────────────
std::string cen(const std::string& s, int w = BW);
std::string hashPwd(const std::string& pw);
double      pct(int got, int max);
int         toGrade(double p);
std::string gradeStr(int g);
std::string gradeMsg(int g);
