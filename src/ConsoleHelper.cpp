#define NOMINMAX

#include "ConsoleHelper.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <atomic>
#include <windows.h>

void clearScreen() {
    std::system("cls");
}

void waitForKeyPress() {
    std::string temp;
    std::getline(std::cin, temp);
}


void setTerminalTitle(const std::string& title) {
    
    std::cout << "\033]0;" << title << "\007" << std::flush;
}

std::string getInputWithTimer(int seconds, bool& timeout) {
    timeout = false;
    std::string answer;

    std::cout << "У вас " << seconds << " секунд! Введите ответ: ";
    std::cout.flush();

    
    struct TitleGuard {
        ~TitleGuard() { setTerminalTitle("ВИКТОРИНА"); }
    } guard;

    std::atomic<bool> done(false);

    
    std::thread timer([&]() {
        auto start = std::chrono::steady_clock::now();
        while (!done.load()) {
            auto now = std::chrono::steady_clock::now();
            int remaining = seconds - static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - start).count());

            if (remaining <= 0) {
                timeout = true;
                done = true;
                break;
            }
            setTerminalTitle("Осталось: " + std::to_string(remaining) + " сек");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        });

    
    std::getline(std::cin, answer);

    done = true;
    timer.join();

    return answer;
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
    std::string temp;
    std::getline(std::cin, temp);
}
