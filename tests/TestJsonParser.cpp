#include "JsonParser.h"
#include "Question.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <windows.h>

void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    file << content;
    file.close();
}

void deleteTestFile(const std::string& filename) {
    std::remove(filename.c_str());
}

#define TEST(name) std::cout << "Тест: " << name << "... ";

void runTests() {
    int passed = 0;
    int total = 0;
    const std::string testFile = "temp_test_quiz.json";

    TEST("Отсутствующий файл (должен выбросить исключение)") {
        total++;
        bool threwException = false;
        try {
            JsonParser::loadQuestions("nonexistent_file_12345.json");
        }
        catch (const std::runtime_error&) {
            threwException = true;
        }
        assert(threwException == true);
        std::cout << "ПРОЙДЕН\n"; passed++;
    }

    TEST("JSON без поля 'questions' (должен выбросить исключение)") {
        total++;
        createTestFile(testFile, "{ \"data\": [1, 2, 3] }");
        bool threwException = false;
        try {
            JsonParser::loadQuestions(testFile);
        }
        catch (const std::runtime_error& e) {
            threwException = (std::string(e.what()).find("не найден массив questions") != std::string::npos);
        }
        deleteTestFile(testFile);
        assert(threwException == true);
        std::cout << "ПРОЙДЕН\n"; passed++;
    }

    TEST("Пустой массив вопросов") {
        total++;
        createTestFile(testFile, "{ \"questions\": [] }");
        auto questions = JsonParser::loadQuestions(testFile);
        deleteTestFile(testFile);
        assert(questions.size() == 0);
        std::cout << "ПРОЙДЕН\n"; passed++;
    }

    TEST("Полностью заполненный вопрос (все поля)") {
        total++;
        createTestFile(testFile, R"({
            "questions": [
                {
                    "text": "Столица Франции?",
                    "answer": "Париж",
                    "explanation": "Исторический факт.",
                    "category": "География",
                    "difficulty": 5,
                    "alternatives": ["париж", "ПАРИЖ"]
                }
            ]
        })");
        auto questions = JsonParser::loadQuestions(testFile);
        deleteTestFile(testFile);

        assert(questions.size() == 1);
        assert(questions[0].text == "Столица Франции?");
        assert(questions[0].answer == "Париж");
        assert(questions[0].explanation == "Исторический факт.");
        assert(questions[0].category == "География");
        assert(questions[0].difficulty == 5);
        assert(questions[0].alternatives.size() == 2);
        assert(questions[0].alternatives[1] == "ПАРИЖ");
        std::cout << "ПРОЙДЕН\n"; passed++;
    }

    TEST("Минимальный вопрос (только text и answer)") {
        total++;
        createTestFile(testFile, R"({
            "questions": [
                {
                    "text": "2+2?",
                    "answer": "4"
                }
            ]
        })");
        auto questions = JsonParser::loadQuestions(testFile);
        deleteTestFile(testFile);

        assert(questions.size() == 1);
        assert(questions[0].text == "2+2?");
        assert(questions[0].answer == "4");
        assert(questions[0].explanation.empty() == true);
        assert(questions[0].difficulty == 0);
        assert(questions[0].alternatives.empty() == true);
        std::cout << "ПРОЙДЕН\n"; passed++;
    }

    TEST("Несколько вопросов подряд") {
        total++;
        createTestFile(testFile, R"({
            "questions": [
                { "text": "Q1", "answer": "A1" },
                { "text": "Q2", "answer": "A2" },
                { "text": "Q3", "answer": "A3" }
            ]
        })");
        auto questions = JsonParser::loadQuestions(testFile);
        deleteTestFile(testFile);

        assert(questions.size() == 3);
        assert(questions[2].text == "Q3");
        assert(questions[0].answer == "A1");
        std::cout << "ПРОЙДЕН\n"; passed++;
    }

    TEST("ФИЧА: Сложность передана как строка '5' (ваш getIntValue это умеет)") {
        total++;
        createTestFile(testFile, R"({
            "questions": [
                {
                    "text": "Тест",
                    "answer": "Ответ",
                    "difficulty": "8"
                }
            ]
        })");
        auto questions = JsonParser::loadQuestions(testFile);
        deleteTestFile(testFile);

        assert(questions[0].difficulty == 8);
        std::cout << "ПРОЙДЕН\n"; passed++;
    }

    TEST("ФИЧА: Игнорирование невалидного вопроса (нет поля text)") {
        total++;
        createTestFile(testFile, R"({
            "questions": [
                {
                    "answer": "Только ответ без вопроса"
                },
                {
                    "text": "Правильный вопрос",
                    "answer": "Правильный ответ"
                }
            ]
        })");
        auto questions = JsonParser::loadQuestions(testFile);
        deleteTestFile(testFile);

        assert(questions.size() == 1);
        assert(questions[0].text == "Правильный вопрос");
        std::cout << "ПРОЙДЕН\n"; passed++;
    }

    TEST("ФИЧА: Игнорирование невалидного вопроса (нет поля answer)") {
        total++;
        createTestFile(testFile, R"({
            "questions": [
                {
                    "text": "Только вопрос без ответа",
                    "explanation": "Пусто"
                }
            ]
        })");
        auto questions = JsonParser::loadQuestions(testFile);
        deleteTestFile(testFile);

        assert(questions.size() == 0);
        std::cout << "ПРОЙДЕН\n"; passed++;
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
    SetConsoleCP(CP_UTF8);

    std::cout << "--- ЗАПУСК ТЕСТОВ JSON PARSER ---\n\n";

    runTests();

    deleteTestFile("temp_test_quiz.json");

    return 0;
}
