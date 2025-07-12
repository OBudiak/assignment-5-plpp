#include <iostream>
#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>
#include <functional>
#include <map>

#include "functions.h"

using namespace std;

int errors = 1;
string line = "";
string command = "";
string previousEl = "";
stack<string> s_stk;
queue<string> s_que;
queue<string> pars_que;


bool isVariable = false;
bool isFunction = false;
vector<string> functionsN;
map<string, vector<string>> functionsA;
map<string, queue<string>> functions;
vector<string> varsN;
map<string, double> vars;

void moveChar(char element) {
    string token(1, element);
    if (find(begin(specSymbols), end(specSymbols), token) != end(specSymbols)) {
        if (!command.empty()) {
            pars_que.push(command);
            command.clear();
        }
        if (element != ' ' && element != ',') {
            pars_que.push(token);
        }
        return;
    }
    command.push_back(element);
}


int parsWithPriority(const string& element) {
    if (isNumber(element)) {
        s_que.push(element);
        return 1;
    }
    auto it = find(begin(priorityCommands), end(priorityCommands), element);
    if (it != end(priorityCommands)) {
        int idx = it - begin(priorityCommands);
        int pri = priorityPower[idx];
        while (!s_stk.empty()) {
            auto top = s_stk.top();
            auto it2 = find(begin(priorityCommands), end(priorityCommands), top);
            int pri2 = (it2 != end(priorityCommands)
                        ? priorityPower[it2 - begin(priorityCommands)]
                        : 0);
            if (pri <= pri2) {
                s_que.push(top);
                s_stk.pop();
            } else break;
        }
        s_stk.push(element);
        return 1;
    }
    s_que.push(element);
    return 1;
}


void clear_s_queue() {
    while (!s_que.empty()) {
        s_que.pop();
    }
}

void parsStatement() {
    clear_s_queue();
    while (!s_stk.empty()) s_stk.pop();

    while (!pars_que.empty()) {
        parsWithPriority(pars_que.front());
        pars_que.pop();
    }

    while (!s_stk.empty()) {
        s_que.push(s_stk.top());
        s_stk.pop();
    }
}


double calculate(queue<string> que) {
    stack<double> stk;
    while (!que.empty()) {
        string item = que.front();
        que.pop();
        if (isNumber(item)) {
            stk.push(stod(item));
        }
        else if (find(varsN.begin(), varsN.end(), item) != varsN.end()) {
            stk.push(vars[item]);
        }
        else if (item == "abs") {
            double a = stk.top(); stk.pop();
            stk.push(abs(a));
        }
        else {
            static const map<string, function<double(double,double)>> ops = {
                { "+", [](double a,double b){ return a+b; } },
                { "-", [](double a,double b){ return a-b; } },
                { "*", [](double a,double b){ return a*b; } },
                { "/", [](double a,double b){ return a/b; } },
                { "max", [](double a,double b){ return max(a,b); } },
                { "min", [](double a,double b){ return min(a,b); } },
                { "pow", [](double a,double b){ return pow(a,b); } }
            };
            auto it = ops.find(item);
            if (it != ops.end() && stk.size() >= 2) {
                double b = stk.top(); stk.pop();
                double a = stk.top(); stk.pop();
                stk.push(it->second(a,b));
            }
        }
    }
    return stk.empty() ? 0.0 : stk.top();
}



int main() {
    while (true) {
        getline(cin, line);
        if (line == "exit") {
            break;
        }

        for (char c : line) moveChar(c);
        if (!command.empty()) {
            pars_que.push(command);
            command.clear();
        }

        line.clear();

        if (pars_que.front() == "var") {
            isVariable = true;
            pars_que.pop();
            string v_name = pars_que.front();
            varsN.push_back(v_name);
            vars[v_name] = 0;
            pars_que.pop();
            if (pars_que.front() != "=") {
                cout << "Error, no = iterator" << endl;
                continue;
            }
            pars_que.pop();
            parsStatement();
            double result = calculate(s_que);
            vars[v_name] = result;
            continue;
        }
        if (pars_que.front() == "def") {
            isFunction = true;
            pars_que.pop();
            string func_name = pars_que.front();
            pars_que.pop();
            if (auto arg = move(pars_que.front()) != "(") {
                cout << "Compiling error" << arg << endl;
                continue;
            }
            int varCount = 0;
            vector<string> functions_args;
            while (pars_que.front() != ")") {
                vars[pars_que.front().c_str()] = 0;
                functions_args.push_back(pars_que.front().c_str());
                pars_que.pop();
                varCount++;
            }
            functionsA[func_name] = move(functions_args);
            pars_que.pop();

            parsStatement();
            functionsN.push_back(func_name);
            func_name = to_string(varCount) + func_name;
            functions[func_name] = s_que;
            continue;
        }
        else if (!pars_que.empty() && find(functionsN.begin(), functionsN.end(), pars_que.front()) != functionsN.end()) {
            string funcName = pars_que.front();
            pars_que.pop();
            if (!pars_que.empty() && pars_que.front() == "(")
                pars_que.pop();
            for (auto& arg : functionsA[funcName]) {
                if (!pars_que.empty() && pars_que.front() == ",")
                    pars_que.pop();
                if (!pars_que.empty()) {
                    vars[arg] = stod(pars_que.front());
                    pars_que.pop();
                }
            }
            if (!pars_que.empty() && pars_que.front() == ")")
                pars_que.pop();
            double result = calculate(functions[funcName]);
            cout << result << endl;
        }
        else {
            parsStatement();
            double result = calculate(s_que);
            cout << result << endl;
        }
        while (!pars_que.empty()) pars_que.pop();

        if (errors == -1) break;
    }
    return errors;
}