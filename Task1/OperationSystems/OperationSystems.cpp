#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

const string MOORE_MACHINE_TYPE = "Mr";
const string MEALY_MACHINE_TYPE = "Ml";

struct CommonParams {
    string machineType;
    int stateNumber = 0;
    int inputSymbolNumber = 0;
    int outputSignalNumber = 0;
};

struct MachineState {
    int outputSignal = 0;
    int outputState = 0;
    int equivalenceGroup = 0;
};

struct EquivalenceGroupsAndMatrix {
    vector<vector<MachineState>> matrix;
    map<int, vector<int>> equivalenceGroupMap;
};

bool AreEqual(vector<vector<MachineState>> src1, vector<vector<MachineState>> src2)
{
    if (src1.size() != src2.size() || src1[0].size() != src2[0].size())
        return false;
    for (int i = 0; i < src1.size(); i++)
        for (int j = 0; j < src1[i].size(); j++)
            if (src1[i][j].outputSignal != src2[i][j].outputSignal)
                return false;
    return true;
}

vector<vector<MachineState>> ReadMealyMachineMatrix(CommonParams params, istream& inputStream)
{
    vector<vector<MachineState>> machineMatrix;
    for (int rowIndex = 0; rowIndex < params.inputSymbolNumber; rowIndex++)
    {
        vector<MachineState> row;
        for (int valueIndex = 0; valueIndex < params.stateNumber * 2; valueIndex++)
        {
            if (valueIndex < params.stateNumber)
            {
                MachineState state;
                inputStream >> state.outputState;
                row.push_back(state);
            }
            else
            {
                inputStream >> row[valueIndex - params.stateNumber].outputSignal;
            }
        }
        machineMatrix.push_back(row);
    }
    return machineMatrix;
}

vector<vector<MachineState>> ReadMooreMachineMatrix(CommonParams params, istream& inputStream)
{
    vector<vector<MachineState>> machineMatrix;
    vector<int> outputStates;
    for (int valueIndex = 0; valueIndex < params.stateNumber; valueIndex++)
    {
        if (valueIndex < params.stateNumber)
        {
            int state;
            inputStream >> state;
            outputStates.push_back(state);
        }
    }
    for (int rowIndex = 0; rowIndex < params.inputSymbolNumber; rowIndex++)
    {
        vector<MachineState> row;
        for (int valueIndex = 0; valueIndex < params.stateNumber; valueIndex++)
        {
            if (valueIndex < params.stateNumber)
            {
                MachineState state;
                inputStream >> state.outputState;
                state.outputSignal = outputStates[valueIndex];
                row.push_back(state);
            }
        }
        machineMatrix.push_back(row);
    }
    return machineMatrix;
}

vector<vector<MachineState>> ReadMachineMatrix(CommonParams params, istream& inputStream)
{
    if (params.machineType == MEALY_MACHINE_TYPE)
    {
        return ReadMealyMachineMatrix(params, inputStream);
    }
    return ReadMooreMachineMatrix(params, inputStream);
}

CommonParams ReadCommonParams(istream& inputStream) {
    CommonParams params;
    inputStream >> params.machineType >> params.stateNumber >> params.inputSymbolNumber >> params.outputSignalNumber;
    return params;
}

int FindEquivalenceGroup(map<int, vector<int>> equivalenceGroupMap, int outputState)
{
    for (auto const& pair : equivalenceGroupMap)
    {
        if (std::find(pair.second.begin(), pair.second.end(), outputState) != pair.second.end())
            return pair.first;
    }
    return -1;
}

bool isStateUsed(vector<int> usedStates, int state)
{
    return std::find(usedStates.begin(), usedStates.end(), state) != usedStates.end();
}

vector<string> SerializeOutputSignals(vector<vector<MachineState>> machineMatrix)
{
    vector<string> outputSignalSerializedVector;
    for (int j = 0; j < machineMatrix[0].size(); j++)
    {
        string outputSignalsSerialized;
        for (int i = 0; i < machineMatrix.size(); i++)
            outputSignalsSerialized += to_string(machineMatrix[i][j].outputSignal);
        outputSignalsSerialized += to_string(machineMatrix[0][j].equivalenceGroup);

        outputSignalSerializedVector.push_back(outputSignalsSerialized);
    }

    return outputSignalSerializedVector;
}

map<int, vector<int>> GroupBySignal(vector<vector<MachineState>> machineMatrix)
{
    auto outputSignalSerializedVector = SerializeOutputSignals(machineMatrix);
    int equivalenceGroupCounter = 0;
    vector<int> usedStates;
    map<int, vector<int>> equivalenceGroupMap;

    for (int i = 0; i < outputSignalSerializedVector.size(); i++)
    {
        if (isStateUsed(usedStates, i))
            continue;
        for (int j = i; j < outputSignalSerializedVector.size(); j++)
        {
            if (isStateUsed(usedStates, j))
                continue;

            if (outputSignalSerializedVector[i] == outputSignalSerializedVector[j])
            {
                equivalenceGroupMap[equivalenceGroupCounter].push_back(j);
                usedStates.push_back(j);
            }
        }
        equivalenceGroupCounter++;
    }

    return equivalenceGroupMap;
}

