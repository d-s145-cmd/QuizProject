#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char byte;
#endif
#include "RussianHelper.h"
#include <windows.h>
#include <string>
#include <clocale>

using namespace std;

void setupRussianLocale() {
   
    SetConsoleOutputCP(1251);
    
    SetConsoleCP(866);
   
    setlocale(LC_ALL, "Russian");
}

string fromConsoleEncoding(const string& str) {
    if (str.empty()) return "";

   
    int size_needed = MultiByteToWideChar(866, 0, str.c_str(), (int)str.length(), NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(866, 0, str.c_str(), (int)str.length(), &wstr[0], size_needed);

    size_needed = WideCharToMultiByte(1251, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    string result(size_needed, 0);
    WideCharToMultiByte(1251, 0, wstr.c_str(), (int)wstr.length(), &result[0], size_needed, NULL, NULL);

    return result;
}