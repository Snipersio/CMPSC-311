# include "mylib.h"
# include <stdio.h>


/*
This is the implementation of the functions in mylib.h. 
Please read the comments in mylib.h before you start working on this file.
*/
float int_to_float(int input){
    // TODO: convert int to float
	return (float)input; //returns inputted int as a float with type casting
}

int factorial(int input){
    // TODO: calculate factorial
    	int product = 1; //Create var to keep track of total calculations when interating through loops
	if (input < 0){ //If the input is out of the range for factorial
		return -1; //returns asked value of -1 for inproper inputs
	}
	while (input > 0){ //beining interation to calculate the factorial of the inputed number
	        product *= input; //multiply the current total of product with the current interaction of the inputed value
       		input--; //subtact 1 to input so that it will iterate the the next value to properly calculate factorial
	}	
	return product; //returns the total product of the calculation from the interations within the while loop
}

int sum(int input){
    // TODO: calculate sum
    int total = 0; //Var created to keep track of the total calculated when interating 
    if (input > 0) { //Interates the sum of a number to 0m if inputted number is greater than 0
        while (input > 0) { //Begins the interation
            total += input; //Total is keeping track of currently calcualted sum while interating
            input--; //Input decreases so that it will iterate through ever whole number until it reaches 0
        }
    } else if (input < 0) { //If the number is less than 0
        while (input < 0) { //Begins interating through all numbers between input and 0
            total += input; //keeps track of total summed during the interations
            input++; //Increases the input value by 1 so that it will eventually reach the looping stop condition of 0.
        }
    }
    return total; //Returns the total summed of all whole numbers between the input number and 0
}


char grade_scale(float score){
    // TODO: convert score to grade
    if (score > 100 || score < 0){ //If Score is out of range of possible scores returns X
	    return 'X';
    }
    if (score >= 90){ //If score is in the range of a A
	    return 'A';
    }
    else if (score >= 80){ //If score is in the range of a B
	    return 'B';
	    }
    else if (score >= 70){ //If score is in the range of a C
	    return 'C';
    }
    else if (score >= 60){ //If score is in the range of a D
	    return 'D';
    }
    else {
	    return 'F'; //If score is in the range of a F
    }
}

bool is_even(int input){
    // TODO: check if input is even
    return (input%2 == 0); //If Module a input and 2 is 0 that means that there is no remainder, meaning it's even
}

int num_factors(int input){
    // TODO: calculate number of factors
	int amount = 0;    //Reponsible for keeping track of currently counted factors during iterations
	if (input <= 0){ //Input can't ne negative so returns -1 if so
	    return -1;
    }

    for (int i = 1; i <= input; i++){ //Iterates through all whole numbers from 1 to input to check if they are each a factor of input
	    if (input%i ==0){ //If input divded by current i gives 0 then it has to be a factor
		    amount++; //Increases factor counter by 1
	    }
    }
    return amount; //returns the total amount of factors for input
}

int max_of_three(int a, int b, int c){
    // TODO: find max of three
    if (a >= b && a >=c){ //If a is greater than b and c
	    return a;
    }
    if (b >= a && b >= c){ //If b is greater than a and c
	    return b;
    }
    else{
	    return c; //If C is the greatest number
    }
}

int days_in_month(int month){
    // TODO: find days in month
    if (month < 1 || month > 12) { //If the inputed month is outside the range of the month numbers
        return -1;
    }
    if (month== 1 || month== 3 || month== 5 || 
        month== 7 || month== 8 || month== 10 || month== 12) { //Includes the number of every month that has 31 days
        return 31;
    } else if (month== 4 || month== 6 || month== 9 || month== 11) { //Inlcudes the number of every month that has 30 days
        return 30;
    } else { //Only Febuary has 28 days
        return 28;
    }
   
}

float divide(int a, int b){
    // TODO: divide a by b
    if ( b==0){ //Within real division the divisor can not be 0
	    return -1;
    }
    return ((float)a / (float)b); //Returns a/b but calculates both as float values with the return type being a float as well
}

char to_uppercase(char input){
    // TODO: convert lowercase to uppercase
    if (input >= 'a' && input<= 'z'){ //To be a lowercase letter the ASCII value has to be between 'a' and 'z'
	    return (input - ('a' - 'A')); //A leter's lower case value minus uppercase value gives the exact amount needed to subtract from a lower to turn it into an uppercase
	}
    else { //If input is not a lowercase character from the alphabet
    return '0';
    }
}
