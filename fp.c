#include <stdio.h>//printf
#include <stdlib.h>//rand,srand,system(cls)
#include <time.h>//time(null) helps to generate different randoms numbers
#include <windows.h>//sleep,setconsole...,beep,cursor and console handling, position function
#include <conio.h>//getch,kbhit,these udes for keyboard control without pressing enter

#define MAX 20//sets maximum size to array
//global variables
//stores current array values
int arr[MAX], original[MAX];//sorted original unsorted values,used when reseting
int n = 10;//current no of elements

int running = 0;//0->not running,1->running
int paused = 0;
int resetRequested = 0;
int speed = 250;   // delay in milliseconds
//control console color
HANDLE h;
//function 1
void gotoxy(int x, int y) {//moves cursor to specific position in console
    COORD c;//creates coordinates variable
    c.X = x;//sets column position
    c.Y = y;//sets row position
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);//move cursor to that position
}//to print text at specific places,like UI heading and bars
//function 2
void hideCursor() {//hide blinking cursor in terminal bcoz UI looks cleaner
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}
//function 3
void setColor(int color) {//changes text/bar color in terminal
    SetConsoleTextAttribute(h, color);
}
//function 4
void clearScreen() {//clears entire screen bcoz to redraw visualizer again and again during animation
    system("cls");
}
//function 5
void copyOriginal() {//copies current array into original becoz reset can bring back to orignal array
    for (int i = 0; i < n; i++) {
        original[i] = arr[i];
    }
}
//function 6
void restoreOriginal() {//restores unsorted array from backup
    for (int i = 0; i < n; i++) {
        arr[i] = original[i];
    }
}
//function 7
void generateArray() {//generates random number
    srand(time(NULL));//intializes random seed using current time
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 15 + 2;//generates values from 2-16 approximately
    }
    copyOriginal();//saves generated array into original backup
}
//function 8
void drawUI(int algo) {//function prints heading and control
    gotoxy(0, 0);
    setColor(11);
    printf(" SORTING VISUALIZER ");
//prints top title
    gotoxy(0, 2);
    setColor(14);
    printf("Controls: ");
    setColor(15);
    printf("[1] Bubble  [2] Insertion  [3] Selection  [S] Start  [P] Pause/Resume  [R] Reset  [N] New Array  [+/-] Speed  [Q] Quit");
//prints control keys
    gotoxy(0, 4);
    setColor(10);
    printf("Current Algorithm: ");
    setColor(15);
//prints current algorithm name
    if (algo == 1) printf("Bubble Sort     ");
    else if (algo == 2) printf("Insertion Sort  ");
    else if (algo == 3) printf("Selection Sort  ");
    else printf("None            ");
//shows selected algorithm
    gotoxy(0, 5);
    setColor(10);
    printf("Speed: ");
    setColor(15);
    printf("%d ms    ", speed);
//shows current speed
    gotoxy(0, 6);
    setColor(10);
    printf("Status: ");
    setColor(15);
    if (running && !paused) printf("Running   ");
    else if (paused) printf("Paused    ");
    else printf("Idle      ");
    //shows current status
}
//function 9
void drawBars(int a, int b, int sortedStart, char *msg) {//main visualization function
    //a,b=1st and 2nd highlightened index
    //sortedstart=from which sorted index part begins
    //message to show above bars

    //it first clears old drawing area
    gotoxy(0, 8);
    for (int i = 0; i < 25; i++) {
        printf("                                                                                                    \n");
    }//this removes old bars
//then message shown
    gotoxy(0, 8);
    setColor(11);
    printf("%s", msg);

    gotoxy(0, 10);
//then bars are drawn row by row
    for (int row = 15; row >= 1; row--) {
        for (int i = 0; i < n; i++) {
            //this goes from top to bottom
            if (arr[i] >= row) {
                //if current value is high enough,print one block
                if (i == a || i == b)
                    setColor(14);      // yellow comparing
                else if (i >= sortedStart)
                    setColor(10);      // green sorted
                else
                    setColor(9);       // blue normal
//print one block
                printf(" ██ ");
            } else {
                //if no block needed print blank spaces
                printf("    ");
            }
        }
        printf("\n");
    }
//after bars,values are printed
    for (int i = 0; i < n; i++) {
        setColor(15);
        printf("%2d  ", arr[i]);
    }

    printf("\n");

    for (int i = 0; i < n; i++) {
        //arrows under compared bars
        if (i == a || i == b)
            printf(" ^^ ");
        else
            printf("    ");
    }

    printf("\n");

    for (int i = 0; i < n; i++) {
        //compared indexes
        if (i == a || i == b)
            printf(" %d  ", i);
        else
            printf("    ");
    }
}
//function 10
void checkControlKeys() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'p' || ch == 'P') {
            paused = !paused;
        } 
        //pause or resume
        else if (ch == 'r' || ch == 'R') {
            resetRequested = 1;
            running = 0;
        } 
        //reset requested
        else if (ch == '+') {
            if (speed > 50) speed -= 50;
        } 
        //faster animation
        else if (ch == '-') {
            speed += 50;
            //slower animation
        }
    }
