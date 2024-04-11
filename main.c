#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//cy part
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
            scanf(line, "%d %s %s %s", &user.userID, user.username, user.password, user.role);
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

// Function to display menus
void studentMenu() {
    int choice;
    int result; // to store the result of scanf
    do {
        printf("\n********** Student Menu **********\n");
        printf("1. View Courses\n");
        printf("2. Enroll in Course\n");
        printf("3. Drop Course\n");
        printf("4. View Grades\n");
        printf("5. Logout\n");
        printf("**************************\n");
        printf("Enter your choice: ");
        result = scanf("%d", &choice);

        // Check if the input was a number and there are no extra characters
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
                    // viewCourses();
                    break;
                case 2:
                    // enrollInCourse();
                    break;
                case 3:
                    // dropCourse();
                    break;
                case 4:
                    // viewGrades();
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
        printf("Enter your choice: ");

        // Check if the input was a number and there are no extra characters
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
//                    readStudentProfiles(students, &numStudents);
//                    viewStudentProfiles(students, numStudents);
                    break;
                case 2:
//                    viewAndUpdateCourseInfo();
                    break;
                case 3:
//                    enrollStudents();
                    break;
                case 4:
//                    assignLecturer();
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

void lecturerMenu() {
    int choice;
    int result; // to store the result of scanf
    do {
        printf("\n********** Lecturer Menu **********\n");
        printf("1. View Courses\n");
        printf("2. Add Grades\n");
        printf("3. Modify Grades\n");
        printf("4. View All Students\n");
        printf("5. Logout\n");
        printf("**************************\n");
        printf("Enter your choice: ");
        result = scanf("%d", &choice);

        // Check if the input was a number and there are no extra characters
        if (result == 1 && getchar() == '\n') {
            switch(choice) {
                case 1:
                    // viewCourses();
                    break;
                case 2:
                    // addGrades();
                    break;
                case 3:
                    // modifyGrades();
                    break;
                case 4:
                    // viewAllStudents();
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

void systemAdminMenu() {
    int choice;
    int result; // to store the result of scanf
    do {
        printf("\n********** System Administrator Menu **********\n");
        printf("1. User Management\n");
        printf("2. Grading System\n");
        printf("4. Logout\n");
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
                case 4:
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
        studentMenu();
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
                        printf("Do you want to exit the program? (1 for yes, 0 for no): ");
                        scanf("%d", &exitProgram);
                        if (exitProgram == 1) {
                            printf("Exiting...\n");
                            return; // If user wants to exit the program, return from the function
                        }
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

