#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COURSES 100
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
} Student;

typedef struct {
    char courseCode[MAX_CODE];
    char courseName[MAX_NAME];
    int numStudentsEnrolled;
    char lecturerName[MAX_NAME];
} Course;

// Function prototypes
void readStudentProfiles(Student students[], int *numStudents);
void writeStudentProfiles(Student students[], int numStudents);
void viewStudentProfiles(Student students[], int numStudents);
void updateCourseInFile(const Course *course, const char *lecturerName);
void viewAndUpdateCourseInfo();
void enrollStudents();
void assignLecturer();

int main() {
    int choice;
    Student students[MAX_STUDENTS];
    int numStudents = 0;

    readStudentProfiles(students, &numStudents);

    do {
        printf("\nProgramme Administrator Menu\n");
        printf("1. View Student Profiles\n");
        printf("2. View and Update Course Information\n");
        printf("3. Enroll Students into Specific Course\n");
        printf("4. Assign Lecturer with Specific Course\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        
        // Check if the input is an integer
        if (scanf("%d", &choice) != 1) {
            // Clear the invalid input from the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
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
                printf("Logged out successfully.\n");
                
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 5);

    return 0;
}

void readStudentProfiles(Student students[], int *numStudents) {
    FILE *file = fopen("student_profiles.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    *numStudents = 0; // Initialize the number of students

    // Read each line from the file and parse student data
    while (*numStudents < MAX_STUDENTS &&
           fscanf(file, "%d,%49[^,\n],", &students[*numStudents].userID, students[*numStudents].name) == 2) {
    
        // Read course enrollment data
        students[*numStudents].numCoursesEnrolled = 0;
        char courseCode[MAX_COURSE_CODE_LENGTH];
        while (fscanf(file, "%9[^,\n]", courseCode) == 1) {
            strcpy(students[*numStudents].coursesEnrolled[students[*numStudents].numCoursesEnrolled], courseCode);
            (students[*numStudents].numCoursesEnrolled)++;
        }

        (*numStudents)++;
    }

    fclose(file);
}







void writeStudentProfiles(Student students[], int numStudents) {
    FILE *file = fopen("student_profiles.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numStudents; i++) {
        fprintf(file, "%d,%s", students[i].userID, students[i].name);
        // Write course enrollment data
        for (int j = 0; j < students[i].numCoursesEnrolled; j++) {
            fprintf(file, ",%s", students[i].coursesEnrolled[j]);
        }
        fprintf(file, "\n"); // Add newline character after each student entry
    }

    fclose(file);
}



void viewStudentProfiles(Student students[], int numStudents) {
    printf("Student Profiles:\n");
    printf("ID\tName\t\tCourses Enrolled\n");

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

void enrollStudents() {
    printf("Enrolling Students into Specific Course...\n");

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

        // Check if the entered course code exists
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

        if (courseFound)
            break; // Exit loop if course code is found
        else
            printf("Course code not found. Please try again.\n");
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

        // Check if the user ID is already in use
        int idInUse = 0;
        for (int i = 0; i < numStudents; i++) {
            if (students[i].userID == userID) {
                idInUse = 1;
                break;
            }
        }

        if (idInUse) {
            printf("User ID %d is already in use. Please enter a different user ID.\n", userID);
            continue;
        }

        // Prompt for student name
        char studentName[MAX_NAME];
        printf("Enter student name: ");
        scanf("%49s", studentName);

        // Write the student information into the student_profiles.txt file
        fprintf(studentFile, "%d,%s,%s\n", userID, studentName, courseCode);
        
        // Add the newly enrolled student to the students array
        students[numStudents].userID = userID;
        strcpy(students[numStudents].name, studentName);
        strcpy(students[numStudents].coursesEnrolled[students[numStudents].numCoursesEnrolled], courseCode);

        numStudents++;
    }

    // Close the student profiles file
    fclose(studentFile);

    // Update the student profiles in memory
    // Re-read the student profiles file in case the list was updated
    readStudentProfiles(students, &numStudents);

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
