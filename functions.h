//
// Created by Fujitsu-H770 on 09.07.2025.
//

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string>

using namespace std;

inline string specSymbols[] = {" ", "{", "}", "(", ")", ",", "=", "{", "}", "(", ")",
    "*", "/",
    "+", "-"};

inline int priorityPower[] = {/*6, 6,*/ 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2/*, 1, 1*/};
inline string priorityCommands[] = {//",", "=",
    "{", "}", "(", ")",
    "max", "min", "pow", "abs",
    "*", "/",
    "+", "-"
    // "def", "var",
};

bool isNumber(const std::string& s);
bool isVar(const string& previous, const string& element);

#endif //FUNCTIONS_H
