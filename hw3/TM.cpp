
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

using namespace std;

struct TMTransition
{
    string currentState;
    char readSymbol;
    char writeSymbol;
    char moveDirection;
    string nextState;
};

struct TMConfig {
    int numberOfVariablesInput;
    int numberOfVariablesTape;
    int numberOfStates;
    string startState;
    vector<string> states;
    string acceptState;
    string rejectState;
    char blankSymbol;
    vector<char> tapeAlphabet;
    vector<char> inputAlphabet;
    vector<TMTransition> transitions;
    vector <string> inputStrings;
    string loopState;
};

struct Tape {
    vector<char> content;
    int currentPosition;
    char blankSymbol;
    char currentSymbol;

    explicit Tape(const TMConfig& config){
        char blankSymbol = config.blankSymbol;
        //cout << "blank symbol in explicit constructutor: " << blankSymbol << endl;
        currentPosition =0;
    }

    Tape(const TMConfig& config, const string& inputString) : blankSymbol(config.blankSymbol)  {
        //char blankSymbol = config.blankSymbol;
        //cout << "Blank symbol inside constructor:  " << blankSymbol << endl;
        for (char c: inputString) {
            content.push_back(c);
        }
        // current position to the beggining
        currentPosition = 0;
        //cout << "Initializing Tape with: " << inputString << endl;

    }
    void moveRight() {
        //cout << "Content's current position if blank? : " << content[currentPosition] << endl;
        currentPosition++;
        //cout << "content size: " << content.size() << endl;
        if (currentPosition >= content.size()) {
            //cout << "am i in" << endl;
            //cout << "Inside if stamement blank symbol: " << blankSymbol << endl;
            content.push_back(blankSymbol);
        }
        
    }

    void moveLeft() {
        if (currentPosition > 0) {
            currentPosition--;
        }
        // if start of the tape as leftmost poisiton, do nothing
    }

    void print() const {
        for(char c: content) {
            cout << c;
        }
        cout << endl;
    }
    char readCurrentSymbol() const {
        if (currentPosition < content.size()) {
            //cout << "Content while reading at current position: " << content[currentPosition] << endl;
            return content[currentPosition];
            
        } else {
            //cout << "am I in? " << endl;
            return blankSymbol; 
        }
    }

     void writeSymbol(const TMTransition& transition) {
        char symbol = transition.writeSymbol;
        if (currentPosition < content.size()) {
            content[currentPosition] = symbol;
        } else {
            content.resize(currentPosition + 1, blankSymbol);
            content[currentPosition] = symbol;
        }
    }
};

class TuringMachine {
    TMConfig config;
    Tape tape;
    string currentState;
    vector<string> route;

public:
    TuringMachine(const TMConfig& cfg): config(cfg), tape(cfg) {}

    void processInputString() {
        for(const string& inputString: config.inputStrings) {
            tape = Tape(config, inputString);
            currentState = config.startState;
            route.push_back(currentState);
            //cout << "Initial tape content for \"" << inputString << "\": ";
            tape.print();

            bool accepted = false;

            while(currentState != config.acceptState && currentState != config.rejectState){
                char currentSymbol = tape.readCurrentSymbol();
                //cout << "Current symbol : " << currentSymbol << endl;
                //cout << "Current State: " << currentState << ", Read Symbol: " << currentSymbol << endl;

                bool transitionFound = false;

                for (const TMTransition& transition: config.transitions) {
                    cout << "Checking transition: " << transition.currentState << " --("
                    << transition.readSymbol << ", " << transition.writeSymbol << ", " 
                    << transition.moveDirection << ")--> " << transition.nextState << endl;

                    if(transition.currentState == currentState && transition.readSymbol == currentSymbol){
                        executeTransition(transition);
                        transitionFound = true;
                        route.push_back(transition.nextState);
                        currentSymbol = tape.readCurrentSymbol();
                    }

                }

                if (!transitionFound) {
                    cout << "No valid transition found. Halting" << endl;
                    break;
                }

                if(currentState == config.acceptState){
                    accepted = true;
                    cout << "Accepted" << endl;
                }

                if (currentState == config.rejectState){
                    accepted = false;
                    cout << "Rejected" << endl;
                    break;
                }
            }

            if(accepted) {
                cout << "Input string \"" << inputString << "\" accepted." << endl;
                cout << "Route taken: ";
                for (const string& state: route) {
                    cout << state << " ";
                }
                cout << endl;
            } else {
                cout << "Input string \"" << inputString << "\" rejected." << endl;
                cout << "Route taken (including rejection): ";
                for (const string& state : route) {
                    cout << state << " ";
                }
                cout << endl;
            }
            route.clear();
        }
        
        
    }
    void executeTransition(const TMTransition& transition){
        cout << "Executing transition to State: " << transition.nextState << endl;
        tape.writeSymbol(transition);
        cout << "Transition's move direction:" << transition.moveDirection << endl;
        if(transition.moveDirection == 'R'){
            cout << "Current position before moving to right : " << tape.currentPosition << ", Element at current position: " << tape.readCurrentSymbol() << endl;
            //cout << "Direction right" << transition.moveDirection << endl;
            tape.moveRight();
            
        } else if(transition.moveDirection == 'L') {
            cout << "Current position before moving to left : " << tape.currentPosition << ", Element at current position: " << tape.readCurrentSymbol() << endl;
            //cout << "Direction left " << transition.moveDirection << endl;
            tape.moveLeft();
        }
        cout << "Current position: " << tape.currentPosition << ", Element at current position: " << tape.readCurrentSymbol() << endl;
        currentState = transition.nextState;
        cout << "Tape content after transition: ";
        tape.print();

    }

};


