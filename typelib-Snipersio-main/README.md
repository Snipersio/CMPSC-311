# typelib.c Programming Assignment

In this assignment, you are going to implement 14 functions in `typelib.c` according to the requirements in `typelib.h`. You will get experience working with structs, unions, typedefs, and enums in C to build a grade management system.

## Accepting the Assignment

[Click here to accept this assignment](https://classroom.github.com/a/TXDMeGRS). Once you accept the invitation, GitHub will create a private repository containing the starter files for the assignment. You will also find an accompanying `README.md`, which is essentially this document you are reading, useful for instructions and further details.

## Assignment File Structure

There are several files in this repository. You only need to read `typelib.h` and edit `typelib.c` to finish the assignment. The following is a description of all files. **You can read the first two and skip the rest.**

### **typelib.h**

This file is a *header* file (due to the `.h` file extension). This file contains:
- **Struct definitions**: `Student`, `Course`, and `Grade` structs for managing student and course data
- **Union definitions**: `GradeValue` union for storing either percentage or letter grades
- **Enum definitions**: `LetterGrade` enum for representing letter grades, `Limits` enum for storing array maximums used in the structs
- **Function declarations**: Forward declarations for all the functions you will implement
- **Detailed documentation**: Comprehensive explanations of what each function should do

Do **NOT** make any changes or edits in this file -- when we grade your project, we will use the original (unchanged) header file, so if you change it you might break your own code in our environment. Simply put, do not edit it.

### **typelib.c**

**This is the only file you should be modifying in this assignment.**

*It is recommended to implement functions in the order they are shown, as the tester will use your own code progressively.*

This file is a *source* file written in C (due to the `.c` file extension). This file implements the functions declared in `typelib.h`. Inside, you will write your own C code in place of the `TODO` comments based on the descriptions you read in `typelib.h`. Additionally, we would like you to adhere to good programming practices; i.e. include comments while writing your code. This will help you understand your code long after writing it and will also help others, such as course staff, to be able to read your code and understand your thought process. To encourage the use of comments, a percentage of this assignment will be based on whether you have written effective comments (telling us what you are doing in the code, why you write the code in this way, etc.).

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

This source file "imports" your functions in `typelib.c` and tests the correctness of your code. The test cases in this file are more or less what we will use to test your code. While test case *values* may change, the test cases themselves will not. Because of this, do NOT hardcode answers for the inputs in the test cases. They will slightly change. Your code should work for all valid inputs.

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
gcc typelib.c driver.c -o test
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
Testing percentage_to_letter...

Test No.1 Input: 98.500000
Test No.1 passed

Test No.2 Input: 95.000000
[ERROR] Test No.2 failed. Expected 0, but got 13.

Test No.3 Input: 92.000000
[ERROR] Test No.3 failed. Expected 1, but got 13.

...

-------------------Summary-------------------
Total tests: 167
Total tests passed: 26
Total tests failed: 141
--------------------------------
Function percentage_to_letter failed
Function letter_to_percentage failed
Function letter_to_gpa failed
Function calculate_student_average failed
Function find_top_student failed
Function calculate_class_average failed
Function count_students_by_year failed
Function is_course_at_capacity failed
Function calculate_total_credit_hours failed
Function find_most_popular_course failed
Function create_student failed
Function create_course failed
Function add_student_to_course failed
Function find_student_in_course failed
Total functions passed: 0
points for correctness: 0.000000/22, the other 3 points are for comments and git commits, please read the document for more details.
```

You can scroll through the output to find the function results you care about / want to test. 

For example, the output below tells you function `percentage_to_letter` failed. For input `95.0`, it outputs `13` (which is `GRADE_INVALID`). However, the expected output should be `0` (which is `GRADE_A`).

```
Test No.2 Input: 95.000000
[ERROR] Test No.2 failed. Expected 0, but got 13.
```

After you change your code, rerun `gcc typelib.c driver.c -o test` to recompile and then rerun `./test` to test your updated code.

If you are confused why your algorithm may not be working, feel free to use `printf` statements to debug your program. Use the following cheat sheet to help you print:

```c
printf("this will print a message \n");
printf("this will print an integer %d \n", myint);
printf("this will print a float %f \n", myfloat);
```

---

## Rubric

This assignment is worth 25 points. There are 14 functions you need to implement in `typelib.c` for a total of 22 points total. Each function is assigned a point value described below:

- `percentage_to_letter` - 0.5 pts
- `letter_to_percentage` - 0.5 pts  
- `letter_to_gpa` - 0.5 pts
- `calculate_student_average` - 2.0 pts
- `find_top_student` - 2.0 pts
- `calculate_class_average` - 3.0 pts
- `count_students_by_year` - 2.0 pts
- `is_course_at_capacity` - 0.5 pts
- `calculate_total_credit_hours` - 1.0 pts
- `find_most_popular_course` - 2.0 pts
- `create_student` - 2.0 pts
- `create_course` - 2.0 pts
- `add_student_to_course` - 2.0 pts
- `find_student_in_course` - 2.0 pts

You will get points for a certain function only if you pass **all** the test cases for that function. For example, in the test output above, you will get 0 points for function `percentage_to_letter` since you failed all of the test cases. 

The remaining 3 points of this assignment come from your effective use of comments (2 pts) and your `git` commits (1 pts). For the `git` commits, you should be regularly committing your progress. A decent rule of thumb would be to make a commit after implementing each function. You should have at least 7 commits in this repository to get the full 1 points.

For late submissions, please refer to syllabus or Canvas website.

## Turnin

Go to Canvas, start the quiz, and submit a commit ID as a single line in the submission box.


