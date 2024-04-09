#include <stdio.h>
#include <string.h>

 // Function to clear the input buffer
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Define the User structure
// Define the User structure
typedef struct {
    int userNumber;
    char username[50];
    char password[50];
    char role[50];
} User;



// Function to register a new user
void registerUser(int userNumber, char *username, char *password, char *role){
    User newUser;
    newUser.userNumber = userNumber;
    strcpy(newUser.username, username);
    strcpy(newUser.password, password);
    strcpy(newUser.role, role);

    FILE *file = fopen("../users.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d %s %s \"%s\"\n", newUser.userNumber, newUser.username, newUser.password, newUser.role);
        fclose(file);
    }
}



void inputAndRegisterUser() {
    int userNumber;
    char username[50];
    char password[50];
    char role[50];
    int roleChoice;

    // Read the current count from the file
    FILE *countFile = fopen("../count.txt", "r");
    if (countFile == NULL || fscanf(countFile, "%d", &userNumber) != 1) {
        // If the file doesn't exist or is empty, start from 1
        userNumber = 1;
    } else {
        // Otherwise, increment the count
        userNumber++;
    }
    fclose(countFile);

    // Update the count file with the new count
    countFile = fopen("count.txt", "w");
    fprintf(countFile, "%d", userNumber);
    fclose(countFile);

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
        scanf("%d", &roleChoice);

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
    } while (roleChoice == 0);

    registerUser(userNumber, username, password, role);
    printf("User registered successfully. Your user number is %d.\n", userNumber);
}


// Function to validate user credentials
int validateCredentials(char *username, char *password) {
    User user;
    FILE *file = fopen("../users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %s %s \"%[^\"]\"", &user.userNumber, user.username, user.password, user.role) != EOF) {
            if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
    return 0;
}

