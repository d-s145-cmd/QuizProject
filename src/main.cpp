#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char byte;
#endif

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <windows.h>
#include "Question.h"
#include "AnswerChecker.h"
#include "ConsoleHelper.h"
#include "ImageHelper.h"
#include "Quiz.h"
#include "RussianHelper.h"

using namespace std;

int main() {
    setupRussianLocale();

    vector<Question> questions = createQuestions();
    int score = 0;
    int total = (int)questions.size();

    clearScreen();
    printHeader("ИНТЕЛЛЕКТУАЛЬНАЯ ВИКТОРИНА");

    cout << "\nВсего вопросов: " << total << "\n";
    cout << "На каждый вопрос дается 30 секунд\n";
    cout << "Ответить можно досрочно - результат покажут сразу!\n\n";
    cout << "Нажмите Enter чтобы начать...";
    cin.get();

    for (int i = 0; i < total; i++) {
        clearScreen();
        printQuestion(questions[i], i + 1, total);

        cout << "У вас 30 секунд! Введите ответ: ";

        
        auto startTime = chrono::steady_clock::now();

        bool timeout = false;
        string userAnswerRaw = getInputWithTimer(30, timeout);

       
        auto endTime = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();

        clearScreen();

        if (timeout) {
            cout << "\n========================================\n";
            cout << "ВРЕМЯ ВЫШЛО! (30 сек.)\n";
            cout << "========================================\n\n";
        }
        else {
            string userAnswer = fromConsoleEncoding(userAnswerRaw);
            bool correct = checkAnswer(userAnswer, questions[i].answer);

            cout << "\n========================================\n";
            cout << "Время ответа: " << elapsed << " секунд\n";
            cout << "----------------------------------------\n";

            if (correct) {
                cout << "ПРАВИЛЬНО! +1 балл\n";
                score++;
            }
            else {
                cout << "НЕПРАВИЛЬНО!\n";
            }
            cout << "========================================\n\n";
        }

        cout << "ПРАВИЛЬНЫЙ ОТВЕТ: " << questions[i].answer << "\n";
        cout << "ПОЯСНЕНИЕ: " << questions[i].explanation << "\n";

        if (!questions[i].image.empty()) {
            cout << "\nОткрываю картинку...\n";
            showImage(questions[i].image);
            this_thread::sleep_for(chrono::seconds(5));
            closeImage();
        }

        if (i < total - 1) {
            cout << "\nЧерез 5 секунд следующий вопрос...\n";
            pauseWithCountdown(5);
        }
    }

    clearScreen();
    printHeader("ИТОГИ");

    cout << "\nПравильных ответов: " << score << " из " << total << "\n";
    cout << "Процент: " << (score * 100 / total) << "%\n\n";

    if (score == total) {
        cout << "ИДЕАЛЬНО! Вы настоящий знаток! \n";
    }
    else if (score >= 3) {
        cout << "ОТЛИЧНО! Хороший результат!\n";
    }
    else {
        cout << "В следующий раз всё получится!\n";
    }

    cout << "\nСпасибо за игру!\n";
    cout << "\nНажмите Enter для выхода...";
    cin.get();

    return 0;
}