# include "arraylib.h"
# include <stdio.h>


/*
This is the implementation of the functions in arraylib.h. 
Please read the comments in arraylib.h before you start working on this file.
*/

unsigned int find_all_matches(int arr[], int size, int target, int results[]){
    unsigned int counter = 0; //Creating counter for number of matches found
    for (int i = 0; i<size; i++){ //loop to iterate through array
	    if (arr[i]==target){ //If current index in array equals the target
		    results[counter]=i; //Adds index that target was found in
		    counter++; //Increments the counter by 1
	    }
    }
    return counter; //returns the number of matches found in the inputted array
}

unsigned int sum_positives(int arr[], int size){
    unsigned int sum = 0; //Creates variable to hold the current sum
    for (int i = 0; i<size; i++){ //Iterate through all the values in the inputted array
	    if (arr[i] > -1){ //Makes sure not to sum values that are negative
		    sum += arr[i]; //Adds value of current element to the sum variable
	    }
    }
    return sum; //returns the total sum of all the values in the array that are posotive
}

bool contains_prime(unsigned int arr[], int size){
    for (int i = 0; i < size; i++) { //Iterates through the input array
        unsigned int value = arr[i]; //Pulls the current value from the current index
        if (value <= 1) { //If current value is equal or less than 1
            continue;
        }
        if (value == 2) { //If current value is equal to 2 that prime
            return true;
        }
        if (value % 2 == 0) { //If current value os even
            continue;
        }
        bool prime = true; //Defaults to true
        for (unsigned int var = 3; var * var <= value; var += 2) { //Checking divisiblity of prime number with calculations
            if (value % var == 0) { //If current value module current var is 0
                prime = false;
                break;
            }
        }
        if (prime == true) { //If the current value is prime than returns true
            return true;
        }
    }
    return false;
}

bool is_reverse_sorted(int arr[], int size){
    if (size == 0) { //A 0 sized array is reversed sorted
        return true;
    }
    for (int i = 1; i < size; i++) { //Iterate through the array
        if (arr[i] <= arr[i - 1]) { //Makes sure that current index value is smaller than last index value
            continue;
        }
        return false; //If the current index value is not smaller than the last index value
    }
    return true; //Returns true if the entire array is reverse sorted
}

int find_max(int arr[], int size){
    if (size == 0) return 0; //If array is empty than returns 0
    int max = arr[0]; //Creating max variable with first index of inputted array
    for (int i = 0; i < size; i++){ //Iterates through inputted array
	if (arr[i] > max){ //If a new max is found during the current iteration
		max = arr[i]; //Changes the current max to the newly found max
	}
    }
    return max; //Returns the max value of the inputted array
}

int strlenm(char arr[], int max_length){
    int counter =0; //Creates a counter for the string length
    while (arr[counter] != '\0'){ //Iterates through the char array until it finds a null terminator
	counter++; //next index for next iteration
	if (counter>max_length){ //If the inputted max_length is incorrect for the inputted array
	return -1;
	}
    }
return counter; //returns the length of the inputted array not including the null terminator
}

unsigned int strcnt(char arr[]){
    unsigned int wordCount = 0; //Creates variable for holding the current word count
    for (int i = 0; arr[i] != '\0'; i++) { //Loops through the entered char array until hitting a null terminator
        if ((arr[i] != ' ' && arr[i] != '\t' && arr[i] != '\n' && arr[i] != '-') && //A new word is beginning if the current char is not a seperator and if it is the first character in the string or a previous character that was a seperator
            (i == 0 || arr[i-1] == ' ' || arr[i-1] == '\t' || arr[i-1] == '\n' || arr[i-1] == '-')) {
            wordCount++;
        }
    }
    return wordCount; //returns the word count of the inputted array
}

int strfind(char arr[], char target){ 
    	int i; //Creates interation variable
	for (i =0; arr[i] != '\0'; i++){ //Iterates through the inputed char array until hitting a null terminator
	    if (arr[i] == target) return i; //if the current iteration value equals the target than that's the output
    }
    	if (target == '\0') return i; //If the target is a null terminator than return the current from itterating previously
    	return -1; //if the target is never found
}

