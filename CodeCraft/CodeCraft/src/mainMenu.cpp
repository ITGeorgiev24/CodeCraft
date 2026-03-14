#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include "../include/mainMenu.h"
using namespace std;
void clearScreen() {
    system("cls");
}
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void getConsoleSize(int& width, int& height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
void printCenteredAt(string text, int y, WORD color = 7) {
    int width, height;
    getConsoleSize(width, height);
    int x = (width - (int)text.length()) / 2;
    if (x < 0) x = 0;
    setCursorPosition(x, y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    cout << text;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
void drawStarBackground(int starCount = 180) {
    int width, height;
    getConsoleSize(width, height);
    char stars[] = { '.', '*', '+', '\'', 'o' };
    for (int i = 0; i < starCount; i++) {
        int x = rand() % width;
        int y = rand() % height;
        setCursorPosition(x, y);
        WORD colors[] = {
            FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // cyan
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // yellow
            FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // purple
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY // white

        };

        WORD color = colors[rand() % 5];

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

        cout << stars[rand() % 5];

    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

}

void displayTitle(int startY) {

    WORD cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    WORD yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    printCenteredAt("  _______    _      _____   _   _____  ______  ", startY++, cyan);

    printCenteredAt(" |  ____|  | |     |  __ \\ | | / ____||  ____| ", startY++, cyan);

    printCenteredAt(" | |__   __| |_   _| |__) || || (___  | |__    ", startY++, cyan);

    printCenteredAt(" |  __| / _` | | | |  _  / | | \\___ \\ |  __|   ", startY++, cyan);

    printCenteredAt(" | |___| (_| | |_| | | \\ \\ | | ____) || |____  ", startY++, cyan);

    printCenteredAt(" |______\\__,_|\\__,||_|  \\_\\|_||_____/ |______| ", startY++, cyan);

    startY++;

    printCenteredAt("~ English Language School ~", startY, yellow);

}

void displayMenu(int selectedOption) {

    WORD cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    WORD yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    WORD green = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    WORD purple = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    clearScreen();

    drawStarBackground();

    int width, height;

    getConsoleSize(width, height);

    int startY = (height - 20) / 2;

    if (startY < 0) startY = 0;

    displayTitle(startY);

    startY += 8;

    printCenteredAt("+==================================================+", startY++, cyan);

    printCenteredAt("|                                                  |", startY++, yellow);

    printCenteredAt("|              M A I N   M E N U                   |", startY++, yellow);

    printCenteredAt("|                                                  |", startY++, yellow);

    printCenteredAt("+==================================================+", startY++, cyan);

    string menuItems[] = {

        "Lessons",

        "Help",

        "Leaderboard",

        "Settings(?)",

        "Exit"

    };

    for (int i = 0; i < 5; i++) {

        string prefix = (i == selectedOption) ? " >>  " : "     ";

        string line = "|" + prefix + menuItems[i];

        while (line.length() < 51) line += " ";

        line += "|";

        printCenteredAt(line, startY++);

    }

    printCenteredAt("|                                                  |", startY++);

    printCenteredAt("+==================================================+", startY++, cyan);

    startY += 2;

    printCenteredAt("Use UP/DOWN arrows | ENTER to select | ESC to exit", startY, purple);

}

void mainMenu() {

    int selectedOption = 0;

    bool running = true;

    WORD cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    WORD yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    WORD green = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    while (running) {

        displayMenu(selectedOption);

        int key = _getch();

        if (key == 0 || key == 224) {

            key = _getch();

            if (key == 72) {

                selectedOption--;

                if (selectedOption < 0) selectedOption = 4;

            }

            else if (key == 80) {

                selectedOption++;

                if (selectedOption > 4) selectedOption = 0;

            }

        }

        else if (key == 13) {

            switch (selectedOption) {

            case 4:

                clearScreen();

                int width, height;

                getConsoleSize(width, height);

                int y = height / 2;

                printCenteredAt("+==================================+", y++, cyan);

                printCenteredAt("|   Thank you for using EduRise!   |", y++, yellow);

                printCenteredAt("|   Hope we meet you again soon!   |", y++, green);

                printCenteredAt("+==================================+", y++, cyan);

                Sleep(2000);

                running = false;

                break;

            }

        }

        else if (key == 27) {

            clearScreen();

            int width, height;

            getConsoleSize(width, height);
            printCenteredAt("Until we meet again...", height / 2,
                FOREGROUND_RED | FOREGROUND_INTENSITY);

            Sleep(1000);

            running = false;

        }

    }

}
