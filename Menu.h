#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
using namespace std;

class Menu {
public:
    Menu();          
    void run();     

private:
    vector<string> items_;    
    int selectedIndex_;       
    string storedText_;       

    // Drawing 
    void drawMenu() const;           
    void drawTopNewButton() const;   
    void drawUsageLine() const;      

    // Key handling
    void handleKey(int key); 
    void onEnter();          
    void runNew();           
    void runDisplay();       

    int getInputKey();      

    void setColorSelected() const;   
    void setColorNormal() const;     
    void resetColor() const;        
    pair<int,int> getTerminalSize() const; 
    void clearScreen() const;        
    void goToXY(int x, int y) const; 
};

#endif
