#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

#define cl_red 68;
#define cl_grey 136;
#define cl_aqua 187;
#define cl_yellow 238;
#define cl_purple 85;
#define cl_blue 17;
#define cl_green 34;
#define cl_black 0;
#define cl_white 255;

// POINTS:
// 1 row = 100
// 2 row = 300
// 3 row = 600
// 4 row = 1000
// go down = 40
// go down slowly = 2

//set state of the board on the start of the game
void startBoard(char stateBefore[20][10], char board[20][10]){
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 10; j++){
            stateBefore[i][j] = ' ';
            board[i][j] = ' ';
        }
}

// set cursor position on coordintes a and b
void gotoxy(int a, int b){
    COORD c;
    c.X = b;
    c.Y = a;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// set cursor position on coordintes a and b to better fit square block shape
void gotoxy2(int a, int b){
    COORD c;
    c.X = b*2 + 2;
    c.Y = a + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//draws one single 'pixel'
void drawBlockColor(char type){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch(type){
    case ' ': SetConsoleTextAttribute(hConsole, 15); break;
    case '0': SetConsoleTextAttribute(hConsole, 34); break;
    case '1': SetConsoleTextAttribute(hConsole, 238); break;
    case '2': SetConsoleTextAttribute(hConsole, 187); break;
    case '3': SetConsoleTextAttribute(hConsole, 17); break;
    case '4': SetConsoleTextAttribute(hConsole, 85); break;
    case '5': SetConsoleTextAttribute(hConsole, 68); break;
    case '6': SetConsoleTextAttribute(hConsole, 136); break;
    case '#': SetConsoleTextAttribute(hConsole, 255); break;
    }
    printf("%c%c", type,type);
    SetConsoleTextAttribute(hConsole,15);
}

//draw start board
void display_board(){
    for (int i = 0 ; i < 12; i++)
        drawBlockColor('#');
    printf("\n");
    for (int i = 0; i < 20; i++){
        drawBlockColor('#');
        for (int j = 0; j < 10; j++)
            printf("  ");
        drawBlockColor('#');
        printf("\n");
    }
    for (int i = 0 ; i < 12; i++)
        drawBlockColor('#');

    gotoxy2(0,12);
    printf("Next block:");
    gotoxy2(5,12);
    printf("Points:");
    gotoxy2(7,12);
    printf("0");
    gotoxy2(10,12);
    printf("A - Left");
    gotoxy2(11,12);
    printf("D - One Down");
    gotoxy2(12,12);
    printf("D - Right");
    gotoxy2(13,12);
    printf("F - Rotate");
    gotoxy2(14,12);
    printf("G - Full Down");
}

//updates board of the next block
void updateBoardNext(char state[2][4]){
    for(int i = 0; i < 2; i++)
        for (int j = 0 ; j < 4; j++){
            gotoxy2(2 + i,13 + j);
            drawBlockColor(state[i][j]);
        }
    gotoxy(22,0);
}

//draw block on a given array
void drawBlockMenu(char board[2][4], char type){
    int positionx = 0;
    int positiony = 0;
    char boardType = type;
    for (int i = 0; i < 2; i ++)
        for (int j = 0; j < 4; j++)
            board[i][j] = ' ';
    switch (type)
    {
    case '0':                                               // ########
        board[positiony][positionx] = boardType; 
        board[positiony][positionx + 1] = boardType; 
        board[positiony][positionx + 2] = boardType; 
        board[positiony][positionx + 3] = boardType;
        break;      
    case '1':                                               // ##
        board[positiony][positionx] = boardType;            // ######
        board[positiony+1][positionx] = boardType;
        board[positiony+1][positionx+1] = boardType;
        board[positiony+1][positionx+2] = boardType; 
        break; 
    case '2':                                               //     ##
        board[positiony][positionx+2] = boardType;          // ######
        board[positiony+1][positionx] = boardType;
        board[positiony+1][positionx+1] = boardType;
        board[positiony+1][positionx+2] = boardType; 
        break; 
    case '3':                                               //   ####
        board[positiony][positionx+1] = boardType;          // ####
        board[positiony][positionx+2] = boardType;
        board[positiony+1][positionx] = boardType;
        board[positiony+1][positionx+1] = boardType;
        break;
    case '4':                                               // ####
        board[positiony][positionx] = boardType;            //   ####
        board[positiony][positionx+1] = boardType;
        board[positiony+1][positionx+1] = boardType;
        board[positiony+1][positionx+2] = boardType;
        break;
    case '5':                                               //   ##
        board[positiony][positionx+1] = boardType;          // ######
        board[positiony+1][positionx] = boardType;
        board[positiony+1][positionx+1] = boardType;
        board[positiony+1][positionx+2] = boardType;
        break;
    case '6':                                               // ####
        board[positiony][positionx+1] = boardType;          // ####
        board[positiony][positionx+2] = boardType;
        board[positiony+1][positionx+1] = boardType;
        board[positiony+1][positionx+2] = boardType;
        break;
    }
}

//draw block on a board and set its start parameters
bool drawBlock(char board[20][10], char type, int positiony, int positionx, int posPoints[4][2], int *currentPositionY, int *currentPositionX, int *blockLength){
    char boardType = type;
    switch (type)
    {
    case '0':                                               // ########
        posPoints[0][0] = positiony; posPoints[0][1] = positionx;
        posPoints[1][0] = positiony; posPoints[1][1] = positionx + 1;
        posPoints[2][0] = positiony; posPoints[2][1] = positionx + 2;
        posPoints[3][0] = positiony; posPoints[3][1] = positionx + 3;
        *currentPositionY = positiony - 1;
        *currentPositionX = positionx;
        *blockLength = 4;
        break;      
    case '1':                                               // ##
        posPoints[0][0] = positiony; posPoints[0][1] = positionx;
        posPoints[1][0] = positiony + 1; posPoints[1][1] = positionx;
        posPoints[2][0] = positiony + 1; posPoints[2][1] = positionx + 1;
        posPoints[3][0] = positiony + 1; posPoints[3][1] = positionx + 2;
        *currentPositionY = positiony - 1;
        *currentPositionX = positionx;
        *blockLength = 3;
        break; 
    case '2':                                               //     ##
        posPoints[0][0] = positiony; posPoints[0][1] = positionx + 2;
        posPoints[1][0] = positiony + 1; posPoints[1][1] = positionx;
        posPoints[2][0] = positiony + 1; posPoints[2][1] = positionx + 1;
        posPoints[3][0] = positiony + 1; posPoints[3][1] = positionx + 2;
        *currentPositionY = positiony - 1;
        *currentPositionX = positionx;
        *blockLength = 3;
        break; 
    case '3':                                               //   ####
        posPoints[0][0] = positiony; posPoints[0][1] = positionx + 1;
        posPoints[1][0] = positiony; posPoints[1][1] = positionx + 2;
        posPoints[2][0] = positiony + 1; posPoints[2][1] = positionx;
        posPoints[3][0] = positiony + 1; posPoints[3][1] = positionx + 1;
        *currentPositionY = positiony - 1;
        *currentPositionX = positionx;
        *blockLength = 3;
        break;
    case '4':                                               // ####
        posPoints[0][0] = positiony; posPoints[0][1] = positionx;
        posPoints[1][0] = positiony; posPoints[1][1] = positionx + 1;
        posPoints[2][0] = positiony + 1; posPoints[2][1] = positionx + 1;
        posPoints[3][0] = positiony + 1; posPoints[3][1] = positionx + 2;
        *currentPositionY = positiony - 1;
        *currentPositionX = positionx;
        *blockLength = 3;
        break;
    case '5':                                               //   ##
        posPoints[0][0] = positiony; posPoints[0][1] = positionx + 1;
        posPoints[1][0] = positiony + 1; posPoints[1][1] = positionx;
        posPoints[2][0] = positiony + 1; posPoints[2][1] = positionx + 1;
        posPoints[3][0] = positiony + 1; posPoints[3][1] = positionx + 2;
        *currentPositionY = positiony - 1;
        *currentPositionX = positionx;
        *blockLength = 3;
        break;
    case '6':                                               // ####
        posPoints[0][0] = positiony; posPoints[0][1] = positionx + 1;
        posPoints[1][0] = positiony; posPoints[1][1] = positionx + 2;
        posPoints[2][0] = positiony + 1; posPoints[2][1] = positionx + 1;
        posPoints[3][0] = positiony + 1; posPoints[3][1] = positionx + 2;
        *currentPositionY = positiony;
        *currentPositionX = positionx + 1;
        *blockLength = 2;
        break;
    }
    for (int i = 0; i < 4; i++)
        if (board[posPoints[i][0]][posPoints[i][1]] != ' ')
            return false;
        else
            board[posPoints[i][0]][posPoints[i][1]] = boardType;
    return true;
}

//update only necessary 'pixels' on board to avoid flickering
void updateBoard(char stateBefore[20][10], char stateAfter[20][10]){
    for(int i = 0; i < 20; i++)
        for (int j = 0 ; j < 10; j++)
            if (stateBefore[i][j] != stateAfter[i][j]){
                gotoxy2(i,j);
                drawBlockColor(stateAfter[i][j]);
                stateBefore[i][j] = stateAfter[i][j];
            }
    gotoxy(22,0);
}

bool isTouchingGround(char board[20][10], int posPoints[4][2]){
    int minn = 30, maxx = -1;
    for (int i = 0; i < 4; i++){                                                    // finding start block X position and its length
        if (posPoints[i][1] > maxx)
            maxx = posPoints[i][1];
        if (posPoints[i][1] < minn)
            minn = posPoints[i][1];
    }
    int floorLength = maxx - minn + 1;
    int floor[4] = {-1,-1,-1,-1};                                                   // floor = every block X position max Y position
    for (int i = 0; i < floorLength; i++){
        for (int j = 0; j < 4; j++){
            if (posPoints[j][1] == minn + i && posPoints[j][0] > floor[i])
                floor[i] = posPoints[j][0];
        }
    }

    for (int i = 0; i < floorLength; i++)                                           // checking if max Y position is touching Ground
        if (floor[i] == 19 || board[floor[i] + 1][minn + i] != ' ')
            return true;
    return false;
}

//fall down 1 block
void fallDown(char board[20][10], int posPoints[4][2], char type, int *currentPositionY){
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = ' ';
    }
    for (int i = 0; i < 4; i++){
        posPoints[i][0] += 1;
        board[posPoints[i][0]][posPoints[i][1]] = type;
    }
    *currentPositionY += 1;
}

//check if coordinates are of block
bool isSameBlock(int posPoints[4][2], int y, int x){
    for (int i = 0; i < 4; i++)
        if (y == posPoints[i][0] && x == posPoints[i][1])
            return true;
    return false;
}

bool rotateBlock(char board[20][10], int posPoints[4][2], int currentPositionY, int currentPositionX, int blockLength, char type){
    if (blockLength <= 2)
        return true;
    int posPointsCopy[4][2];
    for (int i = 0; i < 4; i++){
        posPointsCopy[i][0] = posPoints[i][1] - currentPositionX + currentPositionY;
        posPointsCopy[i][1] = blockLength - (posPoints[i][0] - currentPositionY) - 1 + currentPositionX;
        if (board[posPointsCopy[i][0]][posPointsCopy[i][1]] != ' ' && !isSameBlock(posPoints,posPointsCopy[i][0], posPointsCopy[i][1]))
            return false;
        if (posPointsCopy[i][0] < 0 || posPointsCopy[i][0] > 19 || posPointsCopy[i][1] < 0 || posPointsCopy[i][1] > 9)
            return false;
    }
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = ' ';
        posPoints[i][0] = posPointsCopy[i][0];
        posPoints[i][1] = posPointsCopy[i][1];
    }   
    for (int i = 0; i < 4; i++)
        board[posPoints[i][0]][posPoints[i][1]] = type;
    return true;
}

