#pragma once
#include <string>
#include <vector>

struct Question {
    std::string text;
    std::string answer;
    std::string explanation;
    
    
   
    std::string category;                   
    int difficulty = 0;                      
    std::vector<std::string> alternatives;    
};