vector<vector<MachineState>> RebuildMatrix(vector<vector<MachineState>> machineMatrix, map<int, vector<int>> equivalenceGroupMap)
{
    vector<vector<MachineState>> minimazedMatrix;
    for (int i = 0; i < machineMatrix.size(); i++)
    {
        vector<MachineState> row;
        for (int j = 0; j < machineMatrix[i].size(); j++)
        {
            MachineState state;
            state.outputState = machineMatrix[i][j].outputState;
            state.outputSignal = FindEquivalenceGroup(equivalenceGroupMap, machineMatrix[i][j].outputState - 1);
            state.equivalenceGroup = FindEquivalenceGroup(equivalenceGroupMap, j);
            row.push_back(state);
        }
        minimazedMatrix.push_back(row);
    }

    return minimazedMatrix;
}

EquivalenceGroupsAndMatrix MinimazeMatrix(vector<vector<MachineState>> machineMatrix)
{
    auto equivalenceGroupMap = GroupBySignal(machineMatrix);
    EquivalenceGroupsAndMatrix result;
    result.equivalenceGroupMap = equivalenceGroupMap;
    result.matrix = RebuildMatrix(machineMatrix, equivalenceGroupMap);  
    return result;
}

void WriteMiliResultMatrix(EquivalenceGroupsAndMatrix minimazedMatrix, ostream& output, vector<vector<MachineState>> startMatrix)
{
    vector<int> resultStates;
    for (auto const& group : minimazedMatrix.equivalenceGroupMap)
    {
        resultStates.push_back(group.second[0]);
        output << group.second[0] + 1 << "\t";
    }
    output << endl;

    for (int i = 0; i < minimazedMatrix.matrix.size(); i++)
    {
        for (int j = 0; j < resultStates.size(); j++)
        {
            output << startMatrix[i][resultStates[j]].outputState << "\t";
        }
        output << endl;
        for (int j = 0; j < resultStates.size(); j++)
        {
            output << startMatrix[i][resultStates[j]].outputSignal << "\t";
        }
        output << endl << endl;
    }
}

void WriteMooreResultMatrix(EquivalenceGroupsAndMatrix minimazedMatrix, ostream& output, vector<vector<MachineState>> startMatrix)
{
    vector<int> resultStates;
    for (auto const& group : minimazedMatrix.equivalenceGroupMap)
    {
        resultStates.push_back(group.second[0]);
        output << group.second[0] + 1 << "\t";
    }
    output << endl;
    for (int j = 0; j < resultStates.size(); j++)
    {
        output << startMatrix[0][resultStates[j]].outputSignal << "\t";
    }
    output << endl;

    for (int i = 0; i < minimazedMatrix.matrix.size(); i++)
    {
        for (int j = 0; j < resultStates.size(); j++)
        {
            output << startMatrix[i][resultStates[j]].outputState << "\t";
        }
        output << endl;
    }
}

void WriteEquivalenceGroup(map<int, vector<int>> equivalenceGroupMap, ostream& output)
{
    for (auto const& pair : equivalenceGroupMap)
    {
        output << pair.first + 1 << ": ";
        for (auto state : pair.second)
        {
            output << state + 1 << ' ';
        }
        output << endl;
    }
    output << endl;
}


void WriteResultMatrix(EquivalenceGroupsAndMatrix minimazedMatrix, ostream& output, vector<vector<MachineState>> startMatrix, string machineType)
{
    WriteEquivalenceGroup(minimazedMatrix.equivalenceGroupMap, output);

    if (machineType == MEALY_MACHINE_TYPE)
    {
        WriteMiliResultMatrix(minimazedMatrix, output, startMatrix);
        return;
    }
    WriteMooreResultMatrix(minimazedMatrix, output, startMatrix);
}

int main(int argc, char* argv[])
{
    string inputFileName = argv[1];
    string outputFileName = argv[2];
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    auto commonParams = ReadCommonParams(inputFile);
    auto inputMatrix = ReadMachineMatrix(commonParams, inputFile);
    auto startMatrix = inputMatrix;
    auto outputMatrix = MinimazeMatrix(inputMatrix).matrix;
    EquivalenceGroupsAndMatrix result;
    while (!AreEqual(inputMatrix, outputMatrix))
    {
        inputMatrix = outputMatrix;
        result = MinimazeMatrix(inputMatrix);
        outputMatrix = result.matrix;
    }
    WriteResultMatrix(result, outputFile, startMatrix, commonParams.machineType);
}