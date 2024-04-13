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
    int id = atoi(userID);
    return id >= 100 && id <= 999;
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
    if (strcmp(grade, "A+") == 0) return 4.0;
    if (strcmp(grade, "A") == 0) return 3.70;
    if (strcmp(grade, "B+") == 0) return 3.30;
    if (strcmp(grade, "B") == 0) return 3.00;
    if (strcmp(grade, "C+") == 0) return 2.70;
    if (strcmp(grade, "C") == 0) return 2.30;
    if (strcmp(grade, "C-") == 0) return 2.00;
    if (strcmp(grade, "D") == 0) return 1.60;
    if (strcmp(grade, "F") == 0) return 0.00;
    return 0.0; // Default for unknown grades
}

// Validate the student ID is between 100 and 999.
int isValiduserID(const char* userID) {
    int id = atoi(userID); // Convert string to integer.
    return (id >= 100 && id <= 999);
}

void viewGrades() {
    char targetUserID[MAX_ID];
    int validInput = 0;

    while (!validInput) {
        printf("Enter the user ID to view grades (or type 'exit' to return): ");
        scanf("%s", targetUserID);
        if (strcmp(targetUserID, "exit") == 0) return; // User exits the function.

        if (!isValidUserID(targetUserID)) {
            printf("Invalid user ID. Please try again.\n");
            continue; // Ask again if the user ID is not valid.
        }
        validInput = 1; // Proceed when a valid user ID is provided.
    }

    FILE *gradesFile = fopen("grades.txt", "r");
    FILE *cgpaFile = fopen("cgpa.txt", "r");
    if (!gradesFile || !cgpaFile) {
        perror("Failed to open one or more files");
        if (gradesFile) fclose(gradesFile);
        if (cgpaFile) fclose(cgpaFile);
        return;
    }

    char gradesLine[MAX_LINE], cgpaLine[MAX_LINE];
    int found = 0;
    printf("ID\tCourse Code\tGrade\tCGPA\n");
    printf("----------------------------------------\n");

    // Read both files line by line
    while (fgets(gradesLine, sizeof(gradesLine), gradesFile) && fgets(cgpaLine, sizeof(cgpaLine), cgpaFile)) {
        char userID[MAX_ID], courseCode[MAX_COURSE_ID], grade[MAX_GRADE], cgpaStr[MAX_CGPA];

        if (sscanf(gradesLine, "%[^,],%[^,],%s", userID, courseCode, grade) == 3) {
            sscanf(cgpaLine, "%*[^,],%*[^,],%s", cgpaStr); // Ignore userID and courseCode in cgpa.txt
            if (strcmp(userID, targetUserID) == 0) {
                printf("%s\t%s\t\t%s\t%s\n", userID, courseCode, grade, cgpaStr);
                found = 1;
            }
        }
    }

    fclose(gradesFile);
    fclose(cgpaFile);

    if (!found) {
        printf("No grades found for user ID %s.\n", targetUserID);
    }
}

const char* validGrades[] = {"A+", "A", "B+", "B", "C+", "C", "C-", "D", "F"};
const int numValidGrades = sizeof(validGrades) / sizeof(validGrades[0]);

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
    return -1;
}
int isValidUserID(const char* userID);
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

int isValidGrade(const char* grade) {
    const char* validGrades[] = {"A+", "A", "B+", "B", "C+", "C", "C-", "D", "F"};
    int numValidGrades = sizeof(validGrades) / sizeof(validGrades[0]);

    for (int i = 0; i < numValidGrades; ++i) {
        if (strcmp(grade, validGrades[i]) == 0) {
            return 1;
        }
    }
    return 0;
}


