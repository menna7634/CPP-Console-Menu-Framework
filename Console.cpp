#include "Console.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

namespace console {

//Clear Screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[H";
#endif
}

// Go To X,Y 
void goToXY(int x, int y) {
#ifdef _WIN32
    COORD c{(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
#else
    std::cout << "\033[" << y << ";" << x << "H";
#endif
}

// Colors 
void setColorSelected() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
#else
    std::cout << "\033[44;37m";
#endif
}

void setColorNormal() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#else
    cout << "\033[0m";
#endif
}

void resetColor() { setColorNormal(); }

// Delay
void delayMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

//Terminal Size 
pair<int,int> getTerminalSize() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return { csbi.srWindow.Right - csbi.srWindow.Left + 1,
             csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
#else
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return {w.ws_col, w.ws_row};
#endif
}

Key readKey() {
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 224) { 
        int s = _getch();
        switch(s) {
            case 72: return KeyUp;
            case 80: return KeyDown;
            case 71: return KeyHome;
            case 79: return KeyEnd;
        }
    }
    if (c == 13) return KeyEnter;
    if (c == 27) return KeyEsc;
    if (c == 'n' || c == 'N') return KeyTopNew;
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int c = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (c == 27) { 
        int n1 = getchar();
        if (n1 == '[') {
            int n2 = getchar();
            switch(n2) {
                case 'A': return KeyUp;
                case 'B': return KeyDown;
                case 'H': return KeyHome;
                case 'F': return KeyEnd;
            }
        }
        return KeyEsc;
    }

    if (c == 10) return KeyEnter;
    if (c == 'n' || c=='N') return KeyTopNew;
#endif

    return KeyNone;
}
}
