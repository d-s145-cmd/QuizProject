#include "RussianHelper.h"
#include <windows.h>
#include <clocale>

void setupRussianLocale() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    std::setlocale(LC_ALL, "Russian");
}