// Function to update username
void updateUsername(int userNumber, char *newUsername) {
    User user;
    FILE *file = fopen("../users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    char line[256];
    if (file != NULL && tempFile != NULL) {
        while (fgets(line, sizeof(line), file)) {
            scanf(line, "%d %s %s %s", &user.userNumber, user.username, user.password, user.role);
            if (user.userNumber == userNumber) {
                strcpy(user.username, newUsername);
            }
            fprintf(tempFile, "%d %s %s %s\n", user.userNumber, user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("../users.txt");
        rename("temp.txt", "../users.txt");
    }
}

// Function to update password
void updatePassword(int userNumber, char *newPassword) {
    User user;
    FILE *file = fopen("../users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    char line[256];
    if (file != NULL && tempFile != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s %s", &user.userNumber, user.username, user.password, user.role);
            if (user.userNumber == userNumber) {
                strcpy(user.password, newPassword);
            }
            fprintf(tempFile, "%d %s %s %s\n", user.userNumber, user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("../users.txt");
        rename("temp.txt", "../users.txt");
    }
}

// Function to update user role
void updateUserRole(int userNumber, char *newRole) {
    User user;
    FILE *file = fopen("../users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    char line[256];
    if (file != NULL && tempFile != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s \"%[^\"]\"", &user.userNumber, user.username, user.password, user.role);
            if (user.userNumber == userNumber) {
                strcpy(user.role, newRole);
            }
            fprintf(tempFile, "%d %s %s \"%s\"\n", user.userNumber, user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("../users.txt");
        rename("temp.txt", "../users.txt");
    }
}

// Function to read a specific user's details
void readSpecificUser(int userNumber) {
    User user;
    FILE *file = fopen("../users.txt", "r");
    char line[256];
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s %s", &user.userNumber, user.username, user.password, user.role);
            if (user.userNumber == userNumber) {
                printf("User Number: %d\n", user.userNumber);
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

// Function to modify a user
void modifyUser() {
    int userNumber;
    char newUsername[50];
    char newPassword[50];
    char newRole[50];
    int roleChoice;
    int choice;

    printf("Enter user number of the user to modify: ");
    scanf("%d", &userNumber);

    do {
        printf("What do you want to modify?\n1. Username\n2. Password\n3. User role\n4. Back to modify user\n");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Enter new username: ");
                scanf("%s", newUsername);
                updateUsername(userNumber, newUsername);
                printf("Username updated successfully.\n");
                break;
            case 2:
                printf("Enter new password: ");
                scanf("%s", newPassword);
                updatePassword(userNumber, newPassword);
                printf("Password updated successfully.\n");
                break;
            case 3:
                printf("Enter new role:\n1. Student\n2. Programme administrator\n3. Lecturer\n4. System Administrator\n");
                scanf("%d", &roleChoice);
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
                        printf("Invalid role. Please enter a number between 1 and 4.\n");
                        return;
                }
                updateUserRole(userNumber, newRole);
                printf("User role updated successfully.\n");
                break;
            case 4:
                printf("Returning to modify user...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
                break;
        }
    } while((choice>0)&&(choice <=4));
}

void readAllUsers() {
    User user;
    FILE *file = fopen("../users.txt", "r");
    if (file != NULL) {
        printf("UserNumber Username Password Role\n");
        while (fscanf(file, "%d %s %s \"%[^\"]\"", &user.userNumber, user.username, user.password, user.role) != EOF) {
            printf("%d %s %s %s\n", user.userNumber, user.username, user.password, user.role);
        }
        fclose(file);
    } else {
        printf("Unable to open users.txt\n");
    }
}

// Function to delete a user
void deleteUser() {
    int userNumber;
    printf("Enter user number of the user to delete: ");
    scanf("%d", &userNumber);

    FILE *file = fopen("../users.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    User user;
    int userExists = 0; // flag to check if user exists
    if (file != NULL && tempFile != NULL) {
        while (fscanf(file, "%d %s %s %s\n", &user.userNumber, user.username, user.password, user.role) != EOF) {
            if (user.userNumber == userNumber) {
                userExists = 1; // set flag to true if user is found
            }
            if (user.userNumber != userNumber) {
                fprintf(tempFile, "%d %s %s %s\n", user.userNumber, user.username, user.password, user.role);
            }
        }
        fclose(file);
        fclose(tempFile);
        remove("../users.txt");
        rename("temp.txt", "../users.txt");
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
    int userNumber;

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
        scanf("%d", &choice);

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
                scanf("%d", &userNumber);
                readSpecificUser(userNumber);
                break;
            case 5:
                deleteUser();
                break;
            case 6:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
                break;
        }
    } while(choice != 6);
}

// Function to display menus
void studentMenu() {
    int choice;
    do {
        printf("\n********** Student Menu **********\n");
        printf("1. View Courses\n");
        printf("2. Enroll in Course\n");
        printf("3. Drop Course\n");
        printf("4. View Grades\n");
        printf("5. Logout\n");
        printf("**************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
//                viewCourses();
                break;
            case 2:
//                enrollInCourse();
                break;
            case 3:
//                dropCourse();
                break;
            case 4:
//                viewGrades();
                break;
            case 5:
                printf("Logging out...\n");
                return;  // Return from the function when logout is selected
            default:
                printf("Invalid choice. Please enter a number between 1 and 2.\n");
                break;
        }
    } while(choice != 5);
}

void programmeAdminMenu() {
    int choice;
    do {
        printf("\n********** Programme Administrator Menu **********\n");
        printf("1. Add Course\n");
        printf("2. Modify Course\n");
        printf("3. Delete Course\n");
        printf("4. View All Courses\n");
        printf("5. Logout\n");
        printf("**************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
//                addCourse();
                break;
            case 2:
//                modifyCourse();
                break;
            case 3:
//                deleteCourse();
                break;
            case 4:
//                viewAllCourses();
                break;
            case 5:
                printf("Logging out...\n");
                return;  // Return from the function when logout is selected
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                break;
        }
    } while(choice != 5);
}

void lecturerMenu() {
    int choice;
    do {
        printf("\n********** Lecturer Menu **********\n");
        printf("1. View Courses\n");
        printf("2. Add Grades\n");
        printf("3. Modify Grades\n");
        printf("4. View All Students\n");
        printf("5. Logout\n");
        printf("**************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
//                viewCourses();
                break;
            case 2:
//                addGrades();
                break;
            case 3:
//                modifyGrades();
                break;
            case 4:
//                viewAllStudents();
                break;
            case 5:
                printf("Logging out...\n");
                return;  // Return from the function when logout is selected
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                break;
        }
    } while(choice != 5);
}

void systemAdminMenu() {
    int choice;
    int result; // to store the result of scanf
    do {
        printf("\n********** System Administrator Menu **********\n");
        printf("1. User Management\n");
        printf("2. Logout\n");
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
                    printf("Logging out...\n");
                    return;  // Return from the function when logout is selected
                default:
                    printf("Invalid choice. Please enter a number between 1 and 2.\n");
                    break;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            clearInputBuffer();
        }
    } while(1);
}

int loginSystem() {
    char username[50];
    char password[50];
    int loginSuccess = 0;

    // Login system
    do {
        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        loginSuccess = validateCredentials(username, password);
        if (!loginSuccess) {
            printf("Invalid username or password. Please try again.\n");
        }
    } while (!loginSuccess);

    // Read user role
    User user;
    FILE *file = fopen("../users.txt", "r");
    char line[256];
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d %s %s \"%[^\"]\"", &user.userNumber, user.username, user.password, user.role);
            if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
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

