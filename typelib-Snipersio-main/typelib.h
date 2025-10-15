#include <stdbool.h>

/* 
This is a header file for typelib.c. It has the declarations of the functions in typelib.c and the comments
explaining what the functions do. Please read this file carefully before you start working to complete 
the code in typelib.c. Please do NOT make any changes to this file.

This project focuses on structs, unions, typedefs, and enums. You will work with student and course data
structures to implement a grade management system. All structs are passed and returned by value!

For all of the following functions, you do not need to worry about integer overflow (i.e., a number is too
large to be stored in an int type variable), since the values in test cases are not so large.
*/

// Enum for letter grades
typedef enum LetterGrade {
    GRADE_A_PLUS = 0,
    GRADE_A,
    GRADE_A_MINUS,
    GRADE_B_PLUS,
    GRADE_B,
    GRADE_B_MINUS,
    GRADE_C_PLUS,
    GRADE_C,
    GRADE_C_MINUS,
    GRADE_D_PLUS,
    GRADE_D,
    GRADE_D_MINUS,
    GRADE_F,
    GRADE_INVALID
} letter_grade_t;

// Enum for struct limits
typedef enum Limits {
    MAX_STUDENTS = 300,
    MAX_COURSE_NAME_LENGTH = 64,
    MAX_COURSE_CODE_LENGTH = 16,
    MAX_FIRST_NAME_LENGTH = 32,
    MAX_LAST_NAME_LENGTH = 32,
    MAX_ASSIGNMENT_NAME_LENGTH = 64
} limits_t;


// Union for storing different types of grades
typedef union GradeValue {
    float percentage;    // For percentage grades (0.0 to 100.0)
    letter_grade_t letter;  // For letter grades
} grade_value_t;


// Struct representing a grade entry
typedef struct Grade {
    int student_id;      // Student ID number
    char assignment_name[MAX_ASSIGNMENT_NAME_LENGTH]; // Name of the assignment
    grade_value_t grade;    // The actual grade (percentage or letter)
    bool is_percentage;  // true if grade is percentage, false if letter grade
} grade_t;

// Struct representing a student
typedef struct Student {
    int student_id;      // Unique student ID
    char first_name[MAX_FIRST_NAME_LENGTH]; // Student's first name
    char last_name[MAX_LAST_NAME_LENGTH];  // Student's last name
    int year_level;      // Year level (1=freshman, 2=sophomore, 3=junior, 4=senior)
    bool is_suspended;   // Whether student is suspended from the University.
} student_t;

// Struct representing a course
typedef struct Course {
    char course_code[MAX_COURSE_CODE_LENGTH]; // Course code like "CMPSC311"
    char course_name[MAX_COURSE_NAME_LENGTH]; // Full course name
    int credits;         // Number of credit hours
    int max_students;    // Maximum number of students allowed for this course
    int current_enrollment; // Current number of enrolled students
    student_t students[MAX_STUDENTS]; // Array of students (global limit for any course is MAX_STUDENTS)
} course_t;

/*
This function converts a percentage grade to a letter grade.
@param percentage: A float between 0.0 and 100.0 representing the percentage grade
@return: The corresponding letter_grade_t enum value
@examples:
percentage = 95.5 -> return GRADE_A_PLUS
percentage = 87.0 -> return GRADE_B_PLUS  
percentage = 76.5 -> return GRADE_C_PLUS
percentage = 65.0 -> return GRADE_D_PLUS
percentage = 45.0 -> return GRADE_F
percentage = 101.0 -> return GRADE_INVALID
percentage = -5.0 -> return GRADE_INVALID

Grading scale:
- [97.0-100.0] A+
- [93.0-97.0) A
- [90.0-93.0) A-
- [87.0-90.0) B+
- [83.0-87.0) B
- [80.0-83.0) B-
- [77.0-80.0) C+
- [73.0-77.0) C
- [70.0-73.0) C-
- [67.0-70.0) D+
- [63.0-67.0) D
- [60.0-63.0) D-
- [0.0-60.0) F
*/
letter_grade_t percentage_to_letter(float percentage);

/*
This function converts a letter grade to a percentage grade.
@param letter: A letter_grade_t enum value
@return: The middle percentage value for that letter grade range
@examples:
letter = GRADE_A_PLUS -> return 98.5
letter = GRADE_A -> return 95.0
letter = GRADE_B_PLUS -> return 88.5
letter = GRADE_C -> return 75.0
letter = GRADE_D -> return 65.0
letter = GRADE_F -> return 30.0
letter = GRADE_INVALID -> return -1.0
*/
float letter_to_percentage(letter_grade_t letter);

/*
This function calculates the GPA value for a letter grade.
@param letter: A letter_grade_t enum value
@return: The GPA value (4.0 scale)
@examples:
letter = GRADE_A_PLUS -> return 4.0
letter = GRADE_A -> return 4.0
letter = GRADE_A_MINUS -> return 3.7
letter = GRADE_B_PLUS -> return 3.3
letter = GRADE_B -> return 3.0
letter = GRADE_B_MINUS -> return 2.7
letter = GRADE_C_PLUS -> return 2.3
letter = GRADE_C -> return 2.0
letter = GRADE_C_MINUS -> return 1.7
letter = GRADE_D_PLUS -> return 1.3
letter = GRADE_D -> return 1.0
letter = GRADE_D_MINUS -> return 0.7
letter = GRADE_F -> return 0.0
letter = GRADE_INVALID -> return -1.0
*/
float letter_to_gpa(letter_grade_t letter);

/*
This function creates a new student with the given parameters.
@param student_id: Unique student ID
@param first_name: Student's first name (to be copied)
@param last_name: Student's last name (to be copied)
@param year_level: Year level (1=freshman, 2=sophomore, 3=junior, 4=senior)
@param is_suspended: Whether student is suspended from the University
@return: A student_t struct with the given parameters
@examples:
create_student(123, "John", "Doe", 2, false) -> returns {123, "John", "Doe", 2, false}
create_student(456, "Jane", "Smith", 1, true) -> returns {456, "Jane", "Smith", 1, true}
@hint: Use strcpy or strncpy to copy the name strings into the struct fields, or iterate yourself.
*/
student_t create_student(int student_id, const char first_name[], const char last_name[], int year_level, bool is_suspended);

/*
This function creates a new course with the given parameters.
@param course_code: Course code like "CMPSC311" (to be copied)
@param course_name: Full course name (to be copied)
@param credits: Number of credit hours
@param max_students: Maximum number of students allowed for this course
@return: A course_t struct with the given parameters and empty students array
@examples:
create_course("CMPSC311", "Data Structures", 3, 30) -> returns course with empty students array
create_course("CMPSC312", "Algorithms", 4, 25) -> returns course with empty students array
@hint: Use strcpy to copy the course code and name strings into the struct fields, or iterate yourself.
*/
course_t create_course(const char course_code[], const char course_name[], int credits, int max_students);

/*
This function checks if a course is at capacity.
@param course: A course_t struct
@return: true if course is at capacity, false otherwise
@examples:
course = {max_students:30, current_enrollment:30} -> return true
course = {max_students:30, current_enrollment:25} -> return false
course = {max_students:50, current_enrollment:0} -> return false
*/
bool is_course_at_capacity(course_t course);

/*
This function adds a student to a course's student array regardless of whether they are suspended or not.
@param course: A course_t struct
@param student: A student_t struct to add to the course
@return: Updated course_t struct with student added, or original course if at capacity
@examples:
course = {current_enrollment:0, max_students:30}, student = {student_id:123} -> returns course with student added
course = {current_enrollment:30, max_students:30}, student = {student_id:456} -> returns original course unchanged
*/
course_t add_student_to_course(course_t course, student_t student);

/*
This function calculates the average percentage grade for a student across multiple grades. 
There may be grades for other students in the array, only consider grades for the student with the given student_id!
Grades are not guaranteed to be all percentages or letter grades. Code accordingly!
@param grades: Array of grade_t structs
@param num_grades: Number of grades in the array
@param student_id: The student ID to calculate average for
@return: The average percentage grade, or -1.0 if student has no grades or invalid input
@examples:
grades = [{id:123, grade:85.0, is_percentage:true}, {id:123, grade:90.0, is_percentage:true}, {id:456, grade:75.0, is_percentage:true}], num_grades=3, student_id=123 -> return 87.5
grades = [{id:123, grade:GRADE_B, is_percentage:false}, {id:123, grade:GRADE_A, is_percentage:false}], num_grades=2, student_id=123 -> return 91.5 (average of 83.0 and 95.0)
grades = [], num_grades=0, student_id=123 -> return -1.0
grades = [{id:456, grade:85.0, is_percentage:true}], num_grades=1, student_id=123 -> return -1.0
*/
float calculate_student_average(grade_t grades[], int num_grades, int student_id);

/*
This function finds a non-suspended student with the highest average grade in a course.
@param grades: Array of grade_t structs
@param num_grades: Number of grades in the array
@param students: Array of student_t structs
@param num_students: Number of students in the array
@return: The student_id of the student with the highest average, or -1 if no valid students found
@examples:
grades = [{id:123, grade:85.0, is_percentage:true}, {id:456, grade:90.0, is_percentage:true}, {id:123, grade:95.0, is_percentage:true}]
students = [{id:123, first_name:"John", last_name:"Doe", is_suspended:false}, {id:456, first_name:"Jane", last_name:"Smith", is_suspended:false}]
-> return 123 (John has average 90.0, Jane has average 90.0, but John appears first)
@hint: You can use the `calculate_student_average` function to calculate the average grade for each student.
*/
int find_top_student(grade_t grades[], int num_grades, student_t students[], int num_students);

/*
This function calculates the class average for all non-suspended students in a course.
@param grades: Array of grade_t structs
@param num_grades: Number of grades in the array
@param students: Array of student_t structs
@param num_students: Number of students in the array
@return: The average percentage grade for the entire class, or -1.0 if no valid grades found
@examples:
grades = [{id:123, grade:85.0, is_percentage:true}, {id:456, grade:90.0, is_percentage:true}, {id:123, grade:95.0, is_percentage:true}]
students = [{id:123, first_name:"John", last_name:"Doe", is_suspended:false}, {id:456, first_name:"Jane", last_name:"Smith", is_suspended:false}]
-> return 90.0 (average of 90.0 and 90.0)
*/
float calculate_class_average(grade_t grades[], int num_grades, student_t students[], int num_students);

/*
This function counts how many students are non-suspended in a specific year level.
@param students: Array of student_t structs
@param num_students: Number of students in the array
@param year_level: The year level to count (1=freshman, 2=sophomore, 3=junior, 4=senior)
@return: The number of enrolled, non-suspended students in that year level
@examples:
students = [{year_level:1, is_suspended:false}, {year_level:1, is_suspended:true}, {year_level:2, is_suspended:false}], num_students=3, year_level=1 -> return 1
students = [{year_level:3, is_suspended:false}, {year_level:3, is_suspended:false}, {year_level:4, is_suspended:false}], num_students=3, year_level=3 -> return 2
*/
int count_students_by_year(student_t students[], int num_students, int year_level);

/*
This function calculates the total credit hours for all enrolled and non-suspended students in a course.
Uses the students array within the course struct to count enrolled students.
@param course: A course_t struct containing its own students array
@return: The total credit hours (course.credits * number of enrolled students in course.students), or 0 if no students enrolled
@examples:
course = {credits:3, students:[{is_suspended:false}, {is_suspended:true}, {is_suspended:false}], current_enrollment:3} -> return 6 (3 credits * 2 non-suspended students)
course = {credits:4, students:[], current_enrollment:0} -> return 0
*/
int calculate_total_credit_hours(course_t course);

/*
This function finds the course with the highest enrollment among multiple courses.
@param courses: Array of course_t structs
@param num_courses: Number of courses in the array
@return: The index of the course with highest enrollment, or -1 if no courses or all have 0 enrollment
@examples:
courses = [{current_enrollment:25}, {current_enrollment:30}, {current_enrollment:20}], num_courses=3 -> return 1
courses = [{current_enrollment:0}, {current_enrollment:0}], num_courses=2 -> return -1
courses = [{current_enrollment:15}], num_courses=1 -> return 0
*/
int find_most_popular_course(course_t courses[], int num_courses);

/*
This function finds a student in a course by student ID.
@param course: A course_t struct
@param student_id: The student ID to search for
@return: Index of the student in course.students array if found, -1 if not found
@examples:
course = {students:[{student_id:123}, {student_id:456}], current_enrollment:2}, student_id=123 -> return 0
course = {students:[{student_id:123}, {student_id:456}], current_enrollment:2}, student_id=789 -> return -1
*/
int find_student_in_course(course_t course, int student_id);
