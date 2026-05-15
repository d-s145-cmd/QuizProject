#include <iostream>
#include <vector>
#include "Question.h"
#include "Game.h"           
#include "JsonParser.h"
#include "RussianHelper.h"
#include "ConsoleHelper.h"
#include "Quiz.h"
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
    Game quizGame(questions);  
    quizGame.start();
    return 0;
}
