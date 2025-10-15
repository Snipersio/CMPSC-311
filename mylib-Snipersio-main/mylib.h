#include <stdbool.h>

/* 
This is a header file for mylib.c. It has the declarations of the functions in mylib.c and the comments
explaining what the functions do. Please read this file carefully before you start working to complete 
the code in mylib.c. Please do NOT make any changes to this file.

For all of the following functions, you do not need to worry about integer overflow (i.e., a number is too
large to be stored in an int type variable), since the values in test cases are not so large.
*/

/*
This function converts an integer to a float.
*/
float int_to_float(int input);


/*
This function calculates the factorial of the input integer. The factorial of a non-negative integer n
is the product of all positive integers less than or equal to n. For example, the factorial of 3 is 
3! = 3 * 2 * 1 = 6. If the input is negative, return -1.
*/
int factorial(int input);

/*
This function calculates the sum of all integers from 0 to the input integer. For example, if the input
is 4, the output should be 0 + 1 + 2 + 3 + 4 = 10. If the input is negative, return the sum from input to 0.
For example, if the input is -3, the output should be -3 + -2 + -1 + 0 = -6.
*/
int sum(int input);

/*
This function converts a score into a grade. The input is a float number between 0 and 100. The output 
will be a char type value representing the grade (In uppercase). If the input is not a valid score
(less than 0 or greater than 100), return 'X'.
The mapping is as follows:
- [90-100] A
- [80-90) B
- [70-80) C
- [60-70) D
- [ 0-60) F
*/
char grade_scale(float score);

/*
This function checks if the input integer is an even number. Return true if it is, otherwise return false.
*/
bool is_even(int input);

/*
This functions calculates the number of factors of the input integer. A factor of a number is an integer 
that divides the number exactly, without leaving a remainder. For example, 12 has 6 factors: 1, 2, 3, 4, 6, 12.
The corresponding output should be 6 if the input is 12. If the input is negative or 0, return -1.
*/
int num_factors(int input);

/*
This function takes 3 integers as input, and returns the largest value.
*/
int max_of_three(int a, int b, int c);

/*
This function takes an integer indicating the month of this year (2025), and returns the number of days in that month.
For example, if the input is 2, the output should be 28. If the input is not a valid month, return -1.
*/
int days_in_month(int month);

/*
This function takes two integers as input, and returns the quotient of the first integer divided by the second 
integer. The division is REAL DIVISION, i.e., the result is a float number. For example, if the input is 5 and 2,
the output should be 2.5. If the second integer is 0, return -1.
*/
float divide(int a, int b);

/*
This function converts a lowercase letter to an uppercase letter. For example, if the input is 'a', the output should be 'A'.
If the input is not a lowercase letter, return '0'.
*/
char to_uppercase(char input);
