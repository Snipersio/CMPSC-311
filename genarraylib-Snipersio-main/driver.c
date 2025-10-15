#include <stdio.h>
#include <string.h>
#include "genarraylib.h"

int passed = 0;
int failed = 0;
char * function_name [15] = {"create_generic_element", "is_equal_element", "create_generic_array_chunk", "create_generic_array",
                           "get_element", "set_element_int", "set_element_float", "set_element_double", "set_element_char", 
                           "set_element_short", "set_element_bool", "set_element_string", "is_equal", "full_scope", "create_generic_array_with_elements"};
float function_points [15] = {1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 5, 2};
int failure_count [15]     = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*
You do NOT need to read the code in this file or understand it to complete the assignment.
Please do NOT make any changes to this file.

This file is used to test the correctness of your implementation of the functions in genarraylib.c.

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

void assert_equal_double(double expected, double actual, int test_num, int function_index){
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %lf, but got %lf.\n", test_num, expected, actual);
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

void assert_equal_short(short expected, short actual, int test_num, int function_index){
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %d, but got %d.\n", test_num, expected, actual);
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

void assert_equal_string(const char expected[], const char actual[], int test_num, int function_index){
    if (strcmp(expected, actual) == 0){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected \"%s\", but got \"%8s\".\n", test_num, expected, actual);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_generic_type(generic_type_t expected, generic_type_t actual, int test_num, int function_index){
    if (expected == actual){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %ld, but got %ld.\n", test_num, expected, actual);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_response(bool expected_success, generic_array_response_t actual, int test_num, int function_index){
    if (expected_success == actual.success){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected success %s, but got %s.\n", test_num, expected_success ? "true" : "false", actual.success ? "true" : "false");
        failure_count[function_index]++;
        failed++;
    }
}

// Test functions
void test_create_generic_element(){
    printf("--------------------------------\n");
    printf("Testing create_generic_element...\n\n");

    int idx = 1;
    
    // Test creating VOID element
    generic_element_t elem_void = create_generic_element(true);
    printf("Test No.%d:\nCreating VOID element\n", idx);
    assert_equal_generic_type(GENERIC_TYPE_VOID, elem_void.type, idx++, 0);
    assert_equal_int(0, elem_void.value.int_value, idx++, 0);
    
    // Test creating ERROR element
    generic_element_t elem_error = create_generic_element(false);
    printf("\nTest No.%d:\nCreating ERROR element\n", idx);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, elem_error.type, idx++, 0);
    assert_equal_int(0, elem_error.value.int_value, idx++, 0);
    
    printf("\n");
}

void test_is_equal_element(){
    printf("--------------------------------\n");
    printf("Testing is_equal_element...\n\n");

    int idx = 1;
    
    // Test equal INT elements
    generic_element_t elem1 = create_generic_element(true);
    elem1.type = GENERIC_TYPE_INT;
    elem1.value.int_value = 42;
    
    generic_element_t elem2 = create_generic_element(true);
    elem2.type = GENERIC_TYPE_INT;
    elem2.value.int_value = 42;
    
    printf("Test No.%d:\nEqual INT elements\n", idx);
    assert_equal_bool(true, is_equal_element(elem1, elem2), idx++, 1);
    
    // Test different INT elements
    elem2.value.int_value = 43;
    printf("\nTest No.%d:\nDifferent INT elements\n", idx);
    assert_equal_bool(false, is_equal_element(elem1, elem2), idx++, 1);
    
    // Test different types
    elem2.type = GENERIC_TYPE_FLOAT;
    elem2.value.float_value = 42.0f;
    printf("\nTest No.%d:\nDifferent types (INT vs FLOAT)\n", idx);
    assert_equal_bool(false, is_equal_element(elem1, elem2), idx++, 1);
    
    // Test equal STRING elements
    generic_element_t elem3 = create_generic_element(true);
    elem3.type = GENERIC_TYPE_STRING;
    strcpy(elem3.value.string_value, "hello");
    
    generic_element_t elem4 = create_generic_element(true);
    elem4.type = GENERIC_TYPE_STRING;
    strcpy(elem4.value.string_value, "hello");
    
    printf("\nTest No.%d:\nEqual STRING elements\n", idx);
    assert_equal_bool(true, is_equal_element(elem3, elem4), idx++, 1);
    
    // Test different STRING elements
    strcpy(elem4.value.string_value, "world");
    printf("\nTest No.%d:\nDifferent STRING elements\n", idx);
    assert_equal_bool(false, is_equal_element(elem3, elem4), idx++, 1);
    
    printf("\n");
}

void test_create_generic_array_chunk(){
    printf("--------------------------------\n");
    printf("Testing create_generic_array_chunk...\n\n");

    int idx = 1;
    
    generic_array_chunk_t chunk1 = create_generic_array_chunk(10);
    printf("Test No.%d:\nCapacity: 10\n", idx);
    assert_equal_int(10, chunk1.element_capacity, idx++, 2);
    
    // Test that elements up to capacity are VOID
    printf("\nTest No.%d:\nChecking elements 0-9 are VOID\n", idx);
    for (int i = 0; i < 10; i++) {
        assert_equal_generic_type(GENERIC_TYPE_VOID, chunk1.elements[i].type, idx++, 2);
    }
    
    // Test that elements beyond capacity are ERROR
    printf("\nTest No.%d:\nChecking elements 10-24 are ERROR\n", idx);
    for (int i = 10; i < MAX_ELEMENTS_PER_CHUNK; i++) {
        assert_equal_generic_type(GENERIC_TYPE_ERROR, chunk1.elements[i].type, idx++, 2);
    }
    
    generic_array_chunk_t chunk2 = create_generic_array_chunk(50);
    printf("\nTest No.%d:\nCapacity: 50\n", idx);
    assert_equal_int(25, chunk2.element_capacity, idx++, 2);  // Should be capped at MAX_ELEMENTS_PER_CHUNK
    
    // Test that elements up to capacity are VOID
    printf("\nTest No.%d:\nChecking elements 0-24 are VOID\n", idx);
    for (int i = 0; i < 25; i++) {
        assert_equal_generic_type(GENERIC_TYPE_VOID, chunk2.elements[i].type, idx++, 2);
    }
    
    printf("\n");
}

void test_create_generic_array(){
    printf("--------------------------------\n");
    printf("Testing create_generic_array...\n\n");

    int idx = 1;
    
    // Test with 2 chunks
    int chunk_capacities1[] = {10, 20};
    generic_array_t arr1 = create_generic_array(2, chunk_capacities1, 0);
    printf("Test No.%d:\nChunks: 2, Capacities: [10, 20], Initial elements: 0\n", idx);
    assert_equal_int(2, arr1.chunk_capacity, idx++, 3);
    assert_equal_int(30, arr1.total_elements, idx++, 3);
    
    // Test that elements within capacity are VOID
    printf("\nTest No.%d:\nChecking chunk 0 elements 0-9 are VOID\n", idx);
    for (int i = 0; i < 10; i++) {
        assert_equal_generic_type(GENERIC_TYPE_VOID, arr1.chunks[0].elements[i].type, idx++, 3);
    }
    printf("\nTest No.%d:\nChecking chunk 0 element 10 is ERROR\n", idx);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr1.chunks[0].elements[10].type, idx++, 3);
    
    printf("\nTest No.%d:\nChecking chunk 1 elements 0-19 are VOID\n", idx);
    for (int i = 0; i < 20; i++) {
        assert_equal_generic_type(GENERIC_TYPE_VOID, arr1.chunks[1].elements[i].type, idx++, 3);
    }
    printf("\nTest No.%d:\nChecking chunk 1 element 20 is ERROR\n", idx);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr1.chunks[1].elements[20].type, idx++, 3);

    printf("\nTest No.%d:\nChecking chunk 2 element 0 is ERROR\n", idx);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr1.chunks[2].elements[0].type, idx++, 3);
    
    // Test with 3 chunks
    int chunk_capacities2[] = {5, 5, 5};
    generic_array_t arr2 = create_generic_array(3, chunk_capacities2, 0);
    printf("\nTest No.%d:\nChunks: 3, Capacities: [5, 5, 5]\n", idx);
    assert_equal_int(3, arr2.chunk_capacity, idx++, 3);
    assert_equal_int(15, arr2.total_elements, idx++, 3);
    
    // Test with single chunk
    int chunk_capacities3[] = {MAX_ELEMENTS_PER_CHUNK*2};
    generic_array_t arr3 = create_generic_array(1, chunk_capacities3, 0);
    printf("\nTest No.%d:\nChunks: 1, Capacities: [%d], Initial elements: 0\n", idx, MAX_ELEMENTS_PER_CHUNK*2);
    assert_equal_int(1, arr3.chunk_capacity, idx++, 3);
    assert_equal_int(MAX_ELEMENTS_PER_CHUNK, arr3.total_elements, idx++, 3);

    bool all_elements_are_void = true;
    for (int i = 0; i < MAX_ELEMENTS_PER_CHUNK; i++) {
        if (arr3.chunks[0].elements[i].type != GENERIC_TYPE_VOID) {
            all_elements_are_void = false;
            break;
        }
    }
    printf("\nTest No.%d:\nChecking all elements are VOID\n", idx);
    assert_equal_bool(true, all_elements_are_void, idx++, 3);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr3.chunks[1].elements[0].type, idx++, 3);

    // Test with invalid chunk_capacity (0)
    generic_array_t arr4 = create_generic_array(0, NULL, 0);
    printf("\nTest No.%d:\nChunks: 0 (invalid), should have ERROR elements\n", idx);
    assert_equal_int(0, arr4.chunk_capacity, idx++, 3);
    assert_equal_int(0, arr4.total_elements, idx++, 3);
    
    // Test that all elements are ERROR when chunk_capacity is 0
    printf("\nTest No.%d:\nChecking all elements are ERROR for invalid array\n", idx);
    for (int i = 0; i < MAX_CHUNKS; i++) {
        // just check the first 3 elements in the chunk
        for (int j = 0; j < 3; j++) {
            assert_equal_generic_type(GENERIC_TYPE_ERROR, arr4.chunks[i].elements[j].type, idx++, 3);
        }
    }
    
    printf("\n");
}

