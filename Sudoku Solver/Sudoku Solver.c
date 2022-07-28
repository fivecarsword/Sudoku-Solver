#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void create(int board[9][9]);
int solve(int board[9][9]);
int dfs(int board[9][9], int* points, int index, int count);
int check(int board[9][9]);
void swap(int* a, int* b);
void inputBoard(int board[9][9]);
void printBoard(int board[9][9]);

int main()
{
    srand((unsigned int)time(NULL));

    int board[9][9] = {0};

    int choice;

    while (1) {
        printf("1.스도쿠 해결\n2.스도쿠 문제 생성\n");
        scanf_s("%d", &choice);

        if (choice == 1) {
            inputBoard(board);
            solve(board);
            printf("\n");
            printBoard(board);
            break;
        } else if (choice == 2) {
            int n;
            printf("빈칸의 수 : ");
            scanf_s("%d", &n);
            create(board, n);
            printBoard(board);
            break;
        }
    }

    return 0;
}

void create(int board[9][9], int n) {
    solve(board);

    int* points = (int*)malloc(sizeof(int) * 2 * 9 * 9);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            points[i * 9 * 2 + j * 2] = j;
            points[i * 9 * 2 + j * 2 + 1] = i;
        }
    }

    for (int i = 0; i < 81; i++) {
        int index = rand() % 81;

        swap(points + i * 2, points + index * 2);
        swap(points + i * 2 + 1, points + index * 2);
    }

    for (int i = 0; i < n && i < 81; i++) {
        int x = points[i * 2];
        int y = points[i * 2 + 1];

        board[y][x] = 0;
    }
}

int solve(int board[9][9]) {
    int* points;

    int count = 0;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (!board[i][j]) {
                count++;
            }
        }
    }

    points = (int*)malloc(sizeof(int) * 2 * count);
    int index = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (!board[i][j]) {
                points[2*index] = j;
                points[2*index + 1] = i;
                index++;
            }
        }
    }

    return dfs(board, points, 0, count);
}

int dfs(int board[9][9], int* points, int index, int count) {
    if (index >= count) {
        return 1;
    }

    int x = points[2 * index];
    int y = points[2 * index + 1];

    int temp[9] = { 0 };

    for (int i = 0; i < 9; i++) {
        temp[i] = i + 1;
    }

    for (int i = 0; i < 9; i++) {
        int index = rand() % 9;

        swap(temp + i, temp + index);
    }

    for (int i = 0; i < 9; i++) {
        board[y][x] = temp[i];

        if (!check(board)) {
            continue;
        }

        if (dfs(board, points, index + 1, count)) {
            return 1;
        }
    }

    board[y][x] = 0;

    return 0;
}

int check(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        int temp[10] = {0};
        for (int j = 0; j < 9; j++) {
            int n = board[i][j];
            if (n != 0 && temp[n]) {
                return 0;
            }
            else {
                temp[n] = 1;
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        int temp[10] = { 0 };
        for (int j = 0; j < 9; j++) {
            int n = board[j][i];
            if (n != 0 && temp[n]) {
                return 0;
            }
            else {
                temp[n] = 1;
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        int temp[10] = { 0 };
        for (int j = 0; j < 9; j++) {
            int x = 3 * (i % 3) + j % 3;
            int y = 3 * (i / 3) + j / 3;

            int n = board[y][x];
            if (n != 0 && temp[n]) {
                return 0;
            }
            else {
                temp[n] = 1;
            }
        }
    }

    return 1;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void inputBoard(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int n;
            scanf_s("%d", &n);

            board[i][j] = n;
        }
    }
}

void printBoard(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}