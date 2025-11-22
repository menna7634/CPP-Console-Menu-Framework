#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
using namespace std;

void clearScreen();
void goToXY(int x, int y);
int  getKey();

void drawMenu(const vector<string>& items, int selected);
void runMenu();
void runNew(string &stored);
void runDisplay(const string &stored);

#endif
