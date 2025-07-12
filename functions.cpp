//
// Created by Fujitsu-H770 on 09.07.2025.
//

#include "functions.h"

string specSymbols[] = {" ", "{", "}", "(", ")", ",", "="};

int priorityPower[] = {/*6, 6,*/ 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2/*, 1, 1*/};
string priorityCommands[] = {//",", "=",
                            "{", "}", "(", ")",
                            "max", "min", "pow", "abs",
                            "*", "/",
                            "+", "-",
                            // "def", "var",
};

bool isNumber(const string& s) {
    if (s.empty()) return false;
    size_t i = 0;

    if (s[0] == '-') i++;

    bool hasDigit = false;

    for (; i < s.size(); ++i) {
        if (isdigit(s[i])) {
            hasDigit = true;
            continue;
        }
        return false;

    }
    return hasDigit;
}

bool isVar(const string& previous, const string& s) {
    if (s.empty() || previous.empty()) return false;
    if (previous != "var") return false;
    return true;
}