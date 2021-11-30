#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

struct Options {
    bool isLeftSided = false;
    int lineAmount = 0;
    map<char, map<char, char>> transitionMatrix;
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
        getline(in, outputState);
        istringstream iss(outputState);
        map<char, char> transitions;
        string transition;
        while (getline(iss, transition, '|'))
        {
            if (options.isLeftSided)
                transitions[transition[0]] = transition[1];
            else
                transitions[transition[1]] = transition[0];
        }
        if (options.isLeftSided)
            options.transitionMatrix[inputState[0]] = transitions;
    }
}

map<char, map<char, char>> determinateMatrix(Options options)
{

}


int main()
{
    auto options = parseInput(cin);
}