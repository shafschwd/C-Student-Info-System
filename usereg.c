#include <stdio.h>
#include <string.h>

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
        fprintf(file, "%d %s %s %s\n", newUser.userNumber, newUser.username, newUser.password, newUser.role);
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
    FILE *countFile = fopen("count.txt", "r");
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

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    printf("Choose role:\n1. Student\n2. Programme administrator\n3. Lecturer\n4. System Administrator\n");
    scanf("%d", &roleChoice);

    switch(roleChoice) {
        case 1:
            strcpy(role, "student");
            break;
        case 2:
            strcpy(role, "Programme administrator");
            break;
        case 3:
            strcpy(role, "Lecturer");
            break;
        case 4:
            strcpy(role, "System Administrator");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            return;
    }

    registerUser(userNumber, username, password, role);
    printf("User registered successfully. Your user number is %d.\n", userNumber);
}





// Function to validate user credentials
int validateCredentials(char *username, char *password) {
    User user;
    FILE *file = fopen("../users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s %s\n", user.username, user.password, user.role) != EOF) {
            if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
    return 0;
}

// Function to update user password
void updateUserPassword(int userNumber, char *newPassword) {
    User user;
    FILE *file = fopen("../users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file != NULL && tempFile != NULL) {
        while (fscanf(file, "%d %s %s %s\n", &user.userNumber, user.username, user.password, user.role) != EOF) {
            if (user.userNumber == userNumber) {
                strcpy(user.password, newPassword);
            }
            fprintf(tempFile, "%d %s %s %s\n", user.userNumber, user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("users.txt");
        rename("temp.txt", "../users.txt");
    }
}

// Function to update user details
void updateUserDetails(int userNumber, char *newUsername, char *newRole) {
    User user;
    FILE *file = fopen("../users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file != NULL && tempFile != NULL) {
        while (fscanf(file, "%d %s %s %s\n", &user.userNumber, user.username, user.password, user.role) != EOF) {
            if (user.userNumber == userNumber) {
                if (newUsername != NULL) {
                    strcpy(user.username, newUsername);
                }
                if (newRole != NULL) {
                    strcpy(user.role, newRole);
                }
            }
            fprintf(tempFile, "%d %s %s %s\n", user.userNumber, user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("../users.txt");
        rename("temp.txt", "../users.txt");
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
    int newRole;
    int choice;

    printf("Enter user number of the user to modify: ");
    scanf("%d", &userNumber);

    printf("What do you want to modify?\n1. Username\n2. Password\n3. User role\n4. Back to modify user\n");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("Enter new username: ");
            scanf("%s", newUsername);
            updateUserDetails(userNumber, newUsername, NULL);
            break;
        case 2:
            printf("Enter new password: ");
            scanf("%s", newPassword);
            updateUserPassword(userNumber, newPassword);
            break;
        case 3:
            do {
                printf("Enter new role:\n1. Student\n2. Programme administrator\n3. Lecturer\n4. System Administrator\n");
                scanf("%d", &newRole);
                if(newRole < 1 || newRole > 4) {
                    printf("Invalid role. Please enter a number between 1 and 4.\n");
                }
            } while(newRole < 1 || newRole > 4);
            char roleStr[50];
            switch(newRole) {
                case 1:
                    strcpy(roleStr, "student");
                    break;
                case 2:
                    strcpy(roleStr, "Programme administrator");
                    break;
                case 3:
                    strcpy(roleStr, "Lecturer");
                    break;
                case 4:
                    strcpy(roleStr, "System Administrator");
                    break;
            }
            updateUserDetails(userNumber, NULL, roleStr);
            break;
        case 4:
            printf("Returning to modify user...\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            break;
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
    if (file != NULL && tempFile != NULL) {
        while (fscanf(file, "%d %s %s %s\n", &user.userNumber, user.username, user.password, user.role) != EOF) {
            if (user.userNumber != userNumber) {
                fprintf(tempFile, "%d %s %s %s\n", user.userNumber, user.username, user.password, user.role);
            }
        }
        fclose(file);
        fclose(tempFile);
        remove("users.txt");
        rename("temp.txt", "../users.txt");
    }
    printf("User deleted successfully.\n");
}



int main() {
    int choice;

    do {
        printf("\n********** MENU **********\n");
        printf("1. Create User\n");
        printf("2. Modify User\n");
        printf("3. Delete User\n");
        printf("4. Exit\n");
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
                deleteUser();
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
                break;
        }
    } while(choice != 4);

    return 0;
}