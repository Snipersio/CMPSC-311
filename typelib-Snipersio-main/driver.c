#include "typelib.h"
#include <stdio.h>
#include <string.h>

int passed = 0;
int failed = 0;
char * function_name [14] = {"percentage_to_letter", "letter_to_percentage", "letter_to_gpa", "calculate_student_average", "find_top_student", "calculate_class_average", "count_students_by_year", "is_course_at_capacity", "calculate_total_credit_hours", "find_most_popular_course", "create_student", "create_course", "add_student_to_course", "find_student_in_course"};
float function_points [14] = {0.5, 0.5, 0.5, 2.0, 2.0, 3.0, 2.0, 0.5, 1.0, 2.0, 2.0, 2.0, 2.0, 2.0};
int failure_count [14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*
You do NOT need to read the code in this file or understand it to complete the assignment.
Please do NOT make any changes to this file.

This file is used to test the correctness of your implementation of the functions in typelib.c.

There are no hidden tests in this project. The test cases in this file are all we will use to test 
the correctness of your implementation. However, please do NOT hardcode the answers for the given test cases.
Your function should work for all valid inputs, not just the ones in the test cases.

When you want to test any of your functions, you can follow the instructions in the README.md file
to run the tests. You will see the results of the tests in the terminal.
*/

// Helper functions to check if the first two inputs are equal.
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

void assert_not_equal_int(int not_expected, int actual, int test_num, int function_index){
    if (not_expected != actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected NOT %d, but got %d.\n", test_num, not_expected, actual);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_pointer(void* expected, void* actual, int test_num, int function_index){
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %p, but got %p.\n", test_num, expected, actual);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_student(student_t expected, student_t actual, int test_num, int function_index){
    if (expected.student_id == actual.student_id && 
        strcmp(expected.first_name, actual.first_name) == 0 &&
        strcmp(expected.last_name, actual.last_name) == 0 &&
        expected.year_level == actual.year_level &&
        expected.is_suspended == actual.is_suspended){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected student {id:%d, name:%s %s, year:%d, suspended:%s}, but got {id:%d, name:%s %s, year:%d, suspended:%s}.\n", 
               test_num, expected.student_id, expected.first_name, expected.last_name, expected.year_level, expected.is_suspended ? "true" : "false",
               actual.student_id, actual.first_name, actual.last_name, actual.year_level, actual.is_suspended ? "true" : "false");
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_course(course_t expected, course_t actual, int test_num, int function_index){
    if (strcmp(expected.course_code, actual.course_code) == 0 &&
        strcmp(expected.course_name, actual.course_name) == 0 &&
        expected.credits == actual.credits &&
        expected.max_students == actual.max_students &&
        expected.current_enrollment == actual.current_enrollment){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected course {code:%s, name:%s, credits:%d, max:%d, enrollment:%d}, but got {code:%s, name:%s, credits:%d, max:%d, enrollment:%d}.\n", 
               test_num, expected.course_code, expected.course_name, expected.credits, expected.max_students, expected.current_enrollment,
               actual.course_code, actual.course_name, actual.credits, actual.max_students, actual.current_enrollment);
        failure_count[function_index]++;
        failed++;
    }
}

// Test functions
void test_percentage_to_letter(){
    printf("--------------------------------\n");
    printf("Testing percentage_to_letter...\n\n");

    // Test cases with edge/boundary values
    float inputs[] = {
        100.0, 97.0, 96.99, 97.01,
        96.99, 93.0, 92.99, 93.01,
        92.99, 90.0, 89.99, 90.01,
        89.99, 87.0, 86.99, 87.01,
        86.99, 83.0, 82.99, 83.01,
        82.99, 80.0, 79.99, 80.01,
        79.99, 77.0, 76.99, 77.01,
        76.99, 73.0, 72.99, 73.01,
        72.99, 70.0, 69.99, 70.01,
        69.99, 67.0, 66.99, 67.01,
        66.99, 63.0, 62.99, 63.01,
        62.99, 60.0, 59.99, 60.01,
        59.99, 30.0, 0.0,
        100.01, 101.0, -0.01, -5.0
    };
    
    letter_grade_t expected[] = {
        GRADE_A_PLUS, GRADE_A_PLUS, GRADE_A, GRADE_A_PLUS,
        GRADE_A, GRADE_A, GRADE_A_MINUS, GRADE_A,
        GRADE_A_MINUS, GRADE_A_MINUS, GRADE_B_PLUS, GRADE_A_MINUS,
        GRADE_B_PLUS, GRADE_B_PLUS, GRADE_B, GRADE_B_PLUS,
        GRADE_B, GRADE_B, GRADE_B_MINUS, GRADE_B,
        GRADE_B_MINUS, GRADE_B_MINUS, GRADE_C_PLUS, GRADE_B_MINUS,
        GRADE_C_PLUS, GRADE_C_PLUS, GRADE_C, GRADE_C_PLUS,
        GRADE_C, GRADE_C, GRADE_C_MINUS, GRADE_C,
        GRADE_C_MINUS, GRADE_C_MINUS, GRADE_D_PLUS, GRADE_C_MINUS,
        GRADE_D_PLUS, GRADE_D_PLUS, GRADE_D, GRADE_D_PLUS,
        GRADE_D, GRADE_D, GRADE_D_MINUS, GRADE_D,
        GRADE_D_MINUS, GRADE_D_MINUS, GRADE_F, GRADE_D_MINUS,
        GRADE_F, GRADE_F, GRADE_F,
        GRADE_INVALID, GRADE_INVALID, GRADE_INVALID, GRADE_INVALID
    };
    
    int num_tests = sizeof(inputs) / sizeof(inputs[0]);
    for (int i = 0; i < num_tests; i++){
        letter_grade_t output = percentage_to_letter(inputs[i]);
        printf("Test No.%d Input: %f\n", i + 1, inputs[i]);
        assert_equal_int((int)expected[i], (int)output, i + 1, 0);
    }
    printf("\n");
}

void test_letter_to_percentage(){
    printf("--------------------------------\n");
    printf("Testing letter_to_percentage...\n\n");

    // Test all valid letter grades and invalid case
    letter_grade_t inputs[] = {
        GRADE_A_PLUS, GRADE_A, GRADE_A_MINUS, 
        GRADE_B_PLUS, GRADE_B, GRADE_B_MINUS, 
        GRADE_C_PLUS, GRADE_C, GRADE_C_MINUS, 
        GRADE_D_PLUS, GRADE_D, GRADE_D_MINUS, 
        GRADE_F, GRADE_INVALID
    };
    
    // Expected middle values for each grade range
    float expected[] = { 98.5, 95.0, 91.5, 88.5, 85.0, 81.5, 78.5, 75.0, 71.5, 68.5, 65.0, 61.5, 30.0, -1.0 };
    int num_tests = sizeof(inputs) / sizeof(inputs[0]);
    for (int i = 0; i < num_tests; i++){
        float output = letter_to_percentage(inputs[i]);
        printf("Test No.%d Input: %d\n", i + 1, inputs[i]);
        assert_equal_float(expected[i], output, i + 1, 1);
    }
    printf("\n");
}

void test_letter_to_gpa(){
    printf("--------------------------------\n");
    printf("Testing letter_to_gpa...\n\n");

    // Test all valid letter grades and invalid case
    letter_grade_t inputs[] = { GRADE_A_PLUS, GRADE_A, GRADE_A_MINUS, GRADE_B_PLUS, GRADE_B, GRADE_B_MINUS, GRADE_C_PLUS, GRADE_C, GRADE_C_MINUS, GRADE_D_PLUS, GRADE_D, GRADE_D_MINUS, GRADE_F, GRADE_INVALID };
    
    // Expected GPA values for each letter grade
    float expected[] = {4.0, 4.0, 3.7, 3.3, 3.0, 2.7, 2.3, 2.0, 1.7, 1.3, 1.0, 0.7, 0.0, -1.0 };
    
    int num_tests = sizeof(inputs) / sizeof(inputs[0]);
    for (int i = 0; i < num_tests; i++){
        float output = letter_to_gpa(inputs[i]);
        printf("Test No.%d Input: %d\n", i + 1, inputs[i]);
        assert_equal_float(expected[i], output, i + 1, 2);
    }
    printf("\n");
}

void test_calculate_student_average(){
    printf("--------------------------------\n");
    printf("Testing calculate_student_average...\n\n");

    // Test case 1: All percentage grades
    grade_t grades1[] = {{123, "HW1", {.percentage = 85.0}, true}, {123, "HW2", {.percentage = 90.0}, true}, {456, "HW1", {.percentage = 75.0}, true}};
    float result1 = calculate_student_average(grades1, 3, 123);
    printf("Test No.1: All percentage grades for student 123\n");
    assert_equal_float(87.5, result1, 1, 3);

    // Test case 2: Mixed percentage and letter grades
    grade_t grades2[] = {{123, "HW1", {.percentage = 85.0}, true}, {123, "HW2", {.letter = GRADE_A}, false}};
    float result2 = calculate_student_average(grades2, 2, 123);
    printf("Test No.2: Mixed grades for student 123\n");
    assert_equal_float(90.0, result2, 2, 3);

    // Test case 3: No grades for student
    grade_t grades3[] = {{456, "HW1", {.percentage = 85.0}, true}};
    float result3 = calculate_student_average(grades3, 1, 123);
    printf("Test No.3: No grades for student 123\n");
    assert_equal_float(-1.0, result3, 3, 3);

    // Test case 4: Empty grades array
    float result4 = calculate_student_average(NULL, 0, 123);
    printf("Test No.4: Empty grades array\n");
    assert_equal_float(-1.0, result4, 4, 3);

    // Test case 5: Single grade
    grade_t grades5[] = {{123, "HW1", {.letter = GRADE_D}, false}};
    float result5 = calculate_student_average(grades5, 1, 123);
    printf("Test No.5: Single grade for student 123\n");
    assert_equal_float(65.0, result5, 5, 3);

    // Test case 6: Large mixed dataset - student 123
    grade_t grades6[] = {
        {123, "HW1", {.percentage = 85.0}, true}, {123, "HW2", {.letter = GRADE_B_PLUS}, false}, {123, "HW3", {.percentage = 92.0}, true},
        {123, "HW4", {.letter = GRADE_A_MINUS}, false}, {123, "HW5", {.percentage = 88.0}, true}, {123, "HW6", {.letter = GRADE_B}, false},
        {123, "HW7", {.percentage = 90.0}, true}, {123, "HW8", {.letter = GRADE_A}, false}, {123, "HW9", {.percentage = 87.0}, true},
        {123, "HW10", {.letter = GRADE_B_PLUS}, false}, {456, "HW1", {.percentage = 75.0}, true}, {456, "HW2", {.letter = GRADE_C}, false}
    };
    float result6 = calculate_student_average(grades6, 12, 123);
    printf("Test No.6: Large mixed dataset for student 123\n");
    assert_equal_float((85.0 + 88.5 + 92.0 + 91.5 + 88.0 + 85.0 + 90.0 + 95.0 + 87.0 + 88.5)/10, result6, 6, 3);

    // Test case 7: Large mixed dataset - student 456
    float result7 = calculate_student_average(grades6, 12, 456);
    printf("Test No.7: Large mixed dataset for student 456\n");
    assert_equal_float((75.0 + 75.0)/2, result7, 7, 3);

    // Test case 8: All letter grades
    grade_t grades8[] = {{123, "HW1", {.letter = GRADE_A}, false}, {123, "HW2", {.letter = GRADE_B_PLUS}, false}, {123, "HW3", {.letter = GRADE_A_MINUS}, false}};
    float result8 = calculate_student_average(grades8, 3, 123);
    printf("Test No.8: All letter grades for student 123\n");
    assert_equal_float((95.0 + 88.5 + 91.5)/3, result8, 8, 3);

    // Test case 9: Complex mixed scenario with multiple students
    grade_t grades9[] = {
        {123, "HW1", {.percentage = 95.0}, true}, {123, "HW2", {.letter = GRADE_A_PLUS}, false}, {123, "HW3", {.percentage = 88.0}, true},
        {123, "HW4", {.letter = GRADE_B}, false}, {123, "HW5", {.percentage = 92.0}, true}, {123, "HW6", {.letter = GRADE_A_MINUS}, false},
        {456, "HW1", {.percentage = 78.0}, true}, {456, "HW2", {.letter = GRADE_C_PLUS}, false}, {456, "HW3", {.percentage = 82.0}, true},
        {456, "HW4", {.letter = GRADE_B_MINUS}, false}, {456, "HW5", {.percentage = 85.0}, true}, {456, "HW6", {.letter = GRADE_B}, false},
        {789, "HW1", {.percentage = 90.0}, true}, {789, "HW2", {.letter = GRADE_A}, false}, {789, "HW3", {.percentage = 87.0}, true},
        {789, "HW4", {.letter = GRADE_B_PLUS}, false}, {789, "HW5", {.percentage = 89.0}, true}, {789, "HW6", {.letter = GRADE_A_MINUS}, false}
    };
    float result9a = calculate_student_average(grades9, 18, 123);
    float result9b = calculate_student_average(grades9, 18, 456);
    float result9c = calculate_student_average(grades9, 18, 789);
    printf("Test No.9: Complex mixed scenario - student 123\n");
    assert_equal_float((95.0 + 98.5 + 88.0 + 85.0 + 92.0 + 91.5)/6, result9a, 9, 3);
    printf("Test No.9: Complex mixed scenario - student 456\n");
    assert_equal_float((78.0 + 78.5 + 82.0 + 81.5 + 85.0 + 85.0)/6, result9b, 9, 3);
    printf("Test No.9: Complex mixed scenario - student 789\n");
    assert_equal_float((90.0 + 95.0 + 87.0 + 88.5 + 89.0 + 91.5)/6, result9c, 9, 3);

    printf("\n");
}

void test_find_top_student(){
    printf("--------------------------------\n");
    printf("Testing find_top_student...\n\n");

    // Test case 1: Two students with different averages
    grade_t grades1[] = {{123, "HW1", {.percentage = 85.0}, true}, {456, "HW1", {.percentage = 90.0}, true}, {123, "HW2", {.percentage = 95.0}, true}};
    student_t students1[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}};
    int result1 = find_top_student(grades1, 3, students1, 2);
    printf("Test No.1: Two students with different averages\n");
    assert_equal_int(123, result1, 1, 4);

    // Test case 2: No non-suspended students
    grade_t grades2[] = {{123, "HW1", {.percentage = 85.0}, true}};
    student_t students2[] = {{123, "John", "Doe", 2, true}};
    int result2 = find_top_student(grades2, 1, students2, 1);
    printf("Test No.2: No non-suspended students\n");
    assert_equal_int(-1, result2, 2, 4);

    // Test case 3: Single non-suspended student
    grade_t grades3[] = {{123, "HW1", {.percentage = 85.0}, true}, {123, "HW2", {.percentage = 95.0}, true}};
    student_t students3[] = {{123, "John", "Doe", 2, false}};
    int result3 = find_top_student(grades3, 2, students3, 1);
    printf("Test No.3: Single non-suspended student\n");
    assert_equal_int(123, result3, 3, 4);

    // Test case 4: Multiple students with same average (first wins)
    grade_t grades4[] = {{123, "HW1", {.percentage = 90.0}, true}, {456, "HW1", {.percentage = 90.0}, true}, {789, "HW1", {.percentage = 90.0}, true}};
    student_t students4[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}, {789, "Bob", "Wilson", 2, false}};
    int result4 = find_top_student(grades4, 3, students4, 3);
    printf("Test No.4: Multiple students with same average\n");
    assert_equal_int(123, result4, 4, 4);

    // Test case 5: Mixed suspended and non-suspended students
    grade_t grades5[] = {{123, "HW1", {.percentage = 95.0}, true}, {456, "HW1", {.percentage = 85.0}, true}, {789, "HW1", {.percentage = 90.0}, true}};
    student_t students5[] = {{123, "John", "Doe", 2, true}, {456, "Jane", "Smith", 2, false}, {789, "Bob", "Wilson", 2, false}};
    int result5 = find_top_student(grades5, 3, students5, 3);
    printf("Test No.5: Mixed suspended and non-suspended students\n");
    assert_equal_int(789, result5, 5, 4);

    // Test case 6: No grades for any students
    grade_t grades6[1] = {{0}}; // Dummy array, won't be used
    student_t students6[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}};
    int result6 = find_top_student(grades6, 0, students6, 2);
    printf("Test No.6: No grades for any students\n");
    assert_equal_int(-1, result6, 6, 4);

    // Test case 7: Letter grades mixed with percentages
    grade_t grades7[] = {
        {123, "HW1", {.letter = GRADE_A}, false}, {123, "HW2", {.percentage = 88.0}, true}, {123, "HW3", {.letter = GRADE_B_PLUS}, false},
        {456, "HW1", {.percentage = 95.0}, true}, {456, "HW2", {.letter = GRADE_A_MINUS}, false}, {456, "HW3", {.percentage = 92.0}, true},
        {789, "HW1", {.letter = GRADE_B}, false}, {789, "HW2", {.percentage = 85.0}, true}
    };
    student_t students7[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}, {789, "Bob", "Wilson", 2, false}};
    int result7 = find_top_student(grades7, 8, students7, 3);
    printf("Test No.7: Letter grades mixed with percentages\n");
    assert_equal_int(456, result7, 7, 4);

    printf("\n");
}

