#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <tuple>
#include <string>
#include <algorithm>

using namespace std;

void printVector(const vector<char>& vec) {
    for (const auto& element : vec) {
        cout << element << " ";
    }
    cout << endl;
}

const string EPSILON = "ε";

struct PDAKey {
    string source;
    string variable;
    string popElement;
    string pushElement;
    string dest;
};

struct PDAConfig {
    int numberOfVariablesInput;
    int numberOfVariablesStack;
    int numberOfGoalStates;
    int numberOfStates;
    vector<string> states;
    string startState;
    vector<string> goalStates;
    vector<string> stackAlphabet;
    string initialStackSymbol;
    vector<int> inputAlphabet;
    vector<string> inputStrings;
    vector<PDAKey> transitions;
    vector<string> variables;
    vector<string> pop_elements;
    vector<string> push_elements;
    vector<string> state_tos;
};

class NPDAStack {

public:
vector<string> elements;
    void push(string element) {
        elements.push_back(element);
    }

    void pop() {
        if (!elements.empty()) {
            elements.pop_back();
        }
    }

    string top() const {
        if (!elements.empty()) {
            return elements.back();
        }
    }

    bool empty() const {
        return elements.empty();
    }

    void clear() {
        elements.clear();
    }
};

class NPDAState {
public:
    string state;
    NPDAStack stack;
    NPDAState() {}
    NPDAState(string state, NPDAStack stack) {this->state=state; this->stack=stack;}
};

class NonDeterministicPushdownAutomaton {
private:
    NPDAState currentState;
    PDAConfig config;
    vector<string> acceptedRoute;  // Track the route taken for accepted inputs
    vector<string> currentRoute;   // Track the current route being explored

public:
    NonDeterministicPushdownAutomaton(const PDAConfig& cfg) : config(cfg) {
        NPDAStack stack;
        currentState = NPDAState(cfg.startState, stack);
    }

    bool processNPDA(const string& input) {
        acceptedRoute.clear();
        bool isAccepted = processNPDARecursive(input, 0, currentState);
        
        cout << (isAccepted ? "Accepted" : "Rejected") << endl;
        cout << "Route taken: ";
        if (isAccepted) {
            for (const string& s : acceptedRoute) {
                cout << s << " ";
            }
        } else {
            cout << "No valid route for rejected input";
        }
        cout << endl;

        return isAccepted;
    }

private:
    bool processNPDARecursive(const string& input, int i, NPDAState currentState) {
        if (i > input.size()) {
            return false;
        }

        if (i == input.size() && find(config.goalStates.begin(), config.goalStates.end(), currentState.state) != config.goalStates.end() && currentState.stack.empty()) {
            acceptedRoute = currentRoute;  // Update acceptedRoute with the successful route
            return true;
        }

        char currentInputSymbol = (i < input.size()) ? input[i] : EPSILON[0];
        vector<PDAKey> foundTransitions;

        for (const auto& t : config.transitions) {
            if (t.source == currentState.state && (t.variable == string(1, currentInputSymbol) || t.variable == EPSILON)) {
                foundTransitions.push_back(t);
            }
        }

        for (auto& t : foundTransitions) {
            string stackPop = t.popElement;
            string stackPush = t.pushElement;
            NPDAState newState = currentState;

            if (stackPop != EPSILON && stackPop == newState.stack.top()) {
                newState.stack.pop();
            } else if (stackPop != EPSILON && stackPop != newState.stack.top()) {
                continue;
            }

            if (stackPush != EPSILON) {
                newState.stack.push(stackPush);
            }

            newState.state = t.dest;

            currentRoute.push_back(newState.state);  // Record the current state in the route
            if (processNPDARecursive(input, i + (t.variable == EPSILON ? 0 : 1), newState)) {
                return true;
            }
            currentRoute.pop_back();  // Remove the last state in case of backtracking
        }

        return false;
    }
};


void printVector(const vector<string>& vec, const string& label) {
    cout << label << ": ";
    for (const auto& element : vec) {
        cout << element << " ";
    }
    cout << endl;
}


void printVector(const vector<int>& vec, const string& label) {
    cout << label << ": ";
    for (const auto& element : vec) {
        cout << element << " ";
    }
    cout << endl;
}

PDAConfig readFile(const string& file_path) {
    PDAConfig config;

    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << file_path << endl;
        exit(1);
    }

    string line;
    int line_number = 0;

    while (getline(file, line)) {
        if (line_number == 0) {
            config.numberOfVariablesInput = stoi(line);
        } else if (line_number == 1) {
            config.numberOfVariablesStack = stoi(line);
        } else if (line_number == 2) {
            config.numberOfGoalStates = stoi(line);
        } else if (line_number == 3) {
            config.numberOfStates = stoi(line);
        } else if (line_number == 4) {
            istringstream iss(line);
            for (string token; iss >> token;) {
                config.states.push_back(token);
            }
            printVector(config.states, "States");
        } else if (line_number == 5) {
            config.startState = line;
        } else if (line_number == 6) {
            istringstream iss(line);
            for (string token; iss >> token;) {
                config.goalStates.push_back(token);
            }
            // printVector(config.goalStates, "Goal States");
        } else if (line_number == 7) {
            istringstream iss(line);
            for (string token; iss >> token;) {
                config.stackAlphabet.push_back(token);
            }
            // printVector(config.stackAlphabet, "Stack Alphabet");
        } else if (line_number == 8) {
            config.initialStackSymbol = line;
        } else if (line_number == 9) {
            istringstream iss(line);
            int token;
            while (iss >> token) {
                config.inputAlphabet.push_back(token);
            }
            // printVector(config.inputAlphabet, "Input Alphabet");
        } else if (line_number > 9) {
            size_t space_pos = line.find(' ');
            if (space_pos != string::npos) {
                istringstream iss(line);
                string state_from, variable, pop_element, push_element, state_to;
                iss >> state_from >> variable >> pop_element >> push_element >> state_to;
                if (pop_element == EPSILON && push_element == EPSILON) {
                    config.transitions.push_back(PDAKey{state_from, variable, pop_element, push_element, state_to});
                    config.variables.push_back(EPSILON);
                    config.state_tos.push_back(state_to);
                    config.pop_elements.push_back(pop_element);
                    config.push_elements.push_back(push_element);
                } else {
                    config.transitions.push_back(PDAKey{state_from, variable, pop_element, push_element, state_to});
                    config.variables.push_back(variable);
                    config.pop_elements.push_back(pop_element);
                    config.push_elements.push_back(push_element);
                    config.state_tos.push_back(state_to);
                }
                // cout << "Transition: " << state_from << " --(" << variable << ", " << pop_element << ", " << push_element << ")--> " << state_to << endl;
            } else {
                config.inputStrings.push_back(line);
            }
        }
        line_number++;
    }

    printVector(config.inputStrings, "Input Strings");
    return config;
}

int main() {
    string file_path = "./input_10.txt";
    PDAConfig config = readFile(file_path);

    NonDeterministicPushdownAutomaton npda(config);

    for (const string& input : config.inputStrings) {
        cout << "Processing input: " << input << endl;
        //if (npda.processNPDA(input)) {
        //    cout << input << " is accepted." << endl;
        //} else {
        //    cout << input << " is rejected." << endl;
        //}
        npda.processNPDA(input);
    }

    return 0;
}