void test_create_generic_array_with_elements(){
    printf("--------------------------------\n");
    printf("Testing create_generic_array with initial elements...\n\n");

    int idx = 1;
    
    // Test 1: Single capacity array, elements less than total capacity
    int single_capacity[] = {10};
    generic_array_t arr1 = create_generic_array(1, single_capacity, 5);
    printf("Test No.%d:\nSingle chunk [10], Initial elements: 5\n", idx);
    assert_equal_int(1, arr1.chunk_capacity, idx++, 14);
    assert_equal_int(10, arr1.total_elements, idx++, 14);
    
    // Check that first 5 elements are 0, 1, 2, 3, 4
    for (int i = 0; i < 5; i++) {
        assert_equal_generic_type(GENERIC_TYPE_INT, arr1.chunks[0].elements[i].type, idx++, 14);
        assert_equal_int(i, arr1.chunks[0].elements[i].value.int_value, idx++, 14);
    }
    assert_equal_generic_type(GENERIC_TYPE_VOID, arr1.chunks[0].elements[5].type, idx++, 14);
    
    // Test 2: Single capacity array, elements equal to total capacity
    generic_array_t arr2 = create_generic_array(1, single_capacity, 10);
    printf("\nTest No.%d:\nSingle chunk [10], Initial elements: 10\n", idx);
    assert_equal_int(1, arr2.chunk_capacity, idx++, 14);
    assert_equal_int(10, arr2.total_elements, idx++, 14);
    
    // Check that all 10 elements are 0, 1, 2, ..., 9
    for (int i = 0; i < 10; i++) {
        assert_equal_generic_type(GENERIC_TYPE_INT, arr2.chunks[0].elements[i].type, idx++, 14);
        assert_equal_int(i, arr2.chunks[0].elements[i].value.int_value, idx++, 14);
    }
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr2.chunks[0].elements[10].type, idx++, 14);
    
    // Test 3: Single capacity array, elements larger than total capacity
    generic_array_t arr3 = create_generic_array(1, single_capacity, 15);
    printf("\nTest No.%d:\nSingle chunk [10], Initial elements: 15 (should cap at 10)\n", idx);
    assert_equal_int(1, arr3.chunk_capacity, idx++, 14);
    assert_equal_int(10, arr3.total_elements, idx++, 14);
    
    // Check that all 10 elements are 0, 1, 2, ..., 9
    for (int i = 0; i < 10; i++) {
        assert_equal_generic_type(GENERIC_TYPE_INT, arr3.chunks[0].elements[i].type, idx++, 14);
        assert_equal_int(i, arr3.chunks[0].elements[i].value.int_value, idx++, 14);
    }
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr3.chunks[0].elements[10].type, idx++, 14);
    
    // Test 4: Multiple chunks with weird capacities, elements less than total capacity
    int weird_capacities1[] = {1, 1, 10};
    generic_array_t arr4 = create_generic_array(3, weird_capacities1, 8);
    printf("\nTest No.%d:\nChunks [1, 1, 10], Initial elements: 8\n", idx);
    assert_equal_int(3, arr4.chunk_capacity, idx++, 14);
    assert_equal_int(12, arr4.total_elements, idx++, 14);
    
    // Check elements across chunks: 0 in chunk 0, 1 in chunk 1, 2-9 in chunk 2
    assert_equal_generic_type(GENERIC_TYPE_INT, arr4.chunks[0].elements[0].type, idx++, 14);
    assert_equal_int(0, arr4.chunks[0].elements[0].value.int_value, idx++, 14);
    
    assert_equal_generic_type(GENERIC_TYPE_INT, arr4.chunks[1].elements[0].type, idx++, 14);
    assert_equal_int(1, arr4.chunks[1].elements[0].value.int_value, idx++, 14);
    
    for (int i = 0; i < 6; i++) {
        assert_equal_generic_type(GENERIC_TYPE_INT, arr4.chunks[2].elements[i].type, idx++, 14);
        assert_equal_int(i + 2, arr4.chunks[2].elements[i].value.int_value, idx++, 14);
    }
    assert_equal_generic_type(GENERIC_TYPE_VOID, arr4.chunks[2].elements[8].type, idx++, 14);
    
    // Test 5: Multiple chunks with weird capacities including zero, elements equal to total capacity
    int weird_capacities2[] = {2, 1, 0, 2};
    generic_array_t arr5 = create_generic_array(4, weird_capacities2, 5);
    printf("\nTest No.%d:\nChunks [2, 1, 0, 2], Initial elements: 5\n", idx);
    assert_equal_int(4, arr5.chunk_capacity, idx++, 14);
    assert_equal_int(5, arr5.total_elements, idx++, 14);
    
    // Check elements: 0,1 in chunk 0, 2 in chunk 1, skip chunk 2 (capacity 0), 3,4 in chunk 3
    assert_equal_generic_type(GENERIC_TYPE_INT, arr5.chunks[0].elements[0].type, idx++, 14);
    assert_equal_int(0, arr5.chunks[0].elements[0].value.int_value, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_INT, arr5.chunks[0].elements[1].type, idx++, 14);
    assert_equal_int(1, arr5.chunks[0].elements[1].value.int_value, idx++, 14);
    
    assert_equal_generic_type(GENERIC_TYPE_INT, arr5.chunks[1].elements[0].type, idx++, 14);
    assert_equal_int(2, arr5.chunks[1].elements[0].value.int_value, idx++, 14);
    
    assert_equal_generic_type(GENERIC_TYPE_INT, arr5.chunks[3].elements[0].type, idx++, 14);
    assert_equal_int(3, arr5.chunks[3].elements[0].value.int_value, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_INT, arr5.chunks[3].elements[1].type, idx++, 14);
    assert_equal_int(4, arr5.chunks[3].elements[1].value.int_value, idx++, 14);

    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr5.chunks[0].elements[2].type, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr5.chunks[1].elements[1].type, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr5.chunks[2].elements[0].type, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr5.chunks[3].elements[2].type, idx++, 14);
    
    // Test 6: Multiple chunks with weird capacities, elements larger than total capacity
    generic_array_t arr6 = create_generic_array(4, weird_capacities2, 10);
    printf("\nTest No.%d:\nChunks [2, 1, 0, 2], Initial elements: 10 (should cap at 5)\n", idx);
    assert_equal_int(4, arr6.chunk_capacity, idx++, 14);
    assert_equal_int(5, arr6.total_elements, idx++, 14);
    
    // Should be same as arr5
    assert_equal_generic_type(GENERIC_TYPE_INT, arr6.chunks[0].elements[0].type, idx++, 14);
    assert_equal_int(0, arr6.chunks[0].elements[0].value.int_value, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_INT, arr6.chunks[0].elements[1].type, idx++, 14);
    assert_equal_int(1, arr6.chunks[0].elements[1].value.int_value, idx++, 14);
    
    assert_equal_generic_type(GENERIC_TYPE_INT, arr6.chunks[1].elements[0].type, idx++, 14);
    assert_equal_int(2, arr6.chunks[1].elements[0].value.int_value, idx++, 14);
    
    assert_equal_generic_type(GENERIC_TYPE_INT, arr6.chunks[3].elements[0].type, idx++, 14);
    assert_equal_int(3, arr6.chunks[3].elements[0].value.int_value, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_INT, arr6.chunks[3].elements[1].type, idx++, 14);
    assert_equal_int(4, arr6.chunks[3].elements[1].value.int_value, idx++, 14);

    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr6.chunks[0].elements[2].type, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr6.chunks[1].elements[1].type, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr6.chunks[2].elements[0].type, idx++, 14);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, arr6.chunks[3].elements[2].type, idx++, 14);
    
    printf("\n");
}

