#include <iostream>
#include <queue>
#include <stack>
#include <cmath>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include "functions.h"

using namespace std;

int errors = 1;
string line, command;
stack<string> s_stk;
queue<string> s_que, pars_que;
vector<string> functionsN, varsN;
map<string, vector<string>> functionsA;
map<string, queue<string>> functions;
map<string, double> vars;

void moveChar(char element) {
    string token(1, element);
    if (token == "{" || token == "}") {
        if (!command.empty()) {
            pars_que.push(command);
            command.clear();
        }
        return;
    }
    if (find(begin(specSymbols), end(specSymbols), token) != end(specSymbols)) {
        if (!command.empty()) {
            pars_que.push(command);
            command.clear();
        }
        if (element != ' ' && element != ',') {
            pars_que.push(token);
        } else if (element == ',') {
            pars_que.push(",");
        }
        return;
    }
    command.push_back(element);
}

void clear_s_queue() {
    while (!s_que.empty()) s_que.pop();
}

void parsStatement() {
    clear_s_queue();
    while (!s_stk.empty()) s_stk.pop();
    while (!pars_que.empty()) {
        string token = pars_que.front();
        pars_que.pop();
        if (token == "-" && !pars_que.empty() && isNumber(pars_que.front()) &&
            (s_stk.empty() || s_stk.top() == "(")) {
            string num = pars_que.front();
            pars_que.pop();
            s_que.push(token + num);
            continue;
        }
        if (isNumber(token) || vars.count(token)) {
            s_que.push(token);
        } else if (find(functionsN.begin(), functionsN.end(), token) != functionsN.end() || token == "abs") {
            s_stk.push(token);
        } else if (token == "(") {
            s_stk.push(token);
        } else if (token == ")") {
            while (!s_stk.empty() && s_stk.top() != "(") {
                s_que.push(s_stk.top());
                s_stk.pop();
            }
            if (!s_stk.empty()) s_stk.pop();
            if (!s_stk.empty() && (find(functionsN.begin(), functionsN.end(), s_stk.top()) != functionsN.end() || s_stk.top() == "abs")) {
                s_que.push(s_stk.top());
                s_stk.pop();
            }
        } else if (token == ",") {
            while (!s_stk.empty() && s_stk.top() != "(") {
                s_que.push(s_stk.top());
                s_stk.pop();
            }
        } else {
            auto it = find(begin(priorityCommands), end(priorityCommands), token);
            if (it != end(priorityCommands)) {
                int idx = it - begin(priorityCommands);
                int pri = priorityPower[idx];
                while (!s_stk.empty() && s_stk.top() != "(") {
                    auto top = s_stk.top();
                    auto it2 = find(begin(priorityCommands), end(priorityCommands), top);
                    int pri2 = (it2 != end(priorityCommands) ? priorityPower[it2 - begin(priorityCommands)] : -1);
                    if (pri <= pri2) {
                        s_que.push(top);
                        s_stk.pop();
                    } else break;
                }
                s_stk.push(token);
            }
        }
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
        } else if (vars.find(item) != vars.end()) {
            stk.push(vars[item]);
        } else if (item == "abs") {
            if (!stk.empty()) {
                double a = stk.top(); stk.pop();
                stk.push(::abs(a));
            }
        } else if (functions.count(item)) {
            size_t argc = functionsA[item].size();
            if (stk.size() >= argc) {
                vector<double> args(argc);
                for (size_t i = 0; i < argc; ++i) {
                    args[argc - 1 - i] = stk.top();
                    stk.pop();
                }
                map<string, double> saved;
                for (size_t i = 0; i < argc; ++i) {
                    const string& p = functionsA[item][i];
                    if (vars.count(p)) saved[p] = vars[p];
                    vars[p] = args[i];
                }
                double res = calculate(functions[item]);
                for (const auto& p : functionsA[item]) {
                    if (saved.count(p)) vars[p] = saved[p];
                    else vars.erase(p);
                }
                stk.push(res);
            }
        } else {
            static const map<string, function<double(double,double)>> ops = {
                {"+", [](double a,double b){return a+b;}},
                {"-", [](double a,double b){return a-b;}},
                {"*", [](double a,double b){return a*b;}},
                {"/", [](double a,double b){return a/b;}},
                {"max", [](double a,double b){return max(a,b);}},
                {"min", [](double a,double b){return min(a,b);}},
                {"pow", [](double a,double b){return pow(a,b);}}
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
        if (!getline(cin, line) || line == "exit") break;
        for (char c : line) moveChar(c);
        if (!command.empty()) { pars_que.push(command); command.clear(); }
        if (!pars_que.empty() && pars_que.front() == "var") {
            pars_que.pop();
            string v_name = pars_que.front();
            pars_que.pop();
            varsN.push_back(v_name);
            if (!pars_que.empty() && pars_que.front() == "=") pars_que.pop();
            parsStatement();
            vars[v_name] = calculate(s_que);
        } else if (!pars_que.empty() && pars_que.front() == "def") {
            pars_que.pop();
            string func_name = pars_que.front();
            pars_que.pop();
            if (!pars_que.empty() && pars_que.front() == "(") pars_que.pop();
            vector<string> args;
            while (!pars_que.empty() && pars_que.front() != ")") {
                if (pars_que.front() != ",") {
                    args.push_back(pars_que.front());
                    vars[pars_que.front()] = 0;
                }
                pars_que.pop();
            }
            if (!pars_que.empty() && pars_que.front() == ")") pars_que.pop();
            functionsA[func_name] = args;
            parsStatement();
            functionsN.push_back(func_name);
            functions[func_name] = s_que;
        } else if (!pars_que.empty() && find(functionsN.begin(), functionsN.end(), pars_que.front()) != functionsN.end()) {
            string funcName = pars_que.front();
            pars_que.pop();
            if (!pars_que.empty() && pars_que.front() == "(") pars_que.pop();
            for (auto& paramName : functionsA[funcName]) {
                if (!pars_que.empty() && pars_que.front() == ",") pars_que.pop();
                string token;
                double value = 0;
                if (!pars_que.empty()) {
                    token = pars_que.front();
                    pars_que.pop();
                    if (token == "-" && !pars_que.empty() && isNumber(pars_que.front())) {
                        token += pars_que.front();
                        pars_que.pop();
                    }
                    if (isNumber(token)) value = stod(token);
                    else if (vars.count(token)) value = vars[token];
                    vars[paramName] = value;
                }
            }
            if (!pars_que.empty() && pars_que.front() == ")") pars_que.pop();
            cout << calculate(functions[funcName]) << endl;
        } else {
            vector<string> tmp;
            while (!pars_que.empty()) {
                tmp.push_back(pars_que.front());
                pars_que.pop();
            }
            for (size_t i = 0; i < tmp.size(); ++i) {
                if (tmp[i] == "-" && i + 1 < tmp.size() && isNumber(tmp[i+1]) &&
                    (i == 0 || tmp[i-1] == "(" || tmp[i-1] == "," ||
                     find(begin(priorityCommands), end(priorityCommands), tmp[i-1]) != end(priorityCommands))) {
                    pars_que.push(tmp[i] + tmp[i+1]);
                    ++i;
                } else {
                    pars_que.push(tmp[i]);
                }
            }
            parsStatement();
            cout << calculate(s_que) << endl;
        }
        while (!pars_que.empty()) pars_que.pop();
    }
    return errors;
}
