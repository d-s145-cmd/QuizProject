#pragma once
#include <string>

void setupRussianLocale();
std::string fromConsoleEncoding(const std::string& str);
std::string toConsoleEncoding(const std::string& str);
std::string toLowerRussian(const std::string& str);