void test_calculate_class_average(){
    printf("--------------------------------\n");
    printf("Testing calculate_class_average...\n\n");

    // Test case 1: Two non-suspended students
    grade_t grades1[] = {{123, "HW1", {.percentage = 85.0}, true}, {456, "HW1", {.percentage = 90.0}, true}, {123, "HW2", {.percentage = 95.0}, true}};
    student_t students1[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}};
    float result1 = calculate_class_average(grades1, 3, students1, 2);
    printf("Test No.1: Two non-suspended students\n");
    assert_equal_float(((85.0 + 95.0)/2 + 90.0)/2, result1, 1, 5);

    // Test case 2: No non-suspended students
    grade_t grades2[] = {{123, "HW1", {.percentage = 85.0}, true}};
    student_t students2[] = {{123, "John", "Doe", 2, true}};
    float result2 = calculate_class_average(grades2, 1, students2, 1);
    printf("Test No.2: No non-suspended students\n");
    assert_equal_float(-1.0, result2, 2, 5);

    // Test case 3: Single non-suspended student
    grade_t grades3[] = {{123, "HW1", {.percentage = 85.0}, true}, {123, "HW2", {.percentage = 95.0}, true}};
    student_t students3[] = {{123, "John", "Doe", 2, false}};
    float result3 = calculate_class_average(grades3, 2, students3, 1);
    printf("Test No.3: Single non-suspended student\n");
    assert_equal_float((85.0 + 95.0)/2, result3, 3, 5);

    // Test case 4: Mixed suspended and non-suspended students
    grade_t grades4[] = {{123, "HW1", {.percentage = 95.0}, true}, {456, "HW1", {.percentage = 85.0}, true}, {789, "HW1", {.percentage = 90.0}, true}};
    student_t students4[] = {{123, "John", "Doe", 2, true}, {456, "Jane", "Smith", 2, false}, {789, "Bob", "Wilson", 2, false}};
    float result4 = calculate_class_average(grades4, 3, students4, 3);
    printf("Test No.4: Mixed suspended and non-suspended students\n");
    assert_equal_float((85.0 + 90.0)/2, result4, 4, 5);

    // Test case 5: Letter grades mixed with percentages
    grade_t grades5[] = {{123, "HW1", {.letter = GRADE_A}, false}, {456, "HW1", {.percentage = 88.0}, true}};
    student_t students5[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}};
    float result5 = calculate_class_average(grades5, 2, students5, 2);
    printf("Test No.5: Letter grades mixed with percentages\n");
    assert_equal_float((95.0 + 88.0)/2, result5, 5, 5);

    // Test case 6: No grades for any students
    grade_t grades6[1] = {{0}}; // Dummy array, won't be used
    student_t students6[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}};
    float result6 = calculate_class_average(grades6, 0, students6, 2);
    printf("Test No.6: No grades for any students\n");
    assert_equal_float(-1.0, result6, 6, 5);

    // Test case 7: Multiple assignments per student
    grade_t grades7[] = {{123, "HW1", {.percentage = 80.0}, true}, {123, "HW2", {.percentage = 90.0}, true}, {456, "HW1", {.percentage = 85.0}, true}, {456, "HW2", {.percentage = 95.0}, true}};
    student_t students7[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}};
    float result7 = calculate_class_average(grades7, 4, students7, 2);
    printf("Test No.7: Multiple assignments per student\n");
    assert_equal_float(((80.0 + 90.0)/2 + (85.0 + 95.0)/2)/2, result7, 7, 5);

    // Test case 8: All students have same grade
    grade_t grades8[] = {{123, "HW1", {.percentage = 90.0}, true}, {456, "HW1", {.percentage = 90.0}, true}, {789, "HW1", {.percentage = 90.0}, true}};
    student_t students8[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}, {789, "Bob", "Wilson", 2, false}};
    float result8 = calculate_class_average(grades8, 3, students8, 3);
    printf("Test No.8: All students have same grade\n");
    assert_equal_float((90.0 + 90.0 + 90.0)/3, result8, 8, 5);

    // Test case 9: Multiple students with multiple assignments and different grade types
    grade_t grades9[] = {
        {123, "HW1", {.percentage = 85.0}, true}, {123, "HW2", {.letter = GRADE_B_PLUS}, false}, {123, "HW3", {.percentage = 92.0}, true}, {123, "HW4", {.letter = GRADE_A_MINUS}, false}, {123, "HW5", {.percentage = 88.0}, true},
        {456, "HW1", {.letter = GRADE_A}, false}, {456, "HW2", {.percentage = 95.0}, true}, {456, "HW3", {.letter = GRADE_A_PLUS}, false}, {456, "HW4", {.percentage = 90.0}, true}, {456, "HW5", {.letter = GRADE_A}, false},
        {789, "HW1", {.percentage = 78.0}, true}, {789, "HW2", {.letter = GRADE_C_PLUS}, false}, {789, "HW3", {.percentage = 82.0}, true}, {789, "HW4", {.letter = GRADE_B_MINUS}, false}, {789, "HW5", {.percentage = 80.0}, true}
    };
    student_t students9[] = {{123, "John", "Doe", 2, true}, {456, "Jane", "Smith", 2, false}, {789, "Bob", "Wilson", 2, false}};
    float result9 = calculate_class_average(grades9, 15, students9, 3);
    printf("Test No.9: Multiple students with multiple assignments and different grade types\n");
    assert_equal_float(((95.0 + 95.0 + 98.5 + 90.0 + 95.0)/5 + (78.0 + 78.5 + 82.0 + 81.5 + 80.0)/5)/2, result9, 9, 5);

    printf("\n");
}

