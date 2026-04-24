#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <windows.h>

#include "Question.h"
#include "AnswerChecker.h"
#include "ConsoleHelper.h"
#include "ImageHelper.h"
#include "JsonParser.h"
#include "Quiz.h"
#include "RussianHelper.h"

int main() {
    setupRussianLocale();

    std::vector<Question> questions;

    try {
        questions = JsonParser::loadQuestions("questions.json");
        std::cout << "Вопросы загружены из questions.json\n";
    }
    catch (const std::exception& e) {
        std::cout << "Не удалось загрузить JSON: " << e.what() << "\n";
        std::cout << "Используются встроенные вопросы.\n\n";
        questions = createQuestions();
    }

    if (questions.empty()) {
        std::cout << "Ошибка: нет вопросов для викторины!\n";
        waitForEnter();
        return 1;
    }

    int score = 0;
    int total = static_cast<int>(questions.size());
    int totalDifficulty = 0;
    int earnedDifficulty = 0;

    for (const auto& q : questions) {
        totalDifficulty += q.difficulty;
    }

    clearScreen();
    printHeader("ИНТЕЛЛЕКТУАЛЬНАЯ ВИКТОРИНА");

    std::cout << "\nВсего вопросов: " << total << "\n";
    std::cout << "На каждый вопрос дается 30 секунд\n";
    std::cout << "Ответить можно досрочно — результат покажут сразу!\n\n";
    std::cout << "Нажмите Enter чтобы начать...";
    std::cin.get();

    for (int i = 0; i < total; i++) {
        clearScreen();
        printQuestion(questions[i], i + 1, total);

        if (!questions[i].category.empty()) {
            std::cout << "[Категория: " << questions[i].category << "]";
            if (questions[i].difficulty > 0) {
                std::cout << " [Сложность: " << questions[i].difficulty << "/10]";
            }
            std::cout << "\n\n";
        }

        std::cout << "У вас 30 секунд! Введите ответ: ";

        auto startTime = std::chrono::steady_clock::now();
        bool timeout = false;
        std::string userAnswer = getInputWithTimer(30, timeout);
        auto endTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

        clearScreen();

        if (timeout) {
            std::cout << "\n========================================\n";
            std::cout << "ВРЕМЯ ВЫШЛО! (30 сек.)\n";
            std::cout << "========================================\n\n";
        }
        else {
            bool correct = checkAnswer(userAnswer, questions[i].answer, questions[i].alternatives);

            std::cout << "\n========================================\n";
            std::cout << "Время ответа: " << elapsed << " секунд\n";
            std::cout << "----------------------------------------\n";

            if (correct) {
                std::cout << "ПРАВИЛЬНО!";
                if (questions[i].difficulty > 0) {
                    std::cout << " +" << questions[i].difficulty << " баллов";
                    earnedDifficulty += questions[i].difficulty;
                }
                else {
                    std::cout << " +1 балл";
                    earnedDifficulty += 1;
                }
                std::cout << "\n";
                score++;
            }
            else {
                std::cout << "НЕПРАВИЛЬНО!\n";
            }

            std::cout << "========================================\n\n";
        }

        std::cout << "ПРАВИЛЬНЫЙ ОТВЕТ: " << questions[i].answer << "\n";

        if (!questions[i].alternatives.empty()) {
            std::cout << "Также принимается: ";
            for (std::size_t j = 0; j < questions[i].alternatives.size(); j++) {
                if (j > 0) std::cout << ", ";
                std::cout << questions[i].alternatives[j];
            }
            std::cout << "\n";
        }

        std::cout << "ПОЯСНЕНИЕ: " << questions[i].explanation << "\n";

        if (!questions[i].image.empty()) {
            std::cout << "\nОткрываю картинку...\n";
            showImage(questions[i].image);

            // Обрабатываем сообщения окна 5 секунд
            auto imgStart = std::chrono::steady_clock::now();
            while (true) {
                auto imgElapsed = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::steady_clock::now() - imgStart).count();
                if (imgElapsed >= 5) break;

                // Pump messages для окна картинки
                MSG msg;
                while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            closeImage();
        }

        if (i < total - 1) {
            std::cout << "\nЧерез 5 секунд следующий вопрос...\n";
            pauseWithCountdown(5);
        }
    }

    clearScreen();
    printHeader("ИТОГИ");

    std::cout << "\nПравильных ответов: " << score << " из " << total << "\n";
    std::cout << "Процент: " << (score * 100 / total) << "%\n";

    if (totalDifficulty > 0) {
        std::cout << "Набрано очков: " << earnedDifficulty << " из " << totalDifficulty << "\n";
    }

    std::cout << "\n";

    if (score == total) {
        std::cout << "ИДЕАЛЬНО! Вы настоящий знаток!\n";
    }
    else if (score >= total * 0.7) {
        std::cout << "ОТЛИЧНО! Хороший результат!\n";
    }
    else if (score >= total * 0.4) {
        std::cout << "Неплохо! Есть куда расти.\n";
    }
    else {
        std::cout << "В следующий раз все получится!\n";
    }

    std::cout << "\nСпасибо за игру!\n";
    std::cout << "\nНажмите Enter для выхода...";
    std::cin.get();

    return 0;
}