void test_set_element_int(){
    printf("--------------------------------\n");
    printf("Testing set_element_int...\n\n");

    int idx = 1;
    
    // Create a test array
    int chunk_capacities[] = {10, 10};
    generic_array_t arr = create_generic_array(2, chunk_capacities, 0);
    
    // Test valid set operations
    printf("Test No.%d:\nSetting element at index 0 to 42\n", idx);
    generic_array_response_t response1 = set_element_int(arr, 0, 42);
    assert_equal_response(true, response1, idx++, 5);
    assert_equal_generic_type(GENERIC_TYPE_INT, response1.array.chunks[0].elements[0].type, idx++, 5);
    assert_equal_int(42, response1.array.chunks[0].elements[0].value.int_value, idx++, 5);
    
    printf("\nTest No.%d:\nSetting element at index 5 to -100\n", idx);
    generic_array_response_t response2 = set_element_int(arr, 5, -100);
    assert_equal_response(true, response2, idx++, 5);
    assert_equal_generic_type(GENERIC_TYPE_INT, response2.array.chunks[0].elements[5].type, idx++, 5);
    assert_equal_int(-100, response2.array.chunks[0].elements[5].value.int_value, idx++, 5);
    
    printf("\nTest No.%d:\nSetting element at index 19 to 0\n", idx);
    generic_array_response_t response3 = set_element_int(arr, 19, 0);
    assert_equal_response(true, response3, idx++, 5);
    assert_equal_generic_type(GENERIC_TYPE_INT, response3.array.chunks[1].elements[9].type, idx++, 5);
    assert_equal_int(0, response3.array.chunks[1].elements[9].value.int_value, idx++, 5);
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nSetting element at index 100 to 42 (out of bounds)\n", idx);
    generic_array_response_t response5 = set_element_int(arr, 100, 42);
    assert_equal_response(false, response5, idx++, 5);
    
    printf("\n");
}

void test_set_element_float(){
    printf("--------------------------------\n");
    printf("Testing set_element_float...\n\n");

    int idx = 1;
    
    // Create a test array
    int chunk_capacities[] = {10, 10};
    generic_array_t arr = create_generic_array(2, chunk_capacities, 0);
    
    // Test valid set operations
    printf("Test No.%d:\nSetting element at index 1 to 3.14f\n", idx);
    generic_array_response_t response1 = set_element_float(arr, 1, 3.14f);
    assert_equal_response(true, response1, idx++, 6);
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, response1.array.chunks[0].elements[1].type, idx++, 6);
    assert_equal_float(3.14f, response1.array.chunks[0].elements[1].value.float_value, idx++, 6);
    
    printf("\nTest No.%d:\nSetting element at index 6 to -2.5f\n", idx);
    generic_array_response_t response2 = set_element_float(arr, 6, -2.5f);
    assert_equal_response(true, response2, idx++, 6);
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, response2.array.chunks[0].elements[6].type, idx++, 6);
    assert_equal_float(-2.5f, response2.array.chunks[0].elements[6].value.float_value, idx++, 6);
    
    printf("\nTest No.%d:\nSetting element at index 15 to 0.0f\n", idx);
    generic_array_response_t response3 = set_element_float(arr, 15, 0.0f);
    assert_equal_response(true, response3, idx++, 6);
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, response3.array.chunks[1].elements[5].type, idx++, 6);
    assert_equal_float(0.0f, response3.array.chunks[1].elements[5].value.float_value, idx++, 6);
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nSetting element at index 100 to 3.14f (out of bounds)\n", idx);
    generic_array_response_t response4 = set_element_float(arr, 100, 3.14f);
    assert_equal_response(false, response4, idx++, 6);
    
    printf("\n");
}

