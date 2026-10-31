/*
 Group 5

 Leader: Ralph Symone Manlangit
 Members: Trisha Jade Galimba
          Chris Benedict Pagbilao
          Christian Tuasoc
          June Arvi Batoctoy

 Date: October 31, 2026

*/

#include <stdio.h>

// Global variables as constants for category weights
const float QUIZ_WEIGHT = 0.05f;
const float ACTIVITY_WEIGHT = 0.05f;
const float EXAM_WEIGHT = 0.40f;
const float PERFORMANCE_WEIGHT = 0.50f;
const float MIDTERM_WEIGHT = 0.50f;
const float FINAL_WEIGHT = 0.50f; 

void displayMenu(void); // June's Function Prototype
int getMenuChoice(void); // June's Function Prototype
void displayGradingPolicy(void); // Christian's Function Prototype
int isValidScore(float earned, float possible); // Ralph's Function Prototype
float calculateCategoryPercentage(float totalEarned, float totalPossible); // Ralph's Function Prototype
float getCategoryPercentage(const char categoryName[]); // Ralph's Function Prototype
float calculateWeightedGrade(float categoryPercentage, float categoryWeight); // Trisha's Function Prototype
float calculatePeriodGrade(char periodLabel); // Ralph's Function Prototype
float calculateOverallGrade(float midtermGrade, float finalGrade); // Trisha's Function Prototype
float convertToCollegeGrade(float overallPercentage); // Christian's Function Prototype
char determineRemark(float overallPercentage); // Chris's Function Prototype
void displayResults(float midtermGrade, float finalGrade, float overallPercentage, float collegeGrade, char remark); // Chris's Function Prototype

int main(void) {
    int choice;
    float midtermGrade, finalGrade, overallPercentage, collegeGrade;
    char remark;

    do {
        displayMenu();
        choice = getMenuChoice();

        switch (choice) {
            case 1:
                // Calculate grades for midterm and final periods
                midtermGrade = calculatePeriodGrade('M');
                if (midtermGrade == -1.0f) {
                    printf("Error in Midterm calc.\n");
                    break;
                }

                finalGrade = calculatePeriodGrade('F');
                if (finalGrade == -1.0f) {
                    printf("Error in Final calc.\n");
                    break;
                }

                overallPercentage = calculateOverallGrade(midtermGrade, finalGrade);
                collegeGrade = convertToCollegeGrade(overallPercentage);
                remark = determineRemark(overallPercentage);

                displayResults(midtermGrade, finalGrade, overallPercentage, collegeGrade, remark);
                break;
            case 2:
                // Display the grading system
                displayGradingPolicy();
                break;
            case 3:
                // Exits the program
                printf("Exiting program.\n");
                break;
            default:
                // Handles invalid menu selections
                printf("Invalid selection.\n");
        }
    } while (choice != 3);

    return 0;
}

void displayMenu(void) {
    // This function serves as the main menu
    printf("=================================\n");
    printf("       STUDENT GRADE CALCULATOR\n");
    printf("=================================\n");
    printf("[1] Calculate Student Grade\n");
    printf("[2] View Grading System\n");
    printf("[3] Exit\n");
    printf("=================================\n");
    printf("Enter your choice: ");
}

int getMenuChoice(void) {
    // This function gets the user's menu choice (to be processed in main)
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        choice = 0;
    }
    return choice;
}

void displayGradingPolicy(void) {
    // This function shows the grading system (inspired by the CCS 102 class)
    printf("\n--- GRADING POLICY ---\n");
    printf("Weights: Quizzes(5%%), Activities(5%%), Exams(40%%), Performance(50%%)\n");
    printf("Periods: Midterm(50%%), Final(50%%)\n");
    printf("Conversion: 97-100(1.00), 94-96.99(1.25), 91-93.99(1.50), 88-90.99(1.75), 85-87.99(2.00), 82-84.99(2.25), 79-81.99(2.50), 76-78.99(2.75), 75-75.99(3.00), Below 75(5.00)\n");
    printf("Passing threshold: 75\n");
}

int isValidScore(float earned, float possible) {
    // Checks if the student's score does not exceed the HPS and is not negative
    // Also checks if the HPS is greater than 0
    if (earned >= 0.0f && possible > 0.0f && earned <= possible) {
        return 1;
    } else {
        return 0;
    }
}

float calculateCategoryPercentage(float totalEarned, float totalPossible) {
    // Converts the score into a percentage based on HPS and user's score
    if (totalPossible <= 0.0f) {
        return -1.0f;
    }
    return (totalEarned / totalPossible) * 100.0f;
}

