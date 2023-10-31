#ifndef DFA_H
#define DFA_H

#include <string>
#include <vector>
#include <map>

class DFA {
public:
    DFA(int numStates, int numVariables, int numGoals,
        const std::vector<std::string>& states,
        const std::vector<std::string>& goalStates,
        const std::vector<char>& variables,
        const std::vector<std::string>& transitions);

    void addTransition(const std::string& fromState, char inputSymbol, const std::string& toState);
    bool isGoalState(const std::string& state) const;
    bool processInputString(const std::string& inputString);

private:
    int numStates;
    int numVariables;
    int numGoals;
    std::vector<std::string> states;
    std::vector<std::string> goalStates;
    std::vector<char> variables;
    std::map<std::string, std::map<char, std::string>> transitionTable;
};

#endif
