def read_dfa_from_file(file_path):
    num_states = None
    num_variables = None
    num_goal_states = None
    states = []
    goal_states = []
    variables = []
    transitions = {}
    input_strings = []
    start_state = None

    with open(file_path, 'r') as file:
        lines = file.read().splitlines()

        def extract_number(line):
            return int(line.split('//')[0].strip())

        num_states = extract_number(lines[0])
        num_variables = extract_number(lines[1])
        num_goal_states = extract_number(lines[2])
        states = lines[3].split()
        goal_states = lines[4].split()
        variables = lines[5].split()
        start_state = lines[6].split()[0]
        #print(f"Start state split: {start_state}")

        reading_transitions = False
        reading_input = False  # Initialize reading_input to False
        for line in lines[6:]:
            #print(f"Lines: {line}")
            parts = line.split('//')
            cleaned_line = parts[0].strip()
            #print(f"Cleaned line: {cleaned_line}")
            if len(cleaned_line)==7: 
                reading_transitions = True # reading_transition = True

                if reading_transitions: # sonra buraya girince input string'te buraya giriyo onu handlela
                    #print(f"Cleaned {cleaned_line}")
                    state_from, variable, state_to = cleaned_line.split() # 
                    #print(f"State from: {state_from}")
                    #print(f"Variable: {variable}")
                    #print(f"State to: {state_to}")
                    key = (state_from, variable)
                    print(f"Key for transition meaning from state and variable: {key}")
                    transitions[key] = state_to
                    print(f"new state to take action: {transitions[key]}")
                    #transitions[f"{state_from} {variable} {state_to}"] = state_to
                    reading_transitions = False
                    

            if len(cleaned_line) != 7:
                reading_transitions = False
                reading_input = True

            if reading_input:
                parts = line.split('//')
                cleaned_line = parts[0].strip()
                #print(f"Cleaned line: {cleaned_line}")
                if cleaned_line :
                    input_strings.append(cleaned_line)
                    #print(f"Input strings: {cleaned_line}")


    return num_states, num_variables, num_goal_states, states,start_state, goal_states, variables, transitions, input_strings


def implement_dfa(num_states, num_variables,start_state, goal_states, transitions, input_string):
    print(f"Start state inside implement dfa: {start_state}")
    current_state = start_state
    route_taken = []
    route_taken.append(current_state)

    input_string = input_string.split(' ')
    print(f"input : {input_string}")
    for char in input_string[0]:
        next_state = transitions[(current_state, char)]
        route_taken.append(next_state)
        current_state = next_state
    
    if current_state in goal_states:
        return route_taken, "Accepted"  # Reached a goal state and input string is empty
    else:
        return route_taken, "Rejected"  # Didn't reach a goal state or input string is not empty


def main():
    num_states, num_variables, num_goal_states, states, start_state, goal_states, variables, transitions, input_strings = read_dfa_from_file('./file1.txt')

    # print("Number of states:", num_states)
    # print("Number of variables:", num_variables)
    # print("Number of goal states:", num_goal_states)
    # print("States:", ' '.join(states))
    # print("Goal states:", ' '.join(goal_states))
    # print("Variables:", ' '.join(variables))
    # print("Transitions:")
    # for (state_from, variable), state_to in transitions.items():
    #     print(f"{state_from} --({variable})--> {state_to}")

    #print("Input strings:")
    for input_string in input_strings:
        route_taken, result = implement_dfa(num_states, num_variables, start_state, goal_states, transitions, input_string)

        #route_taken = route_taken[1:]
        print("Input string:", ' '.join(input_string))
        print("Route taken:", ' '.join(route_taken))
        print("Result:", result)

if __name__ == "__main__":
    main()

