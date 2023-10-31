#include "dfa.h"

#include <map>
#include <iostream>

DFA::DFA(int numStates, int numVariables, int numGoals,
    const std::vector<std::string>& states,
    const std::vector<std::string>& goalStates,
    const std::vector<char>& variables,
    const std::vector<DFATransition>& transitions) {
    this->numStates = numStates;
    this->numVariables = numVariables;
    this->numGoals = numGoals;
    this->states = states;
    this->goalStates = goalStates;
    this->variables = variables;

    // Build the transition table
    for (const DFATransition& transition : transitions) {
        transitionTable[transition.fromState][transition.input] = transition.toState;
    }
}

void DFA::addTransition(const std::string& fromState, char inputSymbol, const std::string& toState) {
    transitionTable[fromState][inputSymbol] = toState;
}

bool DFA::isGoalState(const std::string& state) const {
    return std::find(goalStates.begin(), goalStates.end(), state) != goalStates.end();
}

bool DFA::processInputString(const std::string& inputString) {
    std::string currentState = states[0]; // Start from the initial state
    std::string path = currentState; // Initialize the path

    for (char symbol : inputString) {
        // Check if the input symbol is valid
        bool validSymbol = false;
        for (char variable : variables) {
            if (symbol == variable) {
                validSymbol = true;
                break;
            }
        }

        if (validSymbol) {
            // Find the next state using the transition table
            if (transitionTable[currentState].find(symbol) != transitionTable[currentState].end()) {
                currentState = transitionTable[currentState][symbol];
                path += " " + currentState; // Add the new state to the path
            } else {
                std::cerr << "No transition for symbol " << symbol << " from state " << currentState << std::endl;
                return false;
            }
        } else {
            std::cerr << "Invalid input symbol: " << symbol << std::endl;
            return false;
        }
    }

    // Check if the final state is a goal state
    if (isGoalState(currentState)) {
        std::cout << "Accepted" << std::endl;
        std::cout << path << " (route taken)" << std::endl;
        return true;
    } else {
        std::cout << "Rejected" << std::endl;
        return false;
    }
}
