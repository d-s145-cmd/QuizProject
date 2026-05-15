#define NOMINMAX

#include "ConsoleHelper.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <atomic>
#include <windows.h>
#include <algorithm>  
#include <cwctype>   

void clearScreen() {
    std::system("cls");
}

void setTerminalTitle(const std::string& title) {
    std::cout << "\033]0;" << title << "\007" << std::flush;
}

void waitForKeyPress() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    wchar_t buffer[2];
    DWORD read = 0;
    ReadConsoleW(hStdin, buffer, 1, &read, NULL);
    FlushConsoleInputBuffer(hStdin);
}

std::string getInputWithTimer(int seconds, bool& timeout) {
    timeout = false;
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    std::cout << "У вас " << seconds << " секунд! Введите ответ: ";
    std::cout.flush();

    std::wstring wanswer;
    std::atomic<bool> done(false);

    std::thread inputThread([&wanswer, &done, hStdin]() {
        FlushConsoleInputBuffer(hStdin);
        while (true) {
            wchar_t buffer[256];
            DWORD charsRead = 0;

            if (ReadConsoleW(hStdin, buffer, 255, &charsRead, NULL)) {
                buffer[charsRead] = L'\0';
                std::wstring wstr(buffer);

                while (!wstr.empty() && (wstr.back() == L'\r' || wstr.back() == L'\n' || wstr.back() == L' ')) {
                    wstr.pop_back();
                }

                if (!wstr.empty()) {
                    wanswer = wstr;
                    done = true;
                    break;
                }
            }
            else {
                break;
            }
        }
        });
    inputThread.detach();

    auto start = std::chrono::steady_clock::now();
    while (!done) {
        auto now = std::chrono::steady_clock::now();
        int remaining = seconds - static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - start).count());

        if (remaining <= 0) {
            timeout = true;
            done = true;
            FlushConsoleInputBuffer(hStdin);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            break;
        }
        setTerminalTitle("Осталось: " + std::to_string(remaining) + " сек");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    setTerminalTitle("ВИКТОРИНА");

    if (wanswer.empty()) return "";

    int size = WideCharToMultiByte(CP_UTF8, 0, wanswer.c_str(), -1, NULL, 0, NULL, NULL);
    if (size == 0) return "";
    std::string result(size - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wanswer.c_str(), -1, &result[0], size, NULL, NULL);

    return result;
}

void pauseWithCountdown(int seconds) {
    for (int i = seconds; i > 0; i--) {
        setTerminalTitle("Следующий вопрос через " + std::to_string(i) + " сек");
        std::cout << "\rСледующий вопрос через " << i << " сек.   " << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    setTerminalTitle("ВИКТОРИНА");
    std::cout << "\n";
}

void waitForEnter() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cout.flush();

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    wchar_t buffer[256];
    DWORD read = 0;
    ReadConsoleW(hStdin, buffer, 255, &read, NULL);
}


bool compareIgnoreCase(const std::string& str1, const std::string& str2) {
    auto toLowerUtf8 = [](const std::string& str) -> std::wstring {
        if (str.empty()) return L"";

        int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        if (size <= 1) return L"";

        std::wstring wstr(size - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);

        std::transform(wstr.begin(), wstr.end(), wstr.begin(), std::towlower);

        return wstr;
        };

    return toLowerUtf8(str1) == toLowerUtf8(str2);
}
