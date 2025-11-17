#ifndef CONSOLE_H
#define CONSOLE_H

#include <utility>
using namespace std;
namespace console {

enum Key {
    KeyNone,
    KeyUp,
    KeyDown,
    KeyEnter,
    KeyEsc,
    KeyHome,
    KeyEnd,
    KeyTopNew  
};

void clearScreen();
void goToXY(int x, int y);
void setColorSelected();
void setColorNormal();
void resetColor();
void delayMs(int ms);
pair<int,int> getTerminalSize();

Key readKey();

}

#endif