void test_set_element_double(){
    printf("--------------------------------\n");
    printf("Testing set_element_double...\n\n");

    int idx = 1;
    
    // Create a test array
    int chunk_capacities[] = {10, 10};
    generic_array_t arr = create_generic_array(2, chunk_capacities, 0);
    
    // Test valid set operations
    printf("Test No.%d:\nSetting element at index 2 to 2.718281828\n", idx);
    generic_array_response_t response1 = set_element_double(arr, 2, 2.718281828);
    assert_equal_response(true, response1, idx++, 7);
    assert_equal_generic_type(GENERIC_TYPE_DOUBLE, response1.array.chunks[0].elements[2].type, idx++, 7);
    assert_equal_double(2.718281828, response1.array.chunks[0].elements[2].value.double_value, idx++, 7);
    
    printf("\nTest No.%d:\nSetting element at index 7 to -1.5\n", idx);
    generic_array_response_t response2 = set_element_double(arr, 7, -1.5);
    assert_equal_response(true, response2, idx++, 7);
    assert_equal_generic_type(GENERIC_TYPE_DOUBLE, response2.array.chunks[0].elements[7].type, idx++, 7);
    assert_equal_double(-1.5, response2.array.chunks[0].elements[7].value.double_value, idx++, 7);
    
    printf("\nTest No.%d:\nSetting element at index 12 to 0.0\n", idx);
    generic_array_response_t response3 = set_element_double(arr, 12, 0.0);
    assert_equal_response(true, response3, idx++, 7);
    assert_equal_generic_type(GENERIC_TYPE_DOUBLE, response3.array.chunks[1].elements[2].type, idx++, 7);
    assert_equal_double(0.0, response3.array.chunks[1].elements[2].value.double_value, idx++, 7);
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nSetting element at index 100 to 1.0 (out of bounds)\n", idx);
    generic_array_response_t response4 = set_element_double(arr, 100, 1.0);
    assert_equal_response(false, response4, idx++, 7);
    
    printf("\n");
}

void test_set_element_char(){
    printf("--------------------------------\n");
    printf("Testing set_element_char...\n\n");

    int idx = 1;
    
    // Create a test array
    int chunk_capacities[] = {10, 10};
    generic_array_t arr = create_generic_array(2, chunk_capacities, 0);
    
    // Test valid set operations
    printf("Test No.%d:\nSetting element at index 3 to 'A'\n", idx);
    generic_array_response_t response1 = set_element_char(arr, 3, 'A');
    assert_equal_response(true, response1, idx++, 8);
    assert_equal_generic_type(GENERIC_TYPE_CHAR, response1.array.chunks[0].elements[3].type, idx++, 8);
    assert_equal_char('A', response1.array.chunks[0].elements[3].value.char_value, idx++, 8);
    
    printf("\nTest No.%d:\nSetting element at index 8 to 'z'\n", idx);
    generic_array_response_t response2 = set_element_char(arr, 8, 'z');
    assert_equal_response(true, response2, idx++, 8);
    assert_equal_generic_type(GENERIC_TYPE_CHAR, response2.array.chunks[0].elements[8].type, idx++, 8);
    assert_equal_char('z', response2.array.chunks[0].elements[8].value.char_value, idx++, 8);
    
    printf("\nTest No.%d:\nSetting element at index 13 to '0'\n", idx);
    generic_array_response_t response3 = set_element_char(arr, 13, '0');
    assert_equal_response(true, response3, idx++, 8);
    assert_equal_generic_type(GENERIC_TYPE_CHAR, response3.array.chunks[1].elements[3].type, idx++, 8);
    assert_equal_char('0', response3.array.chunks[1].elements[3].value.char_value, idx++, 8);
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nSetting element at index 100 to 'X' (out of bounds)\n", idx);
    generic_array_response_t response4 = set_element_char(arr, 100, 'X');
    assert_equal_response(false, response4, idx++, 8);
    
    printf("\n");
}

void test_set_element_short(){
    printf("--------------------------------\n");
    printf("Testing set_element_short...\n\n");

    int idx = 1;
    
    // Create a test array
    int chunk_capacities[] = {10, 10};
    generic_array_t arr = create_generic_array(2, chunk_capacities, 0);
    
    // Test valid set operations
    printf("Test No.%d:\nSetting element at index 4 to 32767\n", idx);
    generic_array_response_t response1 = set_element_short(arr, 4, 32767);
    assert_equal_response(true, response1, idx++, 9);
    assert_equal_generic_type(GENERIC_TYPE_SHORT, response1.array.chunks[0].elements[4].type, idx++, 9);
    assert_equal_short(32767, response1.array.chunks[0].elements[4].value.short_value, idx++, 9);
    
    printf("\nTest No.%d:\nSetting element at index 9 to -32768\n", idx);
    generic_array_response_t response2 = set_element_short(arr, 9, -32768);
    assert_equal_response(true, response2, idx++, 9);
    assert_equal_generic_type(GENERIC_TYPE_SHORT, response2.array.chunks[0].elements[9].type, idx++, 9);
    assert_equal_short(-32768, response2.array.chunks[0].elements[9].value.short_value, idx++, 9);
    
    printf("\nTest No.%d:\nSetting element at index 14 to 0\n", idx);
    generic_array_response_t response3 = set_element_short(arr, 14, 0);
    assert_equal_response(true, response3, idx++, 9);
    assert_equal_generic_type(GENERIC_TYPE_SHORT, response3.array.chunks[1].elements[4].type, idx++, 9);
    assert_equal_short(0, response3.array.chunks[1].elements[4].value.short_value, idx++, 9);
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nSetting element at index 100 to 100 (out of bounds)\n", idx);
    generic_array_response_t response4 = set_element_short(arr, 100, 100);
    assert_equal_response(false, response4, idx++, 9);
    
    printf("\n");
}

void test_set_element_bool(){
    printf("--------------------------------\n");
    printf("Testing set_element_bool...\n\n");

    int idx = 1;
    
    // Create a test array
    int chunk_capacities[] = {10, 10};
    generic_array_t arr = create_generic_array(2, chunk_capacities, 0);
    
    // Test valid set operations
    printf("Test No.%d:\nSetting element at index 5 to true\n", idx);
    generic_array_response_t response1 = set_element_bool(arr, 5, true);
    assert_equal_response(true, response1, idx++, 10);
    assert_equal_generic_type(GENERIC_TYPE_BOOL, response1.array.chunks[0].elements[5].type, idx++, 10);
    assert_equal_bool(true, response1.array.chunks[0].elements[5].value.bool_value, idx++, 10);
    
    printf("\nTest No.%d:\nSetting element at index 10 to false\n", idx);
    generic_array_response_t response2 = set_element_bool(arr, 10, false);
    assert_equal_response(true, response2, idx++, 10);
    assert_equal_generic_type(GENERIC_TYPE_BOOL, response2.array.chunks[1].elements[0].type, idx++, 10);
    assert_equal_bool(false, response2.array.chunks[1].elements[0].value.bool_value, idx++, 10);
    
    printf("\nTest No.%d:\nSetting element at index 15 to true\n", idx);
    generic_array_response_t response3 = set_element_bool(arr, 15, true);
    assert_equal_response(true, response3, idx++, 10);
    assert_equal_generic_type(GENERIC_TYPE_BOOL, response3.array.chunks[1].elements[5].type, idx++, 10);
    assert_equal_bool(true, response3.array.chunks[1].elements[5].value.bool_value, idx++, 10);
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nSetting element at index 100 to false (out of bounds)\n", idx);
    generic_array_response_t response4 = set_element_bool(arr, 100, false);
    assert_equal_response(false, response4, idx++, 10);
    
    printf("\n");
}

