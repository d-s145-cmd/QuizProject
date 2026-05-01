#include "RussianHelper.h"
#include <windows.h>
#include <iostream>

void setupRussianLocale() {
    
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    setlocale(LC_ALL, "Russian");

   
    _setmode(_fileno(stdout), _O_TEXT);
    _setmode(_fileno(stdin), _O_TEXT);
}

std::string fromConsoleEncoding(const std::string& str) {
    return str;
}

std::string toConsoleEncoding(const std::string& str) {
    return str;
}

std::string toLowerRussian(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
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
    return result;
}
