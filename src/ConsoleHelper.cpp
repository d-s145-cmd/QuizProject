#include "ConsoleHelper.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdlib>

using namespace std;

void clearScreen() {
    system("cls");
}

string getInputWithTimer(int seconds, bool& timeout) {
    string answer;
    timeout = false;
    atomic<bool> finished(false);

    thread inputThread([&answer, &finished]() {
        getline(cin, answer);
        finished = true;
        });

    
    for (int i = 0; i < seconds && !finished; i++) {
        this_thread::sleep_for(chrono::seconds(1));
    }

    if (!finished) {
        timeout = true;
        inputThread.detach();
        return "";
    }

    if (inputThread.joinable()) {
        inputThread.join();
    }

    return answer;
}

void pauseWithCountdown(int seconds) {
    for (int i = seconds; i > 0; i--) {
        cout << "\rСледующий вопрос через " << i << " сек.   " << flush;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << endl;
}

void waitForEnter() {
    cout << "\nНажмите Enter для продолжения...";
    cin.get();
}