void test_set_element_string(){
    printf("--------------------------------\n");
    printf("Testing set_element_string...\n\n");

    int idx = 1;
    
    // Create a test array
    int chunk_capacities[] = {10, 10};
    generic_array_t arr = create_generic_array(2, chunk_capacities, 0);
    
    // Test valid set operations
    printf("Test No.%d:\nSetting element at index 6 to \"hello\"\n", idx);
    generic_array_response_t response1 = set_element_string(arr, 6, "hello");
    assert_equal_response(true, response1, idx++, 11);
    assert_equal_generic_type(GENERIC_TYPE_STRING, response1.array.chunks[0].elements[6].type, idx++, 11);
    assert_equal_string("hello", response1.array.chunks[0].elements[6].value.string_value, idx++, 11);
    
    printf("\nTest No.%d:\nSetting element at index 11 to \"world\"\n", idx);
    generic_array_response_t response2 = set_element_string(arr, 11, "world");
    assert_equal_response(true, response2, idx++, 11);
    assert_equal_generic_type(GENERIC_TYPE_STRING, response2.array.chunks[1].elements[1].type, idx++, 11);
    assert_equal_string("world", response2.array.chunks[1].elements[1].value.string_value, idx++, 11);
    
    printf("\nTest No.%d:\nSetting element at index 16 to \"test\"\n", idx);
    generic_array_response_t response3 = set_element_string(arr, 16, "test");
    assert_equal_response(true, response3, idx++, 11);
    assert_equal_generic_type(GENERIC_TYPE_STRING, response3.array.chunks[1].elements[6].type, idx++, 11);
    assert_equal_string("test", response3.array.chunks[1].elements[6].value.string_value, idx++, 11);
    
    printf("\nTest No.%d:\nSetting element at index 17 to empty string\n", idx);
    generic_array_response_t response4 = set_element_string(arr, 17, "");
    assert_equal_response(true, response4, idx++, 11);
    assert_equal_generic_type(GENERIC_TYPE_STRING, response4.array.chunks[1].elements[7].type, idx++, 11);
    assert_equal_string("", response4.array.chunks[1].elements[7].value.string_value, idx++, 11);
    
    printf("\nTest No.%d:\nSetting element at index 18 to long string\n", idx);
    generic_array_response_t response6 = set_element_string(arr, 18, "hello world test");
    assert_equal_response(true, response6, idx++, 11);
    assert_equal_generic_type(GENERIC_TYPE_STRING, response6.array.chunks[1].elements[8].type, idx++, 11);
    assert_equal_string("hello w", response6.array.chunks[1].elements[8].value.string_value, idx++, 11);
    assert_equal_generic_type(GENERIC_TYPE_VOID, response6.array.chunks[1].elements[9].type, idx++, 11);
    assert_equal_int(0, response6.array.chunks[1].elements[9].value.int_value, idx++, 11);

    printf("\nTest No.%d:\nSetting element at index 7 to character array\n", idx);
    char bad_string[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    generic_array_response_t response7 = set_element_string(arr, 7, bad_string);
    assert_equal_response(true, response7, idx++, 11);
    assert_equal_generic_type(GENERIC_TYPE_STRING, response7.array.chunks[0].elements[7].type, idx++, 11);
    assert_equal_string("abcdefg", response7.array.chunks[0].elements[7].value.string_value, idx++, 11);
    assert_equal_generic_type(GENERIC_TYPE_VOID, response7.array.chunks[0].elements[8].type, idx++, 11);
    assert_equal_int(0, response7.array.chunks[0].elements[8].value.int_value, idx++, 11);
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nSetting element at index 100 to \"nope\" (out of bounds)\n", idx);
    generic_array_response_t response8 = set_element_string(arr, 100, "nope");
    assert_equal_response(false, response8, idx++, 11);
    
    printf("\n");
}

void test_get_element(){
    printf("--------------------------------\n");
    printf("Testing get_element...\n\n");

    int idx = 1;
    
    // Create a test array and populate it
    int chunk_capacities[] = {10, 1, 10};
    generic_array_t arr = create_generic_array(3, chunk_capacities, 0);
    
    // Set some test elements
    arr.chunks[0].elements[0].type = GENERIC_TYPE_INT;
    arr.chunks[0].elements[0].value.int_value = 42;
    arr.chunks[0].elements[1].type = GENERIC_TYPE_FLOAT;
    arr.chunks[0].elements[1].value.float_value = 3.14f;
    arr.chunks[0].elements[2].type = GENERIC_TYPE_DOUBLE;
    arr.chunks[0].elements[2].value.double_value = 2.718281828;
    arr.chunks[0].elements[3].type = GENERIC_TYPE_CHAR;
    arr.chunks[0].elements[3].value.char_value = 'A';
    arr.chunks[0].elements[4].type = GENERIC_TYPE_SHORT;
    arr.chunks[0].elements[4].value.short_value = 32767;
    arr.chunks[0].elements[5].type = GENERIC_TYPE_BOOL;
    arr.chunks[0].elements[5].value.bool_value = true;
    arr.chunks[0].elements[6].type = GENERIC_TYPE_STRING;
    strcpy(arr.chunks[0].elements[6].value.string_value, "hello");
    arr.chunks[1].elements[0].type = GENERIC_TYPE_CHAR;
    arr.chunks[1].elements[0].value.char_value = 'B';
    arr.chunks[2].elements[1].type = GENERIC_TYPE_FLOAT;
    arr.chunks[2].elements[1].value.float_value = 1.23f;
    
    // Test getting valid elements
    printf("Test No.%d:\nGetting element at index 0 (int)\n", idx);
    generic_element_t elem0 = get_element(arr, 0);
    assert_equal_generic_type(GENERIC_TYPE_INT, elem0.type, idx++, 4);
    assert_equal_int(42, elem0.value.int_value, idx++, 4);
    
    printf("\nTest No.%d:\nGetting element at index 1 (float)\n", idx);
    generic_element_t elem1 = get_element(arr, 1);
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, elem1.type, idx++, 4);
    assert_equal_float(3.14f, elem1.value.float_value, idx++, 4);
    
    printf("\nTest No.%d:\nGetting element at index 2 (double)\n", idx);
    generic_element_t elem2 = get_element(arr, 2);
    assert_equal_generic_type(GENERIC_TYPE_DOUBLE, elem2.type, idx++, 4);
    assert_equal_double(2.718281828, elem2.value.double_value, idx++, 4);
    
    printf("\nTest No.%d:\nGetting element at index 3 (char)\n", idx);
    generic_element_t elem3 = get_element(arr, 3);
    assert_equal_generic_type(GENERIC_TYPE_CHAR, elem3.type, idx++, 4);
    assert_equal_char('A', elem3.value.char_value, idx++, 4);
    
    printf("\nTest No.%d:\nGetting element at index 4 (short)\n", idx);
    generic_element_t elem4 = get_element(arr, 4);
    assert_equal_generic_type(GENERIC_TYPE_SHORT, elem4.type, idx++, 4);
    assert_equal_short(32767, elem4.value.short_value, idx++, 4);
    
    printf("\nTest No.%d:\nGetting element at index 5 (bool)\n", idx);
    generic_element_t elem5 = get_element(arr, 5);
    assert_equal_generic_type(GENERIC_TYPE_BOOL, elem5.type, idx++, 4);
    assert_equal_bool(true, elem5.value.bool_value, idx++, 4);
    
    printf("\nTest No.%d:\nGetting element at index 6 (string)\n", idx);
    generic_element_t elem6 = get_element(arr, 6);
    assert_equal_generic_type(GENERIC_TYPE_STRING, elem6.type, idx++, 4);
    assert_equal_string("hello", elem6.value.string_value, idx++, 4);
    
    // Test getting VOID element (uninitialized)
    printf("\nTest No.%d:\nGetting element at index 7 (VOID/uninitialized)\n", idx);
    generic_element_t elem_void = get_element(arr, 7);
    assert_equal_generic_type(GENERIC_TYPE_VOID, elem_void.type, idx++, 4);
    assert_equal_int(0, elem_void.value.int_value, idx++, 4);

    printf("\nTest No.%d:\nGetting element at index 10 (char)\n", idx);
    generic_element_t elem7 = get_element(arr, 10);
    assert_equal_generic_type(GENERIC_TYPE_CHAR, elem7.type, idx++, 4);
    assert_equal_char('B', elem7.value.char_value, idx++, 4);

    printf("\nTest No.%d:\nGetting element at index 12 (float)\n", idx);
    generic_element_t elem8 = get_element(arr, 12);
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, elem8.type, idx++, 4);
    assert_equal_float(1.23f, elem8.value.float_value, idx++, 4);
    
    // Test getting invalid index
    printf("\nTest No.%d:\nGetting element at index 100 (invalid)\n", idx);
    generic_element_t elem_invalid = get_element(arr, 100);
    assert_equal_generic_type(GENERIC_TYPE_ERROR, elem_invalid.type, idx++, 4);
    
    printf("\n");
}

