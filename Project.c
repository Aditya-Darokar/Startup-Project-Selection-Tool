#include <stdio.h>
#include <stdlib.h>

#define MAXP 100
#define MAXR 100

int n;
int cost[MAXP], duration[MAXP], manpower[MAXP], valueArr[MAXP];
int maxBudget, maxTime, maxMan;

int bestBF;
int bestSelBF[MAXP];

// Brute-force approach using backtracking
void bruteRec(int current_index, int current_total_budget, int current_total_time,
            int current_total_manpower, int curVal, int sel[]) {
    if (current_index == n) {
        if (curVal > bestBF) {
            bestBF = curVal;
            for (int i = 0; i < n; i++)
                bestSelBF[i] = sel[i];
        }
        return;
    }

    sel[current_index] = 0;
    bruteRec(current_index + 1, current_total_budget, current_total_time,
            current_total_manpower, curVal, sel);

    if (current_total_budget + cost[current_index] <= maxBudget &&
        current_total_time + duration[current_index] <= maxTime &&
        current_total_manpower + manpower[current_index] <= maxMan) {
        sel[current_index] = 1;
        bruteRec(current_index + 1,
                current_total_budget + cost[current_index],
                current_total_time + duration[current_index],
                current_total_manpower + manpower[current_index],
                curVal + valueArr[current_index], sel);
        sel[current_index] = 0;
    }
}

void bruteForce() {
    int sel[MAXP] = {0};
    bestBF = 0;
    for (int i = 0; i < n; i++) bestSelBF[i] = 0;

    bruteRec(0, 0, 0, 0, 0, sel);

    printf("\n--- BRUTE FORCE (Exact) ---\n");
    printf("Max Value = %d\n", bestBF);
    printf("Selected Projects: ");
    int any = 0;
    for (int i = 0; i < n; i++) {
        if (bestSelBF[i]) {
            printf("P%d ", i + 1);
            any = 1;
        }
    }
    if (!any) printf("None");
    printf("\n");
}

// Greddy approach
void greedy() {
    double ratio[MAXP];
    int idx[MAXP];

    for (int i = 0; i < n; i++) {
        double denom = (double)(cost[i] + duration[i] + manpower[i]);
        if (denom <= 0.0) denom = 1.0;
        ratio[i] = (double)valueArr[i] / denom;
        idx[i] = i;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (ratio[idx[j]] > ratio[idx[i]]) {
                int t = idx[i];
                idx[i] = idx[j];
                idx[j] = t;
            }
        }
    }

    int remB = maxBudget, remT = maxTime, remM = maxMan;
    int total = 0;

    printf("\n--- GREEDY (Value / (Cost+Time+Manpower)) ---\n");
    printf("Selected Projects: ");

    for (int i = 0; i < n; i++) {
        int id = idx[i];
        if (cost[id] <= remB && duration[id] <= remT && manpower[id] <= remM) {
            remB -= cost[id];
            remT -= duration[id];
            remM -= manpower[id];
            total += valueArr[id];
            printf("P%d ", id + 1);
        }
    }

    printf("\nTotal Greedy Value = %d\n", total);
}

//Dynamic-programming approach
void dp() {
    static int DP[MAXR + 1][MAXR + 1][MAXR + 1];

    for (int b = 0; b <= maxBudget; b++)
        for (int t = 0; t <= maxTime; t++)
            for (int m = 0; m <= maxMan; m++)
                DP[b][t][m] = 0;

    for (int i = 0; i < n; i++) {
        int c = cost[i], tr = duration[i], ma = manpower[i], v = valueArr[i];
        for (int b = maxBudget; b >= c; b--) {
            for (int t = maxTime; t >= tr; t--) {
                for (int m = maxMan; m >= ma; m--) {
                    int cand = DP[b - c][t - tr][m - ma] + v;
                    if (cand > DP[b][t][m])
                        DP[b][t][m] = cand;
                }
            }
        }
    }

    printf("\n--- DYNAMIC PROGRAMMING (Multi-dim Exact) ---\n");
    printf("Optimal Value = %d\n", DP[maxBudget][maxTime][maxMan]);
}

//Main-function
int main() {
    int i;

    printf("\n============================================\n");
    printf("  WELCOME TO STARTUP PROJECT SELECTION TOOL\n");
    printf("============================================\n");
    printf("This tool helps you choose the best startup\n");
    printf("projects based on limited resources (budget,\n");
    printf("time and manpower).\n");
    printf("--------------------------------------------\n");

    printf("\nEnter Available Budget (0-%d) dollars: ", MAXR);
    if (scanf("%d", &maxBudget) != 1) return 0;
    if (maxBudget < 0) maxBudget = 0;
    if (maxBudget > MAXR) maxBudget = MAXR;

    printf("Enter Available Time (0-%d) months: ", MAXR);
    if (scanf("%d", &maxTime) != 1) return 0;
    if (maxTime < 0) maxTime = 0;
    if (maxTime > MAXR) maxTime = MAXR;

    printf("Enter Available Manpower (0-%d) workers: ", MAXR);
    if (scanf("%d", &maxMan) != 1) return 0;
    if (maxMan < 0) maxMan = 0;
    if (maxMan > MAXR) maxMan = MAXR;

    printf("\nEnter number of startup project ideas (max %d): ", MAXP);
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1) return 0;
    if (n > MAXP) n = MAXP;

    for (i = 0; i < n; i++) {
        printf("\n--- Enter Details of Project %d ---\n", i + 1);
        printf("Cost: ");
        scanf("%d", &cost[i]);
        if (cost[i] < 0) cost[i] = 0;
        if (cost[i] > MAXR) cost[i] = MAXR;

        printf("Duration (time): ");
        scanf("%d", &duration[i]);
        if (duration[i] < 0) duration[i] = 0;
        if (duration[i] > MAXR) duration[i] = MAXR;

        printf("Manpower: ");
        scanf("%d", &manpower[i]);
        if (manpower[i] < 0) manpower[i] = 0;
        if (manpower[i] > MAXR) manpower[i] = MAXR;

        printf("Expected Value/Profit: ");
        scanf("%d", &valueArr[i]);
        if (valueArr[i] < 0) valueArr[i] = 0;
    }

    int choice;
    while (1) {
        printf("\n============================================\n");
        printf("                 MAIN MENU                  \n");
        printf("============================================\n");
        printf("1. Display All Entered Projects\n");
        printf("2. Brute Force Approach\n");
        printf("3. Greedy Approach\n");
        printf("4. Dynamic Programming Approach\n");
        printf("5. Exit\n");
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) break;

        if (choice == 1) {
            printf("\n--------------------------------------------\n");
            printf("List of All Startup Projects\n");
            printf("--------------------------------------------\n");
            for (i = 0; i < n; i++) {
                printf("Project %d -> Cost: %d  Time: %d  Manpower: %d  Value: %d\n",
                    i + 1, cost[i], duration[i], manpower[i], valueArr[i]);
            }
        } else if (choice == 2) {
            bruteForce();
        } else if (choice == 3) {
            greedy();
        } else if (choice == 4) {
            dp();
        } else if (choice == 5) {
            printf("\nExiting... Thank you!\n");
            break;
        } else {
            printf("\nInvalid choice! Try again.\n");
        }
    }

    return 0;
}