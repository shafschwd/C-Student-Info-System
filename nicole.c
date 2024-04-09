#include <stdio.h>
#include <string.h>

// Define a structure for student
struct Student {
    char name[50];
    char enrolledCourses[5][50];
    int grades[5];
    float CGPA;
    int attendance[5];
};

// Function prototypes
void viewPersonalDetails(struct Student student);
void viewEnrolledCourses(struct Student student);
void viewGrades(struct Student student);
void viewCGPA(struct Student student);
void viewAttendance(struct Student student);

int main() {
    // Sample data of students (can be replaced with a database)
    struct Student students[6] = {
            {"John", {"Math", "English", "Physics", "History", "Biology"}, {90, 85, 75, 80, 95}, 0, {95, 90, 85, 80, 75}},
            {"Jane", {"Chemistry", "Literature", "Computer Science", "Geography", "Art"}, {85, 80, 95, 90, 75}, 0, {90, 85, 80, 95, 70}},
            {"Alice", {"Economics", "Psychology", "Calculus II", "Anthropology", "Political Science"}, {88, 92, 85, 90, 87}, 0, {85, 90, 88, 85, 80}},
            {"Bob", {"Math", "Physics", "Chemistry", "Biology", "Geology"}, {92, 85, 90, 87, 82}, 0, {90, 85, 88, 82, 85}},
            {"Emily", {"History", "Literature", "Geography", "Political Science", "Economics"}, {80, 85, 90, 87, 88}, 0, {85, 90, 88, 82, 85}},
            {"David", {"Computer Science", "Math", "Physics", "Chemistry", "Biology"}, {85, 90, 87, 82, 88}, 0, {90, 85, 88, 82, 85}}
    };

    // Menu-driven system
    int choice;
    int studentIndex;
    do {
        printf("\n-- Student Information System --\n");
        printf("Select a student:\n");
        printf("0. John\n");
        printf("1. Jane\n");
        printf("2. Alice\n");
        printf("3. Bob\n");
        printf("4. Emily\n");
        printf("5. David\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &studentIndex);

        if (studentIndex < 0 || studentIndex > 5) {
            printf("Invalid student index. Please try again.\n");
            continue;
        }

        printf("\n-- Options for %s --\n", students[studentIndex].name);
        printf("1. View Personal Details\n");
        printf("2. View Enrolled Courses\n");
        printf("3. View Grades\n");
        printf("4. View CGPA\n");
        printf("5. View Attendance\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                viewPersonalDetails(students[studentIndex]);
                break;
            case 2:
                viewEnrolledCourses(students[studentIndex]);
                break;
            case 3:
                viewGrades(students[studentIndex]);
                break;
            case 4:
                viewCGPA(students[studentIndex]);
                break;
            case 5:
                viewAttendance(students[studentIndex]);
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 6); // Keep looping until the user chooses to exit

    return 0;
}

// Function to view personal details of a student
void viewPersonalDetails(struct Student student) {
    printf("\n-- Personal Details --\n");
    printf("Name: %s\n", student.name);
    // Add more personal details to display as needed
}

// Function to view enrolled courses of a student
void viewEnrolledCourses(struct Student student) {
    printf("\n-- Enrolled Courses --\n");
    for(int i = 0; i < 5; i++) {
        printf("%d. %s\n", i+1, student.enrolledCourses[i]);
    }
}

// Function to view grades of a student
void viewGrades(struct Student student) {
    printf("\n-- Grades --\n");
    for(int i = 0; i < 5; i++) {
        printf("%s: %d\n", student.enrolledCourses[i], student.grades[i]);
    }
}

// Function to calculate and view CGPA of a student
void viewCGPA(struct Student student) {
    printf("\n-- CGPA --\n");
    float totalCredits = 0, totalGradePoints = 0;
    for(int i = 0; i < 5; i++) {
        totalCredits += 3; // Assuming each course has 3 credits
        totalGradePoints += student.grades[i];
    }
    student.CGPA = totalGradePoints / totalCredits;
    printf("CGPA: %.2f\n", student.CGPA);
}

// Function to view attendance of a student
void viewAttendance(struct Student student) {
    printf("\n-- Attendance --\n");
    for(int i = 0; i < 5; i++) {
        printf("%s: %d%%\n", student.enrolledCourses[i], student.attendance[i]);
    }
}