void test_is_equal(){
    printf("--------------------------------\n");
    printf("Testing is_equal...\n\n");

    int idx = 1;
    
    // Create two identical arrays
    int chunk_capacities1[] = {10, 10};
    int chunk_capacities2[] = {10, 10};
    generic_array_t arr1 = create_generic_array(2, chunk_capacities1, 0);
    generic_array_t arr2 = create_generic_array(2, chunk_capacities2, 0);
    
    // Set identical elements
    arr1.chunks[0].elements[0].type = GENERIC_TYPE_INT;
    arr1.chunks[0].elements[0].value.int_value = 42;
    arr2.chunks[0].elements[0].type = GENERIC_TYPE_INT;
    arr2.chunks[0].elements[0].value.int_value = 42;

    arr1.chunks[0].elements[1].type = GENERIC_TYPE_FLOAT;
    arr1.chunks[0].elements[1].value.float_value = 3.14f;
    arr2.chunks[0].elements[1].type = GENERIC_TYPE_FLOAT;
    arr2.chunks[0].elements[1].value.float_value = 3.14f;
    
    arr1.chunks[0].elements[2].type = GENERIC_TYPE_STRING;
    strcpy(arr1.chunks[0].elements[2].value.string_value, "hello");
    arr2.chunks[0].elements[2].type = GENERIC_TYPE_STRING;
    strcpy(arr2.chunks[0].elements[2].value.string_value, "hello");
    
    printf("Test No.%d:\nComparing identical arrays\n", idx);
    assert_equal_bool(true, is_equal(arr1, arr2), idx++, 12);


    printf("\nTest No.%d:\nComparing different arrays\n", idx);
    arr2.chunks[0].elements[2].value.string_value[0] = 'H';
    assert_equal_bool(false, is_equal(arr1, arr2), idx++, 12);
    
    // Create different arrays
    generic_array_t arr3 = create_generic_array(2, chunk_capacities1, 0);
    arr3.chunks[0].elements[0].type = GENERIC_TYPE_INT;
    arr3.chunks[0].elements[0].value.int_value = 42;
    arr3.chunks[0].elements[1].type = GENERIC_TYPE_FLOAT;
    arr3.chunks[0].elements[1].value.float_value = 3.15f;  // Different value
    arr3.chunks[0].elements[2].type = GENERIC_TYPE_STRING;
    strcpy(arr3.chunks[0].elements[2].value.string_value, "hello");
    
    printf("\nTest No.%d:\nComparing arrays with different values\n", idx);
    assert_equal_bool(false, is_equal(arr1, arr3), idx++, 12);
    
    // Create array with different length
    generic_array_t arr4 = create_generic_array(2, chunk_capacities1, 0);
    arr4.chunks[0].elements[0].type = GENERIC_TYPE_INT;
    arr4.chunks[0].elements[0].value.int_value = 42;
    arr4.chunks[0].elements[1].type = GENERIC_TYPE_FLOAT;
    arr4.chunks[0].elements[1].value.float_value = 3.14f;
    // Missing third element
    
    printf("\nTest No.%d:\nComparing arrays with different lengths\n", idx);
    assert_equal_bool(false, is_equal(arr1, arr4), idx++, 12);

    // Create two empty arrays
    generic_array_t arr5 = create_generic_array(0, NULL, 0);
    generic_array_t arr6 = create_generic_array(0, NULL, 0);

    printf("\nTest No.%d:\nComparing two empty arrays\n", idx);
    assert_equal_bool(true, is_equal(arr5, arr6), idx++, 12);

    // Create one empty array and one non-empty array
    printf("\nTest No.%d:\nComparing empty and non-empty arrays\n", idx);
    assert_equal_bool(false, is_equal(arr1, arr6), idx++, 12);

    // Test with different layouts
    int capacity1[] = {1, 1, 10};
    int capacity2[] = {12};
    generic_array_t arr7 = create_generic_array(3, capacity1, 0);
    generic_array_t arr8 = create_generic_array(1, capacity2, 0);
    arr7.chunks[0].elements[0].type = GENERIC_TYPE_INT;
    arr7.chunks[0].elements[0].value.int_value = 123;
    arr8.chunks[0].elements[0].type = GENERIC_TYPE_INT;
    arr8.chunks[0].elements[0].value.int_value = 123;
    
    arr7.chunks[1].elements[0].type = GENERIC_TYPE_INT;
    arr7.chunks[1].elements[0].value.int_value = 456;
    arr8.chunks[0].elements[1].type = GENERIC_TYPE_INT;
    arr8.chunks[0].elements[1].value.int_value = 456;
    
    arr7.chunks[2].elements[0].type = GENERIC_TYPE_INT;
    arr7.chunks[2].elements[0].value.int_value = 789;
    arr8.chunks[0].elements[2].type = GENERIC_TYPE_INT;
    arr8.chunks[0].elements[2].value.int_value = 789;
    
    arr7.chunks[2].elements[1].type = GENERIC_TYPE_INT;
    arr7.chunks[2].elements[1].value.int_value = 42;
    arr8.chunks[0].elements[3].type = GENERIC_TYPE_INT;
    arr8.chunks[0].elements[3].value.int_value = 42;

    printf("\nTest No.%d:\nComparing arrays with different layouts\n", idx);
    assert_equal_bool(true, is_equal(arr7, arr8), idx++, 12);


    printf("\nTest No.%d:\nComparing arrays with different layouts when different\n", idx);
    arr8.chunks[0].elements[11].type = GENERIC_TYPE_BOOL;
    assert_equal_bool(false, is_equal(arr7, arr8), idx++, 12);
    
    printf("\n");
}

