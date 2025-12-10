# pointerlib.c Programming Assignment

In this assignment, you are going to implement 12 functions in `pointerlib.c` according to the requirements in `pointerlib.h`. You will get experience working with pointers, memory addressing, and linked list data structures in C. This assignment focuses on understanding how pointers work and how to manipulate linked lists.

# Details

## Understanding Linked Lists

A linked list is a fundamental data structure in computer science that consists of a sequence of elements called **nodes**. Each node contains two parts:
- **Data**: The actual information stored (in our case, just an integer)
- **Next pointer**: A reference to another node in the sequence (the next node)

### Visual Representation

Here's how a linked list looks conceptually:

```
Index       0                   1                   2

Head ─> &[ Data:  10 ] ┌─> &[ Data:  42 ] ┌─> &[ Data:  8  ] ┌─> NULL
         [ Next: ────]─┘    [ Next: ────]─┘    [ Next: ────]─┘    
```

Here, head is a pointer to the starting node element, data is some structure (in this assignment just an int), and next is a pointer to another node element of the same type. Linked lists are terminated with NULL pointers since they are safe invalid addresses. Linked lists are like arrays in that we can describe elements via an index, but unlike arrays are not indexable, meaning we must iterate in order to properly access nodes/data.

**Empty List:**
```
Head → NULL
```

Since NULL determines the end of the list, if the head is NULL then there is no starting struct/element/data, therefore the list is empty.

**Single Node:**
```
Head ─> &[ Data:  10 ] ┌─> NULL
         [ Next: ────]─┘    
```

Head points to a non-NULL node, therefore the head points to a real element. This element's next pointer points to NULL meaning there is no next struct element, i.e. the size of the list is just of the one node.

**Multiple Nodes:**
```
Head ─> &[ Data:  42 ] ┌─> &[ Data:  8  ] ┌─> NULL
         [ Next: ────]─┘    [ Next: ────]─┘    
```

Head points to a non-NULL node, therefore the head points to a real element. This element's next pointer points to a non-NULL node, therefore the node points to another real element. In this element there is a NULL next element, meaning that is the end of the list. The size of the list 2.


### Memory Layout in This Assignment

In this assignment, you will use a **preallocated array** (called NODE_LIST) of `MAX_NODE_LIST_SIZE` elements to "create" or use new nodes:

```
NODE_LIST[0]: [Data: 0, Used: false, Next: NULL]
NODE_LIST[1]: [Data: 0, Used: false, Next: NULL]
NODE_LIST[2]: [Data: 0, Used: false, Next: NULL]
...
NODE_LIST[MAX_NODE_LIST_SIZE-1]: [Data: 0, Used: false, Next: NULL]
```

When you create a node, the `create_node()` function finds an unused slot and marks it as used:

```
NODE_LIST[0]: [Data: 42, Used: true,  Next: NULL]  ← Node created here
NODE_LIST[1]: [Data: 0,  Used: false, Next: NULL]
NODE_LIST[2]: [Data: 0,  Used: false, Next: NULL]
```

You will then return the address of this struct so you can add it into a linked list. As another example, consider the following:

```
NODE_LIST[0]: [Data: 42, Used: true,  Next: 0x7ffa...]
NODE_LIST[1]: [Data: 0,  Used: false, Next: NULL     ]  ← Node created here
NODE_LIST[2]: [Data: 0,  Used: true,  Next: 0x7ffb...]
```

Here an unused node doesn't have to be the first or last element, but could be inbetween used elements.

The remaining functions in the assignment will build on these topics.

### Pointer Operations

Understanding these pointer operations is crucial:

- **`&` (Address operator)**: Get the address of a variable
  ```c
  int value = 42;
  int* ptr = &value;  // Get the address of value
  ```

- **`*` (Dereference operator)**: Access the value a pointer points to
  ```c
  int* ptr = ...;    // decared somewhere
  int value = *ptr;  // Get the value pointed to by ptr
  ```

- **`->` (Arrow operator)**: Access struct members through a pointer
  You could dereference struct members like so:
  ```c
  node_t* node = ...  // declared somewhere

  (*node).data    // Get the data field
  (*node).next    // Get the next pointer
  (*node).used    // Get the used flag
  ```
  
  But this is annoying and too verbose. Instead we can use the struct dereference arrow operator to do the exact same dereference with less typing:

  ```c
  node->data    // Get the data field
  node->next    // Get the next pointer
  node->used    // Get the used flag
  ```

### Common Patterns

**Traversing a List:**
```c
for (list_node_t* current = head; current != NULL; current = current->next) {
    // Do something with current->data
}
```

or as a nicer while loop

```c
list_node_t* current = head;
while (current != NULL) {
    // Do something with current->data
    current = current->next;  // Move to next node
}
```

Remember that you should always ensure that before you dereference a pointer (using *ptr or ptr->...), the pointer is a valid address / non-NULL. Otherwise you will get a `Segmentation Fault` or memory error. 

