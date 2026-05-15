#include "ConsoleHelper.h"
#include <iostream>
#include <cassert>
#include <string>
#include <windows.h>

#define TEST(name) std::cout << "Тест: " << name << "... ";

void runTests() {
    int passed = 0;
    int total = 0;

    TEST("Идеальное совпадение (нижний регистр)") {
        total++;
        assert(compareIgnoreCase("москва", "москва") == true);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    TEST("Идеальное совпадение (верхний регистр)") {
        total++;
        assert(compareIgnoreCase("МОСКВА", "МОСКВА") == true);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    TEST("Разный регистр (Caps Lock включен у пользователя)") {
        total++;
        assert(compareIgnoreCase("МОСКВА", "москва") == true);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    TEST("Смешанный регистр") {
        total++;
        assert(compareIgnoreCase("МоСкВа", "мОсКвА") == true);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    TEST("Разные слова (должно вернуть false)") {
        total++;
        assert(compareIgnoreCase("москва", "питер") == false);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    TEST("Разная длина слов") {
        total++;
        assert(compareIgnoreCase("москва", "моск") == false);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    TEST("Обе пустые строки") {
        total++;
        assert(compareIgnoreCase("", "") == true);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    TEST("Одна строка пустая") {
        total++;
        assert(compareIgnoreCase("", "москва") == false);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    TEST("Английские буквы (проверка что UTF-8 не ломает ASCII)") {
        total++;
        assert(compareIgnoreCase("Hello", "hELLO") == true);
        std::cout << "ПРОЙДЕН\n";
        passed++;
    }

    std::cout << "\n========================================\n";
    std::cout << "Всего тестов: " << total << "\n";
    std::cout << "Пройдено: " << passed << "\n";
    if (passed == total) {
        std::cout << "РЕЗУЛЬТАТ: ВСЕ ТЕСТЫ УСПЕШНЫ!\n";
    }
    else {
        std::cout << "РЕЗУЛЬТАТ: ЕСТЬ ОШИБКИ!\n";
    }
    std::cout << "========================================\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "--- ЗАПУСК ТЕСТОВ CONSOLE HELPER ---\n\n";

    runTests();

    return 0;
}
