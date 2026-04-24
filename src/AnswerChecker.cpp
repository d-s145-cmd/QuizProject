#include "AnswerChecker.h"

std::string toLower(std::string str) {
    for (char& c : str) {
        unsigned char uc = static_cast<unsigned char>(c);

        if (uc >= 192 && uc <= 223) {
            c = static_cast<char>(uc + 32);
        }
        else if (uc == 168) {
            c = static_cast<char>(184);
        }
        else if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c + ('a' - 'A'));
        }
    }
    return str;
}

std::string trim(const std::string& str) {
    std::size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";

    std::size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

bool checkAnswer(std::string userAnswer, std::string correctAnswer, const std::vector<std::string>& alternatives) {
    userAnswer = trim(toLower(userAnswer));
    correctAnswer = trim(toLower(correctAnswer));

    if (userAnswer == correctAnswer) return true;

    for (const std::string& alt : alternatives) {
        if (userAnswer == trim(toLower(alt))) {
            return true;
        }
    }

    return false;
}