void test_count_students_by_year(){
    printf("--------------------------------\n");
    printf("Testing count_students_by_year...\n\n");

    // Test case 1: Mixed year levels and suspension status
    student_t students1[] = {{123, "John", "Doe", 1, false}, {456, "Jane", "Smith", 1, true}, {789, "Bob", "Johnson", 2, false}};
    int result1 = count_students_by_year(students1, 3, 1);
    printf("Test No.1: Count freshman students\n");
    assert_equal_int(1, result1, 1, 6);

    // Test case 2: Multiple students in same year
    student_t students2[] = {{123, "John", "Doe", 3, false}, {456, "Jane", "Smith", 3, false}, {789, "Bob", "Johnson", 4, false}};
    int result2 = count_students_by_year(students2, 3, 3);
    printf("Test No.2: Count junior students\n");
    assert_equal_int(2, result2, 2, 6);

    // Test case 3: All students suspended
    student_t students3[] = {{123, "John", "Doe", 2, true}, {456, "Jane", "Smith", 2, true}, {789, "Bob", "Johnson", 2, true}};
    int result3 = count_students_by_year(students3, 3, 2);
    printf("Test No.3: All students suspended\n");
    assert_equal_int(0, result3, 3, 6);

    // Test case 4: No students in requested year
    student_t students4[] = {{123, "John", "Doe", 1, false}, {456, "Jane", "Smith", 3, false}, {789, "Bob", "Johnson", 4, false}};
    int result4 = count_students_by_year(students4, 3, 2);
    printf("Test No.4: No students in requested year\n");
    assert_equal_int(0, result4, 4, 6);

    // Test case 5: Single student
    student_t students5[] = {{123, "John", "Doe", 2, false}};
    int result5 = count_students_by_year(students5, 1, 2);
    printf("Test No.5: Single student\n");
    assert_equal_int(1, result5, 5, 6);

    // Test case 6: Empty student array
    student_t students6[1] = {{0}}; // Dummy array, won't be used
    int result6 = count_students_by_year(students6, 0, 2);
    printf("Test No.6: Empty student array\n");
    assert_equal_int(0, result6, 6, 6);

    // Test case 7: All year levels represented
    student_t students7[] = {{123, "John", "Doe", 1, false}, {456, "Jane", "Smith", 2, false}, {789, "Bob", "Johnson", 3, false}, {101, "Alice", "Wilson", 4, false}};
    int result7a = count_students_by_year(students7, 4, 1);
    int result7b = count_students_by_year(students7, 4, 2);
    int result7c = count_students_by_year(students7, 4, 3);
    int result7d = count_students_by_year(students7, 4, 4);
    printf("Test No.7: All year levels represented\n");
    assert_equal_int(1, result7a, 7, 6);
    assert_equal_int(1, result7b, 7, 6);
    assert_equal_int(1, result7c, 7, 6);
    assert_equal_int(1, result7d, 7, 6);

    // Test case 8: Mixed suspension status in same year
    student_t students8[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, true}, {789, "Bob", "Johnson", 2, false}, {101, "Alice", "Wilson", 2, true}};
    int result8 = count_students_by_year(students8, 4, 2);
    printf("Test No.8: Mixed suspension status in same year\n");
    assert_equal_int(2, result8, 8, 6);

    // Test case 9: Large number of students
    student_t students9[] = {
        {123, "John", "Doe", 1, false}, {456, "Jane", "Smith", 1, true}, {789, "Bob", "Johnson", 1, false},
        {101, "Alice", "Wilson", 2, false}, {202, "Charlie", "Brown", 2, false}, {303, "Diana", "Davis", 2, true},
        {404, "Eve", "Miller", 3, false}, {505, "Frank", "Garcia", 3, false}, {606, "Grace", "Lee", 3, false},
        {707, "Henry", "Taylor", 4, false}, {808, "Ivy", "Anderson", 4, true}, {909, "Jack", "Thomas", 4, false}
    };
    int result9a = count_students_by_year(students9, 12, 1);
    int result9b = count_students_by_year(students9, 12, 2);
    int result9c = count_students_by_year(students9, 12, 3);
    int result9d = count_students_by_year(students9, 12, 4);
    printf("Test No.9: Large number of students\n");
    assert_equal_int(2, result9a, 9, 6);
    assert_equal_int(2, result9b, 9, 6);
    assert_equal_int(3, result9c, 9, 6);
    assert_equal_int(2, result9d, 9, 6);

    printf("\n");
}

