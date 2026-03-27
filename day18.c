#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define MAX_SIZE 50

int comparisons = 0;
int swaps = 0;
int steps = 0;
int delayTime = 200;
int ascending = 1;

void clearScreen() {
    system("cls");
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printHeader(char algoName[], int n, int pass) {
    setColor(11);
    printf("========================================\n");
    printf("       SORTING VISUALIZER - DAY 18      \n");
    printf("========================================\n");

    setColor(15);
    printf("Algorithm   : %s\n", algoName);
    printf("Order       : %s\n", ascending ? "Ascending" : "Descending");
    printf("Size        : %d\n", n);
    printf("Speed       : %d ms\n", delayTime);
    printf("Pass        : %d\n", pass);
    printf("Comparisons : %d\n", comparisons);
    printf("Swaps       : %d\n", swaps);
    printf("Steps       : %d\n", steps);

    printf("\nControls Info:\n");
    printf("1-Bubble  2-Insertion  3-Selection\n");
    printf("A-Ascending  D-Descending\n");
    printf("R-Randomize  C-Reset Counters\n");
    printf("+ Faster  - Slower\n");
    printf("========================================\n\n");
}

void drawBars(int arr[], int n, int a, int b, int sortedIndex) {
    for (int i = 0; i < n; i++) {
        if (i == a || i == b)
            setColor(14);   // yellow
        else if (i >= sortedIndex)
            setColor(10);   // green
        else
            setColor(11);   // cyan

        printf("(%d) ", arr[i]);
        for (int j = 0; j < arr[i]; j++) {
            printf("%c", 219);
        }
        printf("\n");
    }

    setColor(15);
    if (a != -1 && b != -1) {
        printf("\nComparing index %d and %d\n", a, b);
    }
}

int needSwap(int x, int y) {
    if (ascending)
        return x > y;
    else
        return x < y;
}

void bubbleSortVisual(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            steps++;

            clearScreen();
            printHeader("Bubble Sort", n, i + 1);
            drawBars(arr, n, j, j + 1, n - i);

            Sleep(delayTime);

            if (needSwap(arr[j], arr[j + 1])) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swaps++;
                steps++;

                clearScreen();
                printHeader("Bubble Sort", n, i + 1);
                drawBars(arr, n, j, j + 1, n - i);

                Beep(1000, 100);
                Sleep(delayTime);
            }
        }
    }

    clearScreen();
    printHeader("Bubble Sort", n, n - 1);
    drawBars(arr, n, -1, -1, 0);
    setColor(10);
    printf("\nSORTING COMPLETED!\n");
    setColor(15);
}

void insertionSortVisual(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && needSwap(arr[j], key)) {
            comparisons++;
            steps++;

            arr[j + 1] = arr[j];
            swaps++;

            clearScreen();
            printHeader("Insertion Sort", n, i);
            drawBars(arr, n, j, j + 1, n);

            Beep(900, 80);
            Sleep(delayTime);

            j--;
        }

        if (j >= 0) comparisons++;
        arr[j + 1] = key;
        steps++;

        clearScreen();
        printHeader("Insertion Sort", n, i);
        drawBars(arr, n, j + 1, i, n);

        Sleep(delayTime);
    }

    clearScreen();
    printHeader("Insertion Sort", n, n - 1);
    drawBars(arr, n, -1, -1, 0);
    setColor(10);
    printf("\nSORTING COMPLETED!\n");
    setColor(15);
}

void selectionSortVisual(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int selected = i;

        for (int j = i + 1; j < n; j++) {
            comparisons++;
            steps++;

            clearScreen();
            printHeader("Selection Sort", n, i + 1);
            drawBars(arr, n, selected, j, n);

            Sleep(delayTime);

            if (needSwap(arr[selected], arr[j])) {
                selected = j;
            }
        }

        if (selected != i) {
            int temp = arr[i];
            arr[i] = arr[selected];
            arr[selected] = temp;
            swaps++;
            steps++;

            clearScreen();
            printHeader("Selection Sort", n, i + 1);
            drawBars(arr, n, i, selected, n);

            Beep(1200, 100);
            Sleep(delayTime);
        }
    }

    clearScreen();
    printHeader("Selection Sort", n, n - 1);
    drawBars(arr, n, -1, -1, 0);
    setColor(10);
    printf("\nSORTING COMPLETED!\n");
    setColor(15);
}

void randomizeArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 20 + 1;
    }
}

void resetCounters() {
    comparisons = 0;
    swaps = 0;
    steps = 0;
}

int main() {
    int arr[MAX_SIZE], original[MAX_SIZE];
    int n, choice;
    char orderChoice;

    srand(time(NULL));

    printf("Enter array size (max %d): ", MAX_SIZE);
    scanf("%d", &n);

    if (n <= 0 || n > MAX_SIZE) {
        printf("Invalid size!\n");
        return 0;
    }

    randomizeArray(arr, n);

    for (int i = 0; i < n; i++) {
        original[i] = arr[i];
    }

    printf("\nChoose sorting order:\n");
    printf("A - Ascending\n");
    printf("D - Descending\n");
    printf("Enter choice: ");
    scanf(" %c", &orderChoice);

    if (orderChoice == 'D' || orderChoice == 'd')
        ascending = 0;
    else
        ascending = 1;

    printf("\nChoose algorithm:\n");
    printf("1. Bubble Sort\n");
    printf("2. Insertion Sort\n");
    printf("3. Selection Sort\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    printf("\nEnter speed in milliseconds (example 100 / 200 / 500): ");
    scanf("%d", &delayTime);

    resetCounters();

    switch (choice) {
        case 1:
            bubbleSortVisual(arr, n);
            break;
        case 2:
            insertionSortVisual(arr, n);
            break;
        case 3:
            selectionSortVisual(arr, n);
            break;
        default:
            printf("Invalid choice!\n");
    }

    setColor(15);
    return 0;
}