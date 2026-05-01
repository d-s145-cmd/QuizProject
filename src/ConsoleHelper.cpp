#include "ConsoleHelper.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <windows.h>
#include <conio.h>  // Для _kbhit() и _getch()

void clearScreen() {
    std::system("cls");
}

char utf16ToCp1251(wchar_t wc) {
    if (wc < 128) return static_cast<char>(wc);

    char result[2] = { 0 };
    WideCharToMultiByte(1251, 0, &wc, 1, result, 2, NULL, NULL);
    return result[0];
}

std::string getInputWithTimer(int seconds, bool& timeout) {
    timeout = false;
    std::string answer;
    answer.reserve(256);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD originalMode;
    GetConsoleMode(hStdin, &originalMode);

    SetConsoleMode(hStdin, ENABLE_PROCESSED_INPUT);

    auto start = std::chrono::steady_clock::now();
    int lastDisplayedSecond = -1;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        int remaining = seconds - static_cast<int>(elapsed);

        if (remaining != lastDisplayedSecond && remaining >= 0) {
            lastDisplayedSecond = remaining;

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(hStdout, &csbi);
            COORD savedPos = csbi.dwCursorPosition;

            COORD timerPos = { 0, savedPos.Y };
            SetConsoleCursorPosition(hStdout, timerPos);

            std::string timerStr = "[" + std::to_string(remaining) + " сек] ";
            std::cout << timerStr;

            SetConsoleCursorPosition(hStdout, savedPos);
        }

        if (remaining <= 0) {
            timeout = true;
            SetConsoleMode(hStdin, originalMode);
            std::cout << std::endl;
            return "";
        }

        if (_kbhit()) {
            int ch = _getch();

            if (ch == 13) {
                SetConsoleMode(hStdin, originalMode);
                std::cout << std::endl;
                return answer;
            }
            else if (ch == 8) {
                if (!answer.empty()) {
                    answer.pop_back();
                    std::cout << "\b \b";
                }
            }
            else if (ch == -32 || ch == 0) {
                _getch();
            }
            else if (ch >= 32) {
                answer += static_cast<char>(ch);
                std::cout << static_cast<char>(ch);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void pauseWithCountdown(int seconds) {
    for (int i = seconds; i > 0; i--) {
        std::cout << "\rСледующий вопрос через " << i << " сек.   " << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << std::endl;
}

void waitForEnter() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.get();
}
