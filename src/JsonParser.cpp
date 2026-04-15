#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char byte;
#endif
#include "JsonParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

using namespace std;

string JsonParser::readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Ne udalos otkryt fail: " + filename);
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void JsonParser::skipWhitespace(const string& str, size_t& pos) {
    while (pos < str.length() && (str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\r' || str[pos] == '\t')) {
        pos++;
    }
}

string JsonParser::getStringValue(const string& json, const string& key, size_t startPos) {
   
    string searchKey = "\"" + key + "\"";
    size_t keyPos = json.find(searchKey, startPos);
    if (keyPos == string::npos) return "";

  
    size_t colonPos = json.find(":", keyPos);
    if (colonPos == string::npos) return "";

   
    size_t quoteStart = json.find("\"", colonPos);
    if (quoteStart == string::npos) return "";

    
    size_t quoteEnd = json.find("\"", quoteStart + 1);
    if (quoteEnd == string::npos) return "";

    return json.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
}

size_t JsonParser::findNextObject(const string& json, size_t start) {
    size_t pos = start;
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
    return string::npos;
}

vector<Question> JsonParser::loadQuestions(const string& filename) {
    vector<Question> questions;
    string json = readFile(filename);

    // Ischem massiv questions
    size_t arrayPos = json.find("\"questions\"");
    if (arrayPos == string::npos) {
        throw runtime_error("Ne nayden massiv questions v JSON faile");
    }

    // Ischem otkryvayushchuyu skobku massiva
    size_t bracketStart = json.find("[", arrayPos);
    if (bracketStart == string::npos) return questions;

    size_t pos = bracketStart + 1;

    while (true) {
        skipWhitespace(json, pos);

        if (pos >= json.length() || json[pos] == ']') {
            break;
        }

        if (json[pos] == '{') {
            size_t objStart = pos;
            size_t objEnd = findNextObject(json, objStart);

            if (objEnd != string::npos) {
                string obj = json.substr(objStart, objEnd - objStart);

                Question q;
                q.text = getStringValue(obj, "text", 0);
                q.answer = getStringValue(obj, "answer", 0);
                q.explanation = getStringValue(obj, "explanation", 0);
                q.image = getStringValue(obj, "image", 0);

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