#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE 10
#define MAX_NAME 51
#define MAX_LINE 256
#define MAX_COURSE_ID 20
#define MAX_COURSES 5
#define MAX_COURSE_NAME 30
#define MAX_ID 20
#define MAX_GRADE 5
#define MAX_ATTENDANCE 20

// Structure definition
typedef struct {
    char courseCode[MAX_CODE];
    char courseName[MAX_NAME];
    int numStudentsEnrolled;
    char lecturerName[MAX_NAME];
} Course;

typedef struct {
    char studentID[MAX_ID];
    char name[MAX_NAME];
    int numCoursesEnrolled;
    char coursesEnrolled[MAX_COURSES][MAX_COURSE_NAME];
} Student;

// Function prototypes
void lecturerMenu();
void viewGrades();
void modifyGrades();
void viewAttendance();
void updateAttendance();


int main() {
    lecturerMenu();
    return 0;
}

void lecturerMenu() {
    int choice;
    do {
        printf("\n********** Lecturer Menu **********\n");
        printf("1. View Grades\n");
        printf("2. Modify Grades\n");
        printf("3. View Attendance\n");
        printf("4. Update Attendance\n");
        printf("5. Logout\n");
        printf("***********************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch(choice) {
            case 1:
                viewGrades();
                break;
            case 2:
                modifyGrades();
                break;
            case 3:
                viewAttendance();
                break;
            case 4:
                updateAttendance();
                break;
            case 5:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                break;
        }
    } while(choice != 5);
}


