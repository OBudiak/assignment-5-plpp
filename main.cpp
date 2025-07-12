#include <iostream>
#include <queue>
#include <stack>
#include <cmath>
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
// vector<string> returnStatements[128];
vector<string> functionsN;
vector<pair<string, queue<string>>> functions;
vector<string> varsN;
vector<pair<string, double>> vars;
// pair<string, int> funcVars[16];


/*auto pushCom = [=](string& s) {
    if (s.empty()) {
        return;
    }
    if ()
    pars_que.push(s);
};*/

void moveChar(char element) {
    if (specSymbols->find(element)) {
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
            // previousEl = move(element);
            s_stk.pop();
        }
        s_stk.push(element);

        return 1;
    }
    s_que.push(element);
    return 1;

    // //?? ? ??????? ???????, ?? ???? ?? ??????????????? ?? ????
    // if (isVar(pars_que.front(), element)) {
    //
    //     // vars->push(pair<string, int>(element, 0));
    // }
    // cout << "Compiling error!\n" << "Error element - " << element << endl;
    // return -1;
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
        // previousEl = move(pars_que.front());
        pars_que.pop();
    }
}

double calculate(queue<string> que) {
    double num1 = 0, num2 = 0;
    stack<double> _stack;

    for (auto item: que) {

        if (isNumber(item))
        {
            _stack.push(stod(item));
            continue;
        }
        if (auto idx = find(varsN, item)) {
            _stack.push(vars[idx].second);
            continue;
        }
        /*if (auto funcIdx = find(functionsN, item)) {
            auto func = functions[funcIdx];
            auto funcName = func.first;
            for (int i = 0; i <= to_integer<string>(funcName[0]); i++) {
                nums->push_back(_stack.top());
                _stack.pop();
            }
            double funcVal = func.second;
        }*/
        if (item == "abs") {
            num1 = _stack.top();
            _stack.pop();
            _stack.push(abs(num1));
            continue;
        }


        switch (item)
        {
            case "+": _stack.push(num1 + num2); break;
            case "-": _stack.push(num1 - num2); break;
            case "*": _stack.push(num1 * num2); break;
            case "/": _stack.push(num1 / num2); break;
            case "max": _stack.push(max(num1, num2)); break;
            case "min": _stack.push(min(num1, num2)); break;
            case "pow": _stack.push(pow(num1, num2)); break;
        }
    }
    auto result = move(_stack.top());
    _stack.pop();
    return move(result);
}



int main() {
    while (true) {
        cin >> line;
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
            vars.push_back(make_pair(v_name, 0));
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
            while (pars_que.front() != ")") {
                vars.push_back(make_pair(pars_que.front().c_str(), 0));
                pars_que.pop();
                varCount++;
            }
            pars_que.pop();

            parsStatement();
            functionsN.push_back(func_name);
            func_name = to_string(varCount) + func_name;
            functions.push_back(make_pair(func_name, s_que));
            continue;
        }
        parsStatement();
        double result = calculate(s_que);
        cout << result << endl;

        if (errors == -1) break;
    }
    return errors;
}