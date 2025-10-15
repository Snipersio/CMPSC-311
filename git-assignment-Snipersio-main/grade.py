#!/usr/bin/env python3
import os
import re
import subprocess
import base64

DEFAULT_VALUES = {
    'NAME': 'JOHNDOE',
    'PSUID': '000000000',
    'PSUEMAIL': 'abc1234',
    'GITHUB': 'yourgithubusername'
}


def check_file_exists(filename):
    """Check if a file exists in the current directory."""
    return os.path.isfile(filename)

def check_file_not_empty(filename):
    """Check if a file exists and is not empty."""
    if check_file_exists(filename):
        with open(filename, 'r') as f:
            content = f.read().strip()
            return len(content) > 0
    return False

def check_hostname(hostname):
    """Check if the 'hostname' matches the required pattern."""
    # Regex pattern for "e5-cse-135-??.cse.psu.edu", where ?? is a number between 01 and 28
    pattern = r"^e5-cse-135-(0[1-9]|1[0-9]|2[0-8])\.cse\.psu\.edu$"
    return bool(re.match(pattern, hostname))
    
def parse_id_file(id_file_path):
    """Parse the ID.txt file to extract specific variables."""
    data = {}
    try:
        with open(id_file_path, 'r') as f:
            for line in f:
                # Split line by '=' and remove extra whitespace
                key, value = line.strip().split('=')
                # If the key is "email", store only the part before the '@'
                if key == 'PSUEMAIL':
                    value = value.split('@')[0]  # Extract part before '@'
                
                # Add key-value pair to the dictionary
                data[key] = value
    except Exception as e:
        print(f"Error reading {id_file_path}: {e}")
    return data
    
def parse_proof_file(proof_file_path):
    """Parse the proof file to extract the 'user' and 'hostname' variables, decoding Base64-encoded key-value pairs, including the '='."""
    proof_data = {}
    try:
        with open(proof_file_path, 'r') as f:
            for line in f:
                # Decode the whole line (key=value with '=' also Base64 encoded)
                try:
                    decoded_line = base64.b64decode(line.strip()).decode('utf-8')  # Decode the entire line

                    # Split the decoded line by '=' to get the key and value
                    key, value = decoded_line.split('=')

                    # Store the decoded key-value pair
                    proof_data[key] = value.strip()
                except Exception as decode_error:
                    print(f"Base64 decode failed for line: {line.strip()} - {decode_error}")
                    # If decoding fails, store the raw (encoded) line
                    proof_data[line.strip()] = None

    except Exception as e:
        print(f"Error reading {proof_file_path}: {e}")
    
    return proof_data


def get_commit_count():
    try:
        # Get the current branch name
        current_branch = subprocess.check_output(['git', 'rev-parse', '--abbrev-ref', 'HEAD']).strip().decode()
        
        # Get the number of commits in the current branch
        commit_count = subprocess.check_output(['git', 'rev-list', '--count', current_branch]).strip().decode()
        
        return int(commit_count)
    except subprocess.CalledProcessError as e:
        print(f"Error executing git command: {e}")
        return None
        
def grade_assignment():
    print("---------------------------------------------------------------")
    print("| Points |   Task")
    print("---------------------------------------------------------------")
    score = 0

    # Check if 'name.txt' exists and is not empty
    if check_file_exists('name.txt'):
        if check_file_not_empty('name.txt'):
            print("| 5      | Found 'name.txt' file and it is not empty.")
            score += 5  # Award points for non-empty 'name.txt'
        else:
            print("| 1      | Found 'name.txt' file but it is empty.")
            score += 1  # Award points for non-empty 'name.txt'
    else:
        print("| 0      | No 'name.txt' file found.")
    
    # Check if 'ID.txt' exists and parse it
    if check_file_exists('ID.txt'):
        id_data = parse_id_file('ID.txt')
        # Checking if all required keys exist in ID.txt
        required_keys = ['NAME', 'PSUID', 'PSUEMAIL', 'GITHUB']
        missing_keys = [key for key in required_keys if key not in id_data]
        
        if not missing_keys:
            # Now check if the values are not the default ones
            invalid_keys = [key for key in required_keys if id_data[key] == DEFAULT_VALUES[key]]
            if invalid_keys:
                print(f"| 0      | The following keys have default values: {', '.join(invalid_keys)}")
            else:
                print("| 5      | All required keys in 'ID.txt' are present and valid.")
                score += 5  # Award points for correct 'ID.txt' format and valid values 
        else:
            print(f"| 0      | Missing [{', '.join(missing_keys)}] in 'ID.txt'")
    else:
        print("| 0      | No 'ID.txt' file found.")
    
    # Check if 'proof' file exists and parse it
    if check_file_exists('proof'):
        proof_data = parse_proof_file('proof')
        # Compare 'user' from proof file with 'PSUEMAIL' from ID.txt
        if 'user' in proof_data and 'PSUEMAIL' in id_data:
            if proof_data['user'] == id_data['PSUEMAIL']:
                print("| 3      | The 'user' from 'proof' matches 'PSUEMAIL' from 'ID.txt'.")
                score += 3  # Award points for correct user match
            else:
                print(f"| 0      | The 'user' from 'proof' ({proof_data['user']}) does not match 'PSUEMAIL' ({id_data['PSUEMAIL']}) from 'ID.txt'.")
        else:
            print("Missing 'user' in 'proof' file or 'PSUEMAIL' in 'ID.txt'.")
            # Check for 'hostname' in the proof data and if it matches the required pattern
        if 'hostname' in proof_data:
            if check_hostname(proof_data['hostname']):
                print("| 2      | The 'hostname' from 'proof' matches the required pattern.")
                score += 2  # Award points for correct hostname
            else:
                print(f"| 0      | The 'hostname' from 'proof' ({proof_data['hostname']}) does not match the required pattern.")
        else:
            print("Missing 'hostname' in 'proof' file.")
    else:
        print("| 0      | No 'proof' file found.")

    # Check number of commits
    commit_count = get_commit_count();
    if commit_count > 3:
        print("| 10     | 4 or more git commits found")
        score += 10  # Award points for correct hostname
    elif commit_count < 2:
        print("| 0      | less than 2 git commits found")
        score += 0  # Award points for correct hostname
    elif commit_count == 2:
        print("| 3      | Only 2 git commits found")
        score += 3  # Award points for correct hostname
    elif commit_count == 3:
        print("| 6      | Only 3 git commits found")
        score += 6  # Award points for correct hostname

    print("---------------------------------------------------------------")
    print(f"Total score: {score} points.\n")
    return score

# Run the grading function
grade_assignment()