bool moveBlockRight(char board[20][10], int posPoints[4][2], int *currentPositionX, char type){
    int minn = 30, maxx = -1;
    for (int i = 0; i < 4; i++){                                                    // finding start block X position and its length
        if (posPoints[i][0] > maxx)
            maxx = posPoints[i][0];
        if (posPoints[i][0] < minn)
            minn = posPoints[i][0];
    }
    int sideLength = maxx - minn + 1;
    int side[4] = {-1,-1,-1,-1};                                                   // floor = every block X position max Y position
    for (int i = 0; i < sideLength; i++){
        for (int j = 0; j < 4; j++){
            if (posPoints[j][0] == minn + i && posPoints[j][1] > side[i])
                side[i] = posPoints[j][1];
        }
    }
    for (int i = 0; i < sideLength; i++)                                           // checking if max Y position is touching Ground
        if (side[i] == 9 || board[minn + i][side[i] + 1] != ' ')
            return false;
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = ' ';
        posPoints[i][1] += 1;
    }
    *currentPositionX += 1;
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = type;
    }
    return true;
}

bool moveBlockLeft(char board[20][10], int posPoints[4][2], int *currentPositionX, char type){
    int minn = 30, maxx = -1;
    for (int i = 0; i < 4; i++){                                                    // finding start block X position and its length
        if (posPoints[i][0] > maxx)
            maxx = posPoints[i][0];
        if (posPoints[i][0] < minn)
            minn = posPoints[i][0];
    }
    int sideLength = maxx - minn + 1;
    int side[4] = {20,20,20,20};                                                   // floor = every block X position max Y position
    for (int i = 0; i < sideLength; i++){
        for (int j = 0; j < 4; j++){
            if (posPoints[j][0] == minn + i && posPoints[j][1] < side[i])
                side[i] = posPoints[j][1];
        }
    }
    for (int i = 0; i < sideLength; i++)                                           // checking if max Y position is touching Ground
        if (side[i] == 0 || board[minn + i][side[i] - 1] != ' ')
            return false;
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = ' ';
        posPoints[i][1] -= 1;
    }
    *currentPositionX -= 1;
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = type;
    }
    return true;
}

