#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 스도쿠 문제 생성 함수
// board : 생성될 스도쿠 문제를 저장할 배열
// n : 빈칸의 개수
void create(int board[9][9], int n);

// 스도쿠 문제 해결 함수
// board : 해결할 스도쿠 정보를 가지고 있는 배열
int solve(int board[9][9]);

// 깊이 우선 탐색을 이용하여 스도쿠 문제 해결을 위한 재귀함수
// solve함수에서 사용
// board : 해결할 스도쿠 정보를 가지고 있는 배열
// points : 빈칸의 좌표(x, y)들을 가지는 배열
// index : 바꿔야하는 좌표의 인덱스
// count : points의 길이
int dfs(int board[9][9], int* points, int index, int count);

// 스도쿠 규칙에 맞는지 확인하는 함수
/// board : 확인할 스도쿠 배열
int check(int board[9][9]);

// 두 변수의 값을 교환하는 함수
// a, b : 교환할 변수의 포인터
void swap(int* a, int* b);

// 스도쿠 문제를 입력 받아 저장하는 함수
// 0은 빈칸
// board : 문제를 저장할 배열
void inputBoard(int board[9][9]);

// 해결했거나 만든 문제를 출력하는 함수
// board : 출력할 문제를 저장한 배열
void printBoard(int board[9][9]);

int main()
{
    // 랜덤을 위한 시드값을 현재 시간으로 지정
    srand((unsigned int)time(NULL));

    // 스도쿠 정보를 저정할 배열
    int board[9][9] = {0};

    int choice;

    while (1) {
        // 스도쿠 해결 또는 문제 생성 중에 선택
        printf("1.스도쿠 해결\n2.스도쿠 문제 생성\n");
        scanf_s("%d", &choice);

        // 문제 해결
        if (choice == 1) {
            inputBoard(board);
            solve(board);
            printf("\n");
            printBoard(board);
            break;
        // 문제 생성
        } else if (choice == 2) {
            int n;
            printf("빈칸의 수 : ");
            scanf_s("%d", &n);
            create(board, n);
            printBoard(board);
            break;
        }
    }

    // 결과 출력 후 대기
    scanf_s("%s");

    return 0;
}

void create(int board[9][9], int n) {
    solve(board);

    // 스도쿠 모든 좌표를 가지는 배열
    int* points = (int*)malloc(sizeof(int) * 2 * 9 * 9);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            points[i * 9 * 2 + j * 2] = j;
            points[i * 9 * 2 + j * 2 + 1] = i;
        }
    }

    // points의 좌표들을 섞는다
    for (int i = 0; i < 81; i++) {
        int index = rand() % 81;

        swap(points + i * 2, points + index * 2);
        swap(points + i * 2 + 1, points + index * 2);
    }

    // 빈칸의 개수 만큼 points에서 좌표를 가져와 빈칸으로 만든다
    for (int i = 0; i < n && i < 81; i++) {
        int x = points[i * 2];
        int y = points[i * 2 + 1];

        board[y][x] = 0;
    }
}

int solve(int board[9][9]) {
    int* points;

    int count = 0;

    // 빈칸의 수를 센다
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (!board[i][j]) {
                count++;
            }
        }
    }

    // 빈칸의 좌표들을 points에 저장한다
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

    // dfs 함수를 호출하여 해결한다
    return dfs(board, points, 0, count);
}

int dfs(int board[9][9], int* points, int index, int count) {
    // 인덱스가 points의 개수를 넘어가면 문제가 해결되니 1을 반환하여 함수를 종료한다
    if (index >= count) {
        return 1;
    }

    // 현재 인덱스의 좌표
    int x = points[2 * index];
    int y = points[2 * index + 1];

    // 시도 해볼 숫자들의 배열
    int temp[9] = { 0 };

    for (int i = 0; i < 9; i++) {
        temp[i] = i + 1;
    }

    // 문제 생성에도 사용하기 위해 섞는다
    for (int i = 0; i < 9; i++) {
        int index = rand() % 9;

        swap(temp + i, temp + index);
    }

    // temp의 값들을 하나씩 넣어 본다
    for (int i = 0; i < 9; i++) {
        board[y][x] = temp[i];

        // 규칙에 맞지 않으면 다음 값을 시도한다
        if (!check(board)) {
            continue;
        }

        // index값을 + 1 하여 다음 좌표를 시도한 후 성공하면 1을 반환하며 함수를 종료한다
        if (dfs(board, points, index + 1, count)) {
            return 1;
        }
    }

    // 모든 값을 넣어서 실패하면 다시 0으로 바꾼후 0을 반환한다
    board[y][x] = 0;

    return 0;
}

int check(int board[9][9]) {
    // 가로 줄 확인
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

    // 세로 줄 확인
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

    // 3x3 박스들 확인
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

    // 모두 통과하면 1반환
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