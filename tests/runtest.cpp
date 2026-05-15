#include <iostream>
#include "RussianHelper.h"


void runConsoleHelperTests();
void runJsonParserTests();

int main() {
    setupRussianLocale();

    std::cout << "========================================\n";
    std::cout << "        ЗАПУСК ВСЕХ ТЕСТОВ             \n";
    std::cout << "========================================\n\n";

    std::cout << "[1/2] Консольный хелпер (Caps Lock)...\n";
    runConsoleHelperTests();

    std::cout << "\n[2/2] Парсер JSON...\n";
    runJsonParserTests();

    std::cout << "\n========================================\n";
    std::cout << "   ВСЕ ТЕСТЫ УСПЕШНО ЗАВЕРШЕНЫ!     \n";
    std::cout << "========================================\n";

    
    return 0;
}
