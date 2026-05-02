#include "RussianHelper.h"
#include <windows.h>
void setupRussianLocale() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
