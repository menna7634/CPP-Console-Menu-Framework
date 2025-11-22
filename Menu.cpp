#include "menu.h"
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;


int getKey() {
#ifdef _WIN32
    return _getch();
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    if (ch == 27 && getchar() == 91)  // arrow prefix
        ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[H";
#endif
}

void goToXY(int x, int y) {
#ifdef _WIN32
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
    cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
#endif
}


void drawMenu(const vector<string>& items, int selected) {
    clearScreen();

    cout << "======== MENU ========\n";
    for (int i = 0; i < items.size(); i++) {
        if (i == selected)
            cout << " > " << items[i] << "\n";
        else
            cout << "   " << items[i] << "\n";
    }
    cout << "======================\n";
    cout << "↑↓ to move | Enter select | ESC exit\n";
}

void runNew(string &stored) {
    clearScreen();
    cout << "Enter text (Enter = save, Backspace = cancel):\n\n";

    string s;
    bool running = true;

    while (running) {
        int c = getKey();

        if (c == 127 || c == 8) {
            running = false;   // cancel
        }
        else if (c == 13 || c == 10) {
            stored = s;        // save
            running = false;
        }
        else {
            s += (char)c;
            cout << (char)c;
        }
    }
}


void runDisplay(const string &stored) {
    clearScreen();

    if (stored.empty())
        cout << "No text stored.\n\n";
    else {
        cout << "Stored text:\n";
        cout << stored << "\n\n";
    }

    cout << "Press Enter or Backspace to return...";

    while (true) {
        int c = getKey();
        if (c == 13 || c == 10 || c == 127 || c == 8)
            return;
    }
}


void runMenu() {
    vector<string> items = { "New", "Display", "Exit" };
    int selected = 0;
    string storedText = "";

    while (true) {
        drawMenu(items, selected);
        int k = getKey();

        if (k == 27)  // ESC
            return;

        if (k == 'H' || k == 65) // up
            selected = (selected - 1 + items.size()) % items.size();

        else if (k == 'P' || k == 66) // down
            selected = (selected + 1) % items.size();

        else if (k == 13 || k == 10) { // ENTER
            if (items[selected] == "New")
                runNew(storedText);
            else if (items[selected] == "Display")
                runDisplay(storedText);
            else if (items[selected] == "Exit")
                return;
        }
    }
}
