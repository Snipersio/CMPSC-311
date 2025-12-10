# vectorlib.c Programming Assignment

In this assignment, you are going to implement 8 functions in `vectorlib.c` according to the requirements in `vectorlib.h`. You will get experience working with pointers, memory management, and dynamic data structures in C. This assignment focuses on understanding how pointers work, memory allocation/deallocation, and implementing a chunked vector data structure.

## Details

You will be implementing a dynamic length integer array, also known as a vector, in C. 
To enable a dynamic array length, you will use malloc and free for memory management. 
The vector uses a chunked allocation strategy where each extend operation allocates a new chunk of memory, 
but you can also coalesce all chunks into a single large allocation for better performance.

### What is a Vector?

A vector is a dynamic array that can grow and shrink as needed. Unlike a regular array, you don't need to 
know the size beforehand. Our vector implementation uses "chunks" - small arrays linked together to 
create one large logical array.

### Visual Example of Vector Structure

Let's say we create a vector and add some elements:

```c
vector_t* vec = vector_create(3);  // Create vector with initial capacity of 3
```

This creates:
```
vector_t {
    num_chunks: 1
    num_elements: 3
    head: -> [chunk1]
    tail: -> [chunk1]
}

chunk1 {
    elements: [0, 0, 0]  // 3 uninitialized integers
    num_elements: 3
    next: NULL
}
```

Now let's add some values:
```c
vector_set(vec, 0, 10);  // Set element 0 to 10
vector_set(vec, 1, 20);  // Set element 1 to 20
vector_set(vec, 2, 30);  // Set element 2 to 30
```

The vector now looks like:
```
vector_t {
    num_chunks: 1
    num_elements: 3
    head: -> [chunk1]
    tail: -> [chunk1]
}

chunk1 {
    elements: [10, 20, 30]
    num_elements: 3
    next: NULL
}
```

Now let's extend the vector:
```c
vector_extend(vec, 4);  // Add a new chunk with 4 elements
```

The vector now has TWO chunks:
```
vector_t {
    num_chunks: 2
    num_elements: 7  // 3 + 4 = 7 total elements
    head: -> [chunk1] -> [chunk2]
    tail: -> [chunk2]
}

chunk1 {
    elements: [10, 20, 30]
    num_elements: 3
    next: -> [chunk2]
}

chunk2 {
    elements: [0, 0, 0, 0]  // 4 uninitialized integers
    num_elements: 4
    next: NULL
}
```

Let's add more values:
```c
vector_set(vec, 3, 40);  // Set element 3 to 40 (in chunk2)
vector_set(vec, 4, 50);  // Set element 4 to 50 (in chunk2)
vector_set(vec, 5, 60);  // Set element 5 to 60 (in chunk2)
vector_set(vec, 6, 70);  // Set element 6 to 70 (in chunk2)
```

Now the vector looks like:
```
vector_t {
    num_chunks: 2
    num_elements: 7
    head: -> [chunk1] -> [chunk2]
    tail: -> [chunk2]
}

chunk1 {
    elements: [10, 20, 30]
    num_elements: 3
    next: -> [chunk2]
}

chunk2 {
    elements: [40, 50, 60, 70]
    num_elements: 4
    next: NULL
}
```

### Indexing Across Chunks

Even though the data is stored in separate chunks, you access it as one continuous array:

```c
int* elem0 = vector_get(vec, 0);   // Returns pointer to 10 (in chunk1)
int* elem1 = vector_get(vec, 1);   // Returns pointer to 20 (in chunk1)
int* elem2 = vector_get(vec, 2);   // Returns pointer to 30 (in chunk1)
int* elem3 = vector_get(vec, 3);   // Returns pointer to 40 (in chunk2)
int* elem4 = vector_get(vec, 4);   // Returns pointer to 50 (in chunk2)
int* elem5 = vector_get(vec, 5);   // Returns pointer to 60 (in chunk2)
int* elem6 = vector_get(vec, 6);   // Returns pointer to 70 (in chunk2)
```

### Coalescing: Merging Chunks

You can merge all chunks into one large chunk for better performance:

```c
vector_coalesce(vec);  // Merge all chunks into one
```

After coalescing:
```
vector_t {
    num_chunks: 1
    num_elements: 7
    head: -> [chunk1]
    tail: -> [chunk1]
}

chunk1 {
    elements: [10, 20, 30, 40, 50, 60, 70]  // All elements in one chunk
    num_elements: 7
    next: NULL
}
```

### Key Concepts to Understand

