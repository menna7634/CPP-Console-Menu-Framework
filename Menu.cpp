#include "Menu.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
using namespace std;

Menu::Menu()
    : items_({"New", "Display", "Exit"}), selectedIndex_(0) {}

int Menu::getInputKey() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    if (ch == 27) {
        getchar();
        ch = getchar();
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

//  Console helpers
void Menu::clearScreen() const {
    cout << "\033[2J\033[H"; // clear + go home
}

void Menu::goToXY(int x, int y) const {
    cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}

//  Draw Menu
void Menu::drawMenu() const {
    clearScreen();

    cout << "\033[1;34m╔════════ MENU ═══════╗\033[0m\n";

    for (int i = 0; i < items_.size(); ++i) {
        if (i == selectedIndex_) {
            cout << " \033[1;33m›\033[0m \033[1;32m" << items_[i] << "\033[0m\n";
        } else {
            cout << "   " << items_[i] << "\n";
        }
    }

    cout << "\033[1;34m╚═════════════════════╝\033[0m\n";
    cout << "↑↓ Move | Enter Select | Esc Exit | Backspace Main | Home New | End Exit \n";
}

void Menu::run() {
    int key = 0;

    while (!(key == 27 || (key == 10 && items_[selectedIndex_] == "Exit"))) {
        drawMenu();
        key = getInputKey();
        handleKey(key);  
    }
}



//  Key Handling
void Menu::handleKey(int key) {
    switch (key) {
        case 65: // up
            selectedIndex_ = (selectedIndex_ - 1 + items_.size()) % items_.size();
            break;

        case 66: // down
            selectedIndex_ = (selectedIndex_ + 1) % items_.size();
            break;

        case 72: // HOME
            selectedIndex_ = 0;
            break;

        case 70: // END
            selectedIndex_ = items_.size() - 1;
            break;

        case 10: // enter
            onEnter();
            break;

        case 127: // backspace
            // just return to menu
            break;

        case 27: // esc
            exit(0);
    }
}


//  Enter Action
void Menu::onEnter() {
    string sel = items_[selectedIndex_];

    if (sel == "New")      runNew();
    else if (sel == "Display") runDisplay();
    else if (sel == "Exit") exit(0);
}

//  NEW 
void Menu::runNew() {
    clearScreen();
    cout << "Enter text (Enter = save, Backspace = cancel):\n\n";

    string input;

    for (bool running = true; running; ) {
        int c = getInputKey();

        if (c == 127) { // cancel
            cout << "\nCancelled.\n";
            sleep(1);
            running = false;
        }
        else if (c == 10) { // save
            storedText_ = input;
            cout << "\nSaved.\n";
            sleep(1);
            running = false;
        }
        else {
            input += c;
            cout << (char)c;
            cout.flush();
        }
    }
}


//  DISPLAY — show stored text
void Menu::runDisplay() {
    clearScreen();

    if (storedText_.empty())
        cout << "No text stored.\n\n";
    else
        cout << "Stored text:\n" << storedText_ << "\n\n";

    cout << "Press Enter or Backspace to return...";

    while (true) {
        int c = getInputKey();
        if (c == 10 || c == 127) return;
    }
}
