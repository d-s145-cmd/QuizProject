#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Question.h"
inline void printHeader(const std::string& title) {
    std::cout << "========================================\n";
    std::cout << "            " << title << "\n";
    std::cout << "========================================\n";
}
inline void printQuestion(const Question& q, int current, int total) {
    std::cout << "-------- Вопрос " << current << " из " << total << " --------\n";
    std::cout << q.text << "\n";
}
inline std::vector<Question> createQuestions() {
    return {};
}
