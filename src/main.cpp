#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <windows.h>
#include <random>
#include <algorithm>

#include "Question.h"
#include "AnswerChecker.h"
#include "ConsoleHelper.h"
#include "JsonParser.h"
#include "Quiz.h"
#include "RussianHelper.h"

int getPointsByDifficulty(int difficulty) {
    if (difficulty <= 0) return 1;           
    if (difficulty <= 3) return 1;          
    if (difficulty <= 6) return 2;           
    return 3;                                
}

std::string getDifficultyLabel(int difficulty) {
    if (difficulty <= 0) return "не указана";
    if (difficulty <= 3) return "лёгкий";
    if (difficulty <= 6) return "средний";
    return "сложный";
}

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

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(questions.begin(), questions.end(), g);

    int score = 0;             
    int total = static_cast<int>(questions.size());
    int totalPoints = 0;        
    int earnedPoints = 0;      

    
    for (const auto& q : questions) {
        totalPoints += getPointsByDifficulty(q.difficulty);
    }

    clearScreen();
    printHeader("ИНТЕЛЛЕКТУАЛЬНАЯ ВИКТОРИНА");

    std::cout << "\nВсего вопросов: " << total << "\n";
    std::cout << "Максимально возможные баллы: " << totalPoints << "\n";
    std::cout << "На каждый вопрос дается 30 секунд\n";
    std::cout << "Баллы начисляются по сложности:\n";
    std::cout << "  Лёгкий (1-3)   = +1 балл\n";
    std::cout << "  Средний (4-6)  = +2 балла\n";
    std::cout << "  Сложный (7-10) = +3 балла\n\n";
    std::cout << "Нажмите Enter чтобы начать...";
    std::cin.get();

    for (int i = 0; i < total; i++) {
        clearScreen();
        printQuestion(questions[i], i + 1, total);

        int points = getPointsByDifficulty(questions[i].difficulty);
        std::string level = getDifficultyLabel(questions[i].difficulty);

        if (!questions[i].category.empty()) {
            std::cout << "[Категория: " << questions[i].category << "]";
        }
        std::cout << " [Сложность: " << questions[i].difficulty << "/10, " << level << "]";
        std::cout << " [+" << points << " балл";
        if (points > 1) std::cout << "а";  
        std::cout << "]\n\n";

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
                std::cout << "ПРАВИЛЬНО! +" << points << " балл";
                if (points > 1) std::cout << "а";
                std::cout << "\n";
                earnedPoints += points;
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

        if (i < total - 1) {
            std::cout << "\nЧерез 5 секунд следующий вопрос...\n";
            pauseWithCountdown(5);
        }
    }

    clearScreen();
    printHeader("ИТОГИ");

    std::cout << "\nПравильных ответов: " << score << " из " << total << "\n";
    std::cout << "Процент ответов: " << (score * 100 / total) << "%\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Набрано очков: " << earnedPoints << " из " << totalPoints << "\n";

    if (totalPoints > 0) {
        double percentPoints = 100.0 * earnedPoints / totalPoints;
        std::cout << "Эффективность набора: " << percentPoints << "%\n";
    }

    std::cout << "\n";
    if (score == total) {
        std::cout << "ИДЕАЛЬНО! Вы настоящий знаток!\n";
    }
    else if (score >= total * 0.7) {
        std::cout << "Отличный результат!\n";
    }
    else if (score >= total * 0.4) {
        std::cout << "Неплохо, есть куда расти.\n";
    }
    else {
        std::cout << "Стоит подучить материал...\n";
    }

    waitForEnter();
    return 0;
}
