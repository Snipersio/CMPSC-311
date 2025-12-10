#ifndef POINTERLIB_H
#define POINTERLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/* 
This is a header file for pointerlib.c. It has the declarations of the functions in pointerlib.c and the comments
explaining what the functions do. Please read this file carefully before you start working to complete 
the code in pointerlib.c. Please do NOT make any changes to this file.

This project focuses on pointers, memory addressing, and linked list data structures. You will work with
a simple linked list implementation where nodes contain integer data and pointers to the next node.

For all of the following functions, you do not need to worry about integer overflow (i.e., a number is too
large to be stored in an int type variable), since the values in test cases are not so large.
*/

enum Limits {
    MAX_NODE_LIST_SIZE = 25
};


// Struct representing a node
typedef struct ListNode {
    int data;                    // The integer data stored in this node
    bool used;                   // Boolean to determine if this node is used or not
    struct ListNode* next;       // Pointer to the next node in the list (NULL if this is the last node)
} list_node_t;


// Memory preallocated for 100 nodes.
extern list_node_t NODE_LIST[MAX_NODE_LIST_SIZE];


/*
This function swaps the data between int a and int b using pointer dereferencing.
This is just a primer for pointers, and not relevant to the rest of the assignment.
@param a: A pointer to an integer to swap with b
@param b: A pointer to an integer to swap with a
@return: An integer indicating the success of the operation (0 if successful, -1 if not)
@examples:
int x = 0; int y = 10;
swap_int_values(&x, &y) -> returns 0 and x is now 10, y is now 0
swap_int_values(NULL, &y) -> returns -1 and y is still 10
*/
int swap_int_values(int* a, int* b);


/*
This function searches the NODE_LIST for an unused node and returns a pointer to it. 
When a suitable node is found, it is marked as used and the integer data is stored in it. The next pointer is set to NULL.
@param data: The integer value to store in the newly used node
@return: A pointer to the newly used node; if no unused nodes exist, return NULL
@examples:
Assuming NODE_LIST[3] and all are unused;
create_node(42) -> returns a pointer to NODE_LIST[0] with data=42, used=true, next=NULL
create_node(0) -> returns a pointer to NODE_LIST[1] with data=0, used=true, next=NULL
create_node(-5) -> returns a pointer to NODE_LIST[2] with data=-5, used=true, next=NULL
create_node(100) -> returns NULL (no unused nodes left)
*/
list_node_t* create_node(int data);


/*
This function destroys a node by setting its used flag to false, setting the data to 0, and setting the next pointer to NULL.
@param node: Pointer to the node to destroy
@return: An integer indicating the success of the operation (0 if successful, -1 if not)
@examples:
node = [5]->NULL => return 0, used is now false, data is now 0, next is now NULL
node = NULL => return -1, input is not a valid node
*/
int destroy_node(list_node_t* node);


/*
This function links two nodes together by setting the next pointer of node1 to node2.
@param node1: A pointer to the first node
@param node2: A pointer to the second node
@return: An integer indicating the success of the operation (0 if successful, -1 if not)
@note: node2 may be NULL, but node1 must be non-NULL.
@examples:
list_node_t* node1 = create_node(42);
list_node_t* node2 = create_node(0);
link_nodes(node1, node2) -> returns 0, node1->next is now node2. node2->next is NULL
link_nodes(node1, NULL) -> returns 0, node1->next is now NULL
link_nodes(NULL, node2) -> returns -1
link_nodes(NULL, NULL) -> returns -1
*/
int link_nodes(list_node_t* node1, list_node_t* node2);


/*
This function counts the number of nodes in a linked list by traversing the list.
@param head: Pointer to the first node of the list (NULL if list is empty)
@return: The number of nodes in the list
@hint: The end of the list is marked by a next pointer that is NULL.
@examples:
head = NULL => return 0 (empty list)
head = [5]->NULL => return 1 (single node)
head = [1]->[2]->[3]->NULL => return 3 (three nodes)
head = [10]->[20]->[30]->[40]->NULL => return 4 (four nodes)
*/
int list_length(list_node_t* head);


/*
This function creates and links a newly used node to the end of an input list.
@param head: A pointer to the first node of a list (NULL if list is empty)
@param data: The integer value to store in the newly used node
@return: An integer indicating the success of the operation (0 if successful, -1 if not)
@hint: Use the create_node() function
@hint: Iterate to the end of the linked list (that happens when the node next is NULL); link_nodes() there
@hint: If the list is empty, return -1; If no new node is created, return -1; if the new node is not linked to the list, return -1; Otherwise, return 0
@examples:
list_node_t* head = create_node(0) -> head is now [0]->NULL
append_new_node(head, 42) -> returns 0, head is now [0]->[42]->NULL
append_new_node(head, 3) -> returns 0, head is now [0]->[42]->[3]->NULL
append_new_node(head, -5) -> returns 0, head is now [0]->[42]->[3]->[-5]->NULL
append_new_node(NULL, 100) -> returns -1 (list was empty)
*/
int append_new_node(list_node_t* head, int data);