void viewGrades() {
    FILE* file = fopen("grades.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    char targetStudentID[MAX_ID];
    int validID = 0;  // Flag to ensure valid input is provided.

    do {
        printf("Enter the student ID to view grades (or type 'exit' to return): ");
        scanf("%10s", targetStudentID);
        if (strcmp(targetStudentID, "exit") == 0) {
            fclose(file);
            return;
        }

        // Clear the input buffer.
        while (getchar() != '\n');

        char line[MAX_LINE];
        int found = 0;  // To track if at least one grade record is found.

        fseek(file, 0, SEEK_SET);  // Go back to the start of the file for each ID input.

        while (fgets(line, sizeof(line), file)) {
            // Remove possible newline character at the end of the line.
            line[strcspn(line, "\n")] = 0;

            char studentID[MAX_ID], courseCode[MAX_COURSE_ID], grade[MAX_GRADE];
            // Adjusted the sscanf() to match the "StudentID,CourseCode,Grade" format.
            if (sscanf(line, "%[^,],%[^,],%s", studentID, courseCode, grade) == 3) {
                if (strcmp(studentID, targetStudentID) == 0) {
                    if (!found) {
                        printf("Grades for Student ID %s:\n", targetStudentID);
                        printf("ID\tCourse Code\tGrade\n");
                        printf("--------------------------------\n");
                    }
                    printf("%s\t%s\t\t%s\n", studentID, courseCode, grade);
                    found = 1;  // Mark that we've found at least one record.
                    validID = 1;  // Mark valid ID input to exit the loop.
                }
            }
        }

        if (!found) {
            printf("Invalid student ID or no grades found, please try again.\n");
        }
    } while (!validID);

    fclose(file);
}


const char* validGrades[] = {"A+", "A", "B+", "B", "C+", "C", "C-", "D", "F"};
const int numValidGrades = sizeof(validGrades) / sizeof(validGrades[0]);

int isValidGrade(const char* grade) {
    for (int i = 0; i < numValidGrades; ++i) {
        if (strcmp(grade, validGrades[i]) == 0) {
            return 1;
        }
    }
    return 0; 
}


int isValidStudentID(const char* studentID) {
    FILE* file = fopen("grades.txt", "r");
    char line[MAX_LINE], id[MAX_ID];
    if (!file) {
        perror("Unable to open the file");
        return 0;
    }
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,]", id);
        if (strcmp(id, studentID) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int isValidCourseCode(const char* courseCode) {
    FILE* file = fopen("grades.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return 0;
    }

    char line[MAX_LINE];
    int isValid = 0; 

    while (fgets(line, sizeof(line), file) != NULL) {
        char readCourseCode[MAX_COURSE_ID];
        sscanf(line, "%*[^,],%[^,]", readCourseCode);
        if (strcmp(readCourseCode, courseCode) == 0) {
            isValid = 1;
            break; 
        }
    }

    fclose(file);
    return isValid;
}


void modifyGrades() {
    char targetStudentID[MAX_ID];
    char targetCourseCode[MAX_COURSE_ID];
    char newGrade[MAX_GRADE];

    int validStudentID = 0;
    int validCourseCode = 0;

    while (!validStudentID) {
        printf("Enter Student ID to modify grade: ");
        scanf("%10s", targetStudentID);
        getchar(); 
        
        if (isValidStudentID(targetStudentID)) {
            validStudentID = 1;
        } else {
            printf("Invalid student ID, please try again.\n");
        }
    }

    while (!validCourseCode) {
        printf("Enter Course Code: ");
        scanf("%10s", targetCourseCode);
        getchar();
        
        if (isValidCourseCode(targetCourseCode)) {
            validCourseCode = 1;
        } else {
            printf("Invalid course code, please try again.\n");
        }
    }

    while (1) {
        printf("Enter new Grade (A+, A, B+, B, C+, C, C-, D, F): ");
        scanf("%4s", newGrade);
        getchar(); 

        if (isValidGrade(newGrade)) {
            break;
        } else {
            printf("Invalid Grade, please try again.\n");
        }
    }

    FILE* file = fopen("grades.txt", "r");
    FILE* tempFile = fopen("temp_grades.txt", "w");
    char line[MAX_LINE];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char studentID[MAX_ID], courseCode[MAX_COURSE_ID], grade[MAX_GRADE];
        sscanf(line, "%[^,],%[^,],%s", studentID, courseCode, grade);

        if (strcmp(studentID, targetStudentID) == 0 && strcmp(courseCode, targetCourseCode) == 0) {
            fprintf(tempFile, "%s,%s,%s\n", studentID, courseCode, newGrade);
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("grades.txt");
        rename("temp_grades.txt", "grades.txt");
        printf("Updated Successfully.\n");
    } else {
        remove("temp_grades.txt"); 
        printf("No matching record found to update.\n");
    }
}


void viewAttendance() {
    FILE* file = fopen("attendance.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    char targetStudentID[MAX_ID];
    int validID = 0;  // Flag to ensure valid input is provided.

    do {
        printf("Enter the student ID to view attendance (or type 'exit' to return): ");
        scanf("%10s", targetStudentID);
        if (strcmp(targetStudentID, "exit") == 0) {
            fclose(file);
            return;
        }

        // Clear the input buffer.
        while (getchar() != '\n');

        char line[MAX_LINE];
        int found = 0;  // To track if at least one attendance record is found.

        fseek(file, 0, SEEK_SET);  // Go back to the start of the file for each ID input.

        while (fgets(line, sizeof(line), file)) {
            // Remove possible newline character at the end of the line.
            line[strcspn(line, "\n")] = 0;

            char studentID[MAX_ID], courseCode[MAX_COURSE_ID], attendance[MAX_ATTENDANCE];
            if (sscanf(line, "%[^,],%[^,],%s", studentID, courseCode, attendance) == 3) {
                if (strcmp(studentID, targetStudentID) == 0) {
                    if (!found) {
                        printf("Attendance for Student ID %s:\n", targetStudentID);
                        printf("ID\tCourse Code\tAttendance\n");
                        printf("-----------------------------------\n");
                    }
                    printf("%s\t%s\t\t%s\n", studentID, courseCode, attendance);
                    found = 1;  // Mark that we've found at least one record.
                    validID = 1;  // Mark valid ID input to exit the loop.
                }
            }
        }

        if (!found) {
            printf("Invalid student ID or no attendance records found, please try again.\n");
        }
    } while (!validID);

    fclose(file);
}

int isValidStudentID(const char* studentID);
int isValidCourseCode(const char* courseCode);

void updateAttendance() {
    char targetStudentID[MAX_ID], targetCourseID[MAX_COURSE_ID];
    int newAttendance;
    char buffer[MAX_LINE]; 

    printf("Enter Student ID to update attendance or 'exit' to quit: ");
    scanf("%10s", targetStudentID); 
    if (strcmp(targetStudentID, "exit") == 0) return;
    while (!isValidStudentID(targetStudentID)) {
        printf("Invalid student ID, please try again or enter 'exit' to quit: ");
        scanf("%10s", targetStudentID);
        if (strcmp(targetStudentID, "exit") == 0) return;
    }

    printf("Enter Course ID or 'exit' to quit: ");
    scanf("%10s", targetCourseID);
    if (strcmp(targetCourseID, "exit") == 0) return;
    while (!isValidCourseCode(targetCourseID)) {
        printf("Invalid course code, please try again or enter 'exit' to quit: ");
        scanf("%10s", targetCourseID);
        if (strcmp(targetCourseID, "exit") == 0) return;
    }

    printf("Enter new Attendance Status (0-100): ");
    while (scanf("%d", &newAttendance) != 1 || newAttendance < 0 || newAttendance > 100) {
        printf("Invalid number, please try again: ");
        while (getchar() != '\n'); 
    }

    FILE* file = fopen("attendance.txt", "r");
    FILE* tempFile = fopen("temp_attendance.txt", "w");
    if (!file || !tempFile) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char readStudentID[MAX_ID], readCourseID[MAX_COURSE_ID];
        int dummyAttendance; 
        if (sscanf(line, "%[^,],%[^,],%d%%", readStudentID, readCourseID, &dummyAttendance) == 3 &&
            strcmp(readStudentID, targetStudentID) == 0 && strcmp(readCourseID, targetCourseID) == 0) {
            fprintf(tempFile, "%s,%s,%d%%\n", readStudentID, readCourseID, newAttendance); 
            found = 1;
        } else {
            fputs(line, tempFile); 
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("attendance.txt");
        rename("temp_attendance.txt", "attendance.txt");
        printf("Attendance updated successfully.\n");
    } else {
        remove("temp_attendance.txt"); 
        printf("No matching record found to update.\n");
    }
}