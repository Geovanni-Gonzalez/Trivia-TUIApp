#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h> // rand, srand
#include <time.h>   // time
#include <iostream>
#include <windows.h> // SetConsoleTextAttribute, Beep, Sleep, GetTickCount
#include <conio.h>   // _getch, _kbhit

void init_random() {
    srand((unsigned int)time(NULL));
}

// Returns random number between min and max (inclusive)
int random_int(int min, int max) {
    if (max < min) return min;
    return min + (rand() % (max - min + 1));
}

void clear_screen() {
    system("cls");
}

void pause_screen() {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.clear();
    std::cin.sync();
    std::cin.ignore(1000, '\n');
    std::cin.get();
}

// Robust input for integers
int get_int(int min, int max) {
    int val;
    while (true) {
        std::cout << " > ";
        if (std::cin >> val) {
            if (val >= min && val <= max) {
                std::cin.ignore(1000, '\n');
                return val;
            }
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << " (!) Entrada invalida. Intente de nuevo (" << min << "-" << max << "): ";
    }
}

// --- INNOVATION: UI & AUDIO ---

enum Color {
    NEUTRAL = 7,  // Light Gray
    SUCCESS = 10, // Light Green
    FAILURE = 12, // Light Red
    INFO    = 14, // Yellow
    HIGHLIGHT = 11 // Cyan
};

void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void play_sound(bool success) {
    if (success) {
        Beep(1000, 200); // High pitch
        Beep(1200, 200);
    } else {
        Beep(400, 500);  // Low pitch
    }
}

void print_line(int width, char c) {
    for (int i = 0; i < width; i++) std::cout << c;
    std::cout << std::endl;
}

void print_logo() {
    set_color(HIGHLIGHT);
    std::cout << "  ╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "  ║   _______  ______   _____  _     _  _____  _______     ║" << std::endl;
    std::cout << "  ║      |     |_____/    |    |     |    |    |_____|     ║" << std::endl;
    std::cout << "  ║      |     |    \\_  __|__   \\___/   __|__  |     |     ║" << std::endl;
    std::cout << "  ║                                                        ║" << std::endl;
    std::cout << "  ╚════════════════════════════════════════════════════════╝" << std::endl;
    set_color(NEUTRAL);
}

void print_header(const char* text) {
    set_color(INFO);
    int width = 60;
    print_line(width, '=');
    std::cout << "  >> " << text << " <<" << std::endl;
    print_line(width, '=');
    set_color(NEUTRAL);
}

void loading_animation() {
    std::cout << "  Trabajando ";
    for(int i=0; i<3; i++) {
        Sleep(300);
        std::cout << ".";
    }
    std::cout << std::endl;
}

// --- ELITE: INTERACTIVE NAVIGATION ---

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13

int menu_selector(const char* options[], int count, const char* title) {
    int selected = 0;
    while (true) {
        clear_screen();
        print_header(title);
        std::cout << "\n  (Use flechas para navegar, ENTER para seleccionar)\n" << std::endl;
        
        for (int i = 0; i < count; i++) {
            if (i == selected) {
                set_color(11); // Highlight
                std::cout << "  ==> [ " << options[i] << " ] <==" << std::endl;
            } else {
                set_color(7); // Neutral
                std::cout << "      - " << options[i] << std::endl;
            }
        }
        set_color(7);

        int c = _getch();
        if (c == 0 || c == 224) { // Arrow key prefix
            c = _getch();
            if (c == KEY_UP) selected = (selected > 0) ? selected - 1 : count - 1;
            if (c == KEY_DOWN) selected = (selected < count - 1) ? selected + 1 : 0;
        } else if (c == KEY_ENTER) {
            return selected;
        }
    }
}

// Timer helper: returns true if time elapsed
bool check_timer(DWORD start_time, int limit_seconds) {
    DWORD current = GetTickCount();
    return (current - start_time) > (DWORD)(limit_seconds * 1000);
}

#endif // UTILS_H
