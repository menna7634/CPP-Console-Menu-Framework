#include "Menu.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

string storedText = "";

// ---------- Console helpers ----------
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[H"; // clear screen + move cursor top-left
#endif
    cout.flush();
}

void goToXY(int x, int y) {
#ifdef _WIN32
    COORD pos{ (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
    cout << "\033[" << (y+1) << ";" << (x+1) << "H";
#endif
}

// Single char input cross-platform
char getChar() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    char c;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

// Arrow/Home/End/Enter/ESC
int getKey() {
#ifdef _WIN32
    int c = _getch();
    if (c == 224) c = 1000 + _getch(); // special keys
    return c;
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int c = getchar();
    if (c == 27) { // ESC sequences
        if (getchar() == '[') {
            int n = getchar();
            c = 1000 + n;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
#endif
}

// ---------- Menu functions ----------
void drawMenu(const string items[], int count, int selected) {
    clearScreen();
    cout << "======== MENU ========\n";
    for (int i = 0; i < count; i++) {
        if (i == selected) cout << " > " << items[i] << "\n";
        else cout << "   " << items[i] << "\n";
    }
    cout << "======================\n";
    cout << "↑↓ move | Enter select | ESC exit | Home New | End Exit\n";
}

void runNew() {
    goToXY(0, 6);
    cout << "Enter text (Enter=save, Backspace=delete, ESC=cancel):\n> ";
    string input;
    bool running = true;

    while (running) {
        char c = getChar();

        if (c == 27) { // ESC cancel
            running = false;
        }
        else if (c == 10 || c == 13) { // Enter save
            storedText = input;
            running = false;
        }
        else if (c == 8 || c == 127) { // Backspace
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
            }
        }
        else {
            input += c;
            cout << c;
        }
    }
}

void runDisplay() {
    clearScreen();
    if (storedText.empty()) cout << "No stored text.\n";
    else cout << "Stored text: " << storedText << "\n";

    cout << "\nPress ENTER to return...";
    bool waiting = true;
    while (waiting) {
        int k = getKey();
        if (k == 10 || k == 13) waiting = false;
    }
}

void runMenu() {
    const string items[] = { "New", "Display", "Exit" };
    const int count = 3;
    int selected = 0;
    bool running = true;

    while (running) {
        drawMenu(items, count, selected);
        int k = getKey();

        if (k == 27) running = false;               // ESC
        else if (k == 1000+65 || k == 72)           // Up
            selected = (selected - 1 + count) % count;
        else if (k == 1000+66 || k == 80)           // Down
            selected = (selected + 1) % count;
        else if (k == 1000+72) selected = 0;        // Home → New
        else if (k == 1000+70) selected = count-1;  // End → Exit
        else if (k == 10 || k == 13) {              // Enter
            if (items[selected] == "New") runNew();
            else if (items[selected] == "Display") runDisplay();
            else running = false;                    // Exit
        }
    }
    clearScreen(); 
}