void test_full_scope() {
    printf("--------------------------------\n");
    printf("Testing full scope integration...\n\n");

    int idx = 1;
    
    // Create array with strange capacities: 1, 0, 0, 5, 4, 3, 15, 0, 1
    int chunk_capacities[] = {1, 0, 0, 5, 4, 3, 15, 0, 1};
    generic_array_t arr = create_generic_array(9, chunk_capacities, 0);
    
    printf("Test No.%d:\nCreated array with capacities [1, 0, 0, 5, 4, 3, 15, 0, 1]\n", idx);
    assert_equal_int(9, arr.chunk_capacity, idx++, 13);
    assert_equal_int(29, arr.total_elements, idx++, 13);
    
    // Test setting elements across different chunks
    printf("\nTest No.%d:\nSetting element at index 0 (chunk 0, element 0) to int 42\n", idx);
    generic_array_response_t response1 = set_element_int(arr, 0, 42);
    assert_equal_response(true, response1, idx++, 13); arr = response1.array;
    assert_equal_generic_type(GENERIC_TYPE_INT, response1.array.chunks[0].elements[0].type, idx++, 13);
    assert_equal_int(42, response1.array.chunks[0].elements[0].value.int_value, idx++, 13);

    printf("\nTest No.%d:\nSetting element at index 1 to float 3.14f\n", idx);
    generic_array_response_t response2 = set_element_float(arr, 1, 3.14f);
    assert_equal_response(true, response2, idx++, 13); arr = response2.array;
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, response2.array.chunks[3].elements[0].type, idx++, 13);
    assert_equal_float(3.14f, response2.array.chunks[3].elements[0].value.float_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 2 to double 2.718\n", idx);
    generic_array_response_t response3 = set_element_double(arr, 2, 2.718);
    assert_equal_response(true, response3, idx++, 13); arr = response3.array;
    assert_equal_generic_type(GENERIC_TYPE_DOUBLE, response3.array.chunks[3].elements[1].type, idx++, 13);
    assert_equal_double(2.718, response3.array.chunks[3].elements[1].value.double_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 3 to char 'A'\n", idx);
    generic_array_response_t response4 = set_element_char(arr, 3, 'A');
    assert_equal_response(true, response4, idx++, 13); arr = response4.array;
    assert_equal_generic_type(GENERIC_TYPE_CHAR, response4.array.chunks[3].elements[2].type, idx++, 13);
    assert_equal_char('A', response4.array.chunks[3].elements[2].value.char_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 4 to short 32767\n", idx);
    generic_array_response_t response5 = set_element_short(arr, 4, 32767);
    assert_equal_response(true, response5, idx++, 13); arr = response5.array;
    assert_equal_generic_type(GENERIC_TYPE_SHORT, response5.array.chunks[3].elements[3].type, idx++, 13);
    assert_equal_short(32767, response5.array.chunks[3].elements[3].value.short_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 5 to bool true\n", idx);
    generic_array_response_t response6 = set_element_bool(arr, 5, true);
    assert_equal_response(true, response6, idx++, 13); arr = response6.array;
    assert_equal_generic_type(GENERIC_TYPE_BOOL, response6.array.chunks[3].elements[4].type, idx++, 13);
    assert_equal_bool(true, response6.array.chunks[3].elements[4].value.bool_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 6 to string \"hello\"\n", idx);
    generic_array_response_t response7 = set_element_string(arr, 6, "hello");
    assert_equal_response(true, response7, idx++, 13); arr = response7.array;
    assert_equal_generic_type(GENERIC_TYPE_STRING, response7.array.chunks[4].elements[0].type, idx++, 13);
    assert_equal_string("hello", response7.array.chunks[4].elements[0].value.string_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 7 to int -100\n", idx);
    generic_array_response_t response8 = set_element_int(arr, 7, -100);
    assert_equal_response(true, response8, idx++, 13); arr = response8.array;
    assert_equal_generic_type(GENERIC_TYPE_INT, response8.array.chunks[4].elements[1].type, idx++, 13);
    assert_equal_int(-100, response8.array.chunks[4].elements[1].value.int_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 8 to float -2.5f\n", idx);
    generic_array_response_t response9 = set_element_float(arr, 8, -2.5f);
    assert_equal_response(true, response9, idx++, 13); arr = response9.array;
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, response9.array.chunks[4].elements[2].type, idx++, 13);
    assert_equal_float(-2.5f, response9.array.chunks[4].elements[2].value.float_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 9 to char 'z'\n", idx);
    generic_array_response_t response10 = set_element_char(arr, 9, 'z');
    assert_equal_response(true, response10, idx++, 13); arr = response10.array;
    assert_equal_generic_type(GENERIC_TYPE_CHAR, response10.array.chunks[4].elements[3].type, idx++, 13);
    assert_equal_char('z', response10.array.chunks[4].elements[3].value.char_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 10 to short -32768\n", idx);
    generic_array_response_t response11 = set_element_short(arr, 10, -32768);
    assert_equal_response(true, response11, idx++, 13); arr = response11.array;
    assert_equal_generic_type(GENERIC_TYPE_SHORT, response11.array.chunks[5].elements[0].type, idx++, 13);
    assert_equal_short(-32768, response11.array.chunks[5].elements[0].value.short_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 11 to bool false\n", idx);
    generic_array_response_t response12 = set_element_bool(arr, 11, false);
    assert_equal_response(true, response12, idx++, 13); arr = response12.array;
    assert_equal_generic_type(GENERIC_TYPE_BOOL, response12.array.chunks[5].elements[1].type, idx++, 13);
    assert_equal_bool(false, response12.array.chunks[5].elements[1].value.bool_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 12 to string \"world\"\n", idx);
    generic_array_response_t response13 = set_element_string(arr, 12, "world");
    assert_equal_response(true, response13, idx++, 13); arr = response13.array;
    assert_equal_generic_type(GENERIC_TYPE_STRING, response13.array.chunks[5].elements[2].type, idx++, 13);
    assert_equal_string("world", response13.array.chunks[5].elements[2].value.string_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 13 to int 999\n", idx);
    generic_array_response_t response14 = set_element_int(arr, 13, 999);
    assert_equal_response(true, response14, idx++, 13); arr = response14.array;
    assert_equal_generic_type(GENERIC_TYPE_INT, response14.array.chunks[6].elements[0].type, idx++, 13);
    assert_equal_int(999, response14.array.chunks[6].elements[0].value.int_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 14 to float 0.0f\n", idx);
    generic_array_response_t response15 = set_element_float(arr, 14, 0.0f);
    assert_equal_response(true, response15, idx++, 13); arr = response15.array;
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, response15.array.chunks[6].elements[1].type, idx++, 13);
    assert_equal_float(0.0f, response15.array.chunks[6].elements[1].value.float_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 16 to char '0'\n", idx);
    generic_array_response_t response17 = set_element_char(arr, 16, '0');
    assert_equal_response(true, response17, idx++, 13); arr = response17.array;
    assert_equal_generic_type(GENERIC_TYPE_CHAR, response17.array.chunks[6].elements[3].type, idx++, 13);
    assert_equal_char('0', response17.array.chunks[6].elements[3].value.char_value, idx++, 13);
    
    printf("\nTest No.%d:\nSetting element at index 19 to string \"test\"\n", idx);
    generic_array_response_t response20 = set_element_string(arr, 19, "test");
    assert_equal_response(true, response20, idx++, 13);  arr = response20.array;
    assert_equal_generic_type(GENERIC_TYPE_STRING, response20.array.chunks[6].elements[6].type, idx++, 13);
    assert_equal_string("test", response20.array.chunks[6].elements[6].value.string_value, idx++, 13);
    
    // Test element in the last chunk
    printf("\nTest No.%d:\nSetting element at index 28 to int 777\n", idx);
    generic_array_response_t response21 = set_element_int(arr, 28, 777);
    assert_equal_response(true, response21, idx++, 13); arr = response21.array;
    assert_equal_generic_type(GENERIC_TYPE_INT, response21.array.chunks[8].elements[0].type, idx++, 13);
    assert_equal_int(777, response21.array.chunks[8].elements[0].value.int_value, idx++, 13);
    
    // Test invalid indices (out of bounds)
    printf("\nTest No.%d:\nSetting element at index 29 (out of bounds) to int 100\n", idx);
    generic_array_response_t response22 = set_element_int(arr, 29, 100);
    assert_equal_response(false, response22, idx++, 13); arr = response22.array;
    
    printf("\nTest No.%d:\nSetting element at index 100 (out of bounds) to string \"fail\"\n", idx);
    generic_array_response_t response23 = set_element_string(arr, 100, "fail");
    assert_equal_response(false, response23, idx++, 13); arr = response23.array;
    


    // Test getting elements back
    printf("\nTest No.%d:\nGetting element at index 0 (should be int 42)\n", idx);
    generic_element_t elem0 = get_element(arr, 0);
    assert_equal_generic_type(GENERIC_TYPE_INT, elem0.type, idx++, 13);
    assert_equal_int(42, elem0.value.int_value, idx++, 13);
    
    printf("\nTest No.%d:\nGetting element at index 6 (should be string \"hello\")\n", idx);
    generic_element_t elem6 = get_element(arr, 6);
    assert_equal_generic_type(GENERIC_TYPE_STRING, elem6.type, idx++, 13);
    assert_equal_string("hello", elem6.value.string_value, idx++, 13);
    
    printf("\nTest No.%d:\nGetting element at index 28 (should be int 777)\n", idx);
    generic_element_t elem20 = get_element(arr, 28);
    assert_equal_generic_type(GENERIC_TYPE_INT, elem20.type, idx++, 13);
    assert_equal_int(777, elem20.value.int_value, idx++, 13);

    printf("\nTest No.%d:\nGetting element at index 18 (should be VOID)\n", idx);
    generic_element_t elem21 = get_element(arr, 18);
    assert_equal_generic_type(GENERIC_TYPE_VOID, elem21.type, idx++, 13);
    


    // Test overwriting elements
    printf("\nTest No.%d:\nOverwriting element at index 0 with float 1.5f\n", idx);
    generic_array_response_t response24 = set_element_float(arr, 0, 1.5f);
    assert_equal_response(true, response24, idx++, 13); arr = response24.array;
    assert_equal_generic_type(GENERIC_TYPE_FLOAT, response24.array.chunks[0].elements[0].type, idx++, 13);
    assert_equal_float(1.5f, response24.array.chunks[0].elements[0].value.float_value, idx++, 13);
    
    printf("\nTest No.%d:\nOverwriting element at index 6 with int 888\n", idx);
    generic_array_response_t response25 = set_element_int(arr, 6, 888);
    assert_equal_response(true, response25, idx++, 13); arr = response25.array;
    assert_equal_generic_type(GENERIC_TYPE_INT, response25.array.chunks[4].elements[0].type, idx++, 13);
    assert_equal_int(888, response25.array.chunks[4].elements[0].value.int_value, idx++, 13);
    
    // Test is_equal_element function
    printf("\nTest No.%d:\nTesting is_equal_element with two identical int elements\n", idx);
    generic_element_t elem1 = create_generic_element(true);
    elem1.type = GENERIC_TYPE_INT;
    elem1.value.int_value = 42;
    
    generic_element_t elem2 = create_generic_element(true);
    elem2.type = GENERIC_TYPE_INT;
    elem2.value.int_value = 42;
    
    assert_equal_bool(true, is_equal_element(elem1, elem2), idx++, 13);
    
    printf("\nTest No.%d:\nTesting is_equal_element with different int elements\n", idx);
    elem2.value.int_value = 43;
    assert_equal_bool(false, is_equal_element(elem1, elem2), idx++, 13);
    
    // Test is_equal function with arrays
    printf("\nTest No.%d:\nCreating identical array for comparison\n", idx);
    int chunk_capacities2[] = {10, 19};
    generic_array_t arr2 = create_generic_array(2, chunk_capacities2, 0);
    
    // Set some elements in arr2 to match arr
    generic_array_response_t resp;
    resp = set_element_float(arr2, 0, 1.5f); arr2 = resp.array;
    resp = set_element_float(arr2, 1, 3.14f); arr2 = resp.array;
    resp = set_element_double(arr2, 2, 2.718); arr2 = resp.array;
    resp = set_element_char(arr2, 3, 'A'); arr2 = resp.array;
    resp = set_element_short(arr2, 4, 32767); arr2 = resp.array;
    resp = set_element_bool(arr2, 5, true); arr2 = resp.array;
    resp = set_element_int(arr2, 6, 888); arr2 = resp.array;
    resp = set_element_int(arr2, 7, -100); arr2 = resp.array;
    resp = set_element_float(arr2, 8, -2.5f); arr2 = resp.array;
    resp = set_element_char(arr2, 9, 'z'); arr2 = resp.array;
    resp = set_element_short(arr2, 10, -32768); arr2 = resp.array;
    resp = set_element_bool(arr2, 11, false); arr2 = resp.array;
    resp = set_element_string(arr2, 12, "world"); arr2 = resp.array;
    resp = set_element_int(arr2, 13, 999); arr2 = resp.array;
    resp = set_element_float(arr2, 14, 0.0f); arr2 = resp.array;
    resp = set_element_char(arr2, 16, '0'); arr2 = resp.array;
    resp = set_element_string(arr2, 19, "test"); arr2 = resp.array;
    resp = set_element_int(arr2, 28, 777); arr2 = resp.array;
    
    assert_equal_bool(true, is_equal(arr, arr2), idx++, 13);
    
    // Modify one element in arr2
    printf("\nTest No.%d:\nModifying element in arr2 and comparing\n", idx);
    resp = set_element_int(arr2, 0, 999); arr2 = resp.array;
    assert_equal_bool(false, is_equal(arr, arr2), idx++, 13);
    
    printf("\n");
}


// Main function calls all the test functions and summarizes the results.
int main(){
    test_create_generic_element();
    test_is_equal_element();
    test_create_generic_array_chunk();
    test_create_generic_array();
    test_create_generic_array_with_elements();
    test_get_element();
    test_set_element_int();
    test_set_element_float();
    test_set_element_double();
    test_set_element_char();
    test_set_element_short();
    test_set_element_bool();
    test_set_element_string();
    test_is_equal();
    test_full_scope();

    printf("-------------------Summary-------------------\n");
    printf("Total tests: %d\n", passed + failed);
    printf("Total tests passed: %d\n", passed);
    printf("Total tests failed: %d\n", failed);
    printf("--------------------------------\n");
    int num_function_passed = 0;
    float total_points = 0;
    for (int i = 0; i < 15; i++){
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
    printf("points for correctness: %f/23, the other 2 points are for comments and git commits, please read the document for more details.\n", total_points);
    return 0;
}
