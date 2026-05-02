#pragma once
#include <string>
#include <vector>
#include "Question.h"
class JsonParser {
public:
    static std::vector<Question> loadQuestions(const std::string& filename);
private:
    static std::string readFile(const std::string& filename);
    static void skipWhitespace(const std::string& str, std::size_t& pos);
    static std::string getStringValue(const std::string& json, const std::string& key, std::size_t startPos);
    static int getIntValue(const std::string& json, const std::string& key, std::size_t startPos);
    static std::vector<std::string> getStringArray(const std::string& json, const std::string& key, std::size_t startPos);
    static std::size_t findNextObject(const std::string& json, std::size_t start);
};