void printTMConfig(const TMConfig& config) {
    cout << "Number of Variables Input: " << config.numberOfVariablesInput << endl;
    cout << "Number of Variables Tape: " << config.numberOfVariablesTape << endl;
    cout << "Number of States: " << config.numberOfStates << endl;

    cout << "States: ";
    for (const string& state : config.states) {
        cout << state << " ";
    }
    cout << endl;

    cout << "Start State: " << config.startState << endl;
    cout << "Accept State: " << config.acceptState << endl;
    cout << "Reject State: " << config.rejectState << endl;
    cout << "Blank Symbol: " << config.blankSymbol << endl;

    cout << "Tape Alphabet: ";
    for (char symbol : config.tapeAlphabet) {
        cout << symbol << " ";
    }
    cout << endl;

    cout << "Transitions:" << endl;
    for (const TMTransition& transition : config.transitions) {
        cout << "Transition: " << transition.currentState << " --("
             << transition.readSymbol << ", " << transition.moveDirection << ", " << transition.writeSymbol
             << ")--> " << transition.nextState << endl;
    }

    cout << "Input Strings: ";
    for (const string& input : config.inputStrings) {
        cout << input << " ";
    }
    cout << endl;
}



TMConfig readTMFile(const string& file_path) {
    TMConfig config;

    ifstream file(file_path);
    if(!file.is_open()) {
        cerr << "Failed to open file: " << file_path << endl;
        exit(1);
    }

    string line;
    int line_number = 0;

    while(getline(file, line)) {
        try {
            if(line_number == 0) {
                config.numberOfVariablesInput = stoi(line);
                //cout << "Number of Variables Input: " << config.numberOfVariablesInput << endl;
            } else if (line_number == 1) {
                config.numberOfVariablesTape = stoi(line);
                //cout << "Number of Variables Tape: " << config.numberOfVariablesTape << endl;
            } else if (line_number == 2) {
                config.numberOfStates = stoi(line);
                //cout << "Number of States: " << config.numberOfStates << endl;
            } else if (line_number == 3) {
                istringstream iss(line);
                for (string token; iss >> token; ){
                    config.states.push_back(token);
                }
                //cout << "States: ";
                for (const string& state : config.states) {
                    //cout << state << " ";
                }
                //cout << endl;
            } else if (line_number == 4) {
                config.startState = line;
                //cout << "Start State: " << config.startState << endl;
            } else if (line_number == 5) {
                config.acceptState = line;
                //cout << "Accept State: " << config.acceptState << endl;
            } else if (line_number == 6) {
                config.rejectState = line;
                //cout << "Reject State: " << config.rejectState << endl;
            } else if (line_number == 7) {
                config.blankSymbol = line[0];
                //cout << "Blank Symbol: " << config.blankSymbol << endl;
            } else if (line_number == 8) {
                istringstream iss(line);
                for (char token; iss >> token; ){
                    config.tapeAlphabet.push_back(token);
                }
                //cout << "Tape Alphabet: ";
                for (char symbol : config.tapeAlphabet) {
                    //cout << symbol << " ";
                }
                //cout << endl;
            } else if (line_number == 9){
                istringstream iss(line);
                for (char token; iss >> token;){
                    config.inputAlphabet.push_back(token);
                }

                for (char symbol: config.inputAlphabet){
                    //cout << symbol << endl;
                }
            } else if (line_number > 8  && !line.empty()) {
                size_t space_pos = line.find(' ');
                if (space_pos != string::npos){
                    istringstream iss(line);
                    string state_from, readSymbol, moveDirection, writeSymbol, state_to;
                    iss >> state_from >> readSymbol >> moveDirection >> writeSymbol >> state_to;
                    config.transitions.push_back(TMTransition{state_from, readSymbol[0], moveDirection[0], writeSymbol[0], state_to});
                    //cout << "Transition: " << state_from << " --("
                    //     << variable << ", " << pop_element << ", " << push_element
                    //     << ")--> " << state_to << endl;
                } else {
                    if (line_number != 9 && !line.empty())
                    config.inputStrings.push_back(line);
                    //cout << "Input String: " << line << endl;
                }
            }
        } catch (const std::invalid_argument& e) {
            //cerr << "Error converting string to integer at line " << line_number << ": " << line << endl;
            exit(1);
        }

        line_number++;
    }

    return config;
}

int main() {
    TMConfig tmConfig = readTMFile ("./input1.txt");
    //printTMConfig(tmConfig);
    
    TuringMachine tm(tmConfig);
    tm.processInputString();

    return 0;


}