/*
This function finds the sum, average, minimum, and maximum data values in a linked list.
@param head: Pointer to the first node of the list
@param avg: Pointer to a double to store the average of all data values in the list
@param min: Pointer to an integer to store the minimum data value in the list
@param max: Pointer to an integer to store the maximum data value in the list
@param sum: Pointer to an integer to store the sum of all data values in the list
@return: An integer indicating the success of the operation (0 if successful, -1 if not)
@hint: If the list is empty, return -1; If the average, min, max, or sum is not calculated correctly, return -1; Otherwise, return 0
@examples:
head = [5]->NULL => return 0, avg=5, min=5, max=5, sum=5
head = [1]->[2]->[3]->NULL => return 0, avg=2, min=1, max=3, sum=6
head = [10]->[20]->[30]->NULL => return 0, avg=20, min=10, max=30, sum=60
head = [-1]->[2]->[-3]->NULL => return 0, avg=-0.666666, min=-3, max=2, sum=-2
head = NULL => return -1 (empty list)
*/
int list_statistics(list_node_t* head, double* avg, int* min, int* max, int* sum);


/*
This function finds the first occurrence of a target value in a linked list by traversing the list.
@param head: Pointer to the first node of the list
@param target: The value to search for
@return: Pointer to the first node containing the target value, or NULL if not found or an error happens
@examples:
head = NULL, target = 5 => return NULL
head = [5]->NULL, target = 5 => return pointer to the node with data=5
head = [5]->NULL, target = 3 => return NULL
head = [1]->[2]->[3]->NULL, target = 2 => return pointer to the node with data=2
head = [1]->[2]->[3]->NULL, target = 4 => return NULL
head = [5]->[5]->[3]->NULL, target = 5 => return pointer to the first node with data=5
*/
list_node_t* list_find(list_node_t* head, int target);

/*
This function gets the node at a specific index (position) in a linked list.
Indices are 0-indexed just like arrays (first node is at index 0).
@param head: Pointer to the first node of the list (NULL if list is empty)
@param index: The index to get data from
@return: The node at the given index, or NULL if index is invalid
@examples:
head = NULL, index = 0 => return NULL (empty list)
head = [5]->NULL, index = 0 => return pointer to the node with data=5
head = [5]->NULL, index = 1 => return NULL (index out of bounds)
head = [1]->[2]->[3]->NULL, index = 0 => return pointer to the node with data=1
head = [1]->[2]->[3]->NULL, index = 1 => return pointer to the node with data=2
head = [1]->[2]->[3]->NULL, index = 2 => return pointer to the node with data=3
head = [1]->[2]->[3]->NULL, index = 3 => return NULL (index out of bounds)
*/
list_node_t* list_get_index(list_node_t* head, int index);


/*
This function removes the node at a specific index (position) in a linked list. If the index is invalid, 
return the original list. Make sure to set the removed node as unused.
@param head: Pointer to the first node of the list (NULL if list is empty)
@param index: The index of the node to remove
@return: The new list with the node at the given index removed, or the original list if the index is invalid
@examples:
head = NULL, index = 0 => return NULL
head = [5]->NULL, index = 0 => return NULL
head = [5]->NULL, index = 1 => return [5]->NULL (index out of bounds)
head = [1]->[2]->[3]->NULL, index = 0 => return [2]->[3]->NULL
head = [1]->[2]->[3]->NULL, index = 1 => return [1]->[3]->NULL
head = [1]->[2]->[3]->NULL, index = 2 => return [1]->[2]->NULL
head = [1]->[2]->[3]->NULL, index = 3 => return [1]->[2]->[3]->NULL (index out of bounds)
*/
list_node_t* list_remove_index(list_node_t* head, int index);


/*
This function inserts a new node with the given data at a specific index (position) in a linked list. If the index is invalid, return NULL;
@param head: Pointer to the first node of the list (NULL if list is empty)
@param index: The index of the node to insert the new node at
@param data: The data to store in the new node
@return: The new list with the node at the given index inserted, or NULL if the index is invalid
@hint: Use some of the other functions you implemented before to assist in this function
@examples:
head = NULL, index = 0, data = 3 => return [3] -> NULL
head = [5]->NULL, index = 0, data = 3 => return [3]->[5]->NULL
head = [5]->NULL, index = 1, data = 3 => return [5]->[3]->NULL
head = [1]->[2]->[3]->NULL, index = 0, data = 4 => return [4]->[1]->[2]->[3]->NULL
head = [1]->[2]->[3]->NULL, index = 1, data = 4 => return [1]->[4]->[2]->[3]->NULL
head = [1]->[2]->[3]->NULL, index = 2, data = 4 => return [1]->[2]->[4]->[3]->NULL
head = [1]->[2]->[3]->NULL, index = 4, data = 4 => return NULL (index out of bounds)
*/
list_node_t* list_insert_index(list_node_t* head, int index, int data);


/*
Notice in all the other examples we had to return a new pointer to the list because if we had to add to/remove the first node, 
then our head pointer would be invalid. To follow the general style where we return an integer status, we need to update where the head 
is pointing to. Therefore, we need a pointer to the head pointer. That way if the head does change, we can update the pointer to the head. 
Otherwise, we would just leave it be and make our alterations as normal since the head remains valid.
This function does the same thing as list_insert_index, but it takes a pointer to the head pointer instead of a pointer to the head node.
This means that if you insert at index 0, you must update the head pointer via the argument. The return of this function 
will be an integer indicating the success of the operation (0 if successful, -1 if not).
@param head: Pointer to the pointer to the first node of the list
@param index: The index to add the new node at
@param data: The data to store in the new node
@hint: Use some of the other functions you implemented before to assist in this function
@hint: If the index is 0, you must update the head pointer via the argument
@return: An integer indicating the success of the operation (0 if successful, -1 if not)
*/
int list_insert_index_hard(list_node_t** head, int index, int data);


#endif // POINTERLIB_H
