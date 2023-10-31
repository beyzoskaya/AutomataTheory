#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include "dfa.h" // Include your DFA class header here

struct DFATransition {
    std::string fromState;
    char input;
    std::string toState;
};

bool readLinesFromFile(const std::string& filename, std::vector<std::string>& lines) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return false;
    }

    lines.clear();

    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile close();
    return true;
}

int main() {
    int numStates, numVariables, numGoals;

    std::vector<std::string> lines;
    if (readLinesFromFile("file1.txt", lines)) {
        if (lines.size() < 7) {
            std::cerr << "Invalid input file format." << std::endl;
            return 1;
        }

        numStates = std::stoi(lines[0]);
        numVariables = std::stoi(lines[1]);
        numGoals = std::stoi(lines[2]);

        std::cout << numStates << "   // Number of states" << std::endl;
        std::cout << numVariables << "   // Number of variables" << std::endl;
        std::cout << numGoals << "   // Number of goal states" << std::endl;

        std::vector<std::string> states;
        std::vector<std::string> goalStates;
        std::vector<char> variables;
        std::vector<DFATransition> transitions;

        // Read and print states
        std::cout << "States:" << std::endl;
        std::istringstream statesStream(lines[3]);
        std::string state;
        while (statesStream >> state) {
            states.push_back(state);
            std::cout << state << " ";
        }
        std::cout << std::endl;

        // Read and print goal states
        std::cout << "Goal state(s):" << std::endl;
        std::istringstream goalStatesStream(lines[4]);
        while (goalStatesStream >> state) {
            goalStates.push_back(state);
            std::cout << state << " ";
        }
        std::cout << std::endl;

        // Read and print variables
        std::cout << "Variables:" << std::endl;
        std::istringstream variablesStream(lines[5]);
        char variable;
        while (variablesStream >> variable) {
            variables.push_back(variable);
            std::cout << variable << " ";
        }
        std::cout << std::endl;

        // Read and print transitions
        std::cout << "Transitions:" << std::endl;
        for (size_t i = 6; i < 6 + numStates * numVariables; i++) {
            std::istringstream transitionStream(lines[i]);
            std::string fromState, toState;
            char input;

            transitionStream >> fromState >> input >> toState;

            DFATransition transition;
            transition.fromState = fromState;
            transition.input = input;
            transition.toState = toState;
            transitions.push_back(transition);

            std::cout << "State transition: " << fromState << " --(" << input << ")--> " << toState << std::endl;
        }

        // Print the strings to be detected
        std::cout << "Strings to be detected:" << std::endl;
        for (size_t i = 6 + numStates * numVariables; i < lines.size(); i++) {
            std::cout << lines[i] << std::endl;
        }

        // Create a DFA instance
        DFA myDFA(numStates, numVariables, numGoals, states, goalStates, variables, transitions);

        // Process input strings and print results
        for (size_t i = 6 + numStates * numVariables; i < lines.size(); i++) {
            std::string inputString = lines[i];

            if (myDFA.processInputString(inputString)) {
                std::cout << "Accepted" << std::endl;
            } else {
                std::cout << "Rejected" << std::endl;
            }
        }
    } else {
        std::cerr << "Failed to read lines from the input file." << std::endl;
        return 1;
    }

    return 0;
}