bool moveBlockDown(char board[20][10], int posPoints[4][2], int *currentPositionY, char type, int *points){
    int minn = 30, maxx = -1;
    for (int i = 0; i < 4; i++){                                                    // finding start block X position and its length
        if (posPoints[i][1] > maxx)
            maxx = posPoints[i][1];
        if (posPoints[i][1] < minn)
            minn = posPoints[i][1];
    }
    int floorLength = maxx - minn + 1;
    int floor[4] = {-1,-1,-1,-1};                                                   // floor = every block X position max Y position
    for (int i = 0; i < floorLength; i++){
        for (int j = 0; j < 4; j++){
            if (posPoints[j][1] == minn + i && posPoints[j][0] > floor[i])
                floor[i] = posPoints[j][0];
        }
    }
    for (int i = 0; i < floorLength; i++)                                           // checking if max Y position is touching Ground
        if (floor[i] == 19 || board[floor[i] + 1][minn + i] != ' ')
            return false;
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = ' ';
        posPoints[i][0] += 1;
    }
    *currentPositionY += 1;
    *points += 2;
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = type;
    }
    return true;
}

void moveFullDown(char board[20][10], int posPoints[4][2], int *currentPositionY, char type, int *points){
    int minn = 30, maxx = -1;
    for (int i = 0; i < 4; i++){                                                    // finding start block X position and its length
        if (posPoints[i][1] > maxx)
            maxx = posPoints[i][1];
        if (posPoints[i][1] < minn)
            minn = posPoints[i][1];
    }
    int floorLength = maxx - minn + 1;
    int floor[4] = {-1,-1,-1,-1};                                                   // floor = every block X position max Y position
    for (int i = 0; i < floorLength; i++){
        for (int j = 0; j < 4; j++){
            if (posPoints[j][1] == minn + i && posPoints[j][0] > floor[i])
                floor[i] = posPoints[j][0];
        }
    }

    int nearestBlock[4] = {-1,-1,-1,-1};

    for (int i = 0; i < floorLength; i++){
        int lMax = 0;
        for (int j = floor[i] + 1; j < 20; j++){
            if (board[j][minn + i] != ' ')
                break;
            lMax+=1;
        }
        nearestBlock[i] = lMax;
    }
    int nearestRow = 100;
    for (int i = 0; i < floorLength; i++)
        if (nearestBlock[i] < nearestRow)
            nearestRow = nearestBlock[i];
    
    for (int i = 0; i < 4; i++){
        board[posPoints[i][0]][posPoints[i][1]] = ' ';
        posPoints[i][0] += nearestRow;
    }
    *points += 2*nearestRow;
    for (int i = 0; i < 4; i ++)
        board[posPoints[i][0]][posPoints[i][1]] = type;
}

