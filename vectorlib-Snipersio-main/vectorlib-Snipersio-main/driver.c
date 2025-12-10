#include <stdio.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <setjmp.h>
#include <limits.h>
#include <stdbool.h>
#include "vectorlib.h"

int passed = 0;
int failed = 0;
#define NUM_TESTS 10
char * function_name [NUM_TESTS] = {"vector_create", "vector_destroy", "vector_extend", "vector_size", 
                           "vector_extend_array", "vector_get", "vector_set", "vector_coalesce", "memory_stress", "full_scope"};
float function_points [NUM_TESTS] = {2, 2, 3, 1, 2, 1, 1, 3, 2, 3};
int failure_count [NUM_TESTS]     = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
typedef void (*test_function_t)(void);

// Signal handling for segfault protection
static sigjmp_buf segfault_jmp;
static int current_test_function = -1;

// Signal handler for segmentation faults
void segfault_handler(int sig) {
    signal(SIGSEGV, segfault_handler);

    printf("\n[SEGFAULT] Function %s caused a segmentation fault!\n", 
           current_test_function >= 0 ? function_name[current_test_function] : "unknown");
    printf("[SEGFAULT] Marking all remaining tests for this function as failed.\n");
    
    // Mark all remaining tests for this function as failed
    if (current_test_function >= 0) {
        failure_count[current_test_function] = 999; // Mark as segfaulted
    }
    
    // Jump back to the test runner
    siglongjmp(segfault_jmp, 1);
}

