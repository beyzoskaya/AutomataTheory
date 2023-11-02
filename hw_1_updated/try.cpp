#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

void extract_number(const std::string& line, int &result) {
    result = std::stoi(line.substr(0, line.find("//")));
}

void printVector(const std::map<std::pair<std::string, char>, std::string>  v){
    for (const auto& entry : v) {
        const auto& key = entry.first;
        const auto& value = entry.second;
        std::cout << "Key: (" << key.first << ", " << key.second << "), Value: " << value << std::endl;
    }

    
}

void printVector(std::vector<std::string> v){
    std::cout <<  "{ ";
    for (const auto& i : v) {
        std::cout << i << ", "; 
    }
    std::cout << "}" << std::endl;

    
}

void read_dfa_from_file(const std::string& file_path, int &num_states, int &num_variables, int &num_goal_states, std::vector<std::string> &states, std::vector<std::string> &goal_states, std::vector<std::string> &variables, std::map<std::pair<std::string, char>, std::string> &transitions, std::vector<std::string> &input_strings, std::string &start_state) {
    num_states = 0;
    num_variables = 0;
    num_goal_states = 0;
    start_state = "";

    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        exit(1);
    }

    std::string line;
    int line_number = 0;

    bool reading_transitions = false;

    while (std::getline(file, line)) {
        if (line_number == 0) {
            extract_number(line, num_states);
        }
        else if (line_number == 1) {
            extract_number(line, num_variables);
        }
        else if (line_number == 2) {
            extract_number(line, num_goal_states);
        }
        else if (line_number == 3) {
            std::istringstream iss(line);
            for (std::string token; iss >> token; ) {
                states.push_back(token);
            }
            reading_transitions = false;
        }
        else if (line_number == 4) {
            std::istringstream iss(line);
            for (std::string token; iss >> token; ) {
                goal_states.push_back(token);
            }
            reading_transitions = false;
        }
        else if (line_number == 5) {
            std::istringstream iss(line);
            for (std::string token; iss >> token; ) {
                variables.push_back(token);
            }
            reading_transitions = false;
        }
        else if (line_number >= 6) {
            start_state = states[0];
            //std::cout << "Start state: " << start_state << std::endl;
            if (line.length() == 7) {
                reading_transitions = true;
                std::istringstream iss(line);
                std::string state_from, variable, state_to;
                // if (iss >> state_from >> variable >> state_to) {
                //     std::cout << "iam in 7.5" << std::endl;
                //     transitions[std::make_pair(state_from, variable[0])] = state_to;
                //     std::cout << "Transition: " << state_from << " --(" << variable[0] << ")--> " << state_to << std::endl;
                // }
                //parseLine();
                std::vector<std::string> tokens;

                std::string token;
                while (iss >> token)
                    tokens.push_back(token);
                printVector(tokens);
                state_from = tokens[0];
                variable = tokens[1];
                state_to = tokens[2];

                transitions[std::make_pair(state_from, variable[0])] = state_to;
                std::cout << "Transition: " << state_from << " --(" << variable << ")--> " << state_to << std::endl;
                // Remove the following line to handle input strings correctly
                // reading_transitions = false;
            } else {
                reading_transitions = false;
                input_strings.push_back(line);
                }
            }

        line_number++;
    }
    printVector(variables);
    file.close();
}

void implement_dfa(int num_states, int num_variables, const std::string& start_state, const std::vector<std::string>& goal_states, const std::map<std::pair<std::string, char>, std::string>& transitions, const std::vector<std::string>& input_strings) {
    std::cout << "Start state inside implement dfa: " << start_state << std::endl;
    std::string current_state = start_state;
    std::vector<std::string> route_taken;
    //route_taken.push_back(current_state);
    printVector(transitions);
    for (auto input : input_strings) {
        std::cout<<"Input for starting:"<<input<<std::endl;
        current_state = start_state;
        std::cout<<"Current state after starting:"<<current_state;
        //route_taken.push_back(current_state);
        for (char character : input) {
            std::string next_state = transitions.at(std::make_pair(current_state, character));
            route_taken.push_back(next_state);
            current_state = next_state;
        }
        if (std::find(goal_states.begin(), goal_states.end(), current_state) != goal_states.end()) {
            std::cout<<std::endl;
            std::cout<< input << ": Accepted Route Taken: ";
            printVector(route_taken);
        } else {
            std::cout<<std::endl;
            std::cout << input << ": Rejected Route Taken: ";
            printVector(route_taken);
        }
        route_taken.clear();
    }
}

int main() {
    int num_states, num_variables, num_goal_states;
    std::vector<std::string> states, goal_states, variables;
    std::map<std::pair<std::string, char>, std::string> transitions;
    std::vector<std::string> input_strings;
    std::string start_state;
    std::pair<std::vector<std::string>, std::string> result;

    read_dfa_from_file("./file2.txt", num_states, num_variables, num_goal_states, states, goal_states, variables, transitions, input_strings, start_state);
    implement_dfa(num_states, num_variables, start_state, goal_states, transitions, input_strings);
    return 0;
}
