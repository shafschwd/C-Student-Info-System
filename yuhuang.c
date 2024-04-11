#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE 11
#define MAX_NAME 52
#define MAX_LINE 257
#define MAX_COURSE_ID 21
#define MAX_COURSES 5
#define MAX_COURSE_NAME 31
#define MAX_ID 21
#define MAX_GRADE 6
#define MAX_ATTENDANCE 21
#define MAX_CGPA 5



typedef struct {
    char courseCode[MAX_CODE]; 
    char courseName[MAX_NAME];
    int numStudentsEnrolled;
    char lecturerName[MAX_NAME];
} Course;

typedef struct {
    int userID;
    char name[MAX_NAME];
    int numCoursesEnrolled;
    char coursesEnrolled[MAX_COURSES][MAX_COURSE_NAME];
} Student;

int isValidUserID(const char* userID) {
    int id = atoi(userID); // Convert string to integer
    return id >= 100 && id <= 999; // Check if in range
}

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

float getCGPA(const char* grade) {
    // Placeholder for CGPA conversion logic.
    return 4.0; // Example: assume all grades convert to 4.0.
}

// Validate the student ID is between 100 and 999.
int isvalidUserID(const char* userID) {
    int id = atoi(userID); // Convert string to integer.
    return (id >= 100 && id <= 999);
}

void viewGrades() {
    char targetStudentID[MAX_ID];
    char line[MAX_LINE];
    int validInput = 0;

    while (!validInput) {
        printf("Enter the student ID to view grades (or type 'exit' to return): ");
        scanf("%s", targetStudentID);

        if (strcmp(targetStudentID, "exit") == 0) return; // Exit if user types "exit".
        
        // Validate user ID.
        if (!isValidUserID(targetStudentID)) {
            printf("Invalid student ID. Please try again.\n");
            continue; // Skip the rest of the loop and prompt again.
        }

        validInput = 1; // ID is valid, proceed.
    }

    FILE* file = fopen("grades.txt", "r");
    if (!file) {
        perror("Failed to open grades.txt");
        return;
    }

    int found = 0;
    printf("ID\tCourse Code\tGrade\tCGPA\n");
    printf("----------------------------------------\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        char studentID[MAX_ID], courseCode[MAX_COURSE_ID], grade[MAX_GRADE], cgpaStr[MAX_CGPA];

        // Updated to match the new file format.
        if (sscanf(line, "%[^,],%[^,],%[^,],%s", studentID, courseCode, grade, cgpaStr) == 4 && strcmp(studentID, targetStudentID) == 0) {
            printf("%s\t%s\t\t%s\t%s\n", studentID, courseCode, grade, cgpaStr);
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf("No grades found for student ID %s.\n", targetStudentID);
    }
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

float gradeToCGPA(const char* grade) {
    if (strcmp(grade, "A+") == 0) return 4.00;
    if (strcmp(grade, "A") == 0) return 3.70;
    if (strcmp(grade, "B+") == 0) return 3.30;
    if (strcmp(grade, "B") == 0) return 3.00;
    if (strcmp(grade, "C+") == 0) return 2.70;
    if (strcmp(grade, "C") == 0) return 2.30;
    if (strcmp(grade, "C-") == 0) return 2.00;
    if (strcmp(grade, "D") == 0) return 1.60;
    if (strcmp(grade, "F") == 0) return 0.00;
    return -1; // Invalid grade
}

// Validates if the grade entered is valid according to the specified ranges.
int isvalidGrade(const char* grade) {
    return gradeToCGPA(grade) != -1;
}

// Validates student ID to be in the range 100-999.
int isvalidStudentID(const char* studentID) {
    int id = atoi(studentID); // Convert string to integer
    return id >= 100 && id <= 999;
}

// Checks if the course code exists in the grades file.
int isValidCourseCode(const char* courseCode) {
    FILE* file = fopen("grades.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return 0;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        char readCourseCode[MAX_COURSE_ID];
        sscanf(line, "%*[^,],%[^,]", readCourseCode);
        if (strcmp(readCourseCode, courseCode) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void modifyGrades() {
    char targetStudentID[MAX_ID];
    char targetCourseCode[MAX_COURSE_ID];
    char newGrade[MAX_GRADE];
    float cgpa;

    // Loop until a valid student ID is entered or the user decides to exit.
    do {
        printf("Enter Student ID to modify grade or 'exit' to cancel: ");
        scanf("%s", targetStudentID);
        if (strcmp(targetStudentID, "exit") == 0) {
            return; // Early exit.
        }
        if (!isValidStudentID(targetStudentID)) {
            printf("Invalid student ID, must be between 100 and 999. Please try again.\n");
        } else {
            break; // Valid ID entered.
        }
    } while (1);

    // Loop until a valid course code is entered or the user decides to exit.
    do {
        printf("Enter Course Code or 'exit' to cancel: ");
        scanf("%s", targetCourseCode);
        if (strcmp(targetCourseCode, "exit") == 0) {
            return; // Early exit.
        }
        if (!isValidCourseCode(targetCourseCode)) {
            printf("Invalid course code, please try again.\n");
        } else {
            break; // Valid code entered.
        }
    } while (1);

    // Loop until a valid grade is entered or the user decides to exit.
    do {
        printf("Enter new Grade (A+, A, B+, B, C+, C, C-, D, F) or 'exit' to cancel: ");
        scanf("%s", newGrade);
        if (strcmp(newGrade, "exit") == 0) {
            return; // Early exit.
        }
        if (!isValidGrade(newGrade)) {
            printf("Invalid Grade, please try again.\n");
        } else {
            cgpa = gradeToCGPA(newGrade); // Calculate CGPA based on the grade.
            break; // Valid grade entered.
        }
    } while (1);

    // Proceed to modify the grades.txt file based on the input.
    FILE* file = fopen("grades.txt", "r");
    FILE* tempFile = fopen("temp_grades.txt", "w");
    if (!file || !tempFile) {
        perror("Error opening files");
        return;
    }

    char line[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char studentID[MAX_ID], courseCode[MAX_COURSE_ID], grade[MAX_GRADE];
        float fileCgpa;
        if (sscanf(line, "%[^,],%[^,],%[^,],%f", studentID, courseCode, grade, &fileCgpa) == 4) {
            if (strcmp(studentID, targetStudentID) == 0 && strcmp(courseCode, targetCourseCode) == 0) {
                fprintf(tempFile, "%s,%s,%s,%.2f\n", studentID, courseCode, newGrade, cgpa); // Update record.
                found = 1;
            } else {
                fputs(line, tempFile); // Copy line as is.
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("grades.txt");
        rename("temp_grades.txt", "grades.txt");
        printf("Grade and CGPA updated successfully.\n");
    } else {
        printf("No matching record found to update.\n");
        remove("temp_grades.txt"); // Cleanup temporary file if no update was done.
    }
}

void viewAttendance() {
    FILE* file = fopen("attendance.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    char targetStudentID[MAX_ID];
    int validID = 0;  

    do {
        printf("Enter the student ID to view attendance (or type 'exit' to return): ");
        scanf("%10s", targetStudentID);
        if (strcmp(targetStudentID, "exit") == 0) {
            fclose(file);
            return;
        }

        while (getchar() != '\n');

        char line[MAX_LINE];
        int found = 0; 

        fseek(file, 0, SEEK_SET);  

        while (fgets(line, sizeof(line), file)) {
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
                    found = 1;  
                    validID = 1;  
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
