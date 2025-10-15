# include <stdio.h>
# include "mylib.h"

int passed = 0;
int failed = 0;
char * function_name [10] = {"int_to_float", "factorial", "sum", "grade_scale", "is_even", "num_factors", "max_of_three", "days_in_month", "divide", "to_uppercase"};
float function_points [10] = {1, 1.5, 1.5, 1.5, 1, 2.5, 1.5, 1.5, 1, 2};
int failure_count [10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*
You do NOT need to read the code in this file or understand it to complete the assignment.
Please do NOT make any changes to this file.

This file is used to test the correctness of your implementation of the functions in mylib.c.

There are no hidden tests in this project. The test cases in this file are all we will use to test 
the correctness of your implementation. However, please do NOT hardcode the answers for the given test cases.
Your function should work for all valid inputs, not just the ones in the test cases.

When you want to test any of your functions, you can follow the instructions in the README.md file
to run the tests. You will see the results of the tests in the terminal.
*/


// Those assert_equal_xxx are helper functions to check if the first two inputs are equal.
// It will print the details if the test fails.
void assert_equal_int(int expected, int actual, int test_num, int function_index){
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %d, but got %d.\n", test_num, expected, actual);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_float(float expected, float actual, int test_num, int function_index){
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %f, but got %f.\n", test_num, expected, actual);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_char(char expected, char actual, int test_num, int function_index){
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %c, but got %c.\n", test_num, expected, actual);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_bool(bool expected, bool actual, int test_num, int function_index){   
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %s, but got %s.\n", test_num, expected ? "true" : "false", actual ? "true" : "false");
        failure_count[function_index]++;
        failed++;
    }
}

// test_xxx are the test functions.
void test_int_to_float(){
    printf("--------------------------------\n");
    printf("Testing int_to_float...\n");
    int input [] = {0, -2, 1, 4, 10000};
    float expected [] = {0.0, -2.0, 1.0, 4.0, 10000.0};
    for (int i = 0; i < 5; i++){
        float output = int_to_float(input[i]);
        printf("Test No.%d Input: %d\n", i + 1, input[i]);
        assert_equal_float(expected[i], output, i + 1, 0);
    }
}

void test_factorial(){
    printf("--------------------------------\n");
    printf("Testing factorial...\n");
    int input [] = {-2, 0, 1, 9, 10};
    int expected [] = {-1, 1, 1, 362880, 3628800};
    for (int i = 0; i < 5; i++){
        int output = factorial(input[i]);
        printf("Test No.%d Input: %d\n", i + 1, input[i]);
        assert_equal_int(expected[i], output, i + 1, 1);
    }
}

void test_sum(){
    printf("--------------------------------\n");
    printf("Testing sum...\n");
    int input [] = {-6, 0, 1, 10, 100};
    int expected [] = {-21, 0, 1, 55, 5050};
    for (int i = 0; i < 5; i++){
        int output = sum(input[i]);
        printf("Test No.%d Input: %d\n", i + 1, input[i]);
        assert_equal_int(expected[i], output, i + 1, 2);
    }
}

void test_grade_scale(){
    printf("--------------------------------\n");
    printf("Testing grade_scale...\n");
    float input [] = {-10, 0, 100, 60, 1100, 90, 70, 80, 75};
    char expected [] = {'X', 'F', 'A', 'D', 'X', 'A', 'C', 'B', 'C'};
    for (int i = 0; i < 9; i++){
        char output = grade_scale(input[i]);
        printf("Test No.%d Input: %f\n", i + 1, input[i]);
        assert_equal_char(expected[i], output, i + 1, 3);
    }
}

void test_is_even(){
    printf("--------------------------------\n");
    printf("Testing is_even...\n");
    int input [] = {-1, 0, 1, 3000, -35, 678};
    bool expected [] = {false, true, false, true, false, true};
    for (int i = 0; i < 6; i++){
        bool output = is_even(input[i]);
        printf("Test No.%d Input: %d\n", i + 1, input[i]);
        assert_equal_bool(expected[i], output, i + 1, 4 );
    }
}

