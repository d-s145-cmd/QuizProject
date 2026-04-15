#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX

#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char byte;
#endif


#include "ImageHelper.h"
#include <cstdlib>

using namespace std;

void showImage(string path) {
    if (path.empty()) return;
    system(("start " + path).c_str());
}

void closeImage() {
    system("taskkill /F /IM Microsoft.Photos.exe > nul 2>&1");
    system("taskkill /F /IM mspaint.exe > nul 2>&1");
}