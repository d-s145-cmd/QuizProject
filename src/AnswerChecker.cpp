#include "AnswerChecker.h"
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

string toLower(string str) {
    for (char& c : str) {
       
        unsigned char uc = (unsigned char)c;
        if (uc >= 192 && uc <= 223) {      
            c = uc + 32;                   
        } else if (uc == 168) {            
            c = 184;                       
        } else if (c >= 'A' && c <= 'Z') { 
            c = c + ('a' - 'A');
        }
    }
    return str;
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

bool checkAnswer(string userAnswer, string correctAnswer) {
    userAnswer = trim(toLower(userAnswer));
    correctAnswer = trim(toLower(correctAnswer));
    
    if (userAnswer == correctAnswer) return true;
    
    
    if (correctAnswer == "избушка на курьих ножках") {
        if (userAnswer == "изба на курьих ножках" ||
            userAnswer == "изба бабы €ги" ||
            userAnswer == "избушка бабы €ги") {
            return true;
        }
    }
    
    if (correctAnswer == "остров св€той елены") {
        if (userAnswer == "остров св.елены") return true;
    }
    
    if (correctAnswer == "позвонок птерозавра") {
        if (userAnswer == "позвонок птеранодона") return true;
    }
    
    if (correctAnswer == "флеминг") {
        if (userAnswer == "александр флеминг" ||
            userAnswer == "alexander fleming") {
            return true;
        }
    }
    
    if (correctAnswer == "лупа") {
        if (userAnswer == "лупой" ||
            userAnswer == "увеличительное стекло") {
            return true;
        }
    }
    
    return false;
}