Finally, remember you can print addresses and pointers using the `%p` specifier in `printf`.

## Accepting the Assignment

[Click here to accept this assignment](https://classroom.github.com/a/9VEiO6L0). Once you accept the invitation, GitHub will create a private repository containing the starter files for the assignment. You will also find an accompanying `README.md`, which is essentially this document you are reading, useful for instructions and further details.

## Assignment File Structure

There are several files in this repository. You only need to read `pointerlib.h` and edit `pointerlib.c` to finish the assignment. The following is a description of all files. **You can read the first two and skip the rest.**

### **pointerlib.h**

This file is a *header* file (due to the `.h` file extension). This file contains:
- **Struct definition**: `ListNode` struct for representing nodes in a linked list
- **Enum definitions**: `Limits` enum for storing array maximums used in the `NODE_LIST` array
- **Function declarations**: Forward declarations for all the functions you will implement
- **Detailed documentation**: Comprehensive explanations of what each function should do

The functions you need to implement include:
- `swap_int_values`: Swap values between two integer pointers (a primer, not used elsewhere)
- `create_node`: Mark a node as used from the preallocated
- `destroy_node`: Mark a node as unused and reset its data
- `link_nodes`: Connect two nodes together
- `list_length`: Count the number of nodes in a linked list
- `append_new_node`: Add a new node to the end of a list
- `list_statistics`: Calculate sum, average, min, and max of list data
- `list_find`: Find the first occurrence of a target value
- `list_get_index`: Get the node at a specific index
- `list_remove_index`: Remove the node at a specific index and return the new list
- `list_insert_index`: Insert a new node at a specific index and return the new list
- `list_insert_index_hard`: Insert a new node at a specific index using a pointer to the head pointer

Do **NOT** make any changes or edits in this file -- when we grade your project, we will use the original (unchanged) header file, so if you change it you might break your own code in our environment. Simply put, do not edit it.

### **pointerlib.c**

**This is the only file you should be modifying in this assignment.**

This file is a *source* file written in C (due to the `.c` file extension). This file implements the functions declared in `pointerlib.h`. Inside, you will write your own C code in place of the `TODO` comments based on the descriptions you read in `pointerlib.h`. Additionally, we would like you to adhere to good programming practices; i.e. include comments while writing your code. This will help you understand your code long after writing it and will also help others, such as course staff, to be able to read your code and understand your thought process. To encourage the use of comments, a percentage of this assignment will be based on whether you have written effective comments (telling us what you are doing in the code, why you write the code in this way, etc.).

For example, consider these two samples of code:

```c
// f to c
float convert_to_c(float input) {
    float c = 0; // declare var
    float f = input; // declare input var
    c = (f - 32) * 5 / 9; // do math
    return c; // return result
}
```

```c
// Convert an input float in Fahrenheit (f) to an output float in Celsius (c)
float convert_to_c(float input) {
    float c = 0; // declare the output celsius float
    float f = input; // declare the input fahrenheit float, set it as the input
    c = (f - 32) * 5 / 9; // convert F to C using the conversion formula: C = (F - 32) × 5/9
    return c; // return the celcius result
}
```

While both functions do the same thing, they are documented very differently. The latter is much more readable and understandable as to why decisions were made and what the code is doing rather than just describing the code on a surface level. When writing comments, it's not enough to just say "this is what the code does" but instead you should be documenting "why the code is written as it is". Comments should explain your thought process.

### **driver.c**

You don't need to read this file to finish this assignment, but it's a comprehensive test suite that validates your implementation.

This source file "imports" your functions in `pointerlib.c` and tests the correctness of your code with extensive test cases covering all functions and edge cases. The test cases in this file are more or less what we will use to test your code. While test case *values* may change, the test cases themselves will not. Because of this, do NOT hardcode answers for the inputs in the test cases. They will slightly change. Your code should work for all valid inputs.

### **.gitignore**

You don't need to read this file to finish this assignment. 

This file tells git what files or folders it should ignore. Any changes in those files or folders will not be tracked. The content of this file is `test`, which is the executable file generated from your code. Meaning, when you compile your driver program, you do not need to commit it to Git, just your source. We will recompile your code to get the latest executable file while grading.

### **README.md**

The markdown document you are reading now.

---

## Testing Your Code

When you finish a function or reach a stopping point, you can compile and test your code using the following commands.

### Compiling

In the project directory, run this command in your terminal to compile your code:

```shell
gcc pointerlib.c driver.c -o test
```

If your code has no syntax errors, an executable file called `test` will appear in your directory. If there are syntax errors, carefully read the errors or warnings to understand what line of code is causing the syntax error. It will tell you.

### Testing / Running

To test your code and see your proposed grade, run:

```shell
./test
```

to start the tests. You will see results in your terminal like so:

```
Testing list_insert_index...

Test No.1:
Empty list, index = 0, data = 3
[ERROR] Test No.1 failed. Expected 0x607320, but got (nil).

[SEGFAULT] Function list_insert_index caused a segmentation fault!
[SEGFAULT] Marking all remaining tests for this function as failed.

[RECOVERY] Continuing with remaining tests...
--------------------------------
Testing list_insert_index_hard...

Test No.1:
Empty list, index = 0, data = 3
[ERROR] Test No.1 failed. Expected 0, but got -1.
[ERROR] Test No.2 failed. Expected 0x607320, but got (nil).

[SEGFAULT] Function list_insert_index_hard caused a segmentation fault!
[SEGFAULT] Marking all remaining tests for this function as failed.

[RECOVERY] Continuing with remaining tests...
--------------------------------
Testing full scope integration...

Test No.1:
Creating initial list with single node
[ERROR] Test No.1 failed. Expected 0x607320, but got (nil).

[SEGFAULT] Function full_scope caused a segmentation fault!
[SEGFAULT] Marking all remaining tests for this function as failed.

[RECOVERY] Continuing with remaining tests...
-------------------Summary-------------------
Total tests: 82
Total tests passed: 20
Total tests failed: 62
--------------------------------
Function swap_int_values failed
Function create_node SEGFAULTED (all tests failed)
Function destroy_node SEGFAULTED (all tests failed)
Function link_nodes SEGFAULTED (all tests failed)
Function list_length SEGFAULTED (all tests failed)
Function append_new_node SEGFAULTED (all tests failed)
Function list_statistics SEGFAULTED (all tests failed)
Function list_find SEGFAULTED (all tests failed)
Function list_get_index SEGFAULTED (all tests failed)
Function list_remove_index SEGFAULTED (all tests failed)
Function list_insert_index failed
Function list_insert_index_hard SEGFAULTED (all tests failed)
Function full_scope SEGFAULTED (all tests failed)
Total functions tested: 13
Total functions passed: 0
points for correctness: 0.000000/46, the other 4 points are for comments and git commits, please read the document for more details.
```

You can scroll through the output to find the function results you care about / want to test. 

For example, the output below tells you function `list_insert_index` failed (specifically, it caused a segfault). 

```
Testing list_insert_index...

Test No.1:
Empty list, index = 0, data = 3
[ERROR] Test No.1 failed. Expected 0x607320, but got (nil).

[SEGFAULT] Function list_insert_index caused a segmentation fault!
[SEGFAULT] Marking all remaining tests for this function as failed.
```

You can also test individual functions by specifying the function name as an argument:

```shell
./test swap_int_values
./test create_node
./test list_length
./test list_insert_index
```

This will run only the tests for the specified function, making it easier to debug specific issues. Available function names include:
- `swap_int_values`
- `create_node`
- `destroy_node`
- `link_nodes`
- `list_length`
- `append_new_node`
- `list_statistics`
- `list_find`
- `list_get_index`
- `list_remove_index`
- `list_insert_index`
- `list_insert_index_hard`
- `full_scope`

After you change your code, rerun `gcc pointerlib.c driver.c -o test` to recompile and then rerun `./test` (or `./test function_name`) to test your updated code.

If you are confused why your algorithm may not be working, feel free to use `printf` statements to debug your program. Use the following cheat sheet to help you print:

```c
printf("this will print a message \n");
printf("this will print an integer %d \n", myint);
printf("this will print a float %f \n", myfloat);
printf("this will print a char %c \n", mychar);
printf("this will print a string %s \n", mystring);
printf("this will print a pointer %p \n", mypointer);
printf("this will print a pointer %p \n", &my_var);
```

---

## Rubric

This assignment is worth 50 points. There are 12 functions you need to implement in `pointerlib.c` for a total of 46 points. Each function is assigned a point value described below:

- `swap_int_values` - 1 pts
- `create_node` - 2 pts
- `destroy_node` - 1 pts
- `link_nodes` - 1 pts
- `list_length` - 2 pts
- `append_new_node` - 3 pts
- `list_statistics` - 3 pts
- `list_find` - 2 pts
- `list_get_index` - 3 pts
- `list_remove_index` - 5 pts
- `list_insert_index` - 7 pts
- `list_insert_index_hard` - 7 pts
- `full_scope` (integration test) - 9 pts

You will get points for a certain function only if you pass **all** the test cases for that function. For example, in the test output above, you will get 0 points for function `list_length` since you failed 3 of the test cases. 

The remaining 4 points of this assignment come from your effective use of comments (2 pts) and your `git` commits (2 pts). For the `git` commits, you should be regularly committing your progress. A decent rule of thumb would be to make a commit after implementing each function. You should have at least 7 commits in this repository to get the full 2 points.

For late submissions, please refer to syllabus or Canvas website.

## Turnin

Go to Canvas, start the quiz, and submit a commit ID as a single line in the submission box.
