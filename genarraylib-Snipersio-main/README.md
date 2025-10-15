# genarraylib.c Programming Assignment

In this assignment, you are going to implement 13 functions in `genarraylib.c` according to the requirements in `genarraylib.h`. You will get experience writing C code, using the GCC compiler, writing comments, using git, and operating your CLI. This project focuses on implementing a chunked array data structure that can store arbitrary types using unions and structs.

## Details

You will be implementing a special array that has a generic type per element. The `generic_element_t` type is a union of most all primitive types. 
This allows us to have one array that can store any type of data. Of course, we could just declare an array of this type, i.e. `generic_element_t array[N]`, but that would be too easy.
Instead, we will introduce the concept of **used**, **unused**, and **invalid** elements. Consider an array that looks like this:

```c
my_array = []
```

This array is empty, and we cannot add more elements to it. Instead, what if we preallocated elements which we *could* later use.

```c
my_array = [VOID, VOID, VOID]
```

Here, this array has 3 elements; not initialized, but usable. Let's instead add some valid types to the array.

```C
my_array = [INT, FLOAT, VOID]
```

Here, this array has 3 elements, 2 are used/allocated and 1 is not. The last element is not used, but could be later. Finally, let's introduce the idea of invalid elements.

```c
my_array = [VOID, VOID, VOID, ERROR, ERROR]
```

Here, we have declared an array that has 5 elements, 3 are unused but able to be allocated, and 2 are invalid and never able to be used. Essentially this is an array of 3 useable elements. 
Let's use 2 elements:

```c
my_array = [INT, BOOL, VOID, ERROR, ERROR]
```

Now, we have an array with 2 used elements, 1 usable but not yet used, and 2 invalid elements. We can add one more element, but no more than that as we would be overwriting an ERROR element. In the above example, the array has a `MAX_ELEMENT` of 5 but a **capacity** of 3.

What we have above is the `generic_array_chunk_t`. Effectively this struct is an array of a `MAX_ELEMENTS_PER_CHUNK` number of `generic_element_t`'s. This struct also holds a **capacity** which dictates how many elements of that array are valid and usable.

Now, let us create a new chunk with 10 `MAX_ELEMENTS`, a capacity of 5, and 0 initial elements. We would get:

```c
[VOID, VOID, VOID, VOID, VOID, ERROR, ERROR, ERROR, ERROR, ERROR]
```

If instead we created a new chunk with `MAX_ELEMENTS` of 10, a capacity of 5, and initialzed the first 2 elements as `INT`s, we would get:

```C
[INT, INT, VOID, VOID, VOID, ERROR, ERROR, ERROR, ERROR, ERROR]
```

Let's now take it up another step; consider having multiple of these chunks. Each chunk has a different capacity, but all chunks work together as a contiguous array. 
For example, consider I have three chunks:

- chunk A with MAX_ELEMENTS = 5, capacity = 5
- chunk B with MAX_ELEMENTS = 5, capacity = 3
- chunk C with MAX_ELEMENTS = 5, capacity = 2

They would look like this:

```c
chunk A = [VOID, VOID, VOID,  VOID,  VOID]
chunk B = [VOID, VOID, VOID,  ERROR, ERROR]
chunk C = [VOID, VOID, ERROR, ERROR, ERROR]
```

Despite chunk A only having 5 usable elements, chunk B 3 usable elements, and chunk C 2 usable elements, if they worked together we could store 10 elements in the VOID spots.

The `generic_array_t` struct is a collection of these chunks. It has a maximum number of chunks, only some of which are actually used, while the others have capacity 0 / are fully invalid. 

For example, let's consider a new generic array with a `MAX_NUM_CHUNKS` of 5 chunks, 3 of which are used, and those three chunks having capacities of 5, 3, and 2 respectively. We would get:

```C
array = [
    chunk[0] = [VOID,  VOID,  VOID,  VOID,  VOID ]
    chunk[1] = [VOID,  VOID,  VOID,  ERROR, ERROR]
    chunk[2] = [VOID,  VOID,  ERROR, ERROR, ERROR]
    chunk[3] = [ERROR, ERROR, ERROR, ERROR, ERROR]
    chunk[4] = [ERROR, ERROR, ERROR, ERROR, ERROR]
]
```

This array effectively has 10 usable elements, and are indexed from 0 to 9 like a usual array. The indexing of this array however is a bit nuanced due to the ERROR elements and chunking behavior. 

Consider element at array index 6. Chunk 0 has a capacity of 5, so the value for that index must not reside here. Moving on to the chunk 1, we find that `6 (index) - 5 (capacity) = 1 (index in next chunk)` which is within this chunk's range. If we change this element to a BOOL value, we would get:

```C
array = [
    chunk[0] = [VOID,  VOID,  VOID,  VOID,  VOID ]
    chunk[1] = [VOID, *BOOL*, VOID,  ERROR, ERROR]
    chunk[2] = [VOID,  VOID,  ERROR, ERROR, ERROR]
    chunk[3] = [ERROR, ERROR, ERROR, ERROR, ERROR]
    chunk[4] = [ERROR, ERROR, ERROR, ERROR, ERROR]
]
```

Your task is to implement the functions to manipulate and use this array.

## Accepting the Assignment

