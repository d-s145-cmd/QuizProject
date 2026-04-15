#pragma once
#include <string>

void clearScreen();
std::string getInputWithTimer(int seconds, bool& timeout);
void pauseWithCountdown(int seconds);
void waitForEnter();