void test_num_factors(){
    printf("--------------------------------\n");
    printf("Testing num_factors...\n");
    int input [] = {-12, 0, 1, 19, 720, 32768};
    int expected [] = {-1, -1, 1, 2, 30, 16};
    for (int i = 0; i < 6; i++){
        int output = num_factors(input[i]);
        printf("Test No.%d Input: %d\n", i + 1, input[i]);
        assert_equal_int(expected[i], output, i + 1, 5);
    }
}

void test_max_of_three(){
    printf("--------------------------------\n");
    printf("Testing max_of_three...\n");
    int input_a [] = {-100, 9, 3, 1001, -1004};
    int input_b [] = {100, 9, 2, 1002, -1004};
    int input_c [] = {0, 9, 1, 1003, -1003};
    int expected [] = {100, 9, 3, 1003, -1003};
    for (int i = 0; i < 5; i++){
        int output = max_of_three(input_a[i], input_b[i], input_c[i]);
        printf("Test No.%d Input: %d, %d, %d\n", i + 1, input_a[i], input_b[i], input_c[i]);
        assert_equal_int(expected[i], output, i + 1, 6);
    }
}

void test_days_in_month(){
    printf("--------------------------------\n");
    printf("Testing days_in_month...\n");
    int input [] = {9, 1, 2, 0, 15, -50, 3, 4, 5, 6, 7, 8, 10, 11, 12};
    int expected [] = {30, 31, 28, -1, -1, -1, 31, 30, 31, 30, 31, 31, 31, 30, 31};
    for (int i = 0; i < 15; i++){
        int output = days_in_month(input[i]);
        printf("Test No.%d Input: %d\n", i + 1, input[i]);
        assert_equal_int(expected[i], output, i + 1, 7);
    }
}

void test_divide(){
    printf("--------------------------------\n");
    printf("Testing divide...\n");
    int input_a [] = {10, -9, -5, 1000, 2};
    int input_b [] = {16, -9, 4, -16, 0};
    float expected [] = {0.625, 1.0, -1.25, -62.5, -1.0};
    for (int i = 0; i < 5; i++){   
        float output = divide(input_a[i], input_b[i]);
        printf("Test No.%d Input: %d, %d\n", i + 1, input_a[i], input_b[i]);
        assert_equal_float(expected[i], output, i + 1, 8);
    }
}

void test_to_uppercase(){
    printf("--------------------------------\n");
    printf("Testing to_uppercase...\n");
    char input [] = {'a', 'D', '2', 92, 125, 'z', 'f', 'A'};
    char expected [] = {'A', '0', '0', '0', '0', 'Z', 'F', '0'};
    for (int i = 0; i < 8; i++){
        char output = to_uppercase(input[i]);
        printf("Test No.%d Input: %c\n", i + 1, input[i]);
        assert_equal_char(expected[i], output, i + 1, 9);
    }
}


// main function calls all the test functions and summarizes the results.
int main(){
    test_int_to_float();
    test_factorial();
    test_sum();
    test_grade_scale();
    test_is_even();
    test_num_factors();
    test_max_of_three();
    test_days_in_month();
    test_divide();
    test_to_uppercase();
    printf("-------------------Summary-------------------\n");
    printf("Total tests: %d\n", passed + failed);
    printf("Total tests passed: %d\n", passed);
    printf("Total tests failed: %d\n", failed);
    printf("--------------------------------\n");
    int num_function_passed = 0;
    float total_points = 0;
    for (int i = 0; i < 10; i++){
        if (failure_count[i] == 0){
            num_function_passed++;
            total_points += function_points[i];
        }
    }
    printf("Total functions passed: %d\n", num_function_passed);
    printf("points for correctness: %f/15, the other 10 points are for comments and git commits, please read the document for more details.\n", total_points);
    return 0;
}