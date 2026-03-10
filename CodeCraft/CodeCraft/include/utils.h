#pragma once
#include <string>
#include <windows.h>

void clearScreen();
void setCursorPosition(int x, int y);
void getConsoleSize(int& width, int& height);
void printCenteredAt(std::string text, int y, WORD color);