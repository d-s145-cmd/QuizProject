#include "RussianHelper.h"
#include <windows.h>
#ifndef CP_UTF8
#define CP_UTF8 65001
#endif
void setupRussianLocale() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
