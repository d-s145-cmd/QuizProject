#pragma once
#include <vector>
#include "Question.h"
#include "ConsoleHelper.h"
class Game {
public:
    
    Game(const std::vector<Question>& questions);

   
    void start();

private:
    std::vector<Question> questions;

    int getPointsByDifficulty(int difficulty) const;
    std::string getDifficultyLabel(int difficulty) const;
};