//pause loop
    while (paused) {//keep pg in paused state until user resumes
        drawUI(0);
        gotoxy(0, 35);
        setColor(12);
        printf("PAUSED... Press P to Resume or R to Reset   ");

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'p' || ch == 'P') {
                paused = 0;
            } else if (ch == 'r' || ch == 'R') {
                resetRequested = 1;
                running = 0;
                paused = 0;
                break;
            }
        }
        Sleep(100);
    }
}
//function 11
void bubbleSortVisual() {//bubble sort with animation
    running = 1;
    resetRequested = 0;
    for (int i = 0; i < n - 1 && !resetRequested; i++) {//outer loop for passes
        char msg[100];
        sprintf(msg, "Bubble Sort - Pass %d", i + 1);
        for (int j = 0; j < n - i - 1 && !resetRequested; j++) {//inner loop for comparsions
            //before comparing->draws UI,highlights compared elements,plays sound,waits,checks keys
            drawUI(1);
            drawBars(j, j + 1, n - i, msg);
            Beep(700, 80);
            Sleep(speed);
            checkControlKeys();

            if (resetRequested) break;

            if (arr[j] > arr[j + 1]) {//swap condition
                int temp = arr[j];//if left element is bigg,swap
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                drawUI(1);
                drawBars(j, j + 1, n - i, "Swapping...");//redraw with swapping
                Beep(1000, 80);
                Sleep(speed);
                checkControlKeys();
            }
        }
    }

    if (!resetRequested) {
        drawUI(1);
        drawBars(-1, -1, 0, "Bubble Sort Completed!");
        gotoxy(0, 35);
        setColor(10);
        printf("SORTING COMPLETED!                              ");
    }

    running = 0;
}
//functio 12
void insertionSortVisual() {//perform insertion sort
    running = 1;
    resetRequested = 0;

    for (int i = 1; i < n && !resetRequested; i++) {//starts from 2nd element
        int key = arr[i];//key is the current element to insert
        int j = i - 1;
        char msg[100];
        sprintf(msg, "Insertion Sort - Step %d", i);

        while (j >= 0 && arr[j] > key && !resetRequested) {//left elem is bigg shift elem right
            drawUI(2);
            drawBars(j, j + 1, n, msg);
            Beep(700, 80);
            Sleep(speed);
            checkControlKeys();

            arr[j + 1] = arr[j];
            drawUI(2);
            drawBars(j, j + 1, n, "Shifting...");//show shifting...
            Beep(1000, 80);
            Sleep(speed);
            checkControlKeys();

            j--;
        }

        arr[j + 1] = key;//insert key in correct place
        drawUI(2);
        drawBars(j + 1, i, n, "Inserted key");//then show inserted key
        Sleep(speed);
        checkControlKeys();
    }

    if (!resetRequested) {
        drawUI(2);
        drawBars(-1, -1, 0, "Insertion Sort Completed!");
        gotoxy(0, 35);
        setColor(10);
        printf("SORTING COMPLETED!                              ");
    }

    running = 0;
}
//function 13
void selectionSortVisual() {//selection sort
    running = 1;
    resetRequested = 0;

    for (int i = 0; i < n - 1 && !resetRequested; i++) {//each pass finds min elem
        int min = i;//assume current elem is min
        char msg[100];
        sprintf(msg, "Selection Sort - Pass %d", i + 1);

        for (int j = i + 1; j < n && !resetRequested; j++) {//search remaining array
            drawUI(3);
            drawBars(min, j, 0, msg);
            Beep(700, 80);
            Sleep(speed);
            checkControlKeys();

            if (arr[j] < arr[min]) {//if smaller elem found
                min = j;
            }
        }
//after finding min -> swap min with current position
        if (min != i) {
            int temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;

            drawUI(3);
            drawBars(i, min, 0, "Swapping minimum...");//then show swapping minimum
            Beep(1000, 80);
            Sleep(speed);
            checkControlKeys();
        }
    }

    if (!resetRequested) {
        drawUI(3);
        drawBars(-1, -1, 0, "Selection Sort Completed!");
        gotoxy(0, 35);
        setColor(10);
        printf("SORTING COMPLETED!                              ");
    }

    running = 0;
}
//main function
int main() {
    h = GetStdHandle(STD_OUTPUT_HANDLE);//gets console output handle for colors
    hideCursor();//hides cursor
    generateArray();//creates random array
//default algorithm is bubble sort
    int algo = 1;
    char ch;

    while (1) {//runs forever until quits
        clearScreen();//shows idle screen
        drawUI(algo);
        //key input
        drawBars(-1, -1, n + 1, "Press S to Start Sorting");
//controls
        ch = _getch();
//choose algorithm
        if (ch == '1') algo = 1;
        else if (ch == '2') algo = 2;
        else if (ch == '3') algo = 3;
        //generates new random array
        else if (ch == 'n' || ch == 'N') generateArray();
        //restore original unsorted array
        else if (ch == 'r' || ch == 'R') restoreOriginal();
        //faster
        else if (ch == '+') {
            if (speed > 50) speed -= 50;
        }
        //slower
        else if (ch == '-') {
            speed += 50;
        }
        //when start pressed
        else if (ch == 's' || ch == 'S') {
            restoreOriginal();
            clearScreen();
            if (algo == 1) bubbleSortVisual();
            else if (algo == 2) insertionSortVisual();
            else if (algo == 3) selectionSortVisual();
//this restores original array and starts selected sort  |^
            gotoxy(0, 37);
            setColor(15);
            printf("Press any key to continue...");//waits before returning to menu
            _getch();
        }
        //quit
        else if (ch == 'q' || ch == 'Q') {
            break;
        }
    }
//resets color,clear screen,prints exit message
    setColor(15);
    clearScreen();
    printf("Exited Visualizer.\n");
    return 0;
}

