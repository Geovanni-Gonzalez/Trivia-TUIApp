#include <iostream>
#include <windows.h>
#include "game.h"

int main(int argc, char* argv[]) {
    // Enable UTF-8 for Windows Console
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    Game g;
    g.start();

    return 0;
}