[Click here to accept this assignment](https://classroom.github.com/a/sN9UblLs). Once you accept the invitation, GitHub will create a private repository containing the starter files for the assignment. You will also find an accompanying `README.md`, which is essentially this document you are reading, useful for instructions and further details.

## Assignment File Structure

There are several files in this repository. You only need to read `genarraylib.h` and edit `genarraylib.c` to finish the assignment. The following is a description of all files. **You can read the first two and skip the rest.**

### **genarraylib.h**

This file is a *header* file (due to the `.h` file extension). This file contains:
- **Struct definitions**: `GenericElement`, `GenericArrayChunk`, `GenericArray`, and `GenericArrayResponse` structs for storing type values, an array of types, an array of array of types, and a response object, respectively.
- **Enum definitions**: `GenericType` enum for differentiating between different types, `Limits` enum for storing array maximums used in the structs
- **Function declarations**: Forward declarations for all the functions you will implement
- **Detailed documentation**: Comprehensive explanations of what each function should do


Do **NOT** make any changes or edits in this file -- when we grade your project, we will use the original (unchanged) header file, so if you change it you might break your own code in our environment. Simply put, do not edit it.

### **genarraylib.c**

**This is the only file you should be modifying in this assignment.**

This file is a *source* file written in C (due to the `.c` file extension). This file implements the functions declared in `genarraylib.h`. Inside, you will write your own C code in place of the `TODO` comments based on the descriptions you read in `genarraylib.h`. Additionally, we would like you to adhere to good programming practices; i.e. include comments while writing your code. This will help you understand your code long after writing it and will also help others, such as course staff, to be able to read your code and understand your thought process. To encourage the use of comments, a percentage of this assignment will be based on whether you have written effective comments (telling us what you are doing in the code, why you write the code in this way, etc.).

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

You don't need to read this file to finish this assignment. 

This source file "imports" your functions in `genarraylib.c` and tests the correctness of your code. The test cases in this file are more or less what we will use to test your code. While test case *values* may change, the test cases themselves will not. Because of this, do NOT hardcode answers for the inputs in the test cases. They will slightly change. Your code should work for all valid inputs.

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
gcc genarraylib.c driver.c -o test
```

If your code has no syntax errors, an executable file called `test` will appear in your directory. If there are syntax errors, carefully read the errors or warnings to understand what line of code is causing the syntax error. It will tell you.

### Testing / Running

To test your code and see your proposed grade, run:

```shell
./test
```

to start the tests. You will see results in your terminal like so:

```
Test No.72:
Testing is_equal_element with two identical int elements
[ERROR] Test No.72 failed. Expected true, but got false.

Test No.73:
Testing is_equal_element with different int elements
Test No.73 passed

Test No.74:
Creating identical array for comparison
[ERROR] Test No.74 failed. Expected true, but got false.

Test No.75:
Modifying element in arr2 and comparing
Test No.75 passed

-------------------Summary-------------------
Total tests: 433
Total tests passed: 89
Total tests failed: 344
--------------------------------
Function create_generic_element failed
Function is_equal_element failed
Function create_generic_array_chunk failed
Function create_generic_array failed
Function get_element failed
Function set_element_int failed
Function set_element_float failed
Function set_element_double failed
Function set_element_char failed
Function set_element_short failed
Function set_element_bool failed
Function set_element_string failed
Function is_equal failed
Function full_scope failed
Function create_generic_array_with_elements failed
Total functions passed: 0
points for correctness: 0.000000/23, the other 2 points are for comments and git commits, please read the document for more details.
```

You can scroll through the output to find the function results you care about / want to test. 

For example, the output below tells you function `set_element_int` failed. For input `42`, it outputs `0` (which appears to be uninitialized). However, the expected output shoud be what was written, `42`.

```
Test No.1:
Setting element at index 0 to 42
[ERROR] Test No.3 failed. Expected 42, but got 0.
```

After you change your code, rerun `gcc genarraylib.c driver.c -o test` to recompile and then rerun `./test` to test your updated code.

If you are confused why your algorithm may not be working, feel free to use `printf` statements to debug your program. Use the following cheat sheet to help you print:

```c
printf("this will print a message \n");
printf("this will print an integer %d \n", myint);
printf("this will print a float %f \n", myfloat);
printf("this will print a char %c \n", mychar);
printf("this will print a string %s \n", mystring);
```

---

## Rubric

This assignment is worth 25 points. There are 13 functions you need to implement in `genarraylib.c` for a total of 23 points total. We have 15 test functions in `driver.c` file (The are two more test functions than the number of functions you need to implement in `genarraylib.c`, because `full_scope` tests all functions and `create_array` is split into two different tests). Each function is assigned a point value described below:

- `create_generic_element` - 1 pt
- `is_equal_element` - 1 pts
- `create_generic_array_chunk` - 1 pt
- `create_generic_array` - 2 pts
- `create_generic_array (with elements)` - 2 pt
- `get_element` - 2 pts
- `set_element_int` - 1 pt
- `set_element_float` - 1 pt
- `set_element_double` - 1 pt
- `set_element_char` - 1 pt
- `set_element_short` - 1 pt
- `set_element_bool` - 1 pt
- `set_element_string` - 1 pt
- `is_equal` - 2 pts
- `full_scope` - 5 pt


You will get points for a certain function only if you pass **all** the test cases for that function. For example, in the test output above, you will get 0 points for function `set_element_int` if you failed any of the test cases. 

The remaining 2 points of this assignment come from your effective use of comments (1 pts) and your `git` commits (1 pts). For the `git` commits, you should be regularly committing your progress. A decent rule of thumb would be to make a commit after implementing each function. You should have at least 7 commits in this repository to get the full 1 points.

For late submissions, please refer to syllabus or Canvas website.

## Turnin

Go to Canvas, start the quiz, and submit a commit ID as a single line in the submission box.
