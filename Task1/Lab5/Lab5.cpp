#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <sstream>
#include <algorithm>

using namespace std;

struct Line {
    string symbol;
    vector<string> directSymbols;
    bool shift;
    bool error;
    int pointer;
    bool pushToStack;
    bool end;
};

vector<Line> matrix = {
    Line{"S", vector<string>{"9", "a", "-", "("}, false, true, 1, false, false},
    Line{"E", vector<string>{"9", "a", "-", "("}, false, true, 3, true, false},
    Line{"#", vector<string>{"#"}, true, true, -1, false, true},
    Line{"E", vector<string>{"9", "a", "-", "("}, false, true, 4, false, false},
    Line{"T", vector<string>{"9", "a", "-", "("}, false, true, 14, true, false},
    Line{"A", vector<string>{")", "+", " "}, false, true, 6, false, false},
    Line{"A", vector<string>{"+"}, false, false, 11, false, false},
    Line{"A", vector<string>{")"}, false, false, 10, false, false},
    Line{"A", vector<string>{" "}, false, true, 9, false, false},
    Line{" ", vector<string>{" "}, false, true, -1, false, false},
    Line{")", vector<string>{")"}, true, true, -1, false, false},
    Line{"+", vector<string>{"+"}, true, true, 12, false, false},
    Line{"T", vector<string>{"9", "a", "-", "("}, false, true, 14, true, false},
    Line{"A", vector<string>{"+", ")", " "}, false, true, 6, false, false},
    Line{"T", vector<string>{"9", "a", "-", "("}, false, true, 15, false, false},
    Line{"F", vector<string>{"9", "a", "-", "("}, false, true, 23, true, false},
    Line{"B", vector<string>{"*", "+"}, false, true, 17, false, false},
    Line{"B", vector<string>{"*"}, false, false, 20, false, false},
    Line{"B", vector<string>{"+"}, false, true, 19, false, false},
    Line{"+", vector<string>{"+"}, true, true, -1, false, false},
    Line{"*", vector<string>{"*"}, true, true, 21, false, false},
    Line{"F", vector<string>{"9", "a", "-", "("}, false, true, 23, true, false},
    Line{"B", vector<string>{"*", "+"}, false, true, 17, false, false},
    Line{"F", vector<string>{"9"}, false, false, 27, false, false},
    Line{"F", vector<string>{"a"}, false, false, 28, false, false},
    Line{"F", vector<string>{"-"}, false, false, 29, false, false},
    Line{"F", vector<string>{"("}, false, true, 31, false, false},
    Line{"9", vector<string>{"9"}, true, true, -1, false, false},
    Line{"a", vector<string>{"a"}, true, true, -1, false, false},
    Line{"-", vector<string>{"-"}, true, true, 30, false, false},
    Line{"F", vector<string>{"9", "a", "-", "("}, false, true, 23, true, false},
    Line{"(", vector<string>{"("}, true, true, 32, false, false},
    Line{"E", vector<string>{"9", "a", "-", "("}, false, true, 3, true, false},
    Line{")", vector<string>{")"}, true, true, -1, false, false},
};

bool handleStringByGrammar(string value)
{
    stack<int> callStack;
    char ch1;
    istringstream stream(value);
    stream >> ch1;
    string ch{ ch1 };
    int i = 0;
    while (!matrix[i].end)
    {
        if (find(matrix[i].directSymbols.begin(), matrix[i].directSymbols.end(), ch) != matrix[i].directSymbols.end())
        {
            int nextPointer = i;
            if (matrix[i].end)
                return true;
            if (matrix[i].pushToStack)
                callStack.push(i);
            if (matrix[i].pointer != -1)
                nextPointer = matrix[i].pointer;
            if (matrix[i].shift)
            {
                if (callStack.size() != 0 && matrix[i].pointer == -1)
                {
                    nextPointer = callStack.top() + 1;
                    callStack.pop();
                }
                stream >> ch1;
                ch = '\0' + ch1;
            }
            i = nextPointer;
        }
        else if (matrix[i].error)
        {
            return false;
        }
        else
        {
            i++;
        }
    }
    return true;
}

int main()
{
    string input;
    cin >> input;
    cout << handleStringByGrammar(input);
}