void test_is_course_at_capacity(){
    printf("--------------------------------\n");
    printf("Testing is_course_at_capacity...\n\n");

    // Test case 1: At capacity
    course_t course1 = {"CMPSC311", "Data Structures", 3, 30, 30, {0}};
    bool result1 = is_course_at_capacity(course1);
    printf("Test No.1: Course at capacity\n");
    assert_equal_bool(true, result1, 1, 7);

    // Test case 2: Not at capacity
    course_t course2 = {"CMPSC311", "Data Structures", 3, 30, 25, {0}};
    bool result2 = is_course_at_capacity(course2);
    printf("Test No.2: Course not at capacity\n");
    assert_equal_bool(false, result2, 2, 7);

    // Test case 3: Empty course
    course_t course3 = {"CMPSC311", "Data Structures", 3, 30, 0, {0}};
    bool result3 = is_course_at_capacity(course3);
    printf("Test No.3: Empty course\n");
    assert_equal_bool(false, result3, 3, 7);

    // Test case 4: Bigger capacity
    course_t course4 = {"CMPSC311", "Data Structures", 3, 30, 35, {0}};
    bool result4 = is_course_at_capacity(course4);
    printf("Test No.4: Over capacity course\n");
    assert_equal_bool(true, result4, 4, 7);

    printf("\n");
}

