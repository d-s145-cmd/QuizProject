#pragma once
#include <string>
void clearScreen();
std::string getInputWithTimer(int seconds, bool& timeout);
void pauseWithCountdown(int seconds);
void waitForEnter();
void waitForKeyPress();
bool compareIgnoreCase(const std::string& str1, const std::string& str2);


