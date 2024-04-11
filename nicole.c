#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 6
#define MAX_COURSES 5

// Remove the erroneous declaration of gradeToPoint from the struct
struct Student {
    int userID;
    char name[50];
    char gender[7];
    char email[50];
    char dob[20];
    char phone[15];
    char enrolledCourses[MAX_COURSES][50];
    char grades[MAX_COURSES][3];
    float CGPA;
    int attendance[MAX_COURSES];
};

// Declare gradeToPoint function before its first use
float gradeToPoint(char* grade);

// Function prototypes
void readPersonalDetails(struct Student *students, int *numStudents);
void readGrades(struct Student *students, int numStudents);
void readAttendance(struct Student *students, int numStudents);
void readCourseDetails(struct Student *students, int numStudents);
void viewPersonalDetails(struct Student student);
void modifyPersonalDetails(struct Student *students, int numStudents, int userID);
void viewCourseGradesAndCGPA(struct Student student);
void viewAttendance(struct Student student);
void writePersonalDetails(struct Student *students, int numStudents);

// Main function and other functions...


int main() {
    struct Student students[MAX_STUDENTS];
    int numStudents = 0;

    readPersonalDetails(students, &numStudents);
    readCourseDetails(students, numStudents);
    readGrades(students, numStudents);
    readAttendance(students, numStudents);

    int choice;
    int userID;
    do {
        printf("\n-- Student Information System --\n");
        printf("Enter the student's id (or '0' to quit): ");
        scanf("%d", &userID);

        if (userID == 0) {
            printf("Exiting...\n");
            break;
        }

        int studentIndex = -1;
        for (int i = 0; i < numStudents; ++i) {
            if (students[i].userID == userID) {
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
            printf("2. Modify Personal Details\n");
            printf("3. View Enrolled Courses, Grades, and CGPA\n");
            printf("4. View Attendance\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf(" %d", &choice);

            switch(choice) {
                case 1:
                    viewPersonalDetails(students[studentIndex]);
                    break;
                case 2:
                    modifyPersonalDetails(students, numStudents, userID);
                    break;
                case 3:
                    viewCourseGradesAndCGPA(students[studentIndex]);
                    break;
                case 4:
                    viewAttendance(students[studentIndex]);
                    break;
                case 5:
                    printf("Exiting...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while(choice != 5);
    } while(userID != 0);

    return 0;
}

void readPersonalDetails(struct Student *students, int *numStudents) {
    FILE *file = fopen("personal_detail.txt", "r");
    if (file) {
        int i = 0;
        while (fscanf(file, "%d,%49[^,],%6[^,],%49[^,],%19[^,],%14[^\n]",
                      &students[i].userID, students[i].name, students[i].gender,
                      students[i].email, students[i].dob, students[i].phone) == 6) {
            printf("Read student: %d %s\n", students[i].userID, students[i].name);
            i++;
        }
        *numStudents = i;
        fclose(file);
    } else {
        printf("Error reading personal_detail.txt file.\n");
    }
}

void readGrades(struct Student *students, int numStudents) {
    FILE *file = fopen("grades.txt", "r");
    if (file) {
        int userID;
        char courseCode[10];
        char grade[3]; // Updated size to accommodate grades as strings

        while (fscanf(file, "%d,%[^,],%s", &userID, courseCode, grade) == 3) {
            printf("Read grade: %s for user %d in course %s\n", grade, userID, courseCode); // Debug print
            for (int i = 0; i < numStudents; ++i) {
                if (students[i].userID == userID) {
                    for (int j = 0; j < MAX_COURSES; ++j) {
                        if (strcmp(students[i].enrolledCourses[j], courseCode) == 0) {
                            strcpy(students[i].grades[j], grade);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        fclose(file);
    } else {
        printf("Error reading grades.txt file.\n");
    }
}


void readCourseDetails(struct Student *students, int numStudents) {
    FILE *file = fopen("student_profiles.txt", "r");
    if (file) {
        int userID;
        char name[50];
        char courseCode[10]; // Adjusted buffer size for course codes
        while (fscanf(file, "%d,%49[^,],%9[^\n]", &userID, name, courseCode) == 3) {
            printf("Read course: %s for user %d\n", courseCode, userID);
            for (int i = 0; i < numStudents; ++i) {
                if (students[i].userID == userID) {
                    for (int j = 0; j < 5; ++j) {
                        if (strlen(students[i].enrolledCourses[j]) == 0) {
                            strcpy(students[i].enrolledCourses[j], courseCode);
                            break;
                        }
                    }
                }
            }
        }
        fclose(file);
    } else {
        printf("Error reading student_profiles.txt file.\n");
    }
}




void readAttendance(struct Student *students, int numStudents) {
    FILE *file = fopen("attendance.txt", "r");
    if (file) {
        int userID, attendanceIndex;
        char courseCode[10];
        char attendancePercentage[5];

        while (fscanf(file, "%d,%9[^,],%s", &userID, courseCode, attendancePercentage) == 3) {
            for (int i = 0; i < numStudents; ++i) {
                if (students[i].userID == userID) {
                    for (int j = 0; j < MAX_COURSES; ++j) {
                        if (strcmp(students[i].enrolledCourses[j], courseCode) == 0) {
                            students[i].attendance[j] = atoi(attendancePercentage);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        fclose(file);
    } else {
        printf("Error reading attendance.txt file.\n");
    }
}

void viewPersonalDetails(struct Student student) {
    printf("\n-- Personal Details --\n");
    printf("Name: %s\n", student.name);
    printf("Student ID: %d\n", student.userID);
    printf("Gender: %s\n", student.gender);
    printf("Email: %s\n", student.email);
    printf("Date of Birth: %s\n", student.dob);
    printf("Phone: %s\n", student.phone);
}

void writePersonalDetails(struct Student *students, int numStudents) {
    FILE *file = fopen("personal_detail.txt", "w");
    if (file) {
        for (int i = 0; i < numStudents; i++) {
            fprintf(file, "%d,%s,%s,%s,%s,%s\n", students[i].userID, students[i].name,
                    students[i].gender, students[i].email, students[i].dob, students[i].phone);
        }
        fclose(file);
    } else {
        printf("Error writing personal_detail.txt file.\n");
    }
}

void modifyPersonalDetails(struct Student *students, int numStudents, int userID) {
    printf("\n-- Modify Personal Details --\n");

    int studentIndex = -1;
    for (int i = 0; i < numStudents; ++i) {
        if (students[i].userID == userID) {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1) {
        printf("Student not found. Please try again.\n");
        return;
    }

    printf("Enter new email address: ");
    scanf("%s", students[studentIndex].email);
    printf("Enter new phone number: ");
    scanf("%s", students[studentIndex].phone);
    printf("Personal details modified successfully.\n");

    writePersonalDetails(students, numStudents);
}

void viewCourseGradesAndCGPA(struct Student student) {
    printf("\n-- Enrolled Courses and Grades --\n");
    int courseCount = 0;
    float totalGradePoints = 0.0;
    int totalCourses = 0;
    for (int i = 0; i < MAX_COURSES; i++) {
        if (strcmp(student.grades[i], "") != 0) {
            printf("Course %d: %s, Grade: %s\n", ++courseCount, student.enrolledCourses[i], student.grades[i]);
            totalCourses++;
            totalGradePoints += gradeToPoint(student.grades[i]);
            if (courseCount >= 2) // Limiting to only the first two courses
                break;
        }
    }

    printf("\n-- CGPA --\n");
    if (totalCourses > 0) {
        float CGPA = totalGradePoints / totalCourses;
        printf("CGPA: %.2f\n", CGPA);
    } else {
        printf("No grades available to calculate CGPA.\n");
    }
}

float gradeToPoint(char* grade) {
    if (strcmp(grade, "A") == 0) return 4.0;
    else if (strcmp(grade, "A-") == 0) return 3.7;
    else if (strcmp(grade, "B+") == 0) return 3.3;
    else if (strcmp(grade, "B") == 0) return 3.0;
    else if (strcmp(grade, "B-") == 0) return 2.7;
    else if (strcmp(grade, "C+") == 0) return 2.3;
    else if (strcmp(grade, "C") == 0) return 2.0;
    else if (strcmp(grade, "C-") == 0) return 1.7;
    else if (strcmp(grade, "D+") == 0) return 1.3;
    else if (strcmp(grade, "D") == 0) return 1.0;
    else if (strcmp(grade, "F") == 0) return 0.0;
    else return -1.0; // Invalid grade
}


void viewAttendance(struct Student student) {
    printf("\n-- Attendance --\n");
    for (int i = 0; i < MAX_COURSES; i++) {
        printf("Course %d: %d%%\n", i + 1, student.attendance[i]);
    }
}