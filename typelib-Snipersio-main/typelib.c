#include "typelib.h"
#include <stdio.h>
#include <string.h>

/*
This is the implementation of the functions in typelib.h. 
Please read the comments in typelib.h before you start working on this file.
*/

letter_grade_t percentage_to_letter(float percentage) { //Returns letter grade with percentage input
    if (percentage > 100 || percentage < 0) return GRADE_INVALID; //Invalid Cases
    if (percentage >= 97) return GRADE_A_PLUS; //Direct translations returned for percentage ranges...
    if (percentage >= 93) return GRADE_A;
    if (percentage >= 90) return GRADE_A_MINUS;
    if (percentage >= 87) return GRADE_B_PLUS;
    if (percentage >= 83) return GRADE_B;
    if (percentage >= 80) return GRADE_B_MINUS;
    if (percentage >= 77) return GRADE_C_PLUS;
    if (percentage >= 73) return GRADE_C;
    if (percentage >= 70) return GRADE_C_MINUS;
    if (percentage >= 67) return GRADE_D_PLUS;
    if (percentage >= 63) return GRADE_D;
    if (percentage >= 60) return GRADE_D_MINUS;
    return GRADE_F;
}

float letter_to_percentage(letter_grade_t letter) { //Returns percentage grades with letter inputs
    if (letter == GRADE_A_PLUS) return 98.5f; //Direct translations returned for letter ranges...
    if (letter == GRADE_A) return 95.0f;
    if (letter == GRADE_A_MINUS) return 91.5f;
    if (letter == GRADE_B_PLUS) return 88.5f;
    if (letter == GRADE_B) return 85.0f;
    if (letter == GRADE_B_MINUS) return 81.5f;
    if (letter == GRADE_C_PLUS) return 78.5f;
    if (letter == GRADE_C) return 75.0f;
    if (letter == GRADE_C_MINUS) return 71.5f;
    if (letter == GRADE_D_PLUS) return 68.5f;
    if (letter == GRADE_D) return 65.0f;
    if (letter == GRADE_D_MINUS) return 61.5f;
    if (letter == GRADE_F) return 30.0f;
    return -1.0f; //Invalid cases

}

float letter_to_gpa(letter_grade_t letter) { //Translates letter grade to GPA equiavalent
    // TODO: Convert letter grade to GPA value (4.0 scale)
    if (letter == GRADE_A_PLUS) return 4.0f; //Direct translations returned of GPA when given letter grade...
    if (letter == GRADE_A) return 4.0f;
    if (letter == GRADE_A_MINUS) return 3.7f;
    if (letter == GRADE_B_PLUS) return 3.3f;
    if (letter == GRADE_B) return 3.0f;
    if (letter == GRADE_B_MINUS) return 2.7f;
    if (letter == GRADE_C_PLUS) return 2.3f;
    if (letter == GRADE_C) return 2.0f;
    if (letter == GRADE_C_MINUS) return 1.7f;
    if (letter == GRADE_D_PLUS) return 1.3f;
    if (letter == GRADE_D) return 1.0f;
    if (letter == GRADE_D_MINUS) return 0.7f;
    if (letter == GRADE_F) return 0.0f;
    return -1.0f; //Invalid cases
}

student_t create_student(int student_id, const char first_name[], const char last_name[], int year_level, bool is_suspended) { //Creating a new student
    // TODO: Create a new student with the given parameters
	student_t student = { //inputted student atribute
        .student_id = student_id, //inputed stident_id attribute
        .year_level = year_level, //inputted year_level attribute
        .is_suspended = is_suspended //inputted is_suspended attribute
    };
    strncpy(student.first_name, first_name, MAX_FIRST_NAME_LENGTH - 1); //Copy student,first_name and formatting correctly
    student.first_name[MAX_FIRST_NAME_LENGTH - 1] = '\0'; //Setting student.first_name to inputted first name with proper edge case
    strncpy(student.last_name, last_name, MAX_LAST_NAME_LENGTH - 1); //Copy student.last_name and formatting correctly
    student.last_name[MAX_LAST_NAME_LENGTH - 1] = '\0'; //Setting student.last_name to inputted last name with proper edge cases
    return student; //Returns new student structure
}

course_t create_course(const char course_code[], const char course_name[], int credits, int max_students) { //Creates new course struc with inputted information
    // TODO: Create a new course with the given parameters
    course_t course = { //Setting all inputed attributes to struct attributes that don't need string editing
        .credits = credits,
        .max_students = max_students,
        .current_enrollment = 0
    };
    strncpy(course.course_code, course_code, MAX_COURSE_CODE_LENGTH - 1); //Copy course_code and formatting correctly
    course.course_code[MAX_COURSE_CODE_LENGTH - 1] = '\0'; //Setting course-coure_code to the inputted course code with proper edge case
    strncpy(course.course_name, course_name, MAX_COURSE_NAME_LENGTH - 1); //Copy course_name and formatting correctlly
    course.course_name[MAX_COURSE_NAME_LENGTH - 1] = '\0'; //Setting course.course_name to the inputted course code with proper edge case
    return course; //returning new course struc
}

bool is_course_at_capacity(course_t course) { //Returns if inputted course it all full capacity
    // TODO: Check if course enrollment equals max capacity
    return (course.current_enrollment >= course.max_students); //Returns true of if the maximum students or equal or less tha the number of students currently enrolled in course
}

course_t add_student_to_course(course_t course, student_t student) {
    int capacity = course.max_students;
    if (capacity < 0) capacity = 0; //capacity can't be negative
    if (capacity > MAX_STUDENTS) capacity = MAX_STUDENTS; //Just in case an incorrect max students was inputted
    if (course.current_enrollment < 0) { //Checking different cases for if the currently enrolled amount of students is below or eqal to the capacity
        course.current_enrollment = 0;
    } else if (course.current_enrollment > capacity) {
        course.current_enrollment = capacity;
    }
    if (course.current_enrollment >= capacity) {//Aready at capacity
        return course;
    }
    for (int i = 0; i < course.current_enrollment; i++) { //In case there are duplicate students
        if (course.students[i].student_id == student.student_id) {
            return course; //Already enrolled into it so just return
        }
    }
    course.students[course.current_enrollment] = student;//Add new student
    course.current_enrollment += 1; //Increment by 1
    return course;
}

float calculate_student_average(grade_t grades[], int num_grades, int student_id) {
    // TODO: Calculate average percentage grade for a specific student
    if (grades == NULL || num_grades <= 0) { //making sure given lenghts of grades make sense
        return -1.0f;
    }
    float current = 0.0f; //Current student grade counter
    int counter = 0; //Counter
    for (int i = 0; i < num_grades; i++) { //Incrementing through number of grades 
        if (grades[i].student_id == student_id) { //If a grade in grade has the same student_id as the inputted one
            float value;
            if (grades[i].is_percentage) {
                value = grades[i].grade.percentage; //If grade for student is a percentage than that's the grade of the student
            } else {
                value = letter_to_percentage(grades[i].grade.letter); //If grade for student is a letter than using previously made function letter_to_percentage to convert it into a float percentage
            }
            if (value >= 0.0f) { // skip invalid grades
                current += value; //Sums up all the values when itterating
                counter++; //Increments by 1
            }
        }
    }

    if (counter == 0) {
        return -1.0f; // no grades found for this student
    }
    return current / counter;
}

int find_top_student(grade_t grades[], int num_grades, student_t students[], int num_students) {
    // TODO: Find student with highest average grade
    if (students == NULL || num_students <= 0) {
        return -1;
    }
    float top_average = -1.0f;
    int top_id = -1;
    for (int i = 0; i < num_students; i++) {
        if (students[i].is_suspended) {
            continue; // skip suspended students
        }
        float average = calculate_student_average(grades, num_grades, students[i].student_id);
        if (average < 0.0f) {
            continue; // no valid grades for this student
        }
        // Tie-breaker: keep the first student with the top average (strictly greater only)
        if (average > top_average) {
            top_average = average;
            top_id = students[i].student_id;
        }
    }
    return top_id; // -1 if none found
}

float calculate_class_average(grade_t grades[], int num_grades, student_t students[], int num_students) {
    // TODO: Calculate average grade for entire class
     if (students == NULL || num_students <= 0) {
        return -1.0f; // no students to average
    }
    float sum = 0.0f;
    int counter = 0;
    for (int i = 0; i < num_students; i++) {
        if (students[i].is_suspended) {
            continue; // skip suspended students
        }
        float avg = calculate_student_average(grades, num_grades, students[i].student_id);
        if (avg < 0.0f) {
            continue; // this student has no valid grades
        }
        sum += avg;
        counter++;
    }
    if (counter == 0) {
        return -1.0f; // no valid students/grades
    }
    return (sum/counter);
}

int count_students_by_year(student_t students[], int num_students, int year_level) {
    // TODO: Count enrolled students in specific year level
    if (num_students == 0){
	    return 0; //Nothing is said for non complient cases so I just did this
    }
    int count = 0;
    for (int i = 0; i < num_students; i++){ //Iterate through all students in inputted students array
	    if (students[i].is_suspended){ //If suspended than not counted
		    continue;
	    }
	    if(students[i].year_level == year_level){ //If currently itterated student is the desired year then we up the student counter
		    count += 1;
	    }
    }
    return count; //Returns student counter of the amount of students in a specific year that are not suspended
}

int calculate_total_credit_hours(course_t course) {
    // TODO: Calculate total credit hours for enrolled students
    if (course.current_enrollment == 0){ //If no one is taking the course
	    return 0;
    }
    int counter = 0;
    for (int i = 0; i < course.current_enrollment; i++){ //Iteratting students struc by the amount of students that are enrolled in the course
	    if (course.students[i].is_suspended){ //Don't count suspended students
		    continue;
	    }
	    counter ++;
    }
    return (counter * course.credits); //The total number of students in the course multipled by the course credit hours gives the total credit hours
}

int find_most_popular_course(course_t courses[], int num_courses) {
    // TODO: Find course with highest enrollment
    if (num_courses < 1){ //Checking if no courses are given
	    return -1;
    }
    int TopIndex = -1;
    int TopValue = 0;
    for (int i = 0; i < num_courses; i++){ //Iteratting through all courses in inputted courses array
	    if (courses[i].current_enrollment > TopValue){ //If currently iteratting course as a higher enrollment than what ever is currently the top enrollement value
		    TopIndex = i; //Makes new top currently enrolled course the one that is currently being itterated on
		    TopValue = courses[i].current_enrollment; //New value is the new top currently enrolled course
	    }
    }
    return TopIndex; //Returns the index of the course with the most enrollments

}

int find_student_in_course(course_t course, int student_id) {
    // TODO: Find student in course by student ID
    for (int i = 0; i < course.current_enrollment; i++){ //Itterates through all the students in the students array within the course struc
	    if (course.students[i].student_id == student_id){ //If a student is enrolled in the course
		    return i; //returns index of student found in the course
	    }
    }
    return -1; //If no student is found
}
