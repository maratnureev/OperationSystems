#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <set>

using namespace std;

struct Options {
    bool isLeftSided = false;
    int lineAmount = 0;
    map<string, map<string, string>> transitionMatrix;
    map<string, map<string, string>> resultMatrix;
    set<string> symbols;
    set<string> states;
    string startState;
    string endState;
};

Options parseInput(istream& in)
{
    Options options;
    string line;
    getline(in, line);
    options.isLeftSided = line == "L";
    getline(in, line);
    options.lineAmount = stoi(line);
    for (int i = 0; i < options.lineAmount; i++)
    {
        string inputState;
        string outputState;
        getline(in, inputState, ' ');
        if (i == 0)
            options.startState = inputState;
        if (options.isLeftSided)
        {
            options.endState = "S";
            options.startState = "H";
        }
        else
        {
            options.endState = "H";
            options.startState = "S";
        }
        getline(in, outputState);
        istringstream iss(outputState);
        string transition;
        while (getline(iss, transition, '|'))
        {
            if (options.isLeftSided)
            {
                string state{ transition.size() >= 2 ? transition[0] : 'H' };
                string symbol{ transition.size() >= 2 ? transition[1] : transition[0]};
                options.symbols.insert(symbol);
                if (options.transitionMatrix[state][symbol].find(inputState) == string::npos)
                    options.transitionMatrix[state][symbol] += inputState;
            }
            else
            {
                string state = { transition.size() >= 2 ? transition[1] : 'H' };
                string symbol{ transition[0] };
                options.symbols.insert(symbol);
                if (options.transitionMatrix[inputState][symbol].find(state) == string::npos)
                    options.transitionMatrix[inputState][symbol] += state;
            }
        }
    }
    return options;
}

void determinateMatrix(Options& options)
{
    options.states.insert(options.startState);
    options.resultMatrix[options.startState] = options.transitionMatrix[options.startState];
    for (auto& inputState : options.states)
    {
        for (auto& pair : options.transitionMatrix[inputState])
        {
            options.states.insert(pair.second);
            if (pair.second.size() == 1 || options.transitionMatrix.count(pair.second) == 1)
            {
                options.resultMatrix[pair.second] = options.transitionMatrix[pair.second];
                continue;
            }
            map<string, string> transitions;
            for (char charState : pair.second)
            {
                for (auto i = options.symbols.begin(); i != options.symbols.end(); i++) 
                {
                    string symbol = *i;
                    string state{ charState };
                    for (auto it = options.transitionMatrix[state][symbol].begin(); it != options.transitionMatrix[state][symbol].end(); it++)
                        if (transitions[symbol].find(*it) == string::npos)
                            transitions[symbol] += *it;
                }
            }
            options.resultMatrix[pair.second] = transitions;
            options.transitionMatrix[pair.second] = transitions;
        }
    }
}

void writeResult(Options& options, ostream& out)
{
    for (auto& statePair : options.resultMatrix)
    {
        for (auto& pair : statePair.second)
        {
            if (pair.second == "")
                continue;
            out << statePair.first << " " << pair.first << " " << pair.second << endl;
        }
    }
}

int main()
{
    auto options = parseInput(cin);
    determinateMatrix(options);
    writeResult(options, cout);
}