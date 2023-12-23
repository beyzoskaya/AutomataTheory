#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <tuple>
#include <stack>
#include <algorithm>

using namespace std;

const string EPSILON = "ε";

struct PDAKey {
    string source;
    // string state;
    string variable;
    string popElement;
    string pushElement;
    string dest;

    // bool operator<(const PDAKey& other) const {
    //     return tie(state, popElement, pushElement) < tie(other.state, other.popElement, other.pushElement);
    // }
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
    // map<pair<string, string>, string> transitions;
    vector<PDAKey> transitions;
    vector <string> variables;
    vector<string> pop_elements;
    vector<string> push_elements;
    vector<string> state_tos;
    // map<PDAKey, string> stackImplementations;


};

class PushdownAutomaton {
private:
    stack<char> stack_; 
    string currentState;
    PDAConfig config;
    vector<string>route;

public:
    PushdownAutomaton(const PDAConfig& cfg ): config(cfg){
        currentState = config.startState;
        //stack_.push(config.initialStackSymbol[0]);
        cout << "Current state: " << currentState << endl;
        //cout << "Initial stack: " << config.initialStackSymbol[0] << endl;
    }

    bool processPDA(const string& input){
        cout << "Processing input: " << input << endl;
        currentState = config.startState;
        int input_index = 0;
        PDAKey key;
        string processedStr = "";
        route.clear();
        route.push_back(currentState);

        for(int i=0; i<= input.size(); i++){
            bool foundTransition = false;
            //cout << "Input charachter for implement transition: " << symbol << endl;
            char topOfStack = stack_.top();
            //cout << "Stack top: " << stack_.top() << endl;

            
            for (const auto& t : config.transitions){
                std::cout << "currentState: " << currentState << std::endl;
                if (t.source == currentState){
                    if (t.variable == string(1, input[i])){
                        if (t.popElement != EPSILON && !stack_.empty() && stack_.top() == t.popElement[0]){
                            stack_.pop();
                            processedStr += input[i];
                        }
                        if (t.pushElement != EPSILON){
                            stack_.push(t.pushElement[0]);
                            processedStr += input[i];
                        };
                        currentState = t.dest;
                        route.push_back(currentState);
                        foundTransition = true;

                        break;
                    }else if (t.variable == EPSILON){
                        if (t.popElement != EPSILON && !stack_.empty() && stack_.top() == t.popElement[0]){
                            stack_.pop();
                        }
                        if (t.pushElement != EPSILON){
                            stack_.push(t.pushElement[0]);
                        }
                        cout << " am i in " << endl;
                        if (processedStr != input.substr(0, i))
                            currentState = t.dest;
                    }
                }
            }  
            }
            cout << processedStr << " orig " << input << endl;
            if (stack_.top() == config.initialStackSymbol[0] && processedStr == input){
                for (const auto& t : config.transitions){
                    if (t.source == currentState){
                        if (t.variable == EPSILON && t.popElement == config.initialStackSymbol){
                            currentState = t.dest;
                        }
                    }
                }
            }
            std::cout << "currentState: " << currentState << std::endl;
            
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
            printVector(config.goalStates, "Goal States");
        } else if (line_number == 7) {
            istringstream iss(line);
            for (string token; iss >> token;) {
                config.stackAlphabet.push_back(token);
            }
            printVector(config.stackAlphabet, "Stack Alphabet");
        } else if (line_number == 8) {
            config.initialStackSymbol = line;
        } else if (line_number == 9) {
            istringstream iss(line);
            int token;
            while (iss >> token) {
                config.inputAlphabet.push_back(token);
            }
            printVector(config.inputAlphabet, "Input Alphabet");
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
                cout << "Transition: " << state_from << " --(" << variable << ", " << pop_element << ", " << push_element << ")--> " << state_to << endl;
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
    string file_path = "./input1.txt";
    PDAConfig config = readFile(file_path);
    PushdownAutomaton pda(config);
    for(const string& input: config.inputStrings){
        pda.processPDA(input);
    }

    return 0;
}