void test_calculate_total_credit_hours(){
    printf("--------------------------------\n");
    printf("Testing calculate_total_credit_hours...\n\n");

    // Test case 1: Course with non-suspended students
    student_t students1[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, true}, {789, "Bob", "Johnson", 2, false}};
    course_t course1 = {"CMPSC311", "Data Structures", 3, 30, 3, {0}};
    // Copy students into course
    for (int i = 0; i < 3; i++) {
        course1.students[i] = students1[i];
    }
    int result1 = calculate_total_credit_hours(course1);
    printf("Test No.1: Course with 2 non-suspended students, 3 credits each\n");
    assert_equal_int(6, result1, 1, 8);

    // Test case 2: Empty course
    course_t course2 = {"CMPSC311", "Data Structures", 4, 30, 0, {0}};
    int result2 = calculate_total_credit_hours(course2);
    printf("Test No.2: Empty course\n");
    assert_equal_int(0, result2, 2, 8);

    // Test case 3: All students suspended
    student_t students3[] = {{123, "John", "Doe", 2, true}, {456, "Jane", "Smith", 2, true}, {789, "Bob", "Johnson", 2, true}};
    course_t course3 = {"CMPSC312", "Algorithms", 3, 30, 3, {0}};
    for (int i = 0; i < 3; i++) {
        course3.students[i] = students3[i];
    }
    int result3 = calculate_total_credit_hours(course3);
    printf("Test No.3: All students suspended\n");
    assert_equal_int(0, result3, 3, 8);

    // Test case 4: Single non-suspended student
    student_t students4[] = {{123, "John", "Doe", 2, false}};
    course_t course4 = {"CMPSC313", "Systems", 4, 30, 1, {0}};
    course4.students[0] = students4[0];
    int result4 = calculate_total_credit_hours(course4);
    printf("Test No.4: Single non-suspended student\n");
    assert_equal_int(4, result4, 4, 8);

    // Test case 5: Mixed suspension status
    student_t students5[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, true}, {789, "Bob", "Johnson", 2, false}, {101, "Alice", "Wilson", 2, true}};
    course_t course5 = {"CMPSC314", "Networks", 2, 30, 4, {0}};
    for (int i = 0; i < 4; i++) {
        course5.students[i] = students5[i];
    }
    int result5 = calculate_total_credit_hours(course5);
    printf("Test No.5: Mixed suspension status\n");
    assert_equal_int(4, result5, 5, 8);

    // Test case 6: Large number of students
    student_t students6[] = {
        {123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, true}, {789, "Bob", "Johnson", 2, false},
        {101, "Alice", "Wilson", 2, false}, {202, "Charlie", "Brown", 2, true}, {303, "Diana", "Davis", 2, false},
        {404, "Eve", "Miller", 2, false}, {505, "Frank", "Garcia", 2, true}, {606, "Grace", "Lee", 2, false},
        {707, "Henry", "Taylor", 2, false}
    };
    course_t course6 = {"CMPSC315", "Databases", 3, 30, 10, {0}};
    for (int i = 0; i < 10; i++) {
        course6.students[i] = students6[i];
    }
    int result6 = calculate_total_credit_hours(course6);
    printf("Test No.6: Large number of students\n");
    assert_equal_int(21, result6, 6, 8);

    // Test case 7: Zero credits course
    student_t students7[] = {{123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, false}};
    course_t course7 = {"CMPSC316", "Seminar", 0, 30, 2, {0}};
    for (int i = 0; i < 2; i++) {
        course7.students[i] = students7[i];
    }
    int result7 = calculate_total_credit_hours(course7);
    printf("Test No.7: Zero credits course\n");
    assert_equal_int(0, result7, 7, 8);

    // Test case 8: Course at capacity with mixed suspension
    student_t students8[] = {
        {123, "John", "Doe", 2, false}, {456, "Jane", "Smith", 2, true}, {789, "Bob", "Johnson", 2, false},
        {101, "Alice", "Wilson", 2, false}, {202, "Charlie", "Brown", 2, true}
    };
    course_t course8 = {"CMPSC317", "Capstone", 1, 5, 5, {0}};
    for (int i = 0; i < 5; i++) {
        course8.students[i] = students8[i];
    }
    int result8 = calculate_total_credit_hours(course8);
    printf("Test No.8: Course at capacity with mixed suspension\n");
    assert_equal_int(3, result8, 8, 8);

    printf("\n");
}

void test_find_most_popular_course(){
    printf("--------------------------------\n");
    printf("Testing find_most_popular_course...\n\n");

    // Test case 1: Multiple courses with different enrollments
    course_t courses1[] = {{"CMPSC311", "Data Structures", 3, 30, 25, {0}}, {"CMPSC312", "Algorithms", 3, 30, 30, {0}}, {"CMPSC313", "Systems", 3, 30, 20, {0}}};
    int result1 = find_most_popular_course(courses1, 3);
    printf("Test No.1: Multiple courses with different enrollments\n");
    assert_equal_int(1, result1, 1, 9);

    // Test case 2: All courses empty
    course_t courses2[] = {{"CMPSC311", "Data Structures", 3, 30, 0, {0}}, {"CMPSC312", "Algorithms", 3, 30, 0, {0}}};
    int result2 = find_most_popular_course(courses2, 2);
    printf("Test No.2: All courses empty\n");
    assert_equal_int(-1, result2, 2, 9);

    // Test case 3: Single course with students
    course_t courses3[] = {{"CMPSC311", "Data Structures", 3, 30, 15, {0}}};
    int result3 = find_most_popular_course(courses3, 1);
    printf("Test No.3: Single course with students\n");
    assert_equal_int(0, result3, 3, 9);

    // Test case 4: Single course empty
    course_t courses4[] = {{"CMPSC311", "Data Structures", 3, 30, 0, {0}}};
    int result4 = find_most_popular_course(courses4, 1);
    printf("Test No.4: Single course empty\n");
    assert_equal_int(-1, result4, 4, 9);

    // Test case 5: Two courses with same enrollment
    course_t courses5[] = {{"CMPSC311", "Data Structures", 3, 30, 25, {0}}, {"CMPSC312", "Algorithms", 3, 30, 25, {0}}};
    int result5 = find_most_popular_course(courses5, 2);
    printf("Test No.5: Two courses with same enrollment\n");
    assert_equal_int(0, result5, 5, 9);

    // Test case 6: First course most popular
    course_t courses6[] = {{"CMPSC311", "Data Structures", 3, 30, 50, {0}}, {"CMPSC312", "Algorithms", 3, 30, 30, {0}}, {"CMPSC313", "Systems", 3, 30, 40, {0}}};
    int result6 = find_most_popular_course(courses6, 3);
    printf("Test No.6: First course most popular\n");
    assert_equal_int(0, result6, 6, 9);

    // Test case 7: Last course most popular
    course_t courses7[] = {{"CMPSC311", "Data Structures", 3, 30, 20, {0}}, {"CMPSC312", "Algorithms", 3, 30, 30, {0}}, {"CMPSC313", "Systems", 3, 30, 45, {0}}};
    int result7 = find_most_popular_course(courses7, 3);
    printf("Test No.7: Last course most popular\n");
    assert_equal_int(2, result7, 7, 9);

    // Test case 8: Middle course most popular
    course_t courses8[] = {{"CMPSC311", "Data Structures", 3, 30, 15, {0}}, {"CMPSC312", "Algorithms", 3, 30, 35, {0}}, {"CMPSC313", "Systems", 3, 30, 25, {0}}};
    int result8 = find_most_popular_course(courses8, 3);
    printf("Test No.8: Middle course most popular\n");
    assert_equal_int(1, result8, 8, 9);

    // Test case 9: Large number of courses
    course_t courses9[] = {
        {"CMPSC311", "Data Structures", 3, 30, 10, {0}}, 
        {"CMPSC312", "Algorithms", 3, 30, 25, {0}}, 
        {"CMPSC313", "Systems", 3, 30, 5, {0}},
        {"CMPSC314", "Networks", 3, 30, 30, {0}},
        {"CMPSC315", "Databases", 3, 30, 15, {0}}
    };
    int result9 = find_most_popular_course(courses9, 5);
    printf("Test No.9: Large number of courses\n");
    assert_equal_int(3, result9, 9, 9);

    // Test case 10: Zero courses array
    int result10 = find_most_popular_course(NULL, 0);
    printf("Test No.10: Zero courses array\n");
    assert_equal_int(-1, result10, 10, 9);

    printf("\n");
}

void test_create_student(){
    printf("--------------------------------\n");
    printf("Testing create_student...\n\n");

    // Test case 1: Create a basic student (not suspended)
    student_t result1 = create_student(123, "John", "Doe", 2, false);
    student_t expected1 = {123, "John", "Doe", 2, false};
    printf("Test No.1: Create basic student (not suspended)\n");
    assert_equal_student(expected1, result1, 1, 10);

    // Test case 2: Create student with different parameters (suspended)
    student_t result2 = create_student(456, "Jane", "Smith", 1, true);
    student_t expected2 = {456, "Jane", "Smith", 1, true};
    printf("Test No.2: Create student with different parameters (suspended)\n");
    assert_equal_student(expected2, result2, 2, 10);

    printf("\n");
}

void test_create_course(){
    printf("--------------------------------\n");
    printf("Testing create_course...\n\n");

    // Test case 1: Create a basic course
    course_t result1 = create_course("CMPSC311", "Data Structures", 3, 30);
    course_t expected1 = {"CMPSC311", "Data Structures", 3, 30, 0, {0}};
    printf("Test No.1: Create basic course\n");
    assert_equal_course(expected1, result1, 1, 11);

    // Test case 2: Create course with different parameters
    course_t result2 = create_course("CMPSC312", "Algorithms", 4, 25);
    course_t expected2 = {"CMPSC312", "Algorithms", 4, 25, 0, {0}};
    printf("Test No.2: Create course with different parameters\n");
    assert_equal_course(expected2, result2, 2, 11);

    printf("\n");
}

void test_add_student_to_course(){
    printf("--------------------------------\n");
    printf("Testing add_student_to_course...\n\n");

    // Test case 1: Add first student to empty course
    course_t course1 = create_course("CMPSC311", "Data Structures", 3, 2);
    student_t student1 = create_student(123, "John", "Doe", 2, false);
    course1 = add_student_to_course(course1, student1);
    printf("Test No.1: Add first student to empty course\n");
    assert_equal_int(1, course1.current_enrollment, 1, 12);
    assert_equal_int(123, course1.students[0].student_id, 1, 12);

    // Test case 2: Add second student to non-full course
    student_t student2 = create_student(456, "Jane", "Smith", 2, false);
    course1 = add_student_to_course(course1, student2);
    printf("Test No.2: Add second student to non-full course\n");
    assert_equal_int(2, course1.current_enrollment, 2, 12);
    assert_equal_int(456, course1.students[1].student_id, 2, 12);

    // Test case 3: Try to add student to full course
    student_t student3 = create_student(789, "Bob", "Wilson", 2, false);
    course_t result3 = add_student_to_course(course1, student3);
    printf("Test No.3: Try to add student to full course\n");
    assert_equal_int(2, result3.current_enrollment, 3, 12);
    assert_not_equal_int(789, result3.students[2].student_id, 3, 12);
    
    // Test case 4: Add suspended student (should still be added)
    course1.max_students = 3;
    student_t student4 = create_student(999, "Suspended", "Student", 2, true);
    course1 = add_student_to_course(course1, student4);
    printf("Test No.4: Add suspended student\n");
    assert_equal_int(3, course1.current_enrollment, 4, 12);
    assert_equal_int(999, course1.students[2].student_id, 4, 12);

    // Test case 5: Add same student twice
    course1.max_students = 6;
    course_t result5 = add_student_to_course(course1, student2);
    printf("Test No.5: Add same student twice\n");
    assert_equal_int(3, result5.current_enrollment, 5, 12);
    assert_equal_int(456, result5.students[1].student_id, 5, 12);
    assert_not_equal_int(456, result5.students[3].student_id, 5, 12);

    printf("\n");
}

void test_find_student_in_course(){
    printf("--------------------------------\n");
    printf("Testing find_student_in_course...\n\n");

    course_t course1 = create_course("CMPSC311", "Data Structures", 3, 30);
    student_t student1 = create_student(123, "John", "Doe", 2, false);
    student_t student2 = create_student(456, "Jane", "Smith", 2, false);
    student_t student3 = create_student(999, "Suspended", "Student", 2, true);
    course1 = add_student_to_course(course1, student1);
    course1 = add_student_to_course(course1, student2);
    course1 = add_student_to_course(course1, student3);

    // Test case 1: Find existing student
    int result1 = find_student_in_course(course1, 123);
    printf("Test No.1: Find existing student\n");
    assert_equal_int(0, result1, 1, 13);

    // Test case 2: Find non-existing student
    int result2 = find_student_in_course(course1, 789);
    printf("Test No.2: Find non-existing student\n");
    assert_equal_int(-1, result2, 2, 13);

    // Test case 3: Find suspended student
    int result3 = find_student_in_course(course1, 999);
    printf("Test No.3: Find suspended student\n");
    assert_equal_int(2, result3, 3, 13);

    printf("\n");
}

// Main function calls all the test functions and summarizes the results.
int main(){
    test_percentage_to_letter();
    test_letter_to_percentage();
    test_letter_to_gpa();
    test_create_student();
    test_create_course();
    test_is_course_at_capacity();
    test_add_student_to_course();
    test_calculate_student_average();
    test_find_top_student();
    test_calculate_class_average();
    test_count_students_by_year();
    test_calculate_total_credit_hours();
    test_find_most_popular_course();
    test_find_student_in_course();

    printf("-------------------Summary-------------------\n");
    printf("Total tests: %d\n", passed + failed);
    printf("Total tests passed: %d\n", passed);
    printf("Total tests failed: %d\n", failed);
    printf("--------------------------------\n");
    int num_function_passed = 0;
    float total_points = 0;
    for (int i = 0; i < 14; i++){
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
        printf("points for correctness: %f/22, the other 3 points are for comments and git commits, please read the document for more details.\n", total_points);
    return 0;
}