//checking if there's any full row in a field where a block dropped
void isThereRow(char board[20][10], int posPoints[4][2], int row[4], int *sideLength){
    int minn = 30, maxx = -1;

    // finding start block X position and its length
    for (int i = 0; i < 4; i++){
        if (posPoints[i][0] > maxx)
            maxx = posPoints[i][0];
        if (posPoints[i][0] < minn)
            minn = posPoints[i][0];
    }
    *sideLength = maxx - minn + 1;

    for(int i = 0; i < *sideLength; i++){
        row[i] = minn + i;
        for (int j = 0; j < 10; j ++){
            if (board[minn + i][j] == ' ')
                row[i] = -1;
        }
    }
}

//deletes row of a given id
void deleteRow(char board[20][10], int row){
    for (int i = row; i > 0; i--){
        for (int k = 0; k < 10; k++)
            board[i][k] = board[i - 1][k];
    }
    for (int k = 0; k < 10; k++)
        board[0][k] = ' ';
}

//update points on display
void updatePoints(int points, int *pointsBefore){
    if (points != *pointsBefore){
        gotoxy2(7,12);
        printf("%i",points);
        *pointsBefore = points;
    }
}

void gameplay(){
    system("cls");
    display_board();
    srand(time(NULL));

    char c;                                 // keyboard input
    const float fallTime = 0.4f;            // when block goes 1 pixel down
    float timeMeasured;
    clock_t start, end;
    char stateBefore[20][10];
    char board[20][10];
    char currentBlock, nextBlock;
    char nextBlockT[2][4];
    int posPoints[4][2];
    int currentPositionX, currentPositionY, blockLength;
    int points = 0, pointsBefore = 0;
    startBoard(stateBefore, board);
    currentBlock = '0' + rand()%7;
    nextBlock = '0' + rand()%7;
    drawBlockMenu(nextBlockT,nextBlock);
    updateBoardNext(nextBlockT);
    drawBlock(board,currentBlock,0,3, posPoints, &currentPositionY, &currentPositionX, &blockLength);
    updateBoard(stateBefore, board);
    start = clock();
    for(;;){
        if(kbhit()){
            c = getch();
            switch (c)
            {
            case 'f': rotateBlock(board,posPoints,currentPositionY,currentPositionX,blockLength,currentBlock); break;
            case 'd': moveBlockRight(board,posPoints,&currentPositionX,currentBlock); break;
            case 'a': moveBlockLeft(board,posPoints,&currentPositionX,currentBlock); break;
            case 'g': moveFullDown(board,posPoints,&currentPositionY,currentBlock, &points); break;
            case 's': 
                moveBlockDown(board, posPoints, &currentPositionY, currentBlock, &points);
                if (!isTouchingGround(board,posPoints))
                    start = clock(); 
                break;
            }
            updateBoard(stateBefore, board);
            updatePoints(points, &pointsBefore);
        }
        end = clock();
        timeMeasured =  (float)(end - start) / CLOCKS_PER_SEC;
        if (timeMeasured > fallTime){
                if (!isTouchingGround(board,posPoints)){
                    fallDown(board, posPoints, currentBlock, &currentPositionY);
                    updateBoard(stateBefore, board);
                }
            else{
                int row[4];
                int sideLength;
                isThereRow(board,posPoints,row, &sideLength);
                int numberOfRows = 0;
                for (int i = 0; i < sideLength; i++){
                    if (row[i] != -1){
                        deleteRow(board, row[i]);
                        numberOfRows += 1;
                    }
                }
                switch(numberOfRows){
                    case 1: points += 100; break;
                    case 2: points += 300; break;
                    case 3: points += 600; break;
                    case 4: points += 1000; break;
                }
                currentBlock = nextBlock;
                rand();
                nextBlock = '0' + rand()%7;
                drawBlockMenu(nextBlockT,nextBlock);
                updateBoardNext(nextBlockT);
                if (!drawBlock(board,currentBlock,0,3, posPoints, &currentPositionY, &currentPositionX, &blockLength)){
                    gotoxy2(10,1);
                    printf("YOU LOOSE!");
                    return;
                }
                updateBoard(stateBefore, board);
            }
            start = clock();
            updatePoints(points, &pointsBefore);
        }
    }
}

int main(){
    printf("Press any key to start...");
    getch();
    gameplay();
    getch();
    system("cls");
    return 0; 
}