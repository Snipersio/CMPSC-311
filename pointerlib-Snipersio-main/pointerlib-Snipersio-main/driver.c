#include <stdio.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <setjmp.h>
#include <limits.h>
#include "pointerlib.h"

int passed = 0;
int failed = 0;
char * function_name [13] = {"swap_int_values", "create_node", "destroy_node", "link_nodes", 
                           "list_length", "append_new_node", "list_statistics", "list_find", 
                           "list_get_index", "list_remove_index", "list_insert_index", "list_insert_index_hard", "full_scope"};// Function pointer type for test functions
float function_points [13] = {1, 2, 1, 1, 2, 3, 3, 2, 3, 5, 7, 7, 9};
int failure_count [13]     = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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

This file is used to test the correctness of your implementation of the functions in pointerlib.c.

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

void assert_equal_double(double expected, double actual, int test_num, int function_index){
    if (fabs(expected - actual) < 0.000001){
        printf("Test No.%d passed\n", test_num);
        passed++;
    } else {
        printf("[ERROR] Test No.%d failed. Expected %lf, but got %lf.\n", test_num, expected, actual);
        failure_count[function_index]++;
        failed++;
    }
}

void assert_equal_pointer(list_node_t* expected, list_node_t* actual, int test_num, int function_index){
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

// Helper function to reset NODE_LIST for clean testing
void reset_node_list() {
    for (int i = 0; i < MAX_NODE_LIST_SIZE; i++) {
        NODE_LIST[i].data = 0;
        NODE_LIST[i].used = false;
        NODE_LIST[i].next = NULL;
    }
}

// Helper function to count used nodes
int count_used_nodes() {
    int count = 0;
    for (int i = 0; i < MAX_NODE_LIST_SIZE; i++) {
        if (NODE_LIST[i].used) {
            count++;
        }
    }
    return count;
}

// Helper function to print list for debugging
void print_list(list_node_t* head, const char* name) {
    printf("%s: ", name);
    if (head == NULL) {
        printf("(empty)");
    } else {
        printf("[");
        list_node_t* current = head;
        while (current != NULL) {
            printf("%d", current->data);
            if (current->next != NULL) {
                printf("]->[");
            }
            current = current->next;
        }
        printf("]");
    }
    printf("\n");
}

// Test functions
void test_swap_int_values(){
    printf("--------------------------------\n");
    printf("Testing swap_int_values...\n\n");

    int idx = 1;
    
    // Test normal swap
    int x = 5, y = 10;
    printf("Test No.%d:\nSwapping x=5, y=10\n", idx);
    int result = swap_int_values(&x, &y);
    assert_equal_int(0, result, idx++, 0);
    assert_equal_int(10, x, idx++, 0);
    assert_equal_int(5, y, idx++, 0);
    
    // Test swap with same values
    x = 7; y = 7;
    printf("\nTest No.%d:\nSwapping x=7, y=7 (same values)\n", idx);
    result = swap_int_values(&x, &y);
    assert_equal_int(0, result, idx++, 0);
    assert_equal_int(7, x, idx++, 0);
    assert_equal_int(7, y, idx++, 0);
    
    // Test swap with negative values
    x = -3; y = 8;
    printf("\nTest No.%d:\nSwapping x=-3, y=8\n", idx);
    result = swap_int_values(&x, &y);
    assert_equal_int(0, result, idx++, 0);
    assert_equal_int(8, x, idx++, 0);
    assert_equal_int(-3, y, idx++, 0);
    
    // Test with NULL pointer (first parameter)
    printf("\nTest No.%d:\nSwapping with NULL first pointer\n", idx);
    result = swap_int_values(NULL, &y);
    assert_equal_int(-1, result, idx++, 0);
    assert_equal_int(-3, y, idx++, 0); // y should be unchanged
    
    // Test with NULL pointer (second parameter)
    printf("\nTest No.%d:\nSwapping with NULL second pointer\n", idx);
    result = swap_int_values(&x, NULL);
    assert_equal_int(-1, result, idx++, 0);
    assert_equal_int(8, x, idx++, 0); // x should be unchanged
    
    // Test with both NULL pointers
    printf("\nTest No.%d:\nSwapping with both NULL pointers\n", idx);
    result = swap_int_values(NULL, NULL);
    assert_equal_int(-1, result, idx++, 0);
    
    printf("\n");
}

void test_create_node(){
    printf("--------------------------------\n");
    printf("Testing create_node...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test creating first node
    printf("Test No.%d:\nCreating node with data=42\n", idx);
    list_node_t* node1 = create_node(42);
    assert_equal_pointer(&NODE_LIST[0], node1, idx++, 1);
    assert_equal_int(42, node1->data, idx++, 1);
    assert_equal_bool(true, node1->used, idx++, 1);
    assert_equal_pointer(NULL, node1->next, idx++, 1);
    
    // Test creating second node
    printf("\nTest No.%d:\nCreating node with data=0\n", idx);
    list_node_t* node2 = create_node(0);
    assert_equal_pointer(&NODE_LIST[1], node2, idx++, 1);
    assert_equal_int(0, node2->data, idx++, 1);
    assert_equal_bool(true, node2->used, idx++, 1);
    assert_equal_pointer(NULL, node2->next, idx++, 1);
    
    // Set first node to unused and test creating node
    node1->used = false;
    printf("\nTest No.%d:\nCreating node with data=-5\n", idx);
    list_node_t* node3 = create_node(-5);
    assert_equal_pointer(&NODE_LIST[0], node3, idx++, 1);
    assert_equal_int(-5, node3->data, idx++, 1);
    assert_equal_bool(true, node3->used, idx++, 1);
    assert_equal_pointer(NULL, node3->next, idx++, 1);
    
    // Test creating multiple nodes to fill up most of the array
    printf("\nTest No.%d:\nCreating nodes to fill up array\n", idx);
    list_node_t* nodes[MAX_NODE_LIST_SIZE - 3]; // Leave room for the 2 nodes already created plus one more
    for (int i = 0; i < MAX_NODE_LIST_SIZE - 3; i++) {
        nodes[i] = create_node(i + 100);
        assert_equal_pointer(&NODE_LIST[i + 2], nodes[i], idx++, 1);
        assert_equal_int(i + 100, nodes[i]->data, idx++, 1);
        assert_equal_bool(true, nodes[i]->used, idx++, 1);
        assert_equal_pointer(NULL, nodes[i]->next, idx++, 1);
    }
    
    // Test creating the last possible node
    printf("\nTest No.%d:\nCreating the last possible node\n", idx);
    list_node_t* last_node = create_node(999);
    assert_equal_pointer(&NODE_LIST[MAX_NODE_LIST_SIZE - 1], last_node, idx++, 1);
    assert_equal_int(999, last_node->data, idx++, 1);
    assert_equal_bool(true, last_node->used, idx++, 1);
    assert_equal_pointer(NULL, last_node->next, idx++, 1);
    
    // Test creating one more node (should fail)
    printf("\nTest No.%d:\nCreating node when array is full\n", idx);
    list_node_t* fail_node1 = create_node(1000);
    assert_equal_pointer(NULL, fail_node1, idx++, 1);

    // Set random node to unused and test creating node
    nodes[12]->used = false;
    printf("\nTest No.%d:\nCreating node with data=123\n", idx);
    list_node_t* node4 = create_node(123);
    assert_equal_pointer(&NODE_LIST[12+2], node4, idx++, 1);
    assert_equal_int(123, node4->data, idx++, 1);
    assert_equal_bool(true, node4->used, idx++, 1);
    assert_equal_pointer(NULL, node4->next, idx++, 1);

    // Test creating one more node (should fail)
    printf("\nTest No.%d:\nCreating node when array is full\n", idx);
    list_node_t* fail_node2 = create_node(1000);
    assert_equal_pointer(NULL, fail_node2, idx++, 1);
    
    printf("\n");
}

void test_destroy_node(){
    printf("--------------------------------\n");
    printf("Testing destroy_node...\n\n");

    int idx = 1;
    
    // Create a test node
    list_node_t node2 = {.data = 2, .used = true, .next = NULL};
    list_node_t node1 = {.data = 1, .used = true, .next = &node2};
    
    // Test destroying a valid node
    printf("Test No.%d:\nDestroying valid node\n", idx);
    int result1 = destroy_node(&node2);
    assert_equal_int(0, result1, idx++, 2);
    assert_equal_int(0, node2.data, idx++, 2);
    assert_equal_bool(false, node2.used, idx++, 2);
    assert_equal_pointer(NULL, node2.next, idx++, 2);

    // Test destroying a valid node
    printf("Test No.%d:\nDestroying valid node\n", idx);
    int result2 = destroy_node(&node1);
    assert_equal_int(0, result2, idx++, 2);
    assert_equal_int(0, node1.data, idx++, 2);
    assert_equal_bool(false, node1.used, idx++, 2);
    assert_equal_pointer(NULL, node1.next, idx++, 2);
    
    // Test destroying NULL node
    printf("\nTest No.%d:\nDestroying NULL node\n", idx);
    int result3 = destroy_node(NULL);
    assert_equal_int(-1, result3, idx++, 2);
    
    // Test destroying already destroyed node
    printf("\nTest No.%d:\nDestroying already destroyed node\n", idx);
    int result4 = destroy_node(&node1);
    assert_equal_int(0, result4, idx++, 2); // Should still succeed
    
    printf("\n");
}

void test_link_nodes(){
    printf("--------------------------------\n");
    printf("Testing link_nodes...\n\n");

    int idx = 1;
    
    // Create test nodes manually
    list_node_t node1 = {.data = 1, .used = true, .next = NULL};
    list_node_t node2 = {.data = 2, .used = true, .next = NULL};
    list_node_t node3 = {.data = 3, .used = true, .next = NULL};
    
    // Test linking two valid nodes
    printf("Test No.%d:\nLinking two valid nodes\n", idx);
    int result1 = link_nodes(&node1, &node2);
    assert_equal_int(0, result1, idx++, 3);
    assert_equal_pointer(&node2, node1.next, idx++, 3);
    
    // Test linking node to NULL
    printf("\nTest No.%d:\nLinking node to NULL\n", idx);
    int result2 = link_nodes(&node2, NULL);
    assert_equal_int(0, result2, idx++, 3);
    assert_equal_pointer(NULL, node2.next, idx++, 3);
    
    // Test linking NULL to valid node (should fail)
    printf("\nTest No.%d:\nLinking NULL to valid node\n", idx);
    int result3 = link_nodes(NULL, &node3);
    assert_equal_int(-1, result3, idx++, 3);
    
    // Test linking NULL to NULL (should fail)
    printf("\nTest No.%d:\nLinking NULL to NULL\n", idx);
    int result4 = link_nodes(NULL, NULL);
    assert_equal_int(-1, result4, idx++, 3);
    
    // Test creating a chain
    printf("\nTest No.%d:\nCreating a chain of nodes\n", idx);
    int result5 = link_nodes(&node1, &node2);
    assert_equal_int(0, result5, idx++, 3);
    int result6 = link_nodes(&node2, &node3);
    assert_equal_int(0, result6, idx++, 3);
    assert_equal_pointer(&node2, node1.next, idx++, 3);
    assert_equal_pointer(&node3, node2.next, idx++, 3);
    assert_equal_pointer(NULL, node3.next, idx++, 3);
    
    printf("\n");
}

void test_list_length(){
    printf("--------------------------------\n");
    printf("Testing list_length...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test empty list
    printf("Test No.%d:\nEmpty list\n", idx);
    int result = list_length(NULL);
    assert_equal_int(0, result, idx++, 4);
    
    // Test single node
    printf("\nTest No.%d:\nSingle node\n", idx);
    list_node_t* single = create_node(5);
    result = list_length(single);
    assert_equal_int(1, result, idx++, 4);
    
    // Test two nodes
    printf("\nTest No.%d:\nTwo nodes\n", idx);
    list_node_t* two = create_node(1);
    two->next = create_node(2);
    result = list_length(two);
    assert_equal_int(2, result, idx++, 4);
    
    // Test three nodes
    printf("\nTest No.%d:\nThree nodes\n", idx);
    list_node_t* three = create_node(1);
    three->next = create_node(2);
    three->next->next = create_node(3);
    result = list_length(three);
    assert_equal_int(3, result, idx++, 4);
    
    // Test four nodes
    printf("\nTest No.%d:\nFour nodes\n", idx);
    list_node_t* four = create_node(10);
    list_node_t* current = four;
    for (int i = 20; i <= 40; i += 10) {
        current->next = create_node(i);
        current = current->next;
    }
    result = list_length(four);
    assert_equal_int(4, result, idx++, 4);
    
    printf("\n");
}

void test_append_new_node(){
    printf("--------------------------------\n");
    printf("Testing append_new_node...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test appending to NULL list (should fail)
    printf("Test No.%d:\nAppending to NULL list\n", idx);
    int used_before = count_used_nodes();
    int result = append_new_node(NULL, 42);
    int used_after = count_used_nodes();
    assert_equal_int(-1, result, idx++, 5);
    assert_equal_int(0, used_after, idx++, 5); // No nodes should be used
    
    // Test appending to single node list
    printf("\nTest No.%d:\nAppending to single node list\n", idx);
    list_node_t* head = create_node(0);
    used_before = count_used_nodes();
    result = append_new_node(head, 42);
    used_after = count_used_nodes();
    assert_equal_int(0, result, idx++, 5);
    assert_equal_int(1, used_before, idx++, 5);
    assert_equal_int(2, used_after, idx++, 5); // One more node should be used
    assert_equal_int(0, head->data, idx++, 5);
    assert_equal_int(42, head->next->data, idx++, 5);
    assert_equal_bool(true, head->next->used, idx++, 5); // New node should be marked as used
    assert_equal_pointer(NULL, head->next->next, idx++, 5);
    
    // Test appending to two node list
    printf("\nTest No.%d:\nAppending to two node list\n", idx);
    result = append_new_node(head, 3);
    assert_equal_int(0, result, idx++, 5);
    assert_equal_int(3, count_used_nodes(), idx++, 5); // One more node should be used
    assert_equal_int(0, head->data, idx++, 5);
    assert_equal_int(42, head->next->data, idx++, 5);
    assert_equal_int(3, head->next->next->data, idx++, 5);
    assert_equal_bool(true, head->next->next->used, idx++, 5); // New node should be marked as used
    assert_equal_pointer(NULL, head->next->next->next, idx++, 5);
    
    // Test appending negative value
    printf("\nTest No.%d:\nAppending negative value\n", idx);
    result = append_new_node(head, -5);
    assert_equal_int(0, result, idx++, 5);
    assert_equal_int(4, count_used_nodes(), idx++, 5); // One more node should be used
    assert_equal_int(-5, head->next->next->next->data, idx++, 5);
    assert_equal_bool(true, head->next->next->next->used, idx++, 5); // New node should be marked as used
    assert_equal_pointer(NULL, head->next->next->next->next, idx++, 5);
    
    // Test filling up the array and then trying to append (should fail)
    printf("\nTest No.%d:\nFilling up array and trying to append\n", idx);
    
    // Fill up the remaining nodes in the array
    list_node_t* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    // Create nodes to fill up the array
    int nodes_created = 0;
    while (count_used_nodes() < MAX_NODE_LIST_SIZE - 1) { // Leave one node unused for testing
        list_node_t* new_node = create_node(100 + nodes_created);
        if (new_node != NULL) {
            current->next = new_node;
            current = current->next;
            nodes_created++;
        } else {
            break;
        }
    }
    assert_equal_int(MAX_NODE_LIST_SIZE - 1, count_used_nodes(), idx++, 5);
    
    // Now try to append when array is almost full
    result = append_new_node(head, 999);
    assert_equal_int(0, result, idx++, 5); // Should succeed since we left one node unused
    assert_equal_int(MAX_NODE_LIST_SIZE, count_used_nodes(), idx++, 5);
    
    // Now try to append when array is completely full (should fail)
    printf("\nTest No.%d:\nTrying to append no unused nodes\n", idx);
    result = append_new_node(head, 777);
    assert_equal_int(-1, result, idx++, 5); // Should fail - no unused nodes
    assert_equal_int(MAX_NODE_LIST_SIZE, count_used_nodes(), idx++, 5); // No change in used nodes
    
    printf("\n");
}

void test_list_statistics(){
    printf("--------------------------------\n");
    printf("Testing list_statistics...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test empty list
    printf("Test No.%d:\nEmpty list\n", idx);
    double avg;
    int min, max, sum;
    int result = list_statistics(NULL, &avg, &min, &max, &sum);
    assert_equal_int(-1, result, idx++, 6);
    
    // Test single node
    printf("\nTest No.%d:\nSingle node [5]\n", idx);
    list_node_t* single = create_node(5);
    result = list_statistics(single, &avg, &min, &max, &sum);
    assert_equal_int(0, result, idx++, 6);
    assert_equal_double(5.0, avg, idx++, 6);
    assert_equal_int(5, min, idx++, 6);
    assert_equal_int(5, max, idx++, 6);
    assert_equal_int(5, sum, idx++, 6);

    printf("\nTest No.%d:\nSingle node 2 [5]\n", idx);
    avg = 999999;
    min = INT_MIN;
    max = INT_MAX;
    sum = 999999;
    result = list_statistics(single, &avg, &min, &max, &sum);
    assert_equal_int(0, result, idx++, 6);
    assert_equal_double(5.0, avg, idx++, 6);
    assert_equal_int(5, min, idx++, 6);
    assert_equal_int(5, max, idx++, 6);
    assert_equal_int(5, sum, idx++, 6);
    
    // Test three nodes [1]->[2]->[3]
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3]\n", idx);
    list_node_t* three = create_node(1);
    three->next = create_node(2);
    three->next->next = create_node(3);
    result = list_statistics(three, &avg, &min, &max, &sum);
    assert_equal_int(0, result, idx++, 6);
    assert_equal_double(2.0, avg, idx++, 6);
    assert_equal_int(1, min, idx++, 6);
    assert_equal_int(3, max, idx++, 6);
    assert_equal_int(6, sum, idx++, 6);
    
    // Test with negative numbers [-1]->[2]->[-3]
    printf("\nTest No.%d:\nNegative numbers [-1]->[2]->[-3]\n", idx);
    list_node_t* negative = create_node(-1);
    negative->next = create_node(2);
    negative->next->next = create_node(-3);
    result = list_statistics(negative, &avg, &min, &max, &sum);
    assert_equal_int(0, result, idx++, 6);
    assert_equal_double(-0.666667, avg, idx++, 6);
    assert_equal_int(-3, min, idx++, 6);
    assert_equal_int(2, max, idx++, 6);
    assert_equal_int(-2, sum, idx++, 6);
    
    // Test with zeros [0]->[0]->[0]
    printf("\nTest No.%d:\nAll zeros [0]->[0]->[0]\n", idx);
    list_node_t* zeros = create_node(0);
    zeros->next = create_node(0);
    zeros->next->next = create_node(0);
    result = list_statistics(zeros, &avg, &min, &max, &sum);
    assert_equal_int(0, result, idx++, 6);
    assert_equal_double(0.0, avg, idx++, 6);
    assert_equal_int(0, min, idx++, 6);
    assert_equal_int(0, max, idx++, 6);
    assert_equal_int(0, sum, idx++, 6);

    // Test with same numbers [15]->[15]->[15]
    printf("\nTest No.%d:\nAll same numbers [15]->[15]->[15]\n", idx);
    list_node_t* same = create_node(15);
    same->next = create_node(15);
    same->next->next = create_node(15);
    result = list_statistics(same, &avg, &min, &max, &sum);
    assert_equal_int(0, result, idx++, 6);
    assert_equal_double(15.0, avg, idx++, 6);
    assert_equal_int(15, min, idx++, 6);
    assert_equal_int(15, max, idx++, 6);
    assert_equal_int(45, sum, idx++, 6);
    
    printf("\n");
}

void test_list_find(){
    printf("--------------------------------\n");
    printf("Testing list_find...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test empty list
    printf("Test No.%d:\nEmpty list, target = 5\n", idx);
    list_node_t* result = list_find(NULL, 5);
    assert_equal_pointer(NULL, result, idx++, 7);
    
    // Test single node, found
    printf("\nTest No.%d:\nSingle node [5], target = 5\n", idx);
    list_node_t* single = create_node(5);
    result = list_find(single, 5);
    assert_equal_pointer(single, result, idx++, 7);
    assert_equal_int(5, result->data, idx++, 7);
    
    // Test single node, not found
    printf("\nTest No.%d:\nSingle node [5], target = 3\n", idx);
    result = list_find(single, 3);
    assert_equal_pointer(NULL, result, idx++, 7);
    
    // Test multiple nodes, found first occurrence
    printf("\nTest No.%d:\nMultiple nodes [5]->[5]->[3], target = 5\n", idx);
    list_node_t* multiple = create_node(5);
    multiple->next = create_node(5);
    multiple->next->next = create_node(3);
    result = list_find(multiple, 5);
    assert_equal_pointer(multiple, result, idx++, 7);
    assert_equal_int(5, result->data, idx++, 7);
    
    // Test multiple nodes, found middle
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], target = 2\n", idx);
    list_node_t* middle = create_node(1);
    middle->next = create_node(2);
    middle->next->next = create_node(3);
    result = list_find(middle, 2);
    assert_equal_pointer(middle->next, result, idx++, 7);
    assert_equal_int(2, result->data, idx++, 7);
    
    // Test multiple nodes, not found
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], target = 4\n", idx);
    result = list_find(middle, 4);
    assert_equal_pointer(NULL, result, idx++, 7);
    
    printf("\n");
}

void test_list_get_index(){
    printf("--------------------------------\n");
    printf("Testing list_get_index...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test empty list
    printf("Test No.%d:\nEmpty list, index = 0\n", idx);
    list_node_t* result = list_get_index(NULL, 0);
    assert_equal_pointer(NULL, result, idx++, 8);
    
    // Test single node, index 0
    printf("\nTest No.%d:\nSingle node [5], index = 0\n", idx);
    list_node_t* single = create_node(5);
    result = list_get_index(single, 0);
    assert_equal_pointer(single, result, idx++, 8);
    assert_equal_int(5, result->data, idx++, 8);
    
    // Test single node, index out of bounds
    printf("\nTest No.%d:\nSingle node [5], index = 1\n", idx);
    result = list_get_index(single, 1);
    assert_equal_pointer(NULL, result, idx++, 8);
    
    // Test multiple nodes, valid indices
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = 0\n", idx);
    list_node_t* multiple = create_node(1);
    multiple->next = create_node(2);
    multiple->next->next = create_node(3);
    result = list_get_index(multiple, 0);
    assert_equal_pointer(multiple, result, idx++, 8);
    assert_equal_int(1, result->data, idx++, 8);
    
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = 1\n", idx);
    result = list_get_index(multiple, 1);
    assert_equal_pointer(multiple->next, result, idx++, 8);
    assert_equal_int(2, result->data, idx++, 8);
    
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = 2\n", idx);
    result = list_get_index(multiple, 2);
    assert_equal_pointer(multiple->next->next, result, idx++, 8);
    assert_equal_int(3, result->data, idx++, 8);
    
    // Test multiple nodes, index out of bounds
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = 3\n", idx);
    result = list_get_index(multiple, 3);
    assert_equal_pointer(NULL, result, idx++, 8);
    
    // Test negative index
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = -1\n", idx);
    result = list_get_index(multiple, -1);
    assert_equal_pointer(NULL, result, idx++, 8);
    
    printf("\n");
}

void test_list_remove_index(){
    printf("--------------------------------\n");
    printf("Testing list_remove_index...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test empty list
    printf("Test No.%d:\nEmpty list, index = 0\n", idx);
    list_node_t* result = list_remove_index(NULL, 0);
    assert_equal_pointer(NULL, result, idx++, 9);
    
    // Test single node, index 0 (should return NULL - empty list)
    printf("\nTest No.%d:\nSingle node [5], index = 0\n", idx);
    list_node_t* single = create_node(5);
    list_node_t* removed_node = single; // Save reference to the node that will be removed
    int used_before = count_used_nodes();
    result = list_remove_index(single, 0);
    int used_after = count_used_nodes();
    assert_equal_pointer(NULL, result, idx++, 9);
    assert_equal_int(used_before - 1, used_after, idx++, 9); // One less node should be used
    assert_equal_bool(false, removed_node->used, idx++, 9); // Removed node should be marked as unused
    assert_equal_int(0, removed_node->data, idx++, 9); // Removed node data should be reset
    assert_equal_pointer(NULL, removed_node->next, idx++, 9); // Removed node next should be NULL
    
    // Test single node, index out of bounds
    printf("\nTest No.%d:\nSingle node [5], index = 1\n", idx);
    single = create_node(5); // Recreate since previous was removed
    used_before = count_used_nodes();
    result = list_remove_index(single, 1);
    used_after = count_used_nodes();
    assert_equal_pointer(single, result, idx++, 9); // Should return original list
    assert_equal_int(used_before, used_after, idx++, 9); // No nodes should be destroyed

    // Test multiple nodes, remove first (index 0)
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = 0\n", idx);
    list_node_t* multiple = create_node(1);
    multiple->next = create_node(2);
    multiple->next->next = create_node(3);
    list_node_t* removed_first = multiple; // Save reference to the node that will be removed
    list_node_t* original_second = multiple->next; // Save reference to second node
    used_before = count_used_nodes();
    result = list_remove_index(multiple, 0);
    used_after = count_used_nodes();
    assert_equal_pointer(original_second, result, idx++, 9);
    assert_equal_int(2, result->data, idx++, 9);
    assert_equal_int(3, result->next->data, idx++, 9);
    assert_equal_pointer(NULL, result->next->next, idx++, 9);
    assert_equal_int(used_before - 1, used_after, idx++, 9); // One less node should be used
    assert_equal_bool(false, removed_first->used, idx++, 9); // Removed node should be marked as unused
    assert_equal_int(0, removed_first->data, idx++, 9); // Removed node data should be reset
    assert_equal_pointer(NULL, removed_first->next, idx++, 9); // Removed node next should be NULL
    
    // Test multiple nodes, remove middle (index 1)
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = 1\n", idx);
    multiple = create_node(1);
    multiple->next = create_node(2);
    multiple->next->next = create_node(3);
    list_node_t* original_first = multiple; // Save reference to first node
    list_node_t* removed_middle = multiple->next; // Save reference to the node that will be removed
    list_node_t* original_third = multiple->next->next; // Save reference to third node
    used_before = count_used_nodes();
    result = list_remove_index(multiple, 1);
    used_after = count_used_nodes();
    assert_equal_pointer(original_first, result, idx++, 9);
    assert_equal_int(1, result->data, idx++, 9);
    assert_equal_pointer(original_third, result->next, idx++, 9);
    assert_equal_int(3, result->next->data, idx++, 9);
    assert_equal_pointer(NULL, result->next->next, idx++, 9);
    assert_equal_int(used_before - 1, used_after, idx++, 9); // One less node should be used
    assert_equal_bool(false, removed_middle->used, idx++, 9); // Removed node should be marked as unused
    assert_equal_int(0, removed_middle->data, idx++, 9); // Removed node data should be reset
    assert_equal_pointer(NULL, removed_middle->next, idx++, 9); // Removed node next should be NULL
    
    // Test multiple nodes, remove last (index 2)
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = 2\n", idx);
    multiple = create_node(1);
    multiple->next = create_node(2);
    multiple->next->next = create_node(3);
    original_first = multiple; // Save reference to first node
    list_node_t* original_second_node = multiple->next; // Save reference to second node
    list_node_t* removed_last = multiple->next->next; // Save reference to the node that will be removed
    used_before = count_used_nodes();
    result = list_remove_index(multiple, 2);
    used_after = count_used_nodes();
    assert_equal_pointer(original_first, result, idx++, 9);
    assert_equal_int(1, result->data, idx++, 9);
    assert_equal_pointer(original_second_node, result->next, idx++, 9);
    assert_equal_int(2, result->next->data, idx++, 9);
    assert_equal_pointer(NULL, result->next->next, idx++, 9);
    assert_equal_int(used_before - 1, used_after, idx++, 9); // One less node should be used
    assert_equal_bool(false, removed_last->used, idx++, 9); // Removed node should be marked as unused
    assert_equal_int(0, removed_last->data, idx++, 9); // Removed node data should be reset
    assert_equal_pointer(NULL, removed_last->next, idx++, 9); // Removed node next should be NULL
    
    // Test multiple nodes, index out of bounds
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = 3\n", idx);
    multiple = create_node(1);
    multiple->next = create_node(2);
    multiple->next->next = create_node(3);
    list_node_t* original_list = multiple; // Save reference to original list
    used_before = count_used_nodes();
    result = list_remove_index(multiple, 3);
    used_after = count_used_nodes();
    assert_equal_pointer(original_list, result, idx++, 9); // Should return original list
    assert_equal_int(1, result->data, idx++, 9); // Original data should be unchanged
    assert_equal_int(2, result->next->data, idx++, 9);
    assert_equal_int(3, result->next->next->data, idx++, 9);
    assert_equal_pointer(NULL, result->next->next->next, idx++, 9);
    assert_equal_int(used_before, used_after, idx++, 9); // No nodes should be destroyed
    
    // Test negative index
    printf("\nTest No.%d:\nMultiple nodes [1]->[2]->[3], index = -1\n", idx);
    multiple = create_node(1);
    multiple->next = create_node(2);
    multiple->next->next = create_node(3);
    original_list = multiple; // Save reference to original list
    used_before = count_used_nodes();
    result = list_remove_index(multiple, -1);
    used_after = count_used_nodes();
    assert_equal_pointer(original_list, result, idx++, 9); // Should return original list
    assert_equal_int(1, result->data, idx++, 9); // Original data should be unchanged
    assert_equal_int(2, result->next->data, idx++, 9);
    assert_equal_int(3, result->next->next->data, idx++, 9);
    assert_equal_pointer(NULL, result->next->next->next, idx++, 9);
    assert_equal_int(used_before, used_after, idx++, 9); // No nodes should be destroyed
    
    // Test four nodes, remove second (index 1)
    printf("\nTest No.%d:\nFour nodes [10]->[20]->[30]->[40], index = 1\n", idx);
    list_node_t* four_nodes = create_node(10);
    four_nodes->next = create_node(20);
    four_nodes->next->next = create_node(30);
    four_nodes->next->next->next = create_node(40);
    list_node_t* original_first_four = four_nodes; // Save reference to first node
    list_node_t* original_third_four = four_nodes->next->next; // Save reference to third node
    result = list_remove_index(four_nodes, 1);
    assert_equal_pointer(original_first_four, result, idx++, 9);
    assert_equal_int(10, result->data, idx++, 9);
    assert_equal_pointer(original_third_four, result->next, idx++, 9);
    assert_equal_int(30, result->next->data, idx++, 9);
    assert_equal_int(40, result->next->next->data, idx++, 9);
    assert_equal_pointer(NULL, result->next->next->next, idx++, 9);
    
    printf("\n");
}

void test_list_insert_index(){
    printf("--------------------------------\n");
    printf("Testing list_insert_index...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test inserting into empty list at index 0
    printf("Test No.%d:\nEmpty list, index = 0, data = 3\n", idx);
    int used_before = count_used_nodes();
    list_node_t* result = list_insert_index(NULL, 0, 3);
    int used_after = count_used_nodes();
    assert_equal_pointer(&NODE_LIST[0], result, idx++, 10);
    assert_equal_int(3, result->data, idx++, 10);
    assert_equal_bool(true, result->used, idx++, 10); // New node should be marked as used
    assert_equal_pointer(NULL, result->next, idx++, 10);
    assert_equal_int(used_before + 1, used_after, idx++, 10); // One more node should be used
    
    // Test inserting at beginning of single node list
    printf("\nTest No.%d:\nSingle node [5], index = 0, data = 3\n", idx);
    reset_node_list(); // Reset to get clean indices
    list_node_t* single = create_node(5);
    used_before = count_used_nodes();
    result = list_insert_index(single, 0, 3);
    used_after = count_used_nodes();
    assert_equal_pointer(&NODE_LIST[1], result, idx++, 10); // New node should be at index 1
    assert_equal_int(3, result->data, idx++, 10);
    assert_equal_bool(true, result->used, idx++, 10); // New node should be marked as used
    assert_equal_pointer(single, result->next, idx++, 10);
    assert_equal_int(5, result->next->data, idx++, 10);
    assert_equal_int(used_before + 1, used_after, idx++, 10); // One more node should be used
    
    // Test inserting at end of single node list
    printf("\nTest No.%d:\nSingle node [5], index = 1, data = 3\n", idx);
    reset_node_list(); // Reset to get clean indices
    single = create_node(5); // Recreate since previous was modified
    used_before = count_used_nodes();
    result = list_insert_index(single, 1, 3);
    used_after = count_used_nodes();
    assert_equal_pointer(single, result, idx++, 10); // Head should remain the same
    assert_equal_int(5, result->data, idx++, 10);
    assert_equal_pointer(&NODE_LIST[1], result->next, idx++, 10);
    assert_equal_int(3, result->next->data, idx++, 10);
    assert_equal_bool(true, result->next->used, idx++, 10); // New node should be marked as used
    assert_equal_pointer(NULL, result->next->next, idx++, 10);
    assert_equal_int(used_before + 1, used_after, idx++, 10); // One more node should be used
    
    // Test inserting in middle of three node list
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = 1, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    list_node_t* three = create_node(1);
    three->next = create_node(2);
    three->next->next = create_node(3);
    list_node_t* original_first = three; // Save reference to first node
    list_node_t* original_second = three->next; // Save reference to second node
    used_before = count_used_nodes();
    result = list_insert_index(three, 1, 4);
    used_after = count_used_nodes();
    assert_equal_pointer(original_first, result, idx++, 10);
    assert_equal_int(1, result->data, idx++, 10);
    assert_equal_pointer(&NODE_LIST[3], result->next, idx++, 10); // New node
    assert_equal_int(4, result->next->data, idx++, 10);
    assert_equal_bool(true, result->next->used, idx++, 10); // New node should be marked as used
    assert_equal_pointer(original_second, result->next->next, idx++, 10);
    assert_equal_int(2, result->next->next->data, idx++, 10);
    assert_equal_int(3, result->next->next->next->data, idx++, 10);
    assert_equal_int(used_before + 1, used_after, idx++, 10); // One more node should be used
    
    // Test inserting at beginning of three node list
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = 0, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    three = create_node(1);
    three->next = create_node(2);
    three->next->next = create_node(3);
    list_node_t* original_head = three; // Save reference to original head
    used_before = count_used_nodes();
    result = list_insert_index(three, 0, 4);
    used_after = count_used_nodes();
    assert_equal_pointer(&NODE_LIST[3], result, idx++, 10); // New head
    assert_equal_int(4, result->data, idx++, 10);
    assert_equal_bool(true, result->used, idx++, 10); // New node should be marked as used
    assert_equal_pointer(original_head, result->next, idx++, 10);
    assert_equal_int(1, result->next->data, idx++, 10);
    assert_equal_int(2, result->next->next->data, idx++, 10);
    assert_equal_int(3, result->next->next->next->data, idx++, 10);
    assert_equal_int(used_before + 1, used_after, idx++, 10); // One more node should be used
    
    // Test inserting at end of three node list
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = 3, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    three = create_node(1);
    three->next = create_node(2);
    three->next->next = create_node(3);
    original_head = three; // Save reference to original head
    used_before = count_used_nodes();
    result = list_insert_index(three, 3, 4);
    used_after = count_used_nodes();
    assert_equal_pointer(original_head, result, idx++, 10);
    assert_equal_int(1, result->data, idx++, 10);
    assert_equal_int(2, result->next->data, idx++, 10);
    assert_equal_int(3, result->next->next->data, idx++, 10);
    assert_equal_pointer(&NODE_LIST[3], result->next->next->next, idx++, 10);
    assert_equal_int(4, result->next->next->next->data, idx++, 10);
    assert_equal_bool(true, result->next->next->next->used, idx++, 10); // New node should be marked as used
    assert_equal_pointer(NULL, result->next->next->next->next, idx++, 10);
    assert_equal_int(used_before + 1, used_after, idx++, 10); // One more node should be used
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = 4, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    three = create_node(1);
    three->next = create_node(2);
    three->next->next = create_node(3);
    used_before = count_used_nodes();
    result = list_insert_index(three, 4, 4);
    used_after = count_used_nodes();
    assert_equal_pointer(NULL, result, idx++, 10);
    assert_equal_int(used_before, used_after, idx++, 10); // No nodes should be used
    
    // Test negative index
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = -1, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    three = create_node(1);
    three->next = create_node(2);
    three->next->next = create_node(3);
    used_before = count_used_nodes();
    result = list_insert_index(three, -1, 4);
    used_after = count_used_nodes();
    assert_equal_pointer(NULL, result, idx++, 10);
    assert_equal_int(used_before, used_after, idx++, 10); // No nodes should be used
    
    // Test inserting when all nodes are used (should fail)
    printf("\nTest No.%d:\nAll nodes used, trying to insert\n", idx);
    
    // Fill up all remaining nodes in the array
    list_node_t* current = three;
    while (current->next != NULL) {
        current = current->next;
    }
    
    // Create nodes to fill up the array completely
    int nodes_created = 0;
    while (count_used_nodes() < MAX_NODE_LIST_SIZE) {
        list_node_t* new_node = create_node(100 + nodes_created);
        if (new_node != NULL) {
            current->next = new_node;
            current = current->next;
            nodes_created++;
        } else {
            break;
        }
    }
    
    // Verify all nodes are now used
    assert_equal_int(MAX_NODE_LIST_SIZE, count_used_nodes(), idx++, 10);
    
    // Now try to insert when array is completely full (should fail)
    used_before = count_used_nodes();
    result = list_insert_index(three, 1, 777);
    used_after = count_used_nodes();
    assert_equal_pointer(NULL, result, idx++, 10); // Should fail - no unused nodes
    assert_equal_int(used_before, used_after, idx++, 10); // No change in used nodes
    
    printf("\n");
}

void test_list_insert_index_hard(){
    printf("--------------------------------\n");
    printf("Testing list_insert_index_hard...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // Test inserting into empty list at index 0
    printf("Test No.%d:\nEmpty list, index = 0, data = 3\n", idx);
    list_node_t* head = NULL;
    int used_before = count_used_nodes();
    int result = list_insert_index_hard(&head, 0, 3);
    int used_after = count_used_nodes();
    assert_equal_int(0, result, idx++, 11);
    assert_equal_pointer(&NODE_LIST[0], head, idx++, 11);
    assert_equal_int(3, head->data, idx++, 11);
    assert_equal_bool(true, head->used, idx++, 11); // New node should be marked as used
    assert_equal_pointer(NULL, head->next, idx++, 11);
    assert_equal_int(used_before + 1, used_after, idx++, 11); // One more node should be used
    
    // Test inserting at beginning of single node list
    printf("\nTest No.%d:\nSingle node [5], index = 0, data = 3\n", idx);
    reset_node_list(); // Reset to get clean indices
    head = create_node(5);
    list_node_t* original_head = head; // Save reference to original head
    used_before = count_used_nodes();
    result = list_insert_index_hard(&head, 0, 3);
    used_after = count_used_nodes();
    assert_equal_int(0, result, idx++, 11);
    assert_equal_pointer(&NODE_LIST[1], head, idx++, 11); // Head should point to new node
    assert_equal_int(3, head->data, idx++, 11);
    assert_equal_bool(true, head->used, idx++, 11); // New node should be marked as used
    assert_equal_pointer(original_head, head->next, idx++, 11);
    assert_equal_int(5, head->next->data, idx++, 11);
    assert_equal_int(used_before + 1, used_after, idx++, 11); // One more node should be used
    
    // Test inserting at end of single node list
    printf("\nTest No.%d:\nSingle node [5], index = 1, data = 3\n", idx);
    reset_node_list(); // Reset to get clean indices
    head = create_node(5); // Recreate since previous was modified
    list_node_t* original_head_end = head; // Save reference to original head
    used_before = count_used_nodes();
    result = list_insert_index_hard(&head, 1, 3);
    used_after = count_used_nodes();
    assert_equal_int(0, result, idx++, 11);
    assert_equal_pointer(original_head_end, head, idx++, 11); // Head should remain the same
    assert_equal_int(5, head->data, idx++, 11);
    assert_equal_pointer(&NODE_LIST[1], head->next, idx++, 11);
    assert_equal_int(3, head->next->data, idx++, 11);
    assert_equal_bool(true, head->next->used, idx++, 11); // New node should be marked as used
    assert_equal_pointer(NULL, head->next->next, idx++, 11);
    assert_equal_int(used_before + 1, used_after, idx++, 11); // One more node should be used
    
    // Test inserting in middle of three node list
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = 1, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    list_node_t* original_head_middle = head; // Save reference to original head
    list_node_t* original_second = head->next; // Save reference to second node
    used_before = count_used_nodes();
    result = list_insert_index_hard(&head, 1, 4);
    used_after = count_used_nodes();
    assert_equal_int(0, result, idx++, 11);
    assert_equal_pointer(original_head_middle, head, idx++, 11); // Head should remain the same
    assert_equal_int(1, head->data, idx++, 11);
    assert_equal_pointer(&NODE_LIST[3], head->next, idx++, 11); // New node
    assert_equal_int(4, head->next->data, idx++, 11);
    assert_equal_bool(true, head->next->used, idx++, 11); // New node should be marked as used
    assert_equal_pointer(original_second, head->next->next, idx++, 11);
    assert_equal_int(2, head->next->next->data, idx++, 11);
    assert_equal_int(3, head->next->next->next->data, idx++, 11);
    assert_equal_int(used_before + 1, used_after, idx++, 11); // One more node should be used
    
    // Test inserting at beginning of three node list
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = 0, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    list_node_t* original_head_begin = head; // Save reference to original head
    used_before = count_used_nodes();
    result = list_insert_index_hard(&head, 0, 4);
    used_after = count_used_nodes();
    assert_equal_int(0, result, idx++, 11);
    assert_equal_pointer(&NODE_LIST[3], head, idx++, 11); // Head should point to new node
    assert_equal_int(4, head->data, idx++, 11);
    assert_equal_bool(true, head->used, idx++, 11); // New node should be marked as used
    assert_equal_pointer(original_head_begin, head->next, idx++, 11);
    assert_equal_int(1, head->next->data, idx++, 11);
    assert_equal_int(2, head->next->next->data, idx++, 11);
    assert_equal_int(3, head->next->next->next->data, idx++, 11);
    assert_equal_int(used_before + 1, used_after, idx++, 11); // One more node should be used
    
    // Test inserting at end of three node list
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = 3, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    list_node_t* original_head_end_three = head; // Save reference to original head
    used_before = count_used_nodes();
    result = list_insert_index_hard(&head, 3, 4);
    used_after = count_used_nodes();
    assert_equal_int(0, result, idx++, 11);
    assert_equal_pointer(original_head_end_three, head, idx++, 11); // Head should remain the same
    assert_equal_int(1, head->data, idx++, 11);
    assert_equal_int(2, head->next->data, idx++, 11);
    assert_equal_int(3, head->next->next->data, idx++, 11);
    assert_equal_pointer(&NODE_LIST[3], head->next->next->next, idx++, 11);
    assert_equal_int(4, head->next->next->next->data, idx++, 11);
    assert_equal_bool(true, head->next->next->next->used, idx++, 11); // New node should be marked as used
    assert_equal_pointer(NULL, head->next->next->next->next, idx++, 11);
    assert_equal_int(used_before + 1, used_after, idx++, 11); // One more node should be used
    
    // Test invalid index (out of bounds)
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = 4, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    used_before = count_used_nodes();
    result = list_insert_index_hard(&head, 4, 4);
    used_after = count_used_nodes();
    assert_equal_int(-1, result, idx++, 11);
    assert_equal_int(used_before, used_after, idx++, 11); // No nodes should be used
    
    // Test negative index
    printf("\nTest No.%d:\nThree nodes [1]->[2]->[3], index = -1, data = 4\n", idx);
    reset_node_list(); // Reset to get clean indices
    head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    used_before = count_used_nodes();
    result = list_insert_index_hard(&head, -1, 4);
    used_after = count_used_nodes();
    assert_equal_int(-1, result, idx++, 11);
    assert_equal_int(used_before, used_after, idx++, 11); // No nodes should be used
    
    // Test with NULL head pointer
    printf("\nTest No.%d:\nNULL head pointer\n", idx);
    head = NULL;
    used_before = count_used_nodes();
    result = list_insert_index_hard(NULL, 0, 4);
    used_after = count_used_nodes();
    assert_equal_int(-1, result, idx++, 11);
    assert_equal_int(used_before, used_after, idx++, 11); // No nodes should be used
    
    // Test inserting when all nodes are used (should fail)
    printf("\nTest No.%d:\nAll nodes used, trying to insert\n", idx);
    reset_node_list(); // Reset to get clean indices
    
    // Fill up all remaining nodes in the array
    head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    list_node_t* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    // Create nodes to fill up the array completely
    int nodes_created = 0;
    while (count_used_nodes() < MAX_NODE_LIST_SIZE) {
        list_node_t* new_node = create_node(100 + nodes_created);
        if (new_node != NULL) {
            current->next = new_node;
            current = current->next;
            nodes_created++;
        } else {
            break;
        }
    }
    
    // Verify all nodes are now used
    assert_equal_int(MAX_NODE_LIST_SIZE, count_used_nodes(), idx++, 11);
    
    // Now try to insert when array is completely full (should fail)
    used_before = count_used_nodes();
    result = list_insert_index_hard(&head, 1, 777);
    used_after = count_used_nodes();
    assert_equal_int(-1, result, idx++, 11); // Should fail - no unused nodes
    assert_equal_int(used_before, used_after, idx++, 11); // No change in used nodes
    
    printf("\n");
}

void test_full_scope() {
    printf("--------------------------------\n");
    printf("Testing full scope integration...\n\n");

    int idx = 1;
    
    // Reset the node list for clean testing
    reset_node_list();
    
    // === PHASE 1: Create Multiple Lists ===
    printf("=== PHASE 1: Creating Multiple Lists ===\n");
    
    printf("Test No.%d:\nCreating first list with 5 elements\n", idx);
    list_node_t* list1 = create_node(1);
    for (int i = 2; i <= 5; i++) {
        int result = append_new_node(list1, i);
        assert_equal_int(0, result, idx++, 12);
    }
    int length1 = list_length(list1);
    assert_equal_int(5, length1, idx++, 12);
    
    printf("\nTest No.%d:\nCreating second list with 3 elements\n", idx);
    list_node_t* list2 = create_node(10);
    for (int i = 11; i <= 12; i++) {
        int result = append_new_node(list2, i);
        assert_equal_int(0, result, idx++, 12);
    }
    int length2 = list_length(list2);
    assert_equal_int(3, length2, idx++, 12);
    
    printf("\nTest No.%d:\nCreating third list with 7 elements\n", idx);
    list_node_t* list3 = create_node(20);
    for (int i = 21; i <= 26; i++) {
        int result = append_new_node(list3, i);
        assert_equal_int(0, result, idx++, 12);
    }
    int length3 = list_length(list3);
    assert_equal_int(7, length3, idx++, 12);
    
    // === PHASE 2: Use Length in Loops ===
    printf("\n=== PHASE 2: Using Length in Loops ===\n");
    
    printf("Test No.%d:\nUsing length to iterate through list1\n", idx);
    int sum1 = 0;
    for (int i = 0; i < list_length(list1); i++) {
        list_node_t* node = list_get_index(list1, i);
        assert_equal_pointer(&NODE_LIST[i], node, idx++, 12);
        sum1 += node->data;
    }
    assert_equal_int(15, sum1, idx++, 12); // 1+2+3+4+5 = 15
    
    printf("\nTest No.%d:\nUsing length to iterate through list2\n", idx);
    int sum2 = 0;
    for (int i = 0; i < list_length(list2); i++) {
        list_node_t* node = list_get_index(list2, i);
        assert_equal_pointer(&NODE_LIST[i + 5], node, idx++, 12);
        sum2 += node->data;
    }
    assert_equal_int(33, sum2, idx++, 12); // 10+11+12 = 33
    
    printf("\nTest No.%d:\nUsing length to iterate through list3\n", idx);
    int sum3 = 0;
    for (int i = 0; i < list_length(list3); i++) {
        list_node_t* node = list_get_index(list3, i);
        assert_equal_pointer(&NODE_LIST[i + 8], node, idx++, 12);
        sum3 += node->data;
    }
    assert_equal_int(161, sum3, idx++, 12); // 20+21+22+23+24+25+26 = 161
    
    // === PHASE 3: Remove Odd Elements ===
    printf("\n=== PHASE 3: Removing Odd Elements ===\n");
    
    printf("Test No.%d:\nRemoving odd elements from list1\n", idx);
    // Remove elements at odd indices (1, 3) - this removes values 2 and 4
    list1 = list_remove_index(list1, 3); // Remove index 3 (value 4)
    list1 = list_remove_index(list1, 1); // Remove index 1 (value 2)
    
    // Verify remaining elements
    assert_equal_int(1, list_get_index(list1, 0)->data, idx++, 12);
    assert_equal_int(3, list_get_index(list1, 1)->data, idx++, 12);
    assert_equal_int(5, list_get_index(list1, 2)->data, idx++, 12);
    assert_equal_int(3, list_length(list1), idx++, 12);
    
    printf("\nTest No.%d:\nRemoving odd elements from list2\n", idx);
    // Remove element at index 1 (value 11)
    list2 = list_remove_index(list2, 1);
    
    // Verify remaining elements
    assert_equal_int(10, list_get_index(list2, 0)->data, idx++, 12);
    assert_equal_int(12, list_get_index(list2, 1)->data, idx++, 12);
    assert_equal_int(2, list_length(list2), idx++, 12);
    
    printf("\nTest No.%d:\nRemoving odd elements from list3\n", idx);
    // Remove elements at odd indices (1, 3, 5) - this removes values 21, 23, 25
    list3 = list_remove_index(list3, 5); // Remove index 5 (value 25)
    list3 = list_remove_index(list3, 3); // Remove index 3 (value 23)
    list3 = list_remove_index(list3, 1); // Remove index 1 (value 21)
    
    // Verify remaining elements
    assert_equal_int(20, list_get_index(list3, 0)->data, idx++, 12);
    assert_equal_int(22, list_get_index(list3, 1)->data, idx++, 12);
    assert_equal_int(24, list_get_index(list3, 2)->data, idx++, 12);
    assert_equal_int(26, list_get_index(list3, 3)->data, idx++, 12);
    assert_equal_int(4, list_length(list3), idx++, 12);
    
    // === PHASE 4: Continuous Deletion Until Empty ===
    printf("\n=== PHASE 4: Continuous Deletion Until Empty ===\n");
    
    printf("Test No.%d:\nContinuously deleting from list1 until empty\n", idx);
    int deleted_count1 = 0;
    while (list_length(list1) > 0) {
        int current_length = list_length(list1);
        // Delete from the end (last index)
        list1 = list_remove_index(list1, current_length - 1);
        deleted_count1++;
        assert_equal_int(current_length - 1, list_length(list1), idx++, 12);
    }
    assert_equal_int(3, deleted_count1, idx++, 12); // Should have deleted 3 elements
    assert_equal_pointer(NULL, list1, idx++, 12); // List should be NULL
    
    printf("\nTest No.%d:\nContinuously deleting from list2 until empty\n", idx);
    int deleted_count2 = 0;
    while (list_length(list2) > 0) {
        int current_length = list_length(list2);
        // Delete from the beginning (index 0)
        list2 = list_remove_index(list2, 0);
        deleted_count2++;
        assert_equal_int(current_length - 1, list_length(list2), idx++, 12);
    }
    assert_equal_int(2, deleted_count2, idx++, 12); // Should have deleted 2 elements
    assert_equal_pointer(NULL, list2, idx++, 12); // List should be NULL
    
    printf("\nTest No.%d:\nContinuously deleting from list3 until empty\n", idx);
    int deleted_count3 = 0;
    while (list_length(list3) > 0) {
        int current_length = list_length(list3);
        // Alternate between deleting from beginning and end
        int delete_index = (deleted_count3 % 2 == 0) ? 0 : (current_length - 1);
        list3 = list_remove_index(list3, delete_index);
        deleted_count3++;
        assert_equal_int(current_length - 1, list_length(list3), idx++, 12);
    }
    assert_equal_int(4, deleted_count3, idx++, 12); // Should have deleted 4 elements
    assert_equal_pointer(NULL, list3, idx++, 12); // List should be NULL
    
    // === PHASE 5: Verify Final Node List State ===
    printf("\n=== PHASE 5: Final Node List State Verification ===\n");
    
    printf("Test No.%d:\nChecking final state of node list\n", idx);
    int total_used_nodes = count_used_nodes();
    assert_equal_int(0, total_used_nodes, idx++, 12); // All nodes should be unused
    
    // Verify all nodes in NODE_LIST are marked as unused
    for (int i = 0; i < MAX_NODE_LIST_SIZE; i++) {
        assert_equal_bool(false, NODE_LIST[i].used, idx++, 12);
        assert_equal_int(0, NODE_LIST[i].data, idx++, 12);
        assert_equal_pointer(NULL, NODE_LIST[i].next, idx++, 12);
    }
    
    // === PHASE 6: Test Edge Cases After Cleanup ===
    printf("\n=== PHASE 6: Edge Cases After Cleanup ===\n");
    
    printf("Test No.%d:\nTesting operations on empty lists\n", idx);
    assert_equal_int(0, list_length(NULL), idx++, 12);
    
    double avg;
    int min, max, sum;
    int result = list_statistics(NULL, &avg, &min, &max, &sum);
    assert_equal_int(-1, result, idx++, 12);
    
    list_node_t* found = list_find(NULL, 100);
    assert_equal_pointer(NULL, found, idx++, 12);
    
    list_node_t* node_at_index = list_get_index(NULL, 0);
    assert_equal_pointer(NULL, node_at_index, idx++, 12);
    
    printf("\nTest No.%d:\nTesting swap_int_values integration\n", idx);
    int a = 100, b = 200;
    result = swap_int_values(&a, &b);
    assert_equal_int(0, result, idx++, 12);
    assert_equal_int(200, a, idx++, 12);
    assert_equal_int(100, b, idx++, 12);
    
    printf("\n=== FULL SCOPE TEST COMPLETED ===\n");
    printf("All functions tested in comprehensive real-world scenarios!\n");
    printf("Created multiple lists, used length in loops, removed odd elements,\n");
    printf("continuously deleted until empty, and verified final state.\n");
    
    printf("\n");
}

// Array of test functions corresponding to function_name array
test_function_t test_functions[13] = {
    test_swap_int_values,
    test_create_node,
    test_destroy_node,
    test_link_nodes,
    test_list_length,
    test_append_new_node,
    test_list_statistics,
    test_list_find,
    test_list_get_index,
    test_list_remove_index,
    test_list_insert_index,
    test_list_insert_index_hard,
    test_full_scope
};

// Safe test runner that catches segfaults
void run_test_safely(int function_index) {
    current_test_function = function_index;
    
    // Set up jump point for segfault recovery
    if (sigsetjmp(segfault_jmp, 1) == 0) {
        signal(SIGSEGV, segfault_handler);
        // Normal execution - run the test function
        test_functions[function_index]();
    } else {
        // We jumped here due to a segfault
        printf("\n[RECOVERY] Continuing with remaining tests...\n");
        // Reinstall signal handler after segfault recovery
        signal(SIGSEGV, segfault_handler);
    }
    
    current_test_function = -1;
}

// Main function calls all the test functions and summarizes the results.
int main(int argc, char* argv[]){
    // Set up signal handler for SIGSEGV at the start
    signal(SIGSEGV, segfault_handler);
    
    // If no argument provided, run all tests
    if (argc == 1) {
        for (int i = 0; i < 13; i++) {
            run_test_safely(i);
        }
    } else {
        // Look for the specified test function
        char* test_name = argv[1];
        int found = 0;
        
        for (int i = 0; i < 13; i++) {
            if (strcmp(test_name, function_name[i]) == 0) {
                run_test_safely(i);
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("Error: Test function '%s' not found.\n", test_name);
            printf("Available test functions:\n");
            for (int i = 0; i < 13; i++) {
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
        for (int i = 0; i < 13; i++){
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
        for (int i = 0; i < 13; i++) {
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
    printf("points for correctness: %f/46, the other 4 points are for comments and git commits, please read the document for more details.\n", total_points);
    
    // Reset signal handler to default before exiting
    signal(SIGSEGV, SIG_DFL);
    return 0;
}
