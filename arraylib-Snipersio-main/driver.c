# include <stdio.h>
# include "arraylib.h"

int passed = 0;
int failed = 0;
char * function_name [8] = {"find_all_matches", "sum_positives", "contains_prime", "is_reverse_sorted", "find_max", "strlenm", "strcnt", "strfind"};
float function_points [8] = {4, 1.5, 2.5, 2.5, 1.5, 2.5, 4, 1.5};
int failure_count [8] = {0, 0, 0, 0, 0, 0, 0, 0};

/*
You do NOT need to read the code in this file or understand it to complete the assignment.
Please do NOT make any changes to this file.

This file is used to test the correctness of your implementation of the functions in arraylib.c.

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

void assert_equal_int_array(int expected[], int actual[], int size, int test_num, int function_index){
    for (int i = 0; i < size; i++){
        if (expected[i] != actual[i]){
            printf("[ERROR] Test No.%d failed. Expected array and actual array are not equal.\n", test_num);
            printf("Expected: ");
            for (int j = 0; j < size; j++){
                printf("%d ", expected[j]);
            }
            printf("\nActual: ");
            for (int j = 0; j < size; j++){
                printf("%d ", actual[j]);
            }
            printf("\n");
            failure_count[function_index]++;
            failed++;
            return;
        }
    }
    printf("Test No.%d passed\n", test_num);
    passed++;
}

// test_xxx are the test functions.
void test_find_all_matches(){
    printf("--------------------------------\n");
    printf("Testing find_all_matches...\n\n");

    int sizes[] = {5, 4, 0, 5, 4, 6};

    int input_arr0 [5] = {0, -2, 1, 4, 10000};
    int input_arr1 [4]= {4, 5, 7, 4};
    int * input_arr2 = NULL; // empty array, accessing input_arr2[0] will cause a segmentation fault
    int input_arr3 [5] = {-9, -9999, 0, 9999, 1};
    int input_arr4 [4] = {3, 2, 1, 3};
    int input_arr5 [6] = {6, 6, 6, 6, 6, 6};
    int * input_arrs [] = {input_arr0, input_arr1, input_arr2, input_arr3, input_arr4, input_arr5};
    
    int input_targets [6] = {10000, 4, 8, 2, 1, 6};

    int input_results0 [5] = {-1, -1, -1, -1, -1};
    int input_results1 [4] = {-1, -1, -1, -1};
    int * input_results2 = NULL;
    int input_results3 [5] = {-1, -1, -1, -1, -1};
    int input_results4 [4] = {-1, -1, -1, -1};
    int input_results5 [6] = {-1, -1, -1, -1, -1, -1};
    int * input_results [] = {input_results0, input_results1, input_results2, input_results3, input_results4, input_results5};

    int expected_results0 [5] = {4, -1, -1, -1, -1};
    int expected_results1 [4] = {0, 3, -1, -1};
    int * expected_results2 = NULL;
    int expected_results3 [5] = {-1, -1, -1, -1, -1};
    int expected_results4 [4] = {2, -1, -1, -1};
    int expected_results5 [6] = {0, 1, 2, 3, 4, 5};
    int * expected_results []= {expected_results0, expected_results1, expected_results2, expected_results3, expected_results4, expected_results5};

    int expected_returns [6] = {1, 2, 0, 0, 1, 6};
    int actual_returns [6] = {-2, -2, -2, -2, -2, -2};
    for (int i = 0; i < 6; i++){
        printf("Test No.%d:\nInput array: ", i + 1);
        for (int j = 0; j < sizes[i]; j++){
            printf("%d ", input_arrs[i][j]);
        }
        printf("\nTarget: %d\n", input_targets[i]);
        int output = find_all_matches(input_arrs[i], sizes[i], input_targets[i], input_results[i]);
        assert_equal_int(expected_returns[i], output, i + 1, 0);
        if (sizes[i] > 0){
            assert_equal_int_array(expected_results[i], input_results[i], sizes[i], i + 1, 0);
        }
        else{
            if (input_results[i] != NULL){
                printf("[ERROR] Test No.%d failed. For empty array, the input results array should be NULL.\n", i + 1);
                failure_count[0]++;
                failed++;
            }
        }
        printf("\n");
    }
}

void test_sum_positives(){
    printf("--------------------------------\n");
    printf("Testing sum_positives...\n\n");

    int sizes[] = {3, 0, 3, 8};

    int idx = 1;

    int input_arr0[] = {1, -2, 3};
    printf("Test No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[0]; i++){
        printf("%d ", input_arr0[i]);
    }
    printf("\n");
    assert_equal_int(4, (int)sum_positives(input_arr0, sizes[0]), idx++, 1);

    int * input_arr1 = NULL;
    printf("\nTest No.%d:\nInput array: NULL", idx);
    printf("\n");
    assert_equal_int(0, (int)sum_positives(input_arr1, sizes[1]), idx++, 1);

    int input_arr2[] = {-1, -2, -3};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[2]; i++){
        printf("%d ", input_arr2[i]);
    }
    printf("\n");
    assert_equal_int(0, (int)sum_positives(input_arr2, sizes[2]), idx++, 1);

    int input_arr3[] = {0, 0, 3, 100, -90, 1, 3, 100};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[3]; i++){
        printf("%d ", input_arr3[i]);
    }
    printf("\n");
    assert_equal_int(207, (int)sum_positives(input_arr3, sizes[3]), idx++, 1);

    printf("\n");
}

void test_contains_prime(){
    printf("--------------------------------\n");
    printf("Testing contains_prime...\n\n");

    int sizes[] = {3, 0, 3, 3, 5, 1};

    int idx = 1;

    unsigned int input_arr0[] = {1, 2, 3};
    printf("Test No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[0]; i++){
        printf("%d ", input_arr0[i]);
    }
    printf("\n");
    assert_equal_bool(true, contains_prime(input_arr0, sizes[0]), idx++, 2);

    unsigned int * input_arr1 = NULL;
    printf("\nTest No.%d:\nInput array: NULL", idx);
    printf("\n");
    assert_equal_bool(false, contains_prime(input_arr1, sizes[1]), idx++, 2);

    unsigned int input_arr2[] = {0, 1, 0};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[2]; i++){
        printf("%d ", input_arr2[i]);
    }
    printf("\n");
    assert_equal_bool(false, contains_prime(input_arr2, sizes[2]), idx++, 2);

    unsigned int input_arr3[] = {2, 1, 1};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[3]; i++){
        printf("%d ", input_arr3[i]);
    }
    printf("\n");
    assert_equal_bool(true, contains_prime(input_arr3, sizes[3]), idx++, 2);

    unsigned int input_arr4[] = {4, 6, 9, 57, 91};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[4]; i++){
        printf("%d ", input_arr4[i]);
    }
    printf("\n");
    assert_equal_bool(false, contains_prime(input_arr4, sizes[4]), idx++, 2);

    unsigned int input_arr5[] = {509};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[5]; i++){
        printf("%d ", input_arr5[i]);
    }
    printf("\n");
    assert_equal_bool(true, contains_prime(input_arr5, sizes[5]), idx++, 2);

    printf("\n");
}

void test_is_reverse_sorted(){
    printf("--------------------------------\n");
    printf("Testing is_reverse_sorted...\n\n");

    int sizes[] = {3, 0, 3, 3, 6};

    int idx = 1;

    int input_arr0[] = {1, 2, 3};
    printf("Test No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[0]; i++){
        printf("%d ", input_arr0[i]);
    }
    printf("\n");
    assert_equal_bool(false, is_reverse_sorted(input_arr0, sizes[0]), idx++, 3);

    int * input_arr1 = NULL;
    printf("\nTest No.%d:\nInput array: NULL", idx);
    printf("\n");
    assert_equal_bool(true, is_reverse_sorted(input_arr1, sizes[1]), idx++, 3);

    int input_arr2[] = {-1, -2, -3};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[2]; i++){
        printf("%d ", input_arr2[i]);
    }
    printf("\n");
    assert_equal_bool(true, is_reverse_sorted(input_arr2, sizes[2]), idx++, 3);

    int input_arr3[] = {5, 5, 5};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[3]; i++){
        printf("%d ", input_arr3[i]);
    }
    printf("\n");
    assert_equal_bool(true, is_reverse_sorted(input_arr3, sizes[3]), idx++, 3);

    int input_arr4[] = {1001, 1000, 1000, 999, 998, 999};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[4]; i++){
        printf("%d ", input_arr4[i]);
    }
    printf("\n");
    assert_equal_bool(false, is_reverse_sorted(input_arr4, sizes[4]), idx++, 3);

    printf("\n");
}


void test_find_max(){
    printf("--------------------------------\n");
    printf("Testing find_max...\n\n");

    int sizes[] = {3, 0, 3, 3};

    int idx = 1;

    int input_arr0[] = {101, 102, -103};
    printf("Test No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[0]; i++){
        printf("%d ", input_arr0[i]);
    }
    printf("\n");
    assert_equal_int(102, find_max(input_arr0, sizes[0]), idx++, 4);

    int * input_arr1 = NULL;
    printf("\nTest No.%d:\nInput array: NULL", idx);
    printf("\n");
    assert_equal_int(0, find_max(input_arr1, sizes[1]), idx++, 4);
    
    int input_arr2[] = {876, 876, 875};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[2]; i++){
        printf("%d ", input_arr2[i]);
    }
    printf("\n");
    assert_equal_int(876, find_max(input_arr2, sizes[2]), idx++, 4);

    int input_arr3[] = {-234, -235, -239};
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[3]; i++){
        printf("%d ", input_arr3[i]);
    }
    printf("\n");
    assert_equal_int(-234, find_max(input_arr3, sizes[3]), idx++, 4);

    printf("\n");
}


void test_strlenm(){
    printf("--------------------------------\n");
    printf("Testing strlenm...\n\n");

    int sizes[] = {6, 0, 6, 6};

    int idx = 1;

    char input_arr0[] = {'w', 'o', 'r', 'l', 'd', '\0'};
    int max_length = 10;
    printf("Test No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[0]; i++){
        printf("%c", input_arr0[i]);
    }
    printf("\nMax length: %d\n", max_length);
    assert_equal_int(5, strlenm(input_arr0, max_length), idx++, 5);
    printf("\n");

    char input_arr1 [] = {'\0'};
    max_length = 10;
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[1]; i++){
        printf("%c", input_arr1[i]);
    }
    printf("\nMax length: %d\n", max_length);
    assert_equal_int(0, strlenm(input_arr1, max_length), idx++, 5);
    printf("\n");

    char input_arr2[] = {'w', 'o', 'r', 'l', 'd', '\0'};
    max_length = 0;
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[2]; i++){
        printf("%c", input_arr2[i]);
    }
    printf("\nMax length: %d\n", max_length);
    assert_equal_int(-1, strlenm(input_arr2, max_length), idx++, 5);
    printf("\n");

    char input_arr3[] = {'w', 'o', 'r', 'l', 'd', '\0'};
    max_length = 5;
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < sizes[3]; i++){
        printf("%c", input_arr3[i]);
    }
    printf("\nMax length: %d\n", max_length);
    assert_equal_int(5, strlenm(input_arr3, max_length), idx++, 5);
    printf("\n");

    printf("\n");
}


void test_strcnt(){
    printf("--------------------------------\n");
    printf("Testing strcnt...\n\n");

    int idx = 1;

    char input_arr0[] = "Hello";
    int size = 6;
    printf("Test No.%d:\nInput array: ", idx);
    for (int i = 0; i < size; i++){
        printf("%c", input_arr0[i]);
    }
    printf("\n");
    assert_equal_int(1, strcnt(input_arr0), idx++, 6);
    printf("\n");

    char input_arr1[] = "";
    size = 1;
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < size; i++){
        printf("%c", input_arr1[i]);
    }
    printf("\n");
    assert_equal_int(0, strcnt(input_arr1), idx++, 6);
    printf("\n");

    char input_arr2[] = "Hello, world!";
    size = 14;
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < size; i++){
        printf("%c", input_arr2[i]);
    }
    printf("\n");
    assert_equal_int(2, strcnt(input_arr2), idx++, 6);
    printf("\n");

    char input_arr3[] = "How--are--you? \n\t-I'm fine, thank you! and you?";
    size = 48;
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < size; i++){
        printf("%c", input_arr3[i]);
    }
    printf("\n");
    assert_equal_int(9, strcnt(input_arr3), idx++, 6);
    printf("\n");

    printf("\n");
}


void test_strfind(){
    printf("--------------------------------\n");
    printf("Testing strfind...\n\n");

    int idx = 1;

    char input_arr0[] = "abcd1234 1234567";
    int size = 17;
    char target = '1';
    printf("Test No.%d:\nInput array: ", idx);
    for (int i = 0; i < size; i++){
        printf("%c", input_arr0[i]);
    }
    printf("\n");
    printf("Target: %c\n", target);
    assert_equal_int(4, strfind(input_arr0, target), idx++, 7);
    printf("\n");

    char input_arr1[] = "abcd1234 1234567";
    size = 17;
    target = ' ';
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < size; i++){
        printf("%c", input_arr1[i]);
    }
    printf("\n");
    printf("Target: %c (blank space)\n", target);
    assert_equal_int(8, strfind(input_arr1, target), idx++, 7);
    printf("\n");

    char input_arr2[] = "abcd1234 1234567";
    size = 17;
    target = 'z';
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < size; i++){
        printf("%c", input_arr2[i]);
    }
    printf("\n");
    printf("Target: %c\n", target);
    assert_equal_int(-1, strfind(input_arr2, target), idx++, 7);
    printf("\n");

    char input_arr3[] = "abcd1234 1234567";
    size = 17;
    target = '\0';
    printf("\nTest No.%d:\nInput array: ", idx);
    for (int i = 0; i < size; i++){
        printf("%c", input_arr3[i]);
    }
    printf("\n");
    printf("Target: %c (null terminator)\n", target);
    assert_equal_int(16, strfind(input_arr3, target), idx++, 7);
    printf("\n");

    printf("\n");
}


// main function calls all the test functions and summarizes the results.
int main(){
    test_find_all_matches();
    test_sum_positives();
    test_contains_prime();
    test_is_reverse_sorted();
    test_find_max();
    test_strlenm();
    test_strcnt();
    test_strfind();

    printf("-------------------Summary-------------------\n");
    printf("Total tests: %d\n", passed + failed);
    printf("Total tests passed: %d\n", passed);
    printf("Total tests failed: %d\n", failed);
    printf("--------------------------------\n");
    int num_function_passed = 0;
    float total_points = 0;
    for (int i = 0; i < 8; i++){
        if (failure_count[i] == 0){
            num_function_passed++;
            total_points += function_points[i];
            printf("Function %s passed\n", function_name[i]);
        }
        else{
            printf("Function %s failed\n", function_name[i]);
        }
    }
    printf("Total functions passed: %d\n", num_function_passed);
    printf("points for correctness: %f/20, the other 5 points are for comments and git commits, please read the document for more details.\n", total_points);
    return 0;
}