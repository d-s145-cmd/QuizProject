#pragma once
#include <vector>
#include "Question.h"

std::vector<Question> createQuestions();
void printHeader(std::string title);
void printQuestion(Question q, int num, int total);
void printResult(bool correct);