#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h> // dla _kbhit() i _getch()
#include <windows.h>
#include "Deklaracje.h"
using namespace std;


int main() {

    hideCursor();
    Game game;
    game.run();
    return 0;
}