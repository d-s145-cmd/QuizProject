#include "JsonParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <windows.h>

std::string JsonParser::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string JsonParser::utf8ToCp1251(const std::string& utf8) {
    if (utf8.empty()) return "";

    int wideSize = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.length()), NULL, 0);
    if (wideSize == 0) return "";

    std::wstring wstr(wideSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.length()), &wstr[0], wideSize);

    int ansiSize = WideCharToMultiByte(1251, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
    if (ansiSize == 0) return "";

    std::string result(ansiSize, 0);
    WideCharToMultiByte(1251, 0, wstr.c_str(), static_cast<int>(wstr.length()), &result[0], ansiSize, NULL, NULL);

    return result;
}

void JsonParser::skipWhitespace(const std::string& str, std::size_t& pos) {
    while (pos < str.length() &&
        (str[pos] == ' ' || str[pos] == '\n' ||
            str[pos] == '\r' || str[pos] == '\t')) {
        pos++;
    }
}

std::string JsonParser::getStringValue(const std::string& json, const std::string& key, std::size_t startPos) {
    std::string searchKey = "\"" + key + "\"";
    std::size_t keyPos = json.find(searchKey, startPos);
    if (keyPos == std::string::npos) return "";

    std::size_t colonPos = json.find(":", keyPos);
    if (colonPos == std::string::npos) return "";

    std::size_t quoteStart = json.find("\"", colonPos);
    if (quoteStart == std::string::npos) return "";

    std::size_t quoteEnd = json.find("\"", quoteStart + 1);
    if (quoteEnd == std::string::npos) return "";

    return json.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
}

std::vector<std::string> JsonParser::getStringArray(const std::string& json, const std::string& key, std::size_t startPos) {
    std::vector<std::string> result;

    std::string searchKey = "\"" + key + "\"";
    std::size_t keyPos = json.find(searchKey, startPos);
    if (keyPos == std::string::npos) return result;

    std::size_t colonPos = json.find(":", keyPos);
    if (colonPos == std::string::npos) return result;

    std::size_t bracketStart = json.find("[", colonPos);
    if (bracketStart == std::string::npos) return result;

    std::size_t pos = bracketStart + 1;

    while (pos < json.length()) {
        skipWhitespace(json, pos);

        if (pos >= json.length() || json[pos] == ']') {
            break;
        }

        if (json[pos] == '"') {
            std::size_t quoteEnd = json.find("\"", pos + 1);
            if (quoteEnd != std::string::npos) {
                result.push_back(json.substr(pos + 1, quoteEnd - pos - 1));
                pos = quoteEnd + 1;
            }
        }
        else {
            pos++;
        }

        skipWhitespace(json, pos);
        if (pos < json.length() && json[pos] == ',') {
            pos++;
        }
    }

    return result;
}

std::size_t JsonParser::findNextObject(const std::string& json, std::size_t start) {
    std::size_t pos = start;
    int braceCount = 0;
    bool inObject = false;

    while (pos < json.length()) {
        if (json[pos] == '{') {
            if (!inObject) {
                inObject = true;
                braceCount = 1;
            }
            else {
                braceCount++;
            }
        }
        else if (json[pos] == '}') {
            if (inObject) {
                braceCount--;
                if (braceCount == 0) {
                    return pos + 1;
                }
            }
        }
        pos++;
    }
    return std::string::npos;
}

std::vector<Question> JsonParser::loadQuestions(const std::string& filename) {
    std::vector<Question> questions;
    std::string json = readFile(filename);

    json = utf8ToCp1251(json);

    std::size_t arrayPos = json.find("\"questions\"");
    if (arrayPos == std::string::npos) {
        throw std::runtime_error("Не найден массив questions в JSON файле");
    }

    std::size_t bracketStart = json.find("[", arrayPos);
    if (bracketStart == std::string::npos) return questions;

    std::size_t pos = bracketStart + 1;

    while (true) {
        skipWhitespace(json, pos);

        if (pos >= json.length() || json[pos] == ']') {
            break;
        }

        if (json[pos] == '{') {
            std::size_t objStart = pos;
            std::size_t objEnd = findNextObject(json, objStart);

            if (objEnd != std::string::npos) {
                std::string obj = json.substr(objStart, objEnd - objStart);

                Question q;
                q.text = getStringValue(obj, "text", 0);
                q.answer = getStringValue(obj, "answer", 0);
                q.explanation = getStringValue(obj, "explanation", 0);
                q.image = getStringValue(obj, "image", 0);
                q.category = getStringValue(obj, "category", 0);

                std::string diffStr = getStringValue(obj, "difficulty", 0);
                if (!diffStr.empty()) {
                    q.difficulty = std::stoi(diffStr);
                }

                q.alternatives = getStringArray(obj, "alternatives", 0);

                if (!q.text.empty() && !q.answer.empty()) {
                    questions.push_back(q);
                }

                pos = objEnd;
            }
            else {
                break;
            }
        }
        else {
            pos++;
        }

        skipWhitespace(json, pos);
        if (pos < json.length() && json[pos] == ',') {
            pos++;
        }
    }

    return questions;
}