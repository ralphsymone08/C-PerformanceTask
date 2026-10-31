#include <stdio.h>

#define QUIZ_WEIGHT 0.05f
#define ACTIVITY_WEIGHT 0.05f
#define EXAM_WEIGHT 0.40f
#define PERFORMANCE_WEIGHT 0.50f
#define MIDTERM_WEIGHT 0.50f
#define FINAL_WEIGHT 0.50f

void displayMenu(void);
int getMenuChoice(void);
void displayGradingPolicy(void);
int isValidScore(float earned, float possible);
float calculateCategoryPercentage(float totalEarned, float totalPossible);
float getCategoryPercentage(const char categoryName[]);
float calculateWeightedGrade(float categoryPercentage, float categoryWeight);
float calculatePeriodGrade(char periodLabel);
float calculateOverallGrade(float midtermGrade, float finalGrade);
float convertToCollegeGrade(float overallPercentage);
char determineRemark(float overallPercentage);
void displayResults(float midtermGrade, float finalGrade, float overallPercentage, float collegeGrade, char remark);

int main(void) {
    int choice;
    float midtermGrade, finalGrade, overallPercentage, collegeGrade;
    char remark;

    do {
        displayMenu();
        choice = getMenuChoice();

        switch (choice) {
            case 1:
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
                displayGradingPolicy();
                break;
            case 3:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid selection.\n");
        }
    } while (choice != 3);

    return 0;
}

void displayMenu(void) {
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
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        choice = 0;
    }
    return choice;
}

void displayGradingPolicy(void) {
    printf("\n--- GRADING POLICY ---\n");
    printf("Weights: Quizzes(5%%), Activities(5%%), Exams(40%%), Performance(50%%)\n");
    printf("Periods: Midterm(50%%), Final(50%%)\n");
    printf("Conversion: 97-100(1.00), 94-96.99(1.25), 91-93.99(1.50), 88-90.99(1.75), 85-87.99(2.00), 82-84.99(2.25), 79-81.99(2.50), 76-78.99(2.75), 75-75.99(3.00), Below 75(5.00)\n");
    printf("Passing threshold: 75\n");
}

int isValidScore(float earned, float possible) {
    if (earned >= 0.0f && possible > 0.0f && earned <= possible) {
        return 1;
    } else {
        return 0;
    }
}

float calculateCategoryPercentage(float totalEarned, float totalPossible) {
    if (totalPossible <= 0.0f) {
        return -1.0f;
    }
    return (totalEarned / totalPossible) * 100.0f;
}

float getCategoryPercentage(const char categoryName[]) {
    int count;
    int i;
    float earned;
    float possible;
    float totalEarned = 0.0f;
    float totalPossible = 0.0f;

    do {
        printf("How many %s do you have? ", categoryName);
        if (scanf("%d", &count) != 1) {
            while (getchar() != '\n');
            count = -1;
        }
    } while (count <= 0);

    float earnedScores[count];
    float highestPossibleScores[count];

    for (i = 0; i < count; i++) {
        int valid = 0;
        do {
            printf("Assessment %d Earned Score: ", i + 1);
            if (scanf("%f", &earned) != 1) {
                while (getchar() != '\n');
                earned = -1.0f;
            }

            printf("Assessment %d HPS: ", i + 1);
            if (scanf("%f", &possible) != 1) {
                while (getchar() != '\n');
                possible = -1.0f;
            }

            if (isValidScore(earned, possible) == 1) {
                earnedScores[i] = earned;
                highestPossibleScores[i] = possible;
                totalEarned += earnedScores[i];
                totalPossible += highestPossibleScores[i];
                valid = 1;
            } else {
                printf("Invalid input. Earned must be >= 0, HPS > 0, and Earned <= HPS.\n");
            }
        } while (valid == 0);
    }

    return calculateCategoryPercentage(totalEarned, totalPossible);
}

float calculateWeightedGrade(float categoryPercentage, float categoryWeight) {
    return categoryPercentage * categoryWeight;
}

float calculatePeriodGrade(char periodLabel) {
    float quizPercentage, activityPercentage, examPercentage, performancePercentage;
    float quizContribution, activityContribution, examContribution, performanceContribution;
    float periodGrade;

    if (periodLabel == 'M') {
        printf("\n--- MIDTERM PERIOD ---\n");
    } else {
        printf("\n--- FINAL PERIOD ---\n");
    }

    quizPercentage = getCategoryPercentage("Quizzes");
    activityPercentage = getCategoryPercentage("Activities");
    examPercentage = getCategoryPercentage("Major Exams");
    performancePercentage = getCategoryPercentage("Performance Tasks");

    if (quizPercentage == -1.0f || activityPercentage == -1.0f || examPercentage == -1.0f || performancePercentage == -1.0f) {
        return -1.0f;
    }

    quizContribution = calculateWeightedGrade(quizPercentage, QUIZ_WEIGHT);
    activityContribution = calculateWeightedGrade(activityPercentage, ACTIVITY_WEIGHT);
    examContribution = calculateWeightedGrade(examPercentage, EXAM_WEIGHT);
    performanceContribution = calculateWeightedGrade(performancePercentage, PERFORMANCE_WEIGHT);

    periodGrade = quizContribution + activityContribution + examContribution + performanceContribution;
    return periodGrade;
}

float calculateOverallGrade(float midtermGrade, float finalGrade) {
    if (midtermGrade < 0.0f || finalGrade < 0.0f) {
        return -1.0f;
    }
    return (midtermGrade * MIDTERM_WEIGHT) + (finalGrade * FINAL_WEIGHT);
}

float convertToCollegeGrade(float overallPercentage) {
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
    if (overallPercentage >= 75.0f) {
        return 'P';
    } else {
        return 'F';
    }
}

void displayResults(float midtermGrade, float finalGrade, float overallPercentage, float collegeGrade, char remark) {
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