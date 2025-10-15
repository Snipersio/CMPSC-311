#include <stdbool.h>

/* 
This is a header file for arraylib.c. It has the declarations of the functions in arraylib.c and the comments
explaining what the functions do. Please read this file carefully before you start working to complete 
the code in arraylib.c. Please do NOT make any changes to this file.

For all of the following functions, you do not need to worry about integer overflow (i.e., a number is too
large to be stored in an int type variable), since the values in test cases are not so large.
*/

/*
This function finds all the indices of the target value in the array.
@ param arr: the array to search
@ param size: the size of the array
@ param target: the value to search for
@ param results: the array to store the indices of the target value. The size of this array will be the same
as the size of the input array. Initially all elements are set to -1, and you should fill the indices of the 
target value into this array from left to right, and leave the other elements unchanged.
@ return: the number of occurrences of the target value

@ examples:
arr = [4, 3, 2, 3, 4], size = 5, target = 4, results = [-1, -1, -1, -1, -1] -> return 2, results = [0, 4, -1, -1, -1]
arr = [1, 2, 3], size = 3, target = 1, results = [-1, -1, -1] -> return 1, results = [0, -1, -1]
arr = [], size = 0, target = 1, results = [] -> return 0, results = []
arr = [1, 1, 2, 3], size = 4, target = 1, results = [-1, -1, -1, -1] -> return 2, results = [0, 1, -1, -1]
arr = [1, 2, 3], size = 3, target = 4, results = [-1, -1, -1] -> return 0, results = [-1, -1, -1]
*/
unsigned int find_all_matches(int arr[], int size, int target, int results[]);

/*
This function sums up all the positive numbers in the array.
@ param arr: the array to sum up
@ param size: the size of the array
@ return: the sum of all the positive numbers in the array

@ examples:
arr = [1, 2, 3], size = 3 -> return 6
arr = [], size = 0 -> return 0
arr = [-1, -2, -3], size = 3 -> return 0
arr = [1, -2, 3], size = 3 -> return 4
*/
unsigned int sum_positives(int arr[], int size);


/*
This function checks if the array contains any prime numbers. A prime number is a number that is greater than 1 
and has no positive factors other than 1 and itself.
@ param arr: the array to check
@ param size: the size of the array
@ return: true if the array contains any prime numbers, false otherwise

@ examples:
arr = [], size = 0 -> return false
arr = [4, 4, 4], size = 3 -> return false
arr = [1, 4, 4], size = 3 -> return false
arr = [1, 2, 4], size = 3 -> return true
arr = [4, 6, 31], size = 3 -> return true
*/
bool contains_prime(unsigned int arr[], int size);


/*
This function checks if the array is sorted in descending order. A descending order array is an array where 
each element is less than or equal to the element before it.
@ param arr: the array to check
@ param size: the size of the array
@ return: true if the array is sorted in descending order, false otherwise. If the array is empty, return true.

@ examples:
arr = [4, 3, 1], size = 3 -> return true
arr = [4, 4, 4], size = 3 -> return true
arr = [4, 3, 5], size = 3 -> return false
arr = [1, 2, 3], size = 3 -> return false
*/
bool is_reverse_sorted(int arr[], int size);

/*
This function finds the maximum value in the array.
@ param arr: the array to find the maximum value in
@ param size: the size of the array
@ return: the maximum value in the array. If the array is empty, return 0.

@ examples:
arr = [1, 2, 3], size = 3 -> return 3
arr = [], size = 0 -> return 0
arr = [-1, -2, -3], size = 3 -> return -1
arr = [1, -2, 3], size = 3 -> return 3
*/
int find_max(int arr[], int size);


/*
The following functions are related to strings. To review what we have learned in class: 
In C language, a string is an array of characters that ends with a null terminator ('\0'). 
For example, the string "hello" is represented as the array ['h', 'e', 'l', 'l', 'o', '\0']. 
This code:
char str[] = "hello";
is equivalent to:
char str[] = {'h', 'e', 'l', 'l', 'o', '\0'};
The length of a string is usually defined as the number of characters in the array, NOT including the null terminator.
For example, the length of the STRING "hello" is 5, while the size of the ARRAY ['h', 'e', 'l', 'l', 'o', '\0'] is 6.
*/

/*
This function returns the length of the string in the array.
@ param arr: the array which contains the string
@ param max_length: the maximum length of the string (NOT including the null terminator)
@ return: the length of the string (NOT including the null terminator) in the array, if the string is longer than max_length, return -1

@ examples:
arr = "hello", max_length = 10 -> return 5
arr = "hello", max_length = 5 -> return 5
arr = "hello", max_length = 3 -> return -1
*/
int strlenm(char arr[], int max_length);


/*
This function counts the number of words in a string. Words are separated by any of the following tokens: 
[' ', '\t', '\n', '-'].
@ param arr: the array which contains the string, it ends with a null terminator
@ return: the number of words in the string

@ examples:
arr = "hello"-> return 1
arr = ""-> return 0
arr = "hello world"-> return 2
arr = "hello  world"-> return 2
arr = "     "-> return 0
arr = " \t\n-"-> return 0
arr = "a b\tc\nd-e"-> return 5
arr = "abc123 974i"-> return 2
*/
unsigned int strcnt(char arr[]);

/*
This function finds the first occurrence of a character in a string.
@ param arr: the array which contains the string, it ends with a null terminator
@ param target: the character to search for
@ return: the index of the first occurrence of the character in the string, if the character is not found, return -1

@ examples:
	arr = "hello", target = 'h' -> return 0
	arr = "hello", target = 'l' -> return 2
	arr = "hello", target = 'z' -> return -1
	arr = "hello", target = '\0' -> return 5
*/
int strfind(char arr[], char target);