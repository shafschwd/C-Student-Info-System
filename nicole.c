#include <stdio.h>
#include <string.h>

// Define a structure for student
struct Student {
    char id[4];
    char name[50];
    char courseId[7];
    char enrolledCourses[5][50];
    int grades[5];
    float CGPA;
    int attendance[5];
};

// Function prototypes
void viewPersonalDetails(struct Student student);
void viewCourseDetails(struct Student student);
void viewAttendance(struct Student student);
void readStudentProfiles(struct Student students[], int *numStudents);

int main() {
    // Sample data of students
    struct Student students[6];
    int numStudents = 0;
    readStudentProfiles(students, &numStudents);

    // Menu-driven system
    int choice;
    char studentName[50];
    do {
        printf("\n-- Student Information System --\n");
        printf("Enter the student's name (or 'exit' to quit): ");
        scanf("%s", studentName);

        if (strcmp(studentName, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        int studentIndex = -1;
        for (int i = 0; i < numStudents; ++i) {
            if (strcmp(students[i].name, studentName) == 0) {
                studentIndex = i;
                break;
            }
        }

        if (studentIndex == -1) {
            printf("Student not found. Please try again.\n");
            continue;
        }

        do {
            printf("\n-- Options for %s --\n", students[studentIndex].name);
            printf("1. View Personal Details\n");
            printf("2. View Enrolled Courses, Grades, and CGPA\n");
            printf("3. View Attendance\n");
            printf("4. Exit\n");
            printf("Enter your choice: ");
            scanf(" %d", &choice);

            switch(choice) {
                case 1:
                    viewPersonalDetails(students[studentIndex]);
                    break;
                case 2:
                    viewCourseDetails(students[studentIndex]);
                    break;
                case 3:
                    viewAttendance(students[studentIndex]);
                    break;
                case 4:
                    printf("Exiting...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while(choice != 4);
    } while(strcmp(studentName, "exit") != 0);

    return 0;
}

// Function to read student profiles from file
void readStudentProfiles(struct Student students[], int *numStudents) {
    FILE *file = fopen("student_profiles.txt", "r");
    if (file) {
        while ((*numStudents) < 6 && fscanf(file, "%3[^,],%49[^,],%6[^,\n]", students[*numStudents].id, students[*numStudents].name, students[*numStudents].courseId) == 3) {

            (*numStudents)++;
        }
        fclose(file);
    } else {
        printf("Error reading student_profiles.txt file.\n");
    }
}

// Function to view personal details of a student
void viewPersonalDetails(struct Student student) {
    printf("\n-- Personal Details --\n");
    printf("Name: %s\n", student.name);
    printf("Student ID: %s\n", student.id);
    printf("Course ID: %s\n", student.courseId);
}

// Function to view enrolled courses, grades, and CGPA of a student
void viewCourseDetails(struct Student student) {
    printf("\n-- Enrolled Courses, Grades, and CGPA --\n");
    for(int i = 0; i < 5; i++) {
        printf("Course: %s\n", student.enrolledCourses[i]);
        printf("Grade: %d\n", student.grades[i]);
    }
    printf("CGPA: %.2f\n", student.CGPA);
}

// Function to view attendance of a student
void viewAttendance(struct Student student) {
    printf("\n-- Attendance --\n");
    for(int i = 0; i < 5; i++) {
        printf("Course: %s\n", student.enrolledCourses[i]);
        printf("Attendance: %d%%\n", student.attendance[i]);
    }
}