#pragma once
#include <string>
#include <io.h>
#include <fcntl.h>

void setupRussianLocale();
std::string fromConsoleEncoding(const std::string& str);
std::string toConsoleEncoding(const std::string& str);
std::string toLowerRussian(const std::string& str);
