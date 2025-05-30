#include "Game.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <cctype>
#include <windows.h>

void clearConsoleScreen() {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void gotoXY_main(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hConsole, pos);
}

void setColor_main(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    bool playAgain = true;

    while (playAgain) {
        TetrisGame game;
        game.run();

        gotoXY_main(28, 12);
        setColor_main(15);
        std::cout << "Play Again? (Y/N): ";

        char choice = ' ';
        while (true) {
            if (_kbhit()) {
                choice = std::tolower(_getch());
                if (choice == 'y' || choice == 'n') {
                    break;
                }
            }
            Sleep(50);
        }

        if (choice == 'y') {
            playAgain = true;
            clearConsoleScreen();
        }
        else {
            playAgain = false;
        }
    }

    clearConsoleScreen();
    gotoXY_main(25, 12);
    setColor_main(15);
    std::cout << "Thanks for playing! Exiting..." << std::endl;
    Sleep(2000);

    return 0;
}