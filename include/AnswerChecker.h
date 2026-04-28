#pragma once
#include <string>
#include <vector>

bool checkAnswer(std::string userAnswer, std::string correctAnswer, const std::vector<std::string>& alternatives);
std::string toLower(std::string str);
std::string trim(const std::string& str);