void modifyGrades() {
    char targetUserID[MAX_ID], targetCourseCode[MAX_COURSE_ID], newGrade[MAX_GRADE];
    float cgpa;

    // Ensure User ID is valid
    while (1) {
        printf("Enter User ID to modify grade or 'exit' to cancel: ");
        scanf("%s", targetUserID);
        if (strcmp(targetUserID, "exit") == 0) return;
        if (isValidUserID(targetUserID)) break;
        printf("Invalid user ID, must be between 100 and 999. Please try again.\n");
    }

    // Ensure Course Code is valid
    while (1) {
        printf("Enter Course Code or 'exit' to cancel: ");
        scanf("%s", targetCourseCode);
        if (strcmp(targetCourseCode, "exit") == 0) return;
        if (isValidCourseCode(targetCourseCode)) break;
        printf("Invalid course code, please try again.\n");
    }

    // Ensure Grade is valid
    while (1) {
        printf("Enter new Grade (A+, A, B+, B, C+, C, C-, D, F) or 'exit' to cancel: ");
        scanf("%s", newGrade);
        if (strcmp(newGrade, "exit") == 0) return;
        if (isValidGrade(newGrade)) break;
        printf("Invalid Grade, please try again.\n");
    }

    cgpa = gradeToCGPA(newGrade);

    // File handling for grades and CGPA
    FILE *gradesFile = fopen("grades.txt", "r");
    FILE *cgpaFile = fopen("cgpa.txt", "r");
    FILE *tempGradesFile = fopen("temp_grades.txt", "w");
    FILE *tempCgpaFile = fopen("temp_cgpa.txt", "w");

    if (!gradesFile || !cgpaFile || !tempGradesFile || !tempCgpaFile) {
        perror("Error opening one or more files");
        return;
    }

    char line[MAX_LINE], cgpaLine[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), gradesFile) && fgets(cgpaLine, sizeof(cgpaLine), cgpaFile)) {
        char userID[MAX_ID], courseCode[MAX_COURSE_ID], grade[MAX_GRADE];
        if (sscanf(line, "%[^,],%[^,],%s", userID, courseCode, grade) == 3) {
            if (strcmp(userID, targetUserID) == 0 && strcmp(courseCode, targetCourseCode) == 0) {
                fprintf(tempGradesFile, "%s,%s,%s\n", userID, courseCode, newGrade);
                fprintf(tempCgpaFile, "%s,%s,%.2f\n", userID, courseCode, cgpa);
                found = 1;
            } else {
                fputs(line, tempGradesFile);
                fputs(cgpaLine, tempCgpaFile);
            }
        }
    }

    fclose(gradesFile);
    fclose(cgpaFile);
    fclose(tempGradesFile);
    fclose(tempCgpaFile);

    if (found) {
        remove("grades.txt");
        rename("temp_grades.txt", "grades.txt");
        remove("cgpa.txt");
        rename("temp_cgpa.txt", "cgpa.txt");
        printf("Grade and CGPA updated successfully.\n");
    } else {
        printf("No matching record found to update.\n");
    }
}


void viewAttendance() {
    FILE* file = fopen("attendance.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    char targetUserID[MAX_ID];
    int validID = 0;

    do {
        printf("Enter the user ID to view attendance (or type 'exit' to return): ");
        scanf("%10s", targetUserID);
        if (strcmp(targetUserID, "exit") == 0) {
            fclose(file);
            return;
        }

        while (getchar() != '\n');

        char line[MAX_LINE];
        int found = 0;

        fseek(file, 0, SEEK_SET);

        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;

            char userID[MAX_ID], courseCode[MAX_COURSE_ID], attendance[MAX_ATTENDANCE];
            if (sscanf(line, "%[^,],%[^,],%s", userID, courseCode, attendance) == 3) {
                if (strcmp(userID, targetUserID) == 0) {
                    if (!found) {
                        printf("Attendance for User ID %s:\n", targetUserID);
                        printf("ID\tCourse Code\tAttendance\n");
                        printf("-----------------------------------\n");
                    }
                    printf("%s\t%s\t\t%s\n", userID, courseCode, attendance);
                    found = 1;
                    validID = 1;
                }
            }
        }

        if (!found) {
            printf("Invalid user ID or no attendance records found, please try again.\n");
        }
    } while (!validID);

    fclose(file);
}

int isValidUserID(const char* userID);
int isValidCourseCode(const char* courseCode);

void updateAttendance() {
    char targetUserID[MAX_ID], targetCourseID[MAX_COURSE_ID];
    int newAttendance;
    char buffer[MAX_LINE];

    printf("Enter User ID to update attendance or 'exit' to quit: ");
    scanf("%10s", targetUserID);
    if (strcmp(targetUserID, "exit") == 0) return;
    while (!isValidUserID(targetUserID)) {
        printf("Invalid user ID, please try again or enter 'exit' to quit: ");
        scanf("%10s", targetUserID);
        if (strcmp(targetUserID, "exit") == 0) return;
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
        char readUserID[MAX_ID], readCourseID[MAX_COURSE_ID];
        int dummyAttendance;
        if (sscanf(line, "%[^,],%[^,],%d%%", readUserID, readCourseID, &dummyAttendance) == 3 &&
            strcmp(readUserID, targetUserID) == 0 && strcmp(readCourseID, targetCourseID) == 0) {
            fprintf(tempFile, "%s,%s,%d%%\n", readUserID, readCourseID, newAttendance);
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