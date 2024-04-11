#include <stdio.h>
#include <string.h>

// Define a structure for student
struct Student {
    int userID;
    char name[50];
    char courseId[7];
    char email[50];
    char dob[20];
    char phone[15];
    char enrolledCourses[5][50];
    int grades[5];
    float CGPA;
    int attendance[5];
};

// Function prototypes
void readPersonalDetails(struct Student *students, int *numStudents);
void readCourseDetails(struct Student *student);
void readGrades(struct Student *student);
void calculateCGPA(struct Student *student);
void readAttendance(struct Student *student);
void viewPersonalDetails(struct Student student);
void modifyPersonalDetails(struct Student *students, int numStudents);
void viewCourseDetails(struct Student student);
void viewGrades(struct Student student);
void viewAttendance(struct Student student);
void writePersonalDetails(struct Student *students, int numStudents);

int main() {
    // Sample data of students
    struct Student students[6];
    int numStudents = 0;

    // Read student profiles from file
    readPersonalDetails(students, &numStudents);

    // Menu-driven system
    int choice;
    int studentID;
    do {
        printf("\n-- Student Information System --\n");
        printf("Enter the student's id (or '0' to quit): ");
        scanf("%d", &studentID);

        if (studentID == 0) {
            printf("Exiting...\n");
            break;
        }

        int studentIndex = -1;
        for (int i = 0; i < numStudents; ++i) {
            if (students[i].userID == studentID) {
                studentIndex = i;
                break;
            }
        }

        if (studentIndex == -1) {
            printf("Student not found. Please try again.\n");
            continue;
        }

        do {
            printf("\n-- Options for %s --\n", students[studentIndex].name);
            printf("1. View Personal Details\n");
            printf("2. Modify Personal Details\n");
            printf("3. View Enrolled Courses\n");
            printf("4. View Grades\n");
            printf("5. View Attendance\n");
            printf("6. Exit\n");
            printf("Enter your choice: ");
            scanf(" %d", &choice);

            switch(choice) {
                case 1:
                    viewPersonalDetails(students[studentIndex]);
                    break;
                case 2:
                    modifyPersonalDetails(students, numStudents);
                    break;
                case 3:
                    viewCourseDetails(students[studentIndex]);
                    break;
                case 4:
                    viewGrades(students[studentIndex]);
                    break;
                case 5:
                    viewAttendance(students[studentIndex]);
                    break;
                case 6:
                    printf("Exiting...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while(choice != 6);
    } while(studentID != 0);

    return 0;
}

// Function to read personal details of students from file
void readPersonalDetails(struct Student *students, int *numStudents) {
    FILE *file = fopen("personal_detail.txt", "r");
    if (file) {
        int i = 0;
        while (fscanf(file, "%d,%49[^,],%6[^,],%49[^,],%19[^,],%14[^,],",
                      &students[i].userID, students[i].name, students[i].courseId,
                      students[i].email, students[i].dob, students[i].phone) == 6) {
            i++;
        }
        *numStudents = i;
        fclose(file);
    } else {
        printf("Error reading personal_detail.txt file.\n");
    }
}

// Function to read enrolled courses from file
void readCourseDetails(struct Student *student) {
    FILE *file = fopen("course.txt", "r");
    if (file) {
        for (int i = 0; i < 5; i++) {
            fscanf(file, "%49[^,\n]", student->enrolledCourses[i]);
        }
        fclose(file);
    } else {
        printf("Error reading course.txt file.\n");
    }
}

// Function to read grades from file
void readGrades(struct Student *student) {
    FILE *file = fopen("grade.txt", "r");
    if (file) {
        for (int i = 0; i < 5; i++) {
            fscanf(file, "%d", &student->grades[i]);
        }
        fclose(file);
    } else {
        printf("Error reading grade.txt file.\n");
    }
}

// Function to calculate CGPA
void calculateCGPA(struct Student *student) {
    float totalCredits = 0, totalGradePoints = 0;
    for(int i = 0; i < 5; i++) {
        totalCredits += 3;
        totalGradePoints += student->grades[i];
    }
    student->CGPA = totalGradePoints / totalCredits;
}

// Function to read attendance from file
void readAttendance(struct Student *student) {
    FILE *file = fopen("attendance.txt", "r");
    if (file) {
        for (int i = 0; i < 5; i++) {
            fscanf(file, "%d", &student->attendance[i]);
        }
        fclose(file);
    } else {
        printf("Error reading attendance.txt file.\n");
    }
}

// Function to view personal details of a student
void viewPersonalDetails(struct Student student) {
    printf("\n-- Personal Details --\n");
    printf("Name: %s\n", student.name);
    printf("Student ID: %d\n", student.userID);
    printf("Course ID: %s\n", student.courseId);
    printf("Email: %s\n", student.email);
    printf("Date of Birth: %s\n", student.dob);
    printf("Phone: %s\n", student.phone);
}

// Function to modify personal details of a student
void modifyPersonalDetails(struct Student *students, int numStudents) {
    printf("\n-- Modify Personal Details --\n");
    int userID;
    printf("Enter user ID of the student to modify: ");
    scanf("%d", &userID);

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

    // Write modified details back to file
    writePersonalDetails(students, numStudents);
}

// Function to write personal details back to file after modification
void writePersonalDetails(struct Student *students, int numStudents) {
    FILE *file = fopen("personal_detail.txt", "w");
    if (file) {
        for (int i = 0; i < numStudents; i++) {
            fprintf(file, "%d,%s,%s,%s,%s,%s\n", students[i].userID, students[i].name,
                    students[i].courseId, students[i].email, students[i].dob, students[i].phone);
        }
        fclose(file);
    } else {
        printf("Error writing personal_detail.txt file.\n");
    }
}

// Function to view enrolled courses of a student
void viewCourseDetails(struct Student student) {
    printf("\n-- Enrolled Courses --\n");
    for(int i = 0; i < 5; i++) {
        printf("Course %d: %s\n", i + 1, student.enrolledCourses[i]);
    }
}

// Function to view grades of a student
void viewGrades(struct Student student) {
    printf("\n-- Grades --\n");
    for(int i = 0; i < 5; i++) {
        printf("Course %d: %d\n", i + 1, student.grades[i]);
    }
}

// Function to view attendance of a student
void viewAttendance(struct Student student) {
    printf("\n-- Attendance --\n");
    for(int i = 0; i < 5; i++) {
        printf("Course %d: %d%%\n", i + 1, student.attendance[i]);
    }
}