#include <iostream>
#include <queue>
#include <stack>
using namespace std;

string line = "";
string command = "";
stack<string> s_calc;
queue<string> s_que;
queue<string> pars_que;
string specSymbols[] = {" ", "{", "}", "(", ")"};

int priorityPower[] = {1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5};
string priorityCommands[] = {"{", "}", "(", ")",
                            "def", "var",
                            "max", "min", "pow", "abs",
                            "*", "/",
                            "+", "-"};

string retrunStatements[10];
queue<string> functions[128];


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
        if (element != ' ')
            pars_que.push(" ");
        command.clear();
        return;
    }
    command.push_back(element);

}

void makeFunc() {

}

void calculate() {

}

int main() {
    cin >> line;
     for (int i = 0; i < line.length(); i++) {
         moveChar(line[i]);
     }
    line.clear();

    for (int i = 0; i < pars_que.size(); i++) {

    }

    return 0;
}