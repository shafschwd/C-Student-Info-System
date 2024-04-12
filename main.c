#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//nicole part start
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


void readPersonalDetails(struct Student *students, int *numStudents) {
    FILE *file = fopen("personal_detail.txt", "r");
    if (file) {
        int i = 0;
        while (fscanf(file, "%d,%49[^,],%6[^,],%49[^,],%19[^,],%14[^\n]",
                      &students[i].userID, students[i].name, students[i].gender,
                      students[i].email, students[i].dob, students[i].phone) == 6) {
//            printf("Read student: %d %s\n", students[i].userID, students[i].name);
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
//            printf("Read course: %s for user %d\n", courseCode, userID);
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



//nicole part end


// yuhuang part
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
    char targetUserID[MAX_ID];
    char line[MAX_LINE];
    int validInput = 0;

    while (!validInput) {
        printf("Enter the user ID to view grades (or type 'exit' to return): ");
        scanf("%s", targetUserID);

        if (strcmp(targetUserID, "exit") == 0) return; // Exit if user types "exit".

        // Validate user ID.
        if (!isValidUserID(targetUserID)) {
            printf("Invalid user ID. Please try again.\n");
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
        char userID[MAX_ID], courseCode[MAX_COURSE_ID], grade[MAX_GRADE], cgpaStr[MAX_CGPA];

        // Updated to match the new file format.
        if (sscanf(line, "%[^,],%[^,],%[^,],%s", userID, courseCode, grade, cgpaStr) == 4 && strcmp(userID, targetUserID) == 0) {
            printf("%s\t%s\t\t%s\t%s\n", userID, courseCode, grade, cgpaStr);
            found = 1;
        }
    }

    fclose(file);

    if (!found) {
        printf("No grades found for user ID %s.\n", targetUserID);
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


int isValiduserID(const char* userID) {
    FILE* file = fopen("grades.txt", "r");
    char line[MAX_LINE], id[MAX_ID];
    if (!file) {
        perror("Unable to open the file");
        return 0;
    }
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,]", id);
        if (strcmp(id, userID) == 0) {
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
    return -1;
}

int isvalidGrade(const char* grade) {
    return gradeToCGPA(grade) != -1;
}

int isvaliduserID(const char* userID) {
    int id = atoi(userID);
    return id >= 100 && id <= 999;
}

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
    char targetUserID[MAX_ID];
    char targetCourseCode[MAX_COURSE_ID];
    char newGrade[MAX_GRADE];
    float cgpa;

    do {
        printf("Enter User ID to modify grade or 'exit' to cancel: ");
        scanf("%s", targetUserID);
        if (strcmp(targetUserID, "exit") == 0) {
            return;
        }
        if (!isValidUserID(targetUserID)) {
            printf("Invalid user ID, must be between 100 and 999. Please try again.\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("Enter Course Code or 'exit' to cancel: ");
        scanf("%s", targetCourseCode);
        if (strcmp(targetCourseCode, "exit") == 0) {
            return;
        }
        if (!isValidCourseCode(targetCourseCode)) {
            printf("Invalid course code, please try again.\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("Enter new Grade (A+, A, B+, B, C+, C, C-, D, F) or 'exit' to cancel: ");
        scanf("%s", newGrade);
        if (strcmp(newGrade, "exit") == 0) {
            return;
        }
        if (!isValidGrade(newGrade)) {
            printf("Invalid Grade, please try again.\n");
        } else {
            cgpa = gradeToCGPA(newGrade);
            break;
        }
    } while (1);

    FILE* file = fopen("grades.txt", "r");
    FILE* tempFile = fopen("temp_grades.txt", "w");
    if (!file || !tempFile) {
        perror("Error opening files");
        return;
    }

    char line[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char userID[MAX_ID], courseCode[MAX_COURSE_ID], grade[MAX_GRADE];
        float fileCgpa;
        if (sscanf(line, "%[^,],%[^,],%[^,],%f", userID, courseCode, grade, &fileCgpa) == 4) {
            if (strcmp(userID, targetUserID) == 0 && strcmp(courseCode, targetCourseCode) == 0) {
                fprintf(tempFile, "%s,%s,%s,%.2f\n", userID, courseCode, newGrade, cgpa);
                found = 1;
            } else {
                fputs(line, tempFile);
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
        remove("temp_grades.txt");
    }
}

void ltviewAttendance() {
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

//yuhuang part end




//cy part
#define MAX_COURSES 10
#define MAX_COURSE_CODE_LENGTH 10
#define MAX_STUDENTS 100
#define MAX_NAME 50
#define MAX_CODE 10
#define MAX_LINE 100

typedef struct {
    int userID;
    char name[50];
    char coursesEnrolled[MAX_COURSES][MAX_COURSE_CODE_LENGTH]; // Array to store course codes
    int numCoursesEnrolled; // Number of courses enrolled by the student
}student;

typedef struct {
    char courseCode[MAX_CODE];
    char courseName[MAX_NAME];
    int numStudentsEnrolled;
    char lecturerName[MAX_NAME];
}course;

void readStudentProfiles(Student *students, int *numStudents) {
    FILE *file = fopen("student_profiles.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char line[256];  // Buffer for each line from the file
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (token == NULL) continue;  // Skip invalid lines

        // Read student ID
        int userID = atoi(token);

        // Read student name
        token = strtok(NULL, ",");
        if (token == NULL) continue;  // Skip invalid lines
        char *name = token;

        // Initialize a new student
        Student student;
        student.userID = userID;
        strncpy(student.name, name, sizeof(student.name));
        student.name[sizeof(student.name) - 1] = '\0'; // Null-terminate
        student.numCoursesEnrolled = 0;

        // Read courses
        token = strtok(NULL, ",");
        while (token != NULL) {
            // Avoid buffer overflow
            strncpy(student.coursesEnrolled[student.numCoursesEnrolled], token, sizeof(student.coursesEnrolled[student.numCoursesEnrolled]));
            student.coursesEnrolled[student.numCoursesEnrolled][sizeof(student.coursesEnrolled[student.numCoursesEnrolled]) - 1] = '\0';
            student.numCoursesEnrolled++;
            token = strtok(NULL, ",");
        }

        // Check for duplicate student ID before adding the student
        int duplicate = 0;
        for (int i = 0; i < *numStudents; i++) {
            if (students[i].userID == student.userID) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            // Add the student to the students array if not a duplicate
            students[*numStudents] = student;
            (*numStudents)++;
        }
    }

    fclose(file);
}



void viewStudentProfiles(Student students[], int numStudents) {
    printf("Student Profiles:\n");
    printf("ID\tName\t\tCourses Enrolled\n");
    printf("***********************************\n");

    for (int i = 0; i < numStudents; i++) {
        printf("%d\t%s\t\t", students[i].userID, students[i].name);
        for (int j = 0; j < students[i].numCoursesEnrolled; j++) {
            printf("%s", students[i].coursesEnrolled[j]);
            if (j < students[i].numCoursesEnrolled - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}




void updateCourseInFile(const Course *course, const char *lecturerName) {
    // Open the input file in read mode
    FILE *file = fopen("course_info.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Open a temporary file in write mode
    FILE *tempFile = fopen("temp_course_info.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file for writing.\n");
        fclose(file); // Close the input file before returning
        return;
    }

    // Variables to store data read from the file
    char currentCode[MAX_CODE];
    char currentName[MAX_NAME];
    int currentStudents;
    char currentLecturer[MAX_NAME];

    // Read from the input file and write to the temporary file
    while (fscanf(file, "%9[^,],%49[^,],%d,%49[^\n]%*c", currentCode, currentName, &currentStudents, currentLecturer) == 4) {
        // Check if the current line matches the course to be updated
        if (strcmp(currentCode, course->courseCode) == 0) {
            // Write the updated lecturer name to the temporary file
            fprintf(tempFile, "%s,%s,%d,%s\n", course->courseCode, course->courseName, course->numStudentsEnrolled, course->lecturerName);
        } else {
            // Write the current line to the temporary file unchanged
            fprintf(tempFile, "%s,%s,%d,%s\n", currentCode, currentName, currentStudents, currentLecturer);
        }
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Remove the original file
    if (remove("course_info.txt") != 0) {
        printf("Error removing file.\n");
        return;
    }

    // Rename the temporary file to the original file
    if (rename("temp_course_info.txt", "course_info.txt") != 0) {
        printf("Error renaming file.\n");
        return;
    }
}


void viewAndUpdateCourseInfo() {
    char inputChoice;
    char inputCode[MAX_CODE]; // Declare inputCode variable
    int found; // Declare found outside the loop
    char lecturerName[MAX_NAME]; // Declare lecturerName
    do {
        // Display the course information
        FILE *file = fopen("course_info.txt", "r");
        if (file == NULL) {
            printf("Error opening file for reading.\n");
            return;
        }

        Course course;
        printf("\nCourse Information:\n");
        printf("Code\tName\t\t\t\tStudents Enrolled\tLecturer\n");
        printf("**********************************************\n");

        // Display course information from the file
        while (fscanf(file, "%9[^,],%49[^,],%d,%49[^\n]%*c", course.courseCode, course.courseName, &course.numStudentsEnrolled, course.lecturerName) == 4) {
            printf("%-8s%-40s%-15d%-s\n", course.courseCode, course.courseName, course.numStudentsEnrolled, course.lecturerName);
        }

        // Close the file
        fclose(file);


        // Prompt user if they want to update any course information
        printf("\nDo you want to update any course information (y/n): ");
        scanf(" %c", &inputChoice);

        if (inputChoice == 'y' || inputChoice == 'Y') {
            do {
                printf("\nEnter a course code to update (or 'b' for back): ");
                scanf("%s", inputCode);
                if (strcmp(inputCode, "b") == 0 || strcmp(inputCode, "B") == 0)
                    break;

                // Find the course to update
                int found = 0;
                file = fopen("course_info.txt", "r");
                if (file == NULL) {
                    printf("Error opening file for reading.\n");
                    return;
                }

                while (fscanf(file, "%9[^,],%49[^,],%d,%49[^\n]%*c", course.courseCode, course.courseName, &course.numStudentsEnrolled, course.lecturerName) == 4) {
                    if (strcmp(course.courseCode, inputCode) == 0) {
                        found = 1;
                        break;
                    }
                }
                fclose(file);

                if (!found) {
                    printf("Course code not found. Please try again.\n");
                    continue;
                }

                // Loop until a valid choice is made
                while (1) {
                    // Display the course information to be updated
                    printf("\nChoose what to update:\n");
                    printf("1. Course Name\n");
                    printf("2. Number of Students Enrolled\n");
                    printf("3. Back\n");
                    printf("***********************************\n");
                    printf("Enter your choice: ");
                    int updateOption;
                    scanf("%d", &updateOption);

                    // Update the course information in the file
                    switch (updateOption) {
                        case 1:
                            printf("Enter the updated course name: ");
                            getchar(); // Clear input buffer
                            fgets(course.courseName, sizeof(course.courseName), stdin);
                            course.courseName[strcspn(course.courseName, "\n")] = '\0'; // Remove newline character
                            break;
                        case 2:
                            printf("Enter the updated number of students enrolled: ");
                            scanf("%d", &course.numStudentsEnrolled);
                            break;
                        case 3:
                            return; // Exit the function to go back
                        default:
                            printf("Invalid choice. Please enter either '1', '2', or '3'.\n");
                            // Clear input buffer
                            while (getchar() != '\n');
                            continue; // Continue the loop to re-prompt the user
                    }
                    break; // Break out of the loop if a valid choice is made
                }


                updateCourseInFile(&course, lecturerName);
                printf("Course information updated successfully.\n");

            }  while  (1); // Loop until the user chooses to go back
        }
    } while (inputChoice == 'y' || inputChoice == 'Y'); // Loop until the user chooses not to update any more courses
}

void rewriteStudentProfiles(Student students[], int numStudents) {
    // Open the file in write mode to clear existing data and rewrite the profiles
    FILE *studentFile = fopen("student_profiles.txt", "w");
    if (studentFile == NULL) {
        printf("Error opening student profiles file for writing.\n");
        return;
    }

    // Iterate through the student array and write each student's profile to the file
    for (int i = 0; i < numStudents; i++) {
        // Write the student ID and name
        fprintf(studentFile, "%d,%s", students[i].userID, students[i].name);

        // Write the courses enrolled
        for (int j = 0; j < students[i].numCoursesEnrolled; j++) {
            fprintf(studentFile, ",%s", students[i].coursesEnrolled[j]);
        }

        // Write a single newline after each student's data
        fprintf(studentFile,"\n");
    }

    // Close the file
    fclose(studentFile);
}




void enrollStudents() {
    printf("Enrolling Students into Specific Course...\n");

    // Display the course information
    FILE *file = fopen("course_info.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Define the Course structure and initialize it
    Course course;
    printf("\nCourse Information:\n");
    printf("Code\tName\t\t\t\tStudents Enrolled\tLecturer\n");
    printf("*************************************************\n");

    // Display course information from the file
    while (fscanf(file, "%9[^,],%49[^,],%d,%49[^\n]%*c", course.courseCode, course.courseName, &course.numStudentsEnrolled, course.lecturerName) == 4) {
        printf("%-8s%-40s%-15d%-s\n", course.courseCode, course.courseName, course.numStudentsEnrolled, course.lecturerName);
    }
    fclose(file);

    // Prompt user to enter the course code to enroll students
    char courseCode[MAX_CODE];
    while (1) {
        printf("\nEnter the course code to enroll students (or 'b' to go back): ");
        scanf("%s", courseCode);

        if (strcmp(courseCode, "b") == 0 || strcmp(courseCode, "B") == 0) {
            printf("Returning to main menu.\n");
            return;
        }

        // Check if the entered course code exists in the course_info file
        int courseFound = 0;
        file = fopen("course_info.txt", "r");
        if (file == NULL) {
            printf("Error opening file for reading.\n");
            return;
        }

        while (fscanf(file, "%9[^,],%49[^,],%d,%49[^\n]%*c", course.courseCode, course.courseName, &course.numStudentsEnrolled, course.lecturerName) == 4) {
            if (strcmp(course.courseCode, courseCode) == 0) {
                courseFound = 1;
                break;
            }
        }
        fclose(file);

        if (courseFound) {
            break; // Exit loop if course code is found
        } else {
            printf("Course code not found. Please try again.\n");
        }
    }

    // Read existing student profiles to check for duplicate IDs
    int numStudents = 0;
    Student students[MAX_STUDENTS];
    readStudentProfiles(students, &numStudents);

    // Open the student_profiles.txt file to enroll students
    FILE *studentFile = fopen("student_profiles.txt", "a"); // Open in append mode
    if (studentFile == NULL) {
        printf("Error opening student profiles file.\n");
        return;
    }

    // Continuously enroll students until the user decides to stop
    while (1) {
        char input[10];
        printf("Enter student ID (or 'b' to stop): ");
        scanf("%9s", input);

        // Check if the user wants to stop
        if (strcmp(input, "b") == 0 || strcmp(input, "B") == 0) {
            break;
        }

        // Convert the input to an integer
        int userID = atoi(input);

        // Validate the user ID
        if (userID < 100 || userID > 999) {
            printf("User ID must be between 100 and 999. Please try again.\n");
            continue;
        }

        // Prompt for student name
        char studentName[MAX_NAME];
        printf("Enter student name: ");
        scanf("%49s", studentName);

        // Find or add the student in the array
        int studentIndex = -1;
        for (int i = 0; i < numStudents; i++) {
            if (students[i].userID == userID) {
                studentIndex = i;
                break;
            }
        }

        if (studentIndex == -1) {
            // If student does not exist, add a new student
            studentIndex = numStudents++;
            students[studentIndex].userID = userID;
            strcpy(students[studentIndex].name, studentName);
            students[studentIndex].numCoursesEnrolled = 0;
        }

        // Check if the student is already enrolled in the course
        int alreadyEnrolled = 0;
        for (int j = 0; j < students[studentIndex].numCoursesEnrolled; j++) {
            if (strcmp(students[studentIndex].coursesEnrolled[j], courseCode) == 0) {
                alreadyEnrolled = 1;
                break;
            }
        }

        if (alreadyEnrolled) {
            printf("Student is already enrolled in the course. Please try again.\n");
            continue;
        }

        // Enroll the student in the course
        strcpy(students[studentIndex].coursesEnrolled[students[studentIndex].numCoursesEnrolled], courseCode);
        students[studentIndex].numCoursesEnrolled++;
    }

    // Close the student profiles file
    fclose(studentFile);

    // After enrolling students, rewrite the student profiles file with the updated data
    rewriteStudentProfiles(students, numStudents);

    // Display the updated student profiles
    viewStudentProfiles(students, numStudents);

    // Update the number of students enrolled in the course_info file
    FILE *courseFile = fopen("course_info.txt", "r+");
    if (courseFile == NULL) {
        printf("Error opening course information file.\n");
        return;
    }

    int found = 0;
    char fileCourseCode[MAX_CODE];
    char fileCourseName[MAX_NAME];
    int fileNumStudentsEnrolled;

    // Rewind to the beginning of the file
    rewind(courseFile);

    // Read each line from the file and parse course data
    while (fscanf(courseFile, "%9[^,],%49[^,],%d%*c", fileCourseCode, fileCourseName, &fileNumStudentsEnrolled) == 3) {
        // Compare the course code read from the file with the user input
        if (strcmp(fileCourseCode, courseCode) == 0) {
            // Update the number of students enrolled in the course_info file
            fileNumStudentsEnrolled++; // Increment the number of students enrolled
            fseek(courseFile, -sizeof(int), SEEK_CUR); // Move back to overwrite the number of students enrolled
            fprintf(courseFile, "%3d", fileNumStudentsEnrolled); // Write the updated value with fixed width
            found = 1;
            break;
        }
    }

    fclose(courseFile);

    printf("Students enrolled successfully.\n");
}





void assignLecturer() {
    printf("Assigning Lecturer with Specific Course...\n");

    // Display the course information
    FILE *file = fopen("course_info.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    Course course;
    printf("\nCourse Information:\n");
    printf("Code\tName\t\t\t\tStudents Enrolled\tLecturer\n");

    // Display course information from the file
    while (fscanf(file, "%9[^,],%49[^,],%d,%49[^\n]%*c", course.courseCode, course.courseName, &course.numStudentsEnrolled, course.lecturerName) == 4) {
        printf("%-8s%-40s%-15d%-s\n", course.courseCode, course.courseName, course.numStudentsEnrolled, course.lecturerName);
    }

    // Close the file
    fclose(file);

    // Prompt user to enter the course code to assign lecturers
    char courseCode[MAX_CODE];
    while (1) {
        printf("Enter the course code to assign lecturers (or 'b' to go back): ");
        scanf("%s", courseCode);

        if (strcmp(courseCode, "b") == 0 || strcmp(courseCode, "B") == 0)
            return;

        // Check if the entered course code exists
        int courseFound = 0;
        file = fopen("course_info.txt", "r+");
        if (file == NULL) {
            printf("Error opening file for reading.\n");
            return;
        }

        while (fscanf(file, "%9[^,],%49[^,],%d,%49[^\n]%*c", course.courseCode, course.courseName, &course.numStudentsEnrolled, course.lecturerName) == 4) {
            if (strcmp(course.courseCode, courseCode) == 0) {
                courseFound = 1;
                break;
            }
        }
        fclose(file);

        if (courseFound)
            break; // Exit loop if course code is found
        else
            printf("Course code not found. Please try again.\n");
    }

    // Prompt user to enter the name of the lecturer
    printf("Enter the name of the lecturer: ");
    char lecturerName[MAX_NAME];
    scanf(" %[^\n]", lecturerName);
    getchar(); // Clear the input buffer

    // Update the course_info file with the lecturer information
    FILE *courseFile = fopen("course_info.txt", "r+");
    if (courseFile == NULL) {
        printf("Error opening course information file.\n");
        return;
    }

    int found = 0;
    char fileCourseCode[MAX_CODE];
    char fileCourseName[MAX_NAME];
    int fileNumStudentsEnrolled;
    char fileLecturerName[MAX_NAME]; // Add lecturer name field

    // Rewind to the beginning of the file
    rewind(courseFile);

    // Read each line from the file and parse course data
    while (fscanf(courseFile, "%9[^,],%49[^,],%d,%49[^\n]%*c", fileCourseCode, fileCourseName, &fileNumStudentsEnrolled, fileLecturerName) == 4) {
        // Compare the course code read from the file with the user input
        if (strcmp(fileCourseCode, courseCode) == 0) {
            // Update the lecturer name in the course_info file
            fseek(courseFile, -(strlen(fileLecturerName) + 1), SEEK_CUR); // Move back to overwrite the lecturer name
            fprintf(courseFile, "%-*s\n", (int)strlen(fileLecturerName), lecturerName); // Write the updated value
            found = 1;
            break;
        }
    }

    fclose(courseFile);

    if (found) {
        printf("Lecturer %s assigned to course %s\n", lecturerName, courseCode);
    } else {
        printf("Error: Course code not found.\n");
    }
}




 //my part
 // Function to clear the input buffer
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Define the User structure
typedef struct {
    int userID;
    char username[50];
    char password[50];
    char role[50];
} User;

// Function to check if a user exists
int userExists(int userID) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open users.txt\n");
        exit(1);
    }

    User user;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d %s %s \"%[^\"]\"", &user.userID, user.username, user.password, user.role);
        if (user.userID == userID) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}



// Function to register a new user
void registerUser(int userID, char *username, char *password, char *role){
    User newUser;
    newUser.userID = userID;
    strcpy(newUser.username, username);
    strcpy(newUser.password, password);
    strcpy(newUser.role, role);

    FILE *file = fopen("users.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d %s %s \"%s\"\n", newUser.userID, newUser.username, newUser.password, newUser.role);
        fclose(file);
    }
}



void inputAndRegisterUser() {
    int userID;
    char username[50];
    char password[50];
    char role[50];
    int roleChoice;
    int result; // to store the result of scanf

    // Ask for the user number
    do {
        printf("Enter a unique user number (3 digits): ");
        result = scanf("%d", &userID);
        if (result != 1 || getchar() != '\n') {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        if (userID < 100 || userID > 999) {
            printf("Invalid input. Please enter a 3-digit number.\n");
            continue;
        }
        if (userExists(userID)) {
            printf("Error: User number already exists. Please enter a different number.\n");
            continue;
        }
        break;
    } while(1);

    // Validate username
    do {
        printf("Enter username (must be between 1 and 50 characters): ");
        scanf("%s", username);
    } while (strlen(username) < 1 || strlen(username) > 50);

    // Validate password
    do {
        printf("Enter password (must be between 1 and 50 characters): ");
        scanf("%s", password);
    } while (strlen(password) < 1 || strlen(password) > 50);

    // Validate role
    do {
        printf("Choose role:\n1. Student\n2. Programme administrator\n3. Lecturer\n4. System Administrator\n");
        result = scanf("%d", &roleChoice);

        if (result == 1 && getchar() == '\n') {
            switch(roleChoice) {
                case 1:
                    strcpy(role, "STD");
                    break;
                case 2:
                    strcpy(role, "PAD");
                    break;
                case 3:
                    strcpy(role, "LCT");
                    break;
                case 4:
                    strcpy(role, "SAD");
                    break;
                default:
                    printf("Invalid choice. Please enter a number between 1 and 4.\n");
                    roleChoice = 0;  // Reset roleChoice to 0 to continue the loop
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
            roleChoice = 0;  // Reset roleChoice to 0 to continue the loop
        }
    } while (roleChoice == 0);


    registerUser(userID, username, password, role);
    printf("User registered successfully. Your user number is %d.\n", userID);
}



// Function to validate user credentials
int validateCredentials(int userID, char *password) {
    User user;
    FILE *file = fopen("users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %s %s \"%[^\"]\"", &user.userID, user.username, user.password, user.role) != EOF) {
//            printf("Debug: Read userID = %d, username = %s, password = %s, role = %s\n", user.userID, user.username, user.password, user.role); // Print debugging information
            if (user.userID == userID) {
                fclose(file);
                if (strcmp(user.password, password) == 0) {
                    return 1; // userID and password match
                } else {
                    return 0; // userID matches but password does not
                }
            }
        }
        fclose(file);
    }
    return -1; // userID not found
}

// Function to update username
void updateUsername(int userID, char *newUsername) {
    User user;
    FILE *file = fopen("users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    char line[256];
    if (file != NULL && tempFile != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s %s", &user.userID, user.username, user.password, user.role);
            if (user.userID == userID) {
                strcpy(user.username, newUsername);
            }
            fprintf(tempFile, "%d %s %s %s\n", user.userID, user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("users.txt");
        rename("temp.txt", "users.txt");
    }
}

// Function to update password
void updatePassword(int userID, char *newPassword) {
    User user;
    FILE *file = fopen("users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    char line[256];
    if (file != NULL && tempFile != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s %s", &user.userID, user.username, user.password, user.role);
            if (user.userID == userID) {
                strcpy(user.password, newPassword);
            }
            fprintf(tempFile, "%d %s %s %s\n", user.userID, user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("users.txt");
        rename("temp.txt", "users.txt");
    }
}

// Function to update user role
void updateUserRole(int userID, char *newRole) {
    User user;
    FILE *file = fopen("users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    char line[256];
    if (file != NULL && tempFile != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s \"%[^\"]\"", &user.userID, user.username, user.password, user.role);
            if (user.userID == userID) {
                strcpy(user.role, newRole);
            }
            fprintf(tempFile, "%d %s %s \"%s\"\n", user.userID, user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("users.txt");
        rename("temp.txt", "users.txt");
    }
}

// Function to read a specific user's details
void readSpecificUser(int userID) {
    User user;
    FILE *file = fopen("users.txt", "r");
    char line[256];
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s %s", &user.userID, user.username, user.password, user.role);
            if (user.userID == userID) {
                printf("User Number: %d\n", user.userID);
                printf("Username: %s\n", user.username);
                printf("Password: %s\n", user.password);
                printf("Role: %s\n", user.role);
                printf("\n");
                break;
            }
        }
        fclose(file);
    } else {
        printf("Unable to open file.\n");
    }
}


// Function to create a user
void createUser() {
    // Call the function to create a user
    inputAndRegisterUser();
}

void modifyUser() {
    int userID;
    char newUsername[50];
    char newPassword[50];
    char newRole[50];
    int roleChoice;
    int choice;
    int result; // to store the result of scanf

    printf("Enter user number of the user to modify: ");
    result = scanf("%d", &userID);
    if (result != 1 || getchar() != '\n') {
        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
        return;
    }

    if (!userExists(userID)) {
        printf("Error: User number does not exist. Please enter a valid user number.\n");
        return;
    }

    do {
        printf("What do you want to modify?\n1. Username\n2. Password\n3. User role\n4. Back to modify user\n");
        result = scanf("%d", &choice);
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
                    printf("Enter new username: ");
                    scanf("%s", newUsername);
                    updateUsername(userID, newUsername);
                    printf("Username updated successfully.\n");
                    break;
                case 2:
                    printf("Enter new password: ");
                    scanf("%s", newPassword);
                    updatePassword(userID, newPassword);
                    printf("Password updated successfully.\n");
                    break;
                case 3:
                    do {
                        printf("Enter new role:\n1. Student\n2. Programme administrator\n3. Lecturer\n4. System Administrator\n");
                        result = scanf("%d", &roleChoice);
                        if (result == 1 && getchar() == '\n') {
                            switch(roleChoice) {
                                case 1:
                                    strcpy(newRole, "STD");
                                    break;
                                case 2:
                                    strcpy(newRole, "PAD");
                                    break;
                                case 3:
                                    strcpy(newRole, "LCT");
                                    break;
                                case 4:
                                    strcpy(newRole, "SAD");
                                    break;
                                default:
                                    printf("Invalid choice. Please enter a number between 1 and 4.\n");
                                    roleChoice = 0;  // Reset roleChoice to 0 to continue the loop
                            }
                        } else {
                            printf("Invalid input. Please enter a number.\n");
                            // Clear the input buffer
                            while (getchar() != '\n');
                            roleChoice = 0;  // Reset roleChoice to 0 to continue the loop
                        }
                    } while (roleChoice == 0);
                    updateUserRole(userID, newRole);
                    printf("User role updated successfully.\n");
                    break;
                case 4:
                    printf("Returning to modify user...\n");
                    break;
                default:
                    printf("Invalid choice. Please enter a number between 1 and 4.\n");
                    break;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
            roleChoice = 0;  // Reset roleChoice to 0 to continue the loop
        }
    } while(choice != 4);
}

void readAllUsers() {
    User user;
    FILE *file = fopen("users.txt", "r");
    if (file != NULL) {
        printf("UserNumber Username Password Role\n");
        while (fscanf(file, "%d %s %s \"%[^\"]\"", &user.userID, user.username, user.password, user.role) != EOF) {
            printf("%d %s %s %s\n", user.userID, user.username, user.password, user.role);
        }
        fclose(file);
    } else {
        printf("Unable to opusers.txt\n");
    }
}

// Function to delete a user
void deleteUser() {
    int userID;
    printf("Enter user number of the user to delete: ");
    scanf("%d", &userID);

    FILE *file = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    User user;
    int userExists = 0; // flag to check if user exists
    if (file != NULL && tempFile != NULL) {
        while (fscanf(file, "%d %s %s %s\n", &user.userID, user.username, user.password, user.role) != EOF) {
            if (user.userID == userID) {
                userExists = 1; // set flag to true if user is found
            }
            if (user.userID != userID) {
                fprintf(tempFile, "%d %s %s %s\n", user.userID, user.username, user.password, user.role);
            }
        }
        fclose(file);
        fclose(tempFile);
        remove("users.txt");
        rename("temp.txt", "users.txt");
    }
    if (userExists) {
        printf("User deleted successfully.\n");
    } else {
        printf("Error: User does not exist.\n");
        return;
    }
}

void userManagementMenu() {
    int choice;
    int userID;
    int result; // to store the result of scanf
    do {
        printf("\n********** User Management Menu **********\n");
        printf("1. Create User\n");
        printf("2. Modify User\n");
        printf("3. View All Users\n");
        printf("4. View specific user\n");
        printf("5. Delete User\n");
        printf("6. Exit\n");
        printf("**************************\n");
        printf("Enter your choice: ");
        result = scanf("%d", &choice);

        // Check if the input was a number and there are no extra characters
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
                    createUser();
                    break;
                case 2:
                    modifyUser();
                    break;
                case 3:
                    readAllUsers();
                    break;
                case 4:
                    printf("Enter user number: ");
                    scanf("%d", &userID);
                    readSpecificUser(userID);
                    break;
                case 5:
                    deleteUser();
                    break;
                case 6:
                    printf("Exiting...\n");
                    return;  // Return from the function when logout is selected
                default:
                    printf("Invalid choice. Please enter a number between 1 and 6.\n");
                    break;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            clearInputBuffer();
        }
    } while(1);
}

void GradingSystem() {
    FILE *file = fopen("grading_system.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char markRange[10];
    char grade[3];
    float cgpa;

    printf("+------------+---------+-------+\n");
    printf("| %-10s | %-5s | %-5s |\n", "Mark", "Grade", "CGPA"); // Print the header
    printf("+------------+---------+-------+\n");

    while (fscanf(file, "%9[^|]| %2[^|]| %f", markRange, grade, &cgpa) == 3) {
        printf("| %-10s | %-5s | %-5.2f |\n", markRange, grade, cgpa); // Print each row
        printf("+------------+---------+-------+\n");
    }

    fclose(file);
}

// void GradingSystem() {
//     char is_update;
//     printf("\nCurrent grading system: \n");
//     show_grading_system();

//     printf("You want to update grading system (y/n): ");
//     scanf("%c", &is_update);

//     if (is_update == 'y') {
//         FILE *grading_file = fopen("grading_system.txt", "w");

//         if (grading_file == NULL) {
//             printf("Error creating grading system file.\n");
//             return;
//         }

//         char *grades[] = {"A+", "A", "B+", "B", "C+", "C", "C-", "D", "F"};
//         float cpa[] = {4.0, 3.7, 3.3, 3.0, 2.7, 2.3, 2.0, 1.6, 0};

//         int num_ranges = sizeof(grades)/sizeof(grades[0]);

//         for (int i = 0; i < num_ranges; i++) {
//             int lower_bound, upper_bound;

//             printf("Enter the upper bound for %s: ", grades[i]);
//             scanf("%d", &upper_bound);

//             printf("Enter the lower bound for %s: ", grades[i]);
//             scanf("%d", &lower_bound);

//             fprintf(grading_file, "%d - %d %s %.2lf\n", lower_bound, upper_bound, grades[i], cpa[i]);
//         }

//         fclose(grading_file);
//         printf("Grading system defined successfully.\n");
//     } else if (is_update == 'n') {
//         return;
//     } else {
//         printf("Please enter a valid value");
//     }
// }

// Function to display menus

//student menu
void studentMenu(int userID) {
    struct Student students[MAX_STUDENTS];
    int numStudents = 0;

    readPersonalDetails(students, &numStudents);
    readCourseDetails(students, numStudents);
    readGrades(students, numStudents);
    readAttendance(students, numStudents);

    int choice;
    int result; // to store the result of scanf
    do {
        printf("\n-- Student Information System --\n");
        printf("Logged in as user: %d\n", userID);

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

        do {
            printf("\n-- Options for %s --\n", students[studentIndex].name);
            printf("1. View Personal Details\n");
            printf("2. Modify Personal Details\n");
            printf("3. View Enrolled Courses, Grades, and CGPA\n");
            printf("4. View Attendance\n");
            printf("5. Logout\n");
            printf("Enter your choice: ");
            result = scanf(" %d", &choice);

            // Check if the input was a number and there are no extra characters
            if (result != 1 || getchar() != '\n') {
                printf("Invalid input. Please enter a number.\n");
                clearInputBuffer();
                continue;
            }

            // Check if the choice is within the expected range
            if (choice < 1 || choice > 5) {
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                continue;
            }

            // Process the choice
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
                    printf("Logging out...\n");
                    userID = 0; // Set userID to 0 to break the loop
                    break;
            }
        } while(choice < 1 || choice > 5);
    } while(userID != 0);
}


//programme admin menu
void programmeAdminMenu() {
    int choice;
    Student students[MAX_STUDENTS];
    int numStudents = 0;
    readStudentProfiles(students, &numStudents);
    int result; // to store the result of scanf
    do {
        printf("\n********** Programme Administrator Menu **********\n");
        printf("1. View Student Profiles\n");
        printf("2. View and Update Course Information\n");
        printf("3. Enroll Students into Specific Course\n");
        printf("4. Assign Lecturer with Specific Course\n");
        printf("5. Logout\n");
        printf("***********************************\n");
        printf("Enter your choice: ");

        result = scanf("%d", &choice); // Read the user's choice

        // Check if the input was a number and there are no extra characters
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
                    readStudentProfiles(students, &numStudents);
                    viewStudentProfiles(students, numStudents);
                    break;
                case 2:
                    viewAndUpdateCourseInfo();
                    break;
                case 3:
                    enrollStudents();
                    break;
                case 4:
                    assignLecturer();
                    break;
                case 5:
                    printf("Logging out...\n");
                    return;  // Return from the function when logout is selected
                default:
                    printf("Invalid choice. Please enter a number between 1 and 5.\n");
                    break;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            clearInputBuffer();
        }
    } while(1);
}


//lecturer menu
void lecturerMenu() {
    int choice;
    int result; // to store the result of scanf
    do {
        printf("\n********** Lecturer Menu **********\n");
        printf("1. View Grades\n");
        printf("2. Modify Grades\n");
        printf("3. View Attendance\n");
        printf("4. Update Attendance\n");
        printf("5. Logout\n");
        printf("***********************************\n");
        printf("Enter your choice: ");
        result = scanf("%d", &choice);

        // Check if the input was a number and there are no extra characters
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
                    viewGrades();
                    break;
                case 2:
                    modifyGrades();
                    break;
                case 3:
                    ltviewAttendance();
                    break;
                case 4:
                    updateAttendance();
                    break;
                case 5:
                    printf("Logging out...\n");
                    return;  // Return from the function when logout is selected
                default:
                    printf("Invalid choice. Please enter a number between 1 and 5.\n");
                    break;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            clearInputBuffer();
        }
    } while(1);
}


//system admin menu
void systemAdminMenu() {
    int choice;
    int result; // to store the result of scanf
    do {
        printf("\n********** System Administrator Menu **********\n");
        printf("1. User Management\n");
        printf("2. Grading System\n");
        printf("3. Logout\n");
        printf("**************************\n");
        printf("Enter your choice: ");
        result = scanf("%d", &choice);

        // Check if the input was a number and there are no extra characters
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
                    userManagementMenu();
                    break;
                case 2:
                    GradingSystem();
                    break;
                case 3:
                    printf("Logging out...\n");
                    return;  // Return from the function when logout is selected
                default:
                    printf("Invalid choice. Please enter a number between 1 and 4.\n");
                    break;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            clearInputBuffer();
        }
    } while(1);
}

// login system
int loginSystem() {
    int userID;
    char password[50];
    int loginResult = 0;

    // Login system
    do {
        printf("Enter userID: ");
        if(scanf("%d", &userID) != 1) {
            printf("Invalid userID. Please try again.\n");
            clearInputBuffer();
            continue;
        }
        getchar(); // Read and discard the newline character

        printf("Enter password: ");
        if(scanf("%49s", password) != 1) {
            printf("Invalid password. Please try again.\n");
            clearInputBuffer();
            continue;
        }
        getchar(); // Read and discard the newline character

        loginResult = validateCredentials(userID, password);
        if (loginResult == -1) {
            printf("Invalid userID. Please try again.\n");
        } else if (loginResult == 0) {
            printf("Invalid password. Please try again.\n");
        }
    } while (loginResult != 1);



    // Read user role
    User user;
    FILE *file = fopen("users.txt", "r");
    char line[256];
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s \"%[^\"]\"", &user.userID, user.username, user.password, user.role);
            if (user.userID == userID) {
                break;
            }
        }
        fclose(file);
    }

    // Redirect to appropriate menu based on user role
    if (strcmp(user.role, "STD") == 0) {
        studentMenu(userID);
    } else if (strcmp(user.role, "PAD") == 0) {
        programmeAdminMenu();
    } else if (strcmp(user.role, "LCT") == 0) {
        lecturerMenu();
    } else if (strcmp(user.role, "SAD") == 0) {
        systemAdminMenu();
    }

    return 1; // return 1 if login is successful
}

void mainMenu() {
    int choice;
    int exitProgram = 0; // flag to check if user wants to exit the program
    int result; // to store the result of scanf
    int loginSuccess = 0; // flag to check if user has successfully logged in
    do {
        printf("\n********** STUDENT-INFORMATION-SYSTEM **********\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("**************************\n");
        printf("Enter your choice: ");
        result = scanf("%d", &choice);

        // Check if the input was a number and there are no extra characters
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
                    loginSuccess = loginSystem();
                    if (loginSuccess == 1) {
                        int result; // to store the result of scanf
                        do {
                            printf("Do you want to exit the program? (1 for yes, 0 for no): ");
                            result = scanf("%d", &exitProgram);

                            // Check if the input was a number and there are no extra characters
                            if (result == 1 && getchar() == '\n') {
                                if (exitProgram == 0) {
                                    // Continue the program
                                    break;
                                } else if (exitProgram == 1) {
                                    printf("Exiting...\n");
                                    return; // If user wants to exit the program, return from the function
                                } else {
                                    printf("Invalid input. Please enter 0 or 1.\n");
                                }
                            } else {
                                printf("Invalid input. Please enter a number.\n");
                                // Clear the input buffer
                                clearInputBuffer();
                            }
                        } while(1);
                    }
                    break;
                case 2:
                    printf("Exiting...\n");
                    return;  // Return from the function when exit is selected
                default:
                    printf("Invalid choice. Please enter a number between 1 and 2.\n");
                    break;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        }
    } while(choice != 2);
}

int main() {
    mainMenu();
    return 0;
}