1. **Chunked Storage**: Data is stored in multiple small arrays (chunks) linked together
2. **Logical Continuity**: Even though data is in separate chunks, you access it as one array
3. **Dynamic Growth**: You can extend the vector by adding new chunks
4. **Memory Management**: Each chunk is allocated with malloc() and destroyed with free()
5. **Coalescing**: You can merge all chunks into one for better access performance

Your task is to implement all the functions that make this vector work!

## Accepting the Assignment

[Click here to accept this assignment](https://classroom.github.com/a/oZX7MyAK). Once you accept the invitation, GitHub will create a private repository containing the starter files for the assignment. You will also find an accompanying `README.md`, which is essentially this document you are reading, useful for instructions and further details.

## Assignment File Structure

There are several files in this repository. You only need to read `vectorlib.h` and edit `vectorlib.c` to finish the assignment. The following is a description of all files. **You can read the first two and skip the rest.**

### **vectorlib.h**

This file is a *header* file (due to the `.h` file extension). This file contains:
- **Struct definitions**: `vector_chunk_t` and `vector_t` structs for representing the chunked vector data structure
- **Function declarations**: Forward declarations for all the functions you will implement
- **Detailed documentation**: Comprehensive explanations of what each function should do, including examples

The functions you need to implement include:
- `vector_create`: Create a new vector with specified initial capacity
- `vector_destroy`: Destroy a vector and free all associated memory
- `vector_extend`: Extend the vector by allocating a new chunk with specified capacity
- `vector_size`: Get the total number of elements in the vector
- `vector_extend_array`: Extend the vector by copying values from an array
- `vector_get`: Get an element from the vector at the specified index
- `vector_set`: Set an element in the vector at the specified index
- `vector_coalesce`: Coalesce all chunks into a single large allocation

Do **NOT** make any changes or edits in this file -- when we grade your project, we will use the original (unchanged) header file, so if you change it you might break your own code in our environment. Simply put, do not edit it.

### **vectorlib.c**

**This is the only file you should be modifying in this assignment.**

This file is a *source* file written in C (due to the `.c` file extension). This file implements the functions declared in `vectorlib.h`. Inside, you will write your own C code in place of the `TODO` comments based on the descriptions you read in `vectorlib.h`. Additionally, we would like you to adhere to good programming practices; i.e. include comments while writing your code. This will help you understand your code long after writing it and will also help others, such as course staff, to be able to read your code and understand your thought process. To encourage the use of comments, a percentage of this assignment will be based on whether you have written effective comments (telling us what you are doing in the code, why you write the code in this way, etc.).

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

This source file "imports" your functions in `vectorlib.c` and tests the correctness of your code with extensive test cases covering all functions and edge cases. The test cases in this file are more or less what we will use to test your code. While test case *values* may change, the test cases themselves will not. Because of this, do NOT hardcode answers for the inputs in the test cases. They will slightly change. Your code should work for all valid inputs.

### **.gitignore**

You don't need to read this file to finish this assignment. 

This file tells git what files or folders it should ignore. Any changes in those files or folders will not be tracked. The content of this file is `test`, which is the executable file generated from your code. Meaning, when you compile your driver program, you do not need to commit it to Git, just your source. We will recompile your code to get the latest executable file while grading.

### **README.md**

The markdown document you are reading now.

### **grade_valgrind.sh**

You don't need to read this file to finish this assignment.

This is a shell script that runs valgrind on your compiled program to check for memory leaks and other memory-related errors. You can use this script to verify that your code passes valgrind checks, which is worth 3 points of your grade.

---

## Testing Your Code

When you finish a function or reach a stopping point, you can compile and test your code using the following commands.

### Compiling

In the project directory, run this command in your terminal to compile your code:

```shell
gcc vectorlib.c driver.c -o test
```

If your code has no syntax errors, an executable file called `test` will appear in your directory. If there are syntax errors, carefully read the errors or warnings to understand what line of code is causing the syntax error. It will tell you.

### Testing / Running

To test your code and see your proposed grade, run:

```shell
./test
```

to start the tests. You will see results in your terminal like so:

```
--------------------------------
Testing vector_create...

Test No.1:
Creating vector with capacity 0
[ERROR] Test No.1 failed. Expected non-NULL pointer, but got NULL.

[SEGFAULT] Function vector_create caused a segmentation fault!
[SEGFAULT] Marking all remaining tests for this function as failed.
--------------------------------
Testing vector_destroy...

Test No.1:
Destroying NULL vector
Test No.1 passed

Test No.2:
Destroying empty vector
[ERROR] Test No.2 failed. Expected 0, but got -1.

Test No.3:
Destroying vector with multiple chunks
[ERROR] Test No.3 failed. Expected 0, but got -1.

...

[SEGFAULT] Function memory_stress caused a segmentation fault!
[SEGFAULT] Marking all remaining tests for this function as failed.
--------------------------------
Testing full scope integration...

=== PHASE 1: Creating and Populating Vector ===
Test No.1:
Creating vector with initial capacity 5
[ERROR] Test No.1 failed. Expected non-NULL pointer, but got NULL.

[SEGFAULT] Function full_scope caused a segmentation fault!
[SEGFAULT] Marking all remaining tests for this function as failed.
-------------------Summary-------------------
Total tests: 133
Total tests passed: 19
Total tests failed: 114
--------------------------------
Function vector_create SEGFAULTED (all tests failed)
Function vector_destroy failed
Function vector_extend SEGFAULTED (all tests failed)
Function vector_size failed
Function vector_extend_array SEGFAULTED (all tests failed)
Function vector_get SEGFAULTED (all tests failed)
Function vector_set SEGFAULTED (all tests failed)
Function vector_coalesce SEGFAULTED (all tests failed)
Function memory_stress failed
Function full_scope SEGFAULTED (all tests failed)
Total functions tested: 10
Total functions passed: 0
points for correctness: 0.0/20, the other 5 points are for comments (1), git commits (1), and valgrind safety (3), please read the document for more details.
```

You can scroll through the output to find the function results you care about / want to test. 

For example, the output below tells you function `vector_extend_array` failed (specifically, it had some test failures). 

```
Testing vector_extend_array...

Test No.1:
Extending NULL vector with n elements
Test No.1 passed

Test No.2:
Extending NULL vector with 0 elements
Test No.2 passed

Test No.3:
Extending with NULL array
Test No.3 passed

Test No.4:
Extending empty vector with 0 elements
[ERROR] Test No.4 failed. Expected 0, but got 1.
[ERROR] Test No.5 failed. Expected 0, but got 3.
```

You can also test individual functions by specifying the function name as an argument:

```shell
./test vector_create
./test vector_destroy
./test vector_extend
./test vector_get
```

This will run only the tests for the specified function, making it easier to debug specific issues. Available function names include:
- `vector_create`
- `vector_destroy`
- `vector_extend`
- `vector_size`
- `vector_extend_array`
- `vector_get`
- `vector_set`
- `vector_coalesce`
- `memory_stress`
- `full_scope`

After you change your code, rerun `gcc vectorlib.c driver.c -o test` to recompile and then rerun `./test` (or `./test function_name`) to test your updated code. This is also helpful if you wish to test a specific test using GDB.

### Checking Your Valgrind Grade

To check if your code passes valgrind memory checks (worth 3 points), use the provided script:

```shell
# Make the script executable (only needed once)
chmod +x grade_valgrind.sh

# Run valgrind checks
./grade_valgrind.sh
```

This is assuming there is an executable called "test" in your current work directory. Running this script will tell you whether you'll receive the 3 valgrind points or not.

If you are confused why your algorithm may not be working, feel free to use `printf` statements to debug your program, or use GDB as shown in class (preferred). Use the following cheat sheet to help you print:

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

This assignment is worth 25 points total. There are 8 functions you need to implement in `vectorlib.c` for a total of 20 points. Each function is assigned a point value described below:

- `vector_create` - 2 pts
- `vector_destroy` - 2 pts
- `vector_extend` - 3 pts
- `vector_size` - 1 pt
- `vector_extend_array` - 2 pts
- `vector_get` - 1 pts
- `vector_set` - 1 pts
- `vector_coalesce` - 3 pts
- `memory_stress` (stress test) - 2 pts
- `full_scope` (integration test) - 3 pts

You will get points for a certain function only if you pass **all** the test cases for that function. For example, in the test output above, you will get 0 points for function `vector_get` if you fail any of the test cases for that function. 

The remaining 5 points of this assignment come from:
- **Comments** (1 pt): Effective use of comments explaining your thought process and why code is written as it is
- **Git commits** (1 pt): Regular commits showing your progress (at least 5 commits)
- **Memory safety** (3 pts): Your code must pass valgrind memory checks without memory leaks or errors

**Note**: The valgrind safety points (3 pts) are critical for this assignment since you're working with dynamic memory allocation. This represents a significant portion of your grade, so make sure to test your code with valgrind regularly during development to catch memory issues early. Memory management is a core skill in systems programming.

For late submissions, please refer to syllabus or Canvas website.

## Turnin

Go to Canvas, start the quiz, and submit a commit ID as a single line in the submission box.
