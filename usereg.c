#include <stdio.h>
#include <string.h>

// Define the User structure
typedef struct {
    char username[50];
    char password[50];
    char role[50];
} User;



// Function to register a new user
void registerUser(char *username, char *password, char *role) {
    User newUser;
    strcpy(newUser.username, username);
    strcpy(newUser.password, password);
    strcpy(newUser.role, role);

    FILE *file = fopen("../users.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s\n", newUser.username, newUser.password, newUser.role);
        fclose(file);
    }
}

void inputAndRegisterUser() {
    char username[50];
    char password[50];
    char role[50];
    int roleChoice;

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

    registerUser(username, password, role);
    printf("User registered successfully.\n");
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
void updateUserPassword(char *username, char *newPassword) {
    User user;
    FILE *file = fopen("users.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file != NULL && tempFile != NULL) {
        while (fscanf(file, "%s %s %s\n", user.username, user.password, user.role) != EOF) {
            if (strcmp(user.username, username) == 0) {
                strcpy(user.password, newPassword);
            }
            fprintf(tempFile, "%s %s %s\n", user.username, user.password, user.role);
        }
        fclose(file);
        fclose(tempFile);
        remove("users.txt");
        rename("temp.txt", "users.txt");
    }
}

int main() {
    inputAndRegisterUser();
    return 0;
}