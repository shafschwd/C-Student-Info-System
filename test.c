#include <stdio.h>
#include <string.h>


// Define the User structure
typedef struct {
    char username[50];
    char password[50];
    char role[50];
    int number; // new field for user's number
} User;

// Function to register a new user
void registerUser(char *username, char *password, char *role) {
    User newUser;
    strcpy(newUser.username, username);
    strcpy(newUser.password, password);
    strcpy(newUser.role, role);

    FILE *file = fopen("users.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s\n", newUser.username, newUser.password, newUser.role);
        fclose(file);
    }
}

// Function to validate user credentials
int validateCredentials(char *username, char *password) {
    User user;
    FILE *file = fopen("users.txt", "r");
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