/*
You do NOT need to read the code in this file or understand it to complete the assignment.
Please do NOT make any changes to this file.

This file is used to test the correctness of your implementation of the functions in vectorlib.c.

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

void assert_equal_unsigned_int(unsigned int expected, unsigned int actual, int test_num, int function_index){
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %u, but got %u.\n", test_num, expected, actual);
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

void assert_not_null(void* ptr, int test_num, int function_index){
    if (ptr != NULL){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected non-NULL pointer, but got NULL.\n", test_num);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_null(void* ptr, int test_num, int function_index){
    if (ptr == NULL){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected NULL pointer, but got non-NULL.\n", test_num);
        failure_count[function_index]++;
        failed++;
    }
}

// Test functions
void test_vector_create(){
    printf("--------------------------------\n");
    printf("Testing vector_create...\n\n");

    int idx = 1;
    
    // Test creating vector with capacity 0
    printf("Test No.%d:\nCreating vector with capacity 0\n", idx);
    vector_t* vec1 = vector_create(0);
    assert_not_null(vec1, idx++, 0);
    assert_equal_unsigned_int(0, vec1->num_chunks, idx++, 0);
    assert_equal_unsigned_int(0, vec1->num_elements, idx++, 0);
    assert_null(vec1->head, idx++, 0);
    assert_null(vec1->tail, idx++, 0);
    
    // Test creating vector with capacity 5
    printf("\nTest No.%d:\nCreating vector with capacity 5\n", idx);
    vector_t* vec2 = vector_create(5);
    assert_not_null(vec2, idx++, 0);
    assert_equal_unsigned_int(1, vec2->num_chunks, idx++, 0);
    assert_equal_unsigned_int(5, vec2->num_elements, idx++, 0);
    assert_not_null(vec2->head, idx++, 0);
    assert_not_null(vec2->tail, idx++, 0);
    assert_equal_pointer(vec2->head, vec2->tail, idx++, 0); // head and tail should point to same chunk
    assert_equal_unsigned_int(5, vec2->head->num_elements, idx++, 0);
    assert_null(vec2->head->next, idx++, 0);
    
    // Test creating vector with capacity 10
    printf("\nTest No.%d:\nCreating vector with capacity 10\n", idx);
    vector_t* vec3 = vector_create(10);
    assert_not_null(vec3, idx++, 0);
    assert_equal_unsigned_int(1, vec3->num_chunks, idx++, 0);
    assert_equal_unsigned_int(10, vec3->num_elements, idx++, 0);
    assert_not_null(vec3->head, idx++, 0);
    assert_not_null(vec3->tail, idx++, 0);
    assert_equal_pointer(vec3->head, vec3->tail, idx++, 0); // head and tail should point to same chunk
    assert_equal_unsigned_int(10, vec3->head->num_elements, idx++, 0);
    assert_null(vec3->head->next, idx++, 0);
    
    // Test creating vector with large capacity
    printf("\nTest No.%d:\nCreating vector with large capacity\n", idx);
    vector_t* vec4 = vector_create(100000);
    assert_not_null(vec4, idx++, 0);
    assert_equal_unsigned_int(1, vec4->num_chunks, idx++, 0);
    assert_equal_unsigned_int(100000, vec4->num_elements, idx++, 0);
    assert_not_null(vec4->head, idx++, 0);
    assert_not_null(vec4->tail, idx++, 0);
    assert_equal_pointer(vec4->head, vec4->tail, idx++, 0); // head and tail should point to same chunk
    assert_equal_unsigned_int(100000, vec4->head->num_elements, idx++, 0);
    assert_null(vec4->head->next, idx++, 0);
    
    // Clean up
    vector_destroy(vec1);
    vector_destroy(vec2);
    vector_destroy(vec3);
    vector_destroy(vec4);
    
    printf("\n");
}

void test_vector_destroy(){
    printf("--------------------------------\n");
    printf("Testing vector_destroy...\n\n");

    int idx = 1;
    
    // Test destroying NULL vector
    printf("Test No.%d:\nDestroying NULL vector\n", idx);
    int result = vector_destroy(NULL);
    assert_equal_int(-1, result, idx++, 1);
    
    // Test destroying empty vector
    printf("\nTest No.%d:\nDestroying empty vector\n", idx);
    vector_t* vec1 = vector_create(0);
    result = vector_destroy(vec1);
    assert_equal_int(0, result, idx++, 1);
    
    // Test destroying vector with multiple chunks
    printf("\nTest No.%d:\nDestroying vector with multiple chunks\n", idx);
    vector_t* vec2 = vector_create(3);
    vector_extend(vec2, 4, false);
    vector_extend(vec2, 2, true);
    result = vector_destroy(vec2);
    assert_equal_int(0, result, idx++, 1);
    
    printf("\n");
}

void test_vector_extend(){
    printf("--------------------------------\n");
    printf("Testing vector_extend...\n\n");

    int idx = 1;
    
    // Test extending NULL vector with n elements
    printf("Test No.%d:\nExtending NULL vector with n elements\n", idx);
    int result = vector_extend(NULL, 5, false);
    assert_equal_int(-1, result, idx++, 2);
    
    // Test extending NULL vector with 0 elements
    printf("\nTest No.%d:\nExtending NULL vector with 0 elements\n", idx);
    result = vector_extend(NULL, 0, false);
    assert_equal_int(-1, result, idx++, 2);
    
    // Test extending empty vector with 0 elements
    printf("\nTest No.%d:\nExtending empty vector with 0 elements\n", idx);
    vector_t* empty_vec = vector_create(0);
    result = vector_extend(empty_vec, 0, false);
    assert_equal_int(0, result, idx++, 2); // Should succeed but do nothing
    assert_equal_unsigned_int(0, empty_vec->num_chunks, idx++, 2);
    assert_equal_unsigned_int(0, empty_vec->num_elements, idx++, 2);
    
    // Test extending empty vector with n elements
    printf("\nTest No.%d:\nExtending empty vector with n elements\n", idx);
    result = vector_extend(empty_vec, 3, false);
    assert_equal_int(0, result, idx++, 2);
    assert_equal_unsigned_int(1, empty_vec->num_chunks, idx++, 2);
    assert_equal_unsigned_int(3, empty_vec->num_elements, idx++, 2);
    
    // Test extending m vector with 0 elements
    printf("\nTest No.%d:\nExtending m vector with 0 elements\n", idx);
    vector_t* vec = vector_create(3);
    result = vector_extend(vec, 0, false);
    assert_equal_int(0, result, idx++, 2); // Should succeed but do nothing
    assert_equal_unsigned_int(1, vec->num_chunks, idx++, 2);
    assert_equal_unsigned_int(3, vec->num_elements, idx++, 2);
    
    // Test extending m vector with n elements
    printf("\nTest No.%d:\nExtending m vector with n elements\n", idx);
    result = vector_extend(vec, 4, false);
    assert_equal_int(0, result, idx++, 2);
    assert_equal_unsigned_int(2, vec->num_chunks, idx++, 2);
    assert_equal_unsigned_int(7, vec->num_elements, idx++, 2);
    
    // Test extending with zero fill
    printf("\nTest No.%d:\nExtending vector with zero fill\n", idx);
    result = vector_extend(vec, 2, true);
    assert_equal_int(0, result, idx++, 2);
    assert_equal_unsigned_int(3, vec->num_chunks, idx++, 2);
    assert_equal_unsigned_int(9, vec->num_elements, idx++, 2);
    
    // Verify zero fill worked
    assert_equal_int(0, vec->tail->elements[0], idx++, 2);
    assert_equal_int(0, vec->tail->elements[1], idx++, 2);
    
    // Test extending with very large number
    printf("\nTest No.%d:\nExtending vector with very large number\n", idx);
    vector_t* large_vec = vector_create(1);
    result = vector_extend(large_vec, 10000, true);
    assert_equal_int(0, result, idx++, 2);
    assert_equal_unsigned_int(2, large_vec->num_chunks, idx++, 2);
    assert_equal_unsigned_int(10001, large_vec->num_elements, idx++, 2);
    
    vector_destroy(vec);
    vector_destroy(empty_vec);
    vector_destroy(large_vec);
    
    printf("\n");
}

void test_vector_size(){
    printf("--------------------------------\n");
    printf("Testing vector_size...\n\n");

    int idx = 1;
    
    // Test size of NULL vector
    printf("Test No.%d:\nSize of NULL vector\n", idx);
    unsigned int size = vector_size(NULL);
    assert_equal_unsigned_int(0, size, idx++, 3);
    
    // Test size of empty vector
    printf("\nTest No.%d:\nSize of empty vector\n", idx);
    vector_t* vec1 = vector_create(0);
    size = vector_size(vec1);
    assert_equal_unsigned_int(0, size, idx++, 3);
    
    // Test size of normal vector
    printf("\nTest No.%d:\nSize of normal vector\n", idx);
    vector_t* vec2 = vector_create(5);
    size = vector_size(vec2);
    assert_equal_unsigned_int(5, size, idx++, 3);
    
    // Test size after extending
    printf("\nTest No.%d:\nSize after extending\n", idx);
    vector_extend(vec2, 3, false);
    size = vector_size(vec2);
    assert_equal_unsigned_int(8, size, idx++, 3);

    printf("\nTest No.%d:\nSize after extending again\n", idx);
    vector_extend(vec2, 10, true);
    size = vector_size(vec2);
    assert_equal_unsigned_int(18, size, idx++, 3);
    
    vector_destroy(vec1);
    vector_destroy(vec2);
    
    printf("\n");
}

void test_vector_extend_array(){
    printf("--------------------------------\n");
    printf("Testing vector_extend_array...\n\n");

    int idx = 1;
    
    int array1[] = {42, 23, 15};
    
    // Test extending NULL vector with n elements
    printf("Test No.%d:\nExtending NULL vector with n elements\n", idx);
    int result = vector_extend_array(NULL, array1, 3);
    assert_equal_int(-1, result, idx++, 4);
    
    // Test extending NULL vector with 0 elements
    printf("\nTest No.%d:\nExtending NULL vector with 0 elements\n", idx);
    result = vector_extend_array(NULL, array1, 0);
    assert_equal_int(-1, result, idx++, 4);
    
    // Test extending with NULL array
    printf("\nTest No.%d:\nExtending vector with NULL array\n", idx);
    vector_t* vec = vector_create(2);
    result = vector_extend_array(vec, NULL, 3);
    assert_equal_int(-1, result, idx++, 4);
    
    // Test extending empty vector with 0 elements
    printf("\nTest No.%d:\nExtending empty vector with 0 elements\n", idx);
    vector_t* empty_vec = vector_create(0);
    result = vector_extend_array(empty_vec, array1, 0);
    assert_equal_int(0, result, idx++, 4); // Should succeed but do nothing
    assert_equal_unsigned_int(0, empty_vec->num_chunks, idx++, 4);
    assert_equal_unsigned_int(0, empty_vec->num_elements, idx++, 4);
    
    // Test extending empty vector with n elements
    printf("\nTest No.%d:\nExtending empty vector with n elements\n", idx);
    result = vector_extend_array(empty_vec, array1, 3);
    assert_equal_int(0, result, idx++, 4);
    assert_equal_unsigned_int(1, empty_vec->num_chunks, idx++, 4);
    assert_equal_unsigned_int(3, empty_vec->num_elements, idx++, 4);
    assert_equal_unsigned_int(3, empty_vec->head->num_elements, idx++, 4);
    assert_equal_unsigned_int(array1[0], empty_vec->head->elements[0], idx++, 4);
    assert_equal_unsigned_int(array1[1], empty_vec->head->elements[1], idx++, 4);
    assert_equal_unsigned_int(array1[2], empty_vec->head->elements[2], idx++, 4);
    
    // Test extending m vector with 0 elements
    printf("\nTest No.%d:\nExtending m vector with 0 elements\n", idx);
    result = vector_extend_array(vec, array1, 0);
    assert_equal_int(0, result, idx++, 4); // Should succeed but do nothing
    assert_equal_unsigned_int(1, vec->num_chunks, idx++, 4);
    assert_equal_unsigned_int(2, vec->num_elements, idx++, 4);
    assert_null(vec->head->next, idx++, 4);
    
    // Test extending m vector with n elements
    printf("\nTest No.%d:\nExtending m vector with n elements\n", idx);
    result = vector_extend_array(vec, array1, 3);
    assert_equal_int(0, result, idx++, 4);
    assert_equal_unsigned_int(2, vec->num_chunks, idx++, 4);
    assert_equal_unsigned_int(5, vec->num_elements, idx++, 4);
    
    // Verify array values were copied
    printf("\nTest No.%d:\nVerifying array values were copied\n", idx);
    assert_equal_int(array1[0], vec->tail->elements[0], idx++, 4);
    assert_equal_int(array1[1], vec->tail->elements[1], idx++, 4);
    assert_equal_int(array1[2], vec->tail->elements[2], idx++, 4);
    
    // Verify values in empty vector extension
    assert_equal_int(array1[0], empty_vec->head->elements[0], idx++, 4);
    assert_equal_int(array1[1], empty_vec->head->elements[1], idx++, 4);
    assert_equal_int(array1[2], empty_vec->head->elements[2], idx++, 4);
    
    vector_destroy(vec);
    vector_destroy(empty_vec);
    
    printf("\n");
}

void test_vector_get(){
    printf("--------------------------------\n");
    printf("Testing vector_get...\n\n");

    int idx = 1;
    int* elem = NULL;
    
    // Test getting from NULL vector
    printf("Test No.%d:\nGetting from NULL vector\n", idx);
    elem = vector_get(NULL, 0);
    assert_null(elem, idx++, 5);

    elem = vector_get(NULL, 10);
    assert_null(elem, idx++, 5);
    
    // Test getting from empty vector
    printf("\nTest No.%d:\nGetting from empty vector\n", idx);
    vector_t* vec1 = vector_create(0);
    elem = vector_get(vec1, 0);
    assert_null(elem, idx++, 5);

    elem = vector_get(vec1, 10);
    assert_null(elem, idx++, 5);
    
    // Test getting with invalid index
    printf("\nTest No.%d:\nGetting with invalid index\n", idx);
    vector_t* vec2 = vector_create(3);
    elem = vector_get(vec2, 5);
    assert_null(elem, idx++, 5);
    elem = vector_get(vec2, 3);
    assert_null(elem, idx++, 5);
    elem = vector_get(vec2, 4);
    assert_null(elem, idx++, 5);
    
    // Test getting valid elements
    printf("\nTest No.%d:\nGetting valid elements\n", idx);
    vec2->head->elements[0] = 10;
    vec2->head->elements[1] = 20;
    vec2->head->elements[2] = 30;
    
    elem = vector_get(vec2, 0);
    assert_not_null(elem, idx++, 5);
    assert_equal_int(10, *elem, idx++, 5);
    
    elem = vector_get(vec2, 1);
    assert_not_null(elem, idx++, 5);
    assert_equal_int(20, *elem, idx++, 5);
    
    elem = vector_get(vec2, 2);
    assert_not_null(elem, idx++, 5);
    assert_equal_int(30, *elem, idx++, 5);
    
    // Test getting from multi-chunk vector
    printf("\nTest No.%d:\nGetting from multi-chunk vector\n", idx);
    vector_extend(vec2, 2, false);
    vec2->tail->elements[0] = 40;
    vec2->tail->elements[1] = 50;
    
    elem = vector_get(vec2, 3);
    assert_not_null(elem, idx++, 5);
    assert_equal_unsigned_int(40, *elem, idx++, 5);
    
    elem = vector_get(vec2, 4);
    assert_not_null(elem, idx++, 5);
    assert_equal_unsigned_int(50, *elem, idx++, 5);
    
    // Test getting with boundary indices
    printf("\nTest No.%d:\nGetting with boundary indices\n", idx);
    elem = vector_get(vec2, 0); // First element
    assert_not_null(elem, idx++, 5);
    assert_equal_int(10, *elem, idx++, 5);
    
    elem = vector_get(vec2, 4); // Last element
    assert_not_null(elem, idx++, 5);
    assert_equal_int(50, *elem, idx++, 5);
    
    // Test getting with index exactly at size (should fail)
    printf("\nTest No.%d:\nGetting with index at size boundary\n", idx);
    elem = vector_get(vec2, 5); // Index equals size
    assert_null(elem, idx++, 5);
    
    // Test getting with very large index
    printf("\nTest No.%d:\nGetting with very large index\n", idx);
    elem = vector_get(vec2, UINT_MAX);
    assert_null(elem, idx++, 5);
    
    vector_destroy(vec1);
    vector_destroy(vec2);
    
    printf("\n");
}

void test_vector_set(){
    printf("--------------------------------\n");
    printf("Testing vector_set...\n\n");

    int idx = 1;
    
    // Test setting in NULL vector
    printf("Test No.%d:\nSetting in NULL vector\n", idx);
    int result = vector_set(NULL, 0, 10);
    assert_equal_int(-1, result, idx++, 6);
    
    // Test setting in empty vector
    printf("\nTest No.%d:\nSetting in empty vector\n", idx);
    vector_t* vec1 = vector_create(0);
    result = vector_set(vec1, 0, 10);
    assert_equal_int(-1, result, idx++, 6);
    
    // Test setting with invalid index
    printf("\nTest No.%d:\nSetting with invalid index\n", idx);
    vector_t* vec2 = vector_create(3);
    result = vector_set(vec2, 5, 10);
    assert_equal_int(-1, result, idx++, 6);
    
    // Test setting valid elements
    printf("\nTest No.%d:\nSetting valid elements\n", idx);
    result = vector_set(vec2, 0, 100);
    assert_equal_int(0, result, idx++, 6);
    
    result = vector_set(vec2, 1, 200);
    assert_equal_int(0, result, idx++, 6);
    
    result = vector_set(vec2, 2, 300);
    assert_equal_int(0, result, idx++, 6);
    
    // Verify values were set
    printf("\nTest No.%d:\nVerifying values were set\n", idx);
    assert_equal_int(100, vec2->head->elements[0], idx++, 6);
    assert_equal_int(200, vec2->head->elements[1], idx++, 6);
    assert_equal_int(300, vec2->head->elements[2], idx++, 6);
    
    // Test setting in multi-chunk vector
    printf("\nTest No.%d:\nSetting in multi-chunk vector\n", idx);
    vector_extend(vec2, 3, true);
    result = vector_set(vec2, 3, 400);
    assert_equal_int(0, result, idx++, 6);
    
    result = vector_set(vec2, 4, 500);
    assert_equal_int(0, result, idx++, 6);
    
    // Verify multi-chunk values
    printf("\nTest No.%d:\nVerifying multi-chunk values\n", idx);
    assert_equal_int(400, vec2->tail->elements[0], idx++, 6);
    assert_equal_int(500, vec2->tail->elements[1], idx++, 6);
    assert_equal_int(0, vec2->tail->elements[2], idx++, 6);
    
    // Test setting with boundary indices
    printf("\nTest No.%d:\nSetting with boundary indices\n", idx);
    result = vector_set(vec2, 0, 1000); // First element
    assert_equal_int(0, result, idx++, 6);
    
    result = vector_set(vec2, 5, 2000); // Last element
    assert_equal_int(0, result, idx++, 6);
    
    // Verify boundary values
    assert_equal_int(1000, vec2->head->elements[0], idx++, 6);
    assert_equal_int(2000, vec2->tail->elements[2], idx++, 6);
    
    // Test setting with index exactly at size (should fail)
    printf("\nTest No.%d:\nSetting with index at size boundary\n", idx);
    result = vector_set(vec2, 6, 3000); // Index equals size
    assert_equal_int(-1, result, idx++, 6);
    
    // Test setting with very large index
    printf("\nTest No.%d:\nSetting with very large index\n", idx);
    result = vector_set(vec2, UINT_MAX, 4000);
    assert_equal_int(-1, result, idx++, 6);
    
    vector_destroy(vec1);
    vector_destroy(vec2);
    
    printf("\n");
}

void test_vector_coalesce(){
    printf("--------------------------------\n");
    printf("Testing vector_coalesce...\n\n");

    int idx = 1;
    
    // Test coalescing NULL vector
    printf("Test No.%d:\nCoalescing NULL vector\n", idx);
    int result = vector_coalesce(NULL);
    assert_equal_int(-1, result, idx++, 7);
    
    // Test coalescing empty vector
    printf("\nTest No.%d:\nCoalescing empty vector\n", idx);
    vector_t* vec1 = vector_create(0);
    result = vector_coalesce(vec1);
    assert_equal_int(0, result, idx++, 7); // Should succeed but do nothing
    assert_equal_unsigned_int(0, vec1->num_chunks, idx++, 7);
    assert_equal_unsigned_int(0, vec1->num_elements, idx++, 7);
    
    // Test coalescing single-chunk vector
    printf("\nTest No.%d:\nCoalescing single-chunk vector\n", idx);
    vector_t* vec2 = vector_create(5);
    vec2->head->elements[0] = 1;
    vec2->head->elements[1] = 2;
    vec2->head->elements[2] = 3;
    vec2->head->elements[3] = 4;
    vec2->head->elements[4] = 5;
    
    result = vector_coalesce(vec2);
    assert_equal_int(0, result, idx++, 7);
    assert_equal_unsigned_int(1, vec2->num_chunks, idx++, 7);
    assert_equal_unsigned_int(5, vec2->num_elements, idx++, 7);
    assert_equal_int(1, vec2->head->elements[0], idx++, 7);
    assert_equal_int(2, vec2->head->elements[1], idx++, 7);
    assert_equal_int(3, vec2->head->elements[2], idx++, 7);
    assert_equal_int(4, vec2->head->elements[3], idx++, 7);
    assert_equal_int(5, vec2->head->elements[4], idx++, 7);
    assert_null(vec2->head->next, idx++, 7);
    
    // Test coalescing multi-chunk vector
    printf("\nTest No.%d:\nCoalescing multi-chunk vector\n", idx);
    vector_t* vec3 = vector_create(3);
    vec3->head->elements[0] = 1;
    vec3->head->elements[1] = 2;
    vec3->head->elements[2] = 3;
    vector_extend(vec3, 2, false);
    vec3->tail->elements[0] = 4;
    vec3->tail->elements[1] = 5;
    vector_extend(vec3, 1, false);
    vec3->tail->elements[0] = 6;
    
    assert_equal_unsigned_int(3, vec3->num_chunks, idx++, 7);
    assert_equal_unsigned_int(6, vec3->num_elements, idx++, 7);
    
    result = vector_coalesce(vec3);
    assert_equal_int(0, result, idx++, 7);
    assert_equal_unsigned_int(1, vec3->num_chunks, idx++, 7);
    assert_equal_unsigned_int(6, vec3->num_elements, idx++, 7);
    assert_null(vec3->head->next, idx++, 7);
    
    // Verify values are preserved after coalescing
    printf("\nTest No.%d:\nVerifying values preserved after coalescing\n", idx);
    assert_equal_int(1, vec3->head->elements[0], idx++, 7);
    assert_equal_int(2, vec3->head->elements[1], idx++, 7);
    assert_equal_int(3, vec3->head->elements[2], idx++, 7);
    assert_equal_int(4, vec3->head->elements[3], idx++, 7);
    assert_equal_int(5, vec3->head->elements[4], idx++, 7);
    assert_equal_int(6, vec3->head->elements[5], idx++, 7);
    
    // Test coalescing vector with single element
    printf("\nTest No.%d:\nCoalescing vector with single element\n", idx);
    vector_t* vec4 = vector_create(1);
    vec4->head->elements[0] = 42;
    result = vector_coalesce(vec4);
    assert_equal_int(0, result, idx++, 7);
    assert_equal_unsigned_int(1, vec4->num_chunks, idx++, 7);
    assert_equal_unsigned_int(1, vec4->num_elements, idx++, 7);
    assert_null(vec4->head->next, idx++, 7);

    // Verify single element is preserved
    assert_equal_int(42, vec4->head->elements[0], idx++, 7);
    
    vector_destroy(vec1);
    vector_destroy(vec2);
    vector_destroy(vec3);
    vector_destroy(vec4);
    
    printf("\n");
}

void test_memory_stress() {
    printf("--------------------------------\n");
    printf("Testing memory stress scenarios...\n\n");

    int idx = 1;
    
    // Test creating many vectors and destroying them
    printf("Test No.%d:\nTesting multiple vector creation and destruction\n", idx);
    vector_t* vectors[25];
    for (int i = 0; i < 25; i++) {
        vectors[i] = vector_create(10);
        assert_not_null(vectors[i], idx++, 8);
    }
    
    // Destroy all vectors
    for (int i = 0; i < 25; i++) {
        int result = vector_destroy(vectors[i]);
        assert_equal_int(0, result, idx++, 8);
    }
    
    // Test extending vector many times
    printf("\nTest No.%d:\nTesting multiple extensions\n", idx);
    vector_t* stress_vec = vector_create(1);
    assert_not_null(stress_vec, idx++, 8);
    
    for (int i = 0; i < 50; i++) {
        int result = vector_extend(stress_vec, i, true);
        assert_equal_int(0, result, idx++, 8);
    }
    
    assert_equal_unsigned_int(50, stress_vec->num_chunks, idx++, 8);
    assert_equal_unsigned_int((int)(49*(49+1)/2+1), stress_vec->num_elements, idx++, 8);
    
    // Test coalescing large multi-chunk vector
    printf("\nTest No.%d:\nTesting coalescing large multi-chunk vector\n", idx);
    int result = vector_coalesce(stress_vec);
    assert_equal_int(0, result, idx++, 8);
    assert_equal_unsigned_int(1, stress_vec->num_chunks, idx++, 8);
    assert_equal_unsigned_int((int)(49*(49+1)/2+1), stress_vec->num_elements, idx++, 8);
    assert_null(stress_vec->head->next, idx++, 8);
    
    // Verify values are still accessible after coalescing (last element)
    int* elem = vector_get(stress_vec, (int)(49*(49+1)/2));
    assert_equal_int(0, *elem, idx++, 8);
    
    vector_destroy(stress_vec);
    
    printf("\n");
}

void test_full_scope() {
    printf("--------------------------------\n");
    printf("Testing full scope integration...\n\n");

    int idx = 1;
    
    // === PHASE 1: Create and Populate Vector ===
    printf("=== PHASE 1: Creating and Populating Vector ===\n");
    
    printf("Test No.%d:\nCreating vector with initial capacity 5\n", idx);
    vector_t* vec = vector_create(5);
    assert_not_null(vec, idx++, 8);
    assert_equal_unsigned_int(1, vec->num_chunks, idx++, 8);
    assert_equal_unsigned_int(5, vec->num_elements, idx++, 8);
    assert_null(vec->head->next, idx++, 8);
    
    // Populate initial chunk
    printf("\nTest No.%d:\nPopulating initial chunk\n", idx);
    for (unsigned int i = 0; i < 5; i++) {
        int result = vector_set(vec, i, (i + 1) * 10);
        assert_equal_int(0, result, idx++, 8);
    }
    
    // === PHASE 2: Extend Vector ===
    printf("\n=== PHASE 2: Extending Vector ===\n");
    
    printf("\nTest No.%d:\nExtending vector by 3 elements\n", idx);
    int result = vector_extend(vec, 3, false);
    assert_equal_int(0, result, idx++, 8);
    assert_equal_unsigned_int(2, vec->num_chunks, idx++, 8);
    assert_equal_unsigned_int(8, vec->num_elements, idx++, 8);
    assert_not_null(vec->head->next, idx++, 8);
    assert_null(vec->head->next->next, idx++, 8);
    assert_equal_pointer(vec->head->next, vec->tail, idx++, 8);
    
    // Populate new chunk
    printf("\nTest No.%d:\nPopulating new chunk\n", idx);
    vector_set(vec, 5, 60);
    vector_set(vec, 6, 70);
    vector_set(vec, 7, 80);
    
    // === PHASE 3: Extend with Array ===
    printf("\n=== PHASE 3: Extending with Array ===\n");
    
    printf("\nTest No.%d:\nExtending vector with array\n", idx);
    int array[] = {90, 100, 110};
    result = vector_extend_array(vec, array, 3);
    assert_equal_int(0, result, idx++, 8);
    assert_equal_unsigned_int(3, vec->num_chunks, idx++, 8);
    assert_equal_unsigned_int(11, vec->num_elements, idx++, 8);
    assert_not_null(vec->head->next, idx++, 8);
    assert_null(vec->head->next->next->next, idx++, 8);
    assert_equal_pointer(vec->head->next->next, vec->tail, idx++, 8);
    assert_equal_unsigned_int(3, vec->head->next->num_elements, idx++, 8);
    
    
    // === PHASE 4: Verify All Values ===
    printf("\n=== PHASE 4: Verifying All Values ===\n");
    
    printf("\nTest No.%d:\nVerifying all values across chunks\n", idx);
    for (unsigned int i = 0; i < 11; i++) {
        int* elem = vector_get(vec, i);
        assert_not_null(elem, idx++, 8);
        assert_equal_int((i + 1) * 10, *elem, idx++, 8);
    }
    
    // === PHASE 5: Test Coalescing ===
    printf("\n=== PHASE 5: Testing Coalescing ===\n");
    
    printf("\nTest No.%d:\nCoalescing vector\n", idx);
    result = vector_coalesce(vec);
    assert_equal_int(0, result, idx++, 8);
    assert_equal_unsigned_int(1, vec->num_chunks, idx++, 8);
    assert_equal_unsigned_int(11, vec->num_elements, idx++, 8);
    assert_null(vec->head->next, idx++, 8);
    assert_equal_pointer(vec->head, vec->tail, idx++, 8);
    
    // Verify values are still correct after coalescing
    printf("\nTest No.%d:\nVerifying values after coalescing\n", idx);
    for (unsigned int i = 0; i < 11; i++) {
        int* elem = vector_get(vec, i);
        assert_not_null(elem, idx++, 8);
        assert_equal_int((i + 1) * 10, *elem, idx++, 8);
    }
    
    // === PHASE 6: Edge Cases ===
    printf("\n=== PHASE 6: Testing Edge Cases ===\n");
    
    printf("\nTest No.%d:\nTesting edge case operations\n", idx);
    unsigned int size = vector_size(vec);
    assert_equal_unsigned_int(11, size, idx++, 8);
    
    // Test getting last element
    int* last_elem = vector_get(vec, 10);
    assert_not_null(last_elem, idx++, 8);
    assert_equal_int(110, *last_elem, idx++, 8);
    
    // Test setting last element
    result = vector_set(vec, 10, 999);
    assert_equal_int(0, result, idx++, 8);
    last_elem = vector_get(vec, 10);
    assert_not_null(last_elem, idx++, 8);
    assert_equal_int(999, *last_elem, idx++, 8);
    
    // Test boundary conditions
    printf("\nTest No.%d:\nTesting boundary conditions\n", idx);
    
    // Test getting first element
    int* first_elem = vector_get(vec, 0);
    assert_not_null(first_elem, idx++, 8);
    assert_equal_int(10, *first_elem, idx++, 8);
    
    // Test setting first element
    result = vector_set(vec, 0, 777);
    assert_equal_int(0, result, idx++, 8);
    first_elem = vector_get(vec, 0);
    assert_not_null(first_elem, idx++, 8);
    assert_equal_int(777, *first_elem, idx++, 8);
    
    // Test invalid index access
    printf("\nTest No.%d:\nTesting invalid index access\n", idx);
    int* invalid_elem = vector_get(vec, 11); // Out of bounds
    assert_null(invalid_elem, idx++, 8);
    
    result = vector_set(vec, 11, 123); // Out of bounds
    assert_equal_int(-1, result, idx++, 8);
    
    // Test extending with 0 elements after coalescing
    printf("\nTest No.%d:\nTesting extend with 0 elements after coalescing\n", idx);
    result = vector_extend(vec, 0, true);
    assert_equal_int(0, result, idx++, 8);
    assert_equal_unsigned_int(1, vec->num_chunks, idx++, 8); // Should still be 1 chunk
    assert_equal_unsigned_int(11, vec->num_elements, idx++, 8); // Should still be 11 elements
    
    // === PHASE 7: Cleanup ===
    printf("\n=== PHASE 7: Cleanup ===\n");
    
    printf("\nTest No.%d:\nDestroying vector\n", idx);
    result = vector_destroy(vec);
    assert_equal_int(0, result, idx++, 8);
    
    printf("\n=== FULL SCOPE TEST COMPLETED ===\n");
    printf("\n");
}

// Array of test functions corresponding to function_name array
test_function_t test_functions[10] = {
    test_vector_create,
    test_vector_destroy,
    test_vector_extend,
    test_vector_size,
    test_vector_extend_array,
    test_vector_get,
    test_vector_set,
    test_vector_coalesce,
    test_memory_stress,
    test_full_scope
};

// Safe test runner that catches segfaults
void run_test_safely(int function_index) {
    current_test_function = function_index;
    
    if (sigsetjmp(segfault_jmp, 1) == 0) {
        // Set up signal handler
        signal(SIGSEGV, segfault_handler);
        
        // Run the test
        test_functions[function_index]();
        
        // Reset signal handler
        signal(SIGSEGV, SIG_DFL);
    }
}

// Main function calls all the test functions and summarizes the results.
int main(int argc, char* argv[]){
    // Set up signal handler for SIGSEGV at the start
    signal(SIGSEGV, segfault_handler);
    
    // If no argument provided, run all tests
    if (argc == 1) {
        for (int i = 0; i < NUM_TESTS; i++) {
            run_test_safely(i);
        }
    } else {
        // Look for the specified test function
        char* test_name = argv[1];
        int found = 0;
        
        for (int i = 0; i < NUM_TESTS; i++) {
            if (strcmp(test_name, function_name[i]) == 0) {
                run_test_safely(i);
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("Error: Test function '%s' not found.\n", test_name);
            printf("Available test functions:\n");
            for (int i = 0; i < NUM_TESTS; i++) {
                printf("  %s\n", function_name[i]);
            }
            return 1;
        }
    }
    
    printf("-------------------Summary-------------------\n");
    printf("Total tests: %d\n", passed + failed);
    printf("Total tests passed: %d\n", passed);
    printf("Total tests failed: %d\n", failed);
    printf("--------------------------------\n");
    
    int num_function_passed = 0;
    float total_points = 0;
    int functions_tested = 0;
    
    // If no argument provided, show all functions
    if (argc == 1) {
        for (int i = 0; i < NUM_TESTS; i++){
            if (failure_count[i] == 0){
                num_function_passed++;
                total_points += function_points[i];
                printf("Function %s passed\n", function_name[i]);
            }
            else if (failure_count[i] == 999){
                printf("Function %s SEGFAULTED (all tests failed)\n", function_name[i]);
            }
            else{
                printf("Function %s failed\n", function_name[i]);
            }
            functions_tested++;
        }
    } else {
        // Show only the tested function
        char* test_name = argv[1];
        for (int i = 0; i < NUM_TESTS; i++) {
            if (strcmp(test_name, function_name[i]) == 0) {
                if (failure_count[i] == 0){
                    num_function_passed++;
                    total_points += function_points[i];
                    printf("Function %s passed\n", function_name[i]);
                }
                else if (failure_count[i] == 999){
                    printf("Function %s SEGFAULTED (all tests failed)\n", function_name[i]);
                }
                else{
                    printf("Function %s failed\n", function_name[i]);
                }
                functions_tested++;
                break;
            }
        }
    }
    
    printf("Total functions tested: %d\n", functions_tested);
    printf("Total functions passed: %d\n", num_function_passed);
     printf("points for correctness: %.1f/20, the other 5 points are for comments (1), git commits (1), and valgrind safety (3), please read the document for more details.\n", total_points);
    
    // Reset signal handler to default before exiting
    signal(SIGSEGV, SIG_DFL);
    return 0;
}
