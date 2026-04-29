#pragma once
#include <string>
#include <vector>
#include "Question.h"

class JsonParser {
public:
    static std::vector<Question> loadQuestions(const std::string& filename);

private:
    static std::string readFile(const std::string& filename);
    static std::string utf8ToCp1251(const std::string& utf8);
    static std::string getStringValue(const std::string& json, const std::string& key, size_t startPos);
    static std::vector<std::string> getStringArray(const std::string& json, const std::string& key, size_t startPos);
    static void skipWhitespace(const std::string& str, size_t& pos);
    static size_t findNextObject(const std::string& json, size_t start);
};