float getCategoryPercentage(const char categoryName[]) {
    // Serves as the central function of the entire system
    // Gathers all the scores of a certain activity
    // Calculates the percentage of that activity
    int count;
    float earned;
    float possible;
    float totalEarned = 0.0f;
    float totalPossible = 0.0f;

    // Asks the user how many activities they have in a certain category
    // Serves as the number of times the system loops to gather all the scores
    do {
        printf("How many %s do you have? ", categoryName);
        if (scanf("%d", &count) != 1) {
            printf("Invalid input. Please enter a positive integer.\n");
            while (getchar() != '\n');
            count = -1;
        }
    } while (count <= 0);

    // Arrays in which all scores will be stored for later calculations
    float earnedScores[count];
    float highestPossibleScores[count];

    // Main loop engine
    for (int i = 0; i < count; i++) {
        int valid = 0;
        do {
            // Collects the HPS of an activity
            do {
                printf("Assessment %d HPS: ", i + 1);
                if (scanf("%f", &possible) != 1) {
                    while (getchar() != '\n');
                    possible = -1.0f;
                    printf("Invalid input. Please enter a non-negative number.\n");
                } else if (possible <= 0.0f) {
                    printf("Invalid input. HPS must be > 0.\n");
                }
            } while (possible <= 0.0f);

            // Collects the student's score of an activity
            do {
                printf("Assessment %d Earned Score: ", i + 1);
                if (scanf("%f", &earned) != 1) {
                    while (getchar() != '\n');
                    earned = -1.0f;
                    printf("Invalid input. Please enter a non-negative number.\n");
                } else if (earned < 0.0f) {
                    printf("Invalid input. Earned score must be >= 0.\n");
                }
            } while (earned < 0.0f);
            
            // Checks whether the collected scores is valid
            // If valid, the scores will be stored in the arrays and added to the total scores
            if (isValidScore(earned, possible) == 1) {
                earnedScores[i] = earned;
                highestPossibleScores[i] = possible;
                totalEarned += earnedScores[i];
                totalPossible += highestPossibleScores[i];

                // Displays the current total scores after every 2 assessments
                if ((i + 1) % 2 == 0) {
                    printf("Current earned total: %.2f\n", totalEarned);
                    printf("Current possible total: %.2f\n", totalPossible);
                }

                valid = 1;
            } else {
                printf("Invalid input. Earned must be >= 0, HPS > 0, and Earned <= HPS.\n");
            }
        } while (valid == 0);
    }

    return calculateCategoryPercentage(totalEarned, totalPossible);
}

float calculateWeightedGrade(float categoryPercentage, float categoryWeight) {
    // Calculates the grade of a certain activity considering its weight in the overall grade
    return categoryPercentage * categoryWeight;
}

float calculatePeriodGrade(char periodLabel) {
    // Calculates the grade of a certain period (midterm or final)
    float quizPercentage, activityPercentage, examPercentage, performancePercentage;
    float quizContribution, activityContribution, examContribution, performanceContribution;
    float periodGrade;

    if (periodLabel == 'M') {
        printf("\n--- MIDTERM PERIOD ---\n");
    } else {
        printf("\n--- FINAL PERIOD ---\n");
    }

    // Stores the percentage of each activity in a certain period
    quizPercentage = getCategoryPercentage("Quizzes");
    activityPercentage = getCategoryPercentage("Activities");
    examPercentage = getCategoryPercentage("Major Exams");
    performancePercentage = getCategoryPercentage("Performance Tasks");

    // Checks if there are any errors in the percentage calculations
    // Serves as an internal error handling mechanism
    if (quizPercentage == -1.0f || activityPercentage == -1.0f || examPercentage == -1.0f || performancePercentage == -1.0f) {
        return -1.0f;
    }

    // Calculates the grade of each activity given its percentage and weight
    quizContribution = calculateWeightedGrade(quizPercentage, QUIZ_WEIGHT);
    activityContribution = calculateWeightedGrade(activityPercentage, ACTIVITY_WEIGHT);
    examContribution = calculateWeightedGrade(examPercentage, EXAM_WEIGHT);
    performanceContribution = calculateWeightedGrade(performancePercentage, PERFORMANCE_WEIGHT);

    // Adds the total contributions of each activity to get the final grade of a certain period
    periodGrade = quizContribution + activityContribution + examContribution + performanceContribution;
    return periodGrade;
}

float calculateOverallGrade(float midtermGrade, float finalGrade) {
    // Calculates the final grade
    if (midtermGrade < 0.0f || finalGrade < 0.0f) {
        return -1.0f;
    }
    return (midtermGrade * MIDTERM_WEIGHT) + (finalGrade * FINAL_WEIGHT);
}

float convertToCollegeGrade(float overallPercentage) {
    // Converts the 0-based percentage to a 5.00-based college grade
    if (overallPercentage > 100.0f || overallPercentage < 0.0f) return -1.0f;
    if (overallPercentage >= 97.0f) return 1.00f;
    else if (overallPercentage >= 94.0f) return 1.25f;
    else if (overallPercentage >= 91.0f) return 1.50f;
    else if (overallPercentage >= 88.0f) return 1.75f;
    else if (overallPercentage >= 85.0f) return 2.00f;
    else if (overallPercentage >= 82.0f) return 2.25f;
    else if (overallPercentage >= 79.0f) return 2.50f;
    else if (overallPercentage >= 76.0f) return 2.75f;
    else if (overallPercentage >= 75.0f) return 3.00f;
    else return 5.00f;
}

char determineRemark(float overallPercentage) {
    // Checks whether the student passed based on the overall percentage
    if (overallPercentage >= 75.0f) {
        return 'P';
    } else {
        return 'F';
    }
}

void displayResults(float midtermGrade, float finalGrade, float overallPercentage, float collegeGrade, char remark) {
    // Displays the final results of the student's grades
    printf("\n========================================\n");
    printf("           STUDENT GRADE RESULTS\n");
    printf("========================================\n\n");
    printf("Midterm Percentage : %.2f%%\n", midtermGrade);
    printf("Final Percentage   : %.2f%%\n\n", finalGrade);
    printf("Overall Percentage : %.2f%%\n", overallPercentage);
    printf("College Grade      : %.2f\n", collegeGrade);
    printf("Final Remark       : %s\n", (remark == 'P') ? "PASSED" : "FAILED");
    printf("\n========================================\n");
}