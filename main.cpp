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
    if (specSymbols->find(to_string(element))) {
        if (line.empty()) {
            return;
        }
        pars_que.push(command);
        if (element != ' ' || element != ',')
            pars_que.push(string(1, element));
        command.clear();
        return;
    }
    command.push_back(element);

}

int parsWithPriority(string element) {
    //?? ? ??????
    if (isNumber(element)) {
        s_que.push(element);
        return 1;
    }

    //???? ???????/?????? ? ? ?????? ?????? ? ????????
    if (auto idx = priorityCommands->find(element)) {
        while (pars_que.front() != "" && priorityPower[idx] <= priorityPower[priorityCommands->find(s_stk.top())]) {
            s_que.push(move(s_stk.top()));
            s_stk.pop();
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

    for (int i = 0; i < pars_que.size(); i++) {
        errors = parsWithPriority(pars_que.front());
        if (errors == -1) break;
        pars_que.pop();
    }
}

double calculate(queue<string> que) {
    double num1 = 0, num2 = 0;
    stack<double> _stack;

    while (!que.empty()) {
        auto item = move(que.front());

        if (isNumber(item))
        {
            _stack.push(stod(item));
            continue;
        }
        if (varsN.end() != find(varsN.begin(), varsN.end(), item)) {
            _stack.push(vars[item]);
            continue;
        }
        if (item == "abs") {
            num1 = _stack.top();
            _stack.pop();
            _stack.push(abs(num1));
            continue;
        }


        static const map<std::string, function<double(double,double)>> ops = {
            { "+", [](double a,double b){ return a+b; } },
            { "-", [](double a,double b){ return a-b; } },
            { "*", [](double a,double b){ return a*b; } },
            { "/", [](double a,double b){ return a/b; } },
            { "max", [](double a,double b){ return std::max(a,b); } },
            { "min", [](double a,double b){ return std::min(a,b); } },
            { "pow", [](double a,double b){ return std::pow(a,b); } }
        };

        auto it = ops.find(item);
        if (it != ops.end()) {
            _stack.push(it->second(num1, num2));
        }
    }
    auto result = move(_stack.top());
    _stack.pop();
    return move(result);
}



int main() {
    while (true) {
        getline(cin, line);
        if (line == "exit") {
            break;
        }

        for (int i = 0; i < line.length(); i++) {
            moveChar(line[i]);
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
            vars.end()->second = result;
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
        auto var = find(functionsN.begin(), functionsN.end(), pars_que.front());
        if (functionsN.end() != var) {
            auto funcName = pars_que.front();
            size_t funcIdx = var - functionsN.begin();
            for (auto arg : functionsA[funcName]) {
                vars[arg] = stod(pars_que.front());
                pars_que.pop();
            }
            double result = calculate(functions[funcName]);
            cout << result << endl;
            continue;
        }
        parsStatement();
        double result = calculate(s_que);
        cout << result << endl;

        if (errors == -1) break;
    }
    return errors;
}