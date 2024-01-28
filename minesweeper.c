#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FLUSH fflush(stdin)

void initializeMinesweeper(void);
void printGameBoard(void);
void makeMove(void);
void handleLoss(void);
void uncoverAdjacentEmptyCells(int row, int col);
void printFinalBoard(void);
void handleWin(void);
void askToPlayAgain(void);
void endGame(void);
void customizeGame(void);

int row, col;
int numRows, numCols;
float difficulty;
int totalMines = 0;
int minesRemaining = 0;

int gameBoard[50][50];
int hiddenBoard[50][50];
int finalBoard[50][50];

int main()
{
    printf("\t\tWelcome to Minesweeper\n");
    customizeGame();
    initializeMinesweeper();
    makeMove();
    return 0;
}

void customizeGame(void)
{
    numRows = 0;
    numCols = 0;
    totalMines = 0;

    printf("\t\tPlease enter the number of rows you want: ");
    scanf("%d", &numRows);
    printf("\t\tPlease enter the number of columns you want: ");
    scanf("%d", &numCols);
    printf("\t\tPlease enter the number of mines you want on the board (be cautious): ");
    scanf("%d", &totalMines);

    minesRemaining = totalMines;
}

void initializeMinesweeper(void)        // generates random mines on the board acc to time
{
    int i, j;

    srand(time(NULL));

    for (i = 0; i < numRows; i++)
    {
        for (j = 0; j < numCols; j++)
        {
            gameBoard[i][j] = '-';
            hiddenBoard[i][j] = gameBoard[i][j];
            finalBoard[i][j] = gameBoard[i][j];
        }
    }

    int mines = 0;
    while (mines < totalMines)
    {
        int randRow = rand() % numRows;
        int randCol = rand() % numCols;

        if (gameBoard[randRow][randCol] != '*')
        {
            gameBoard[randRow][randCol] = '*';
            finalBoard[randRow][randCol] = gameBoard[randRow][randCol];
            mines++;
        }
    }

    for (i = 0; i < numRows; i++)
    {
        for (j = 0; j < numCols; j++)
        {
            if (gameBoard[i][j] != '*')
            {
                gameBoard[i][j] = '0';
            }
        }
    }
}

void printGameBoard(void)
{
    int i, j;

    printf("\n\t\tCurrent Minesweeper Board:\n\n");

    for (i = 0; i < numRows + 1; i++)
    {
        printf("|%d|\t", i);
    }
    printf("\n\n");

    for (j = 1; j < numCols +1; j++)
    {
        printf("|%d|\t", j);
        for (i = 0; i < numRows; i++)
        {
            if (hiddenBoard[i][j] == '*' || hiddenBoard[i][j] == '-')
            {
                // Print a placeholder for mines and unrevealed cells (e.g., '-')
                printf("|%c|\t", '-');
            }
            else
            {
                if (hiddenBoard[i][j] == '#') {
                    // Print adjacent non-mine cells
                    printf("|%c|\t", hiddenBoard[i][j]);
                } else {
                    // Print directly uncovered cells
                    printf("|%c|\t", '-');
                }
            }
        }
        printf("\n");
    }
}


void uncoverAdjacentEmptyCells(int row, int col)
{
    if (row < 0 || row >= numRows || col < 0 || col >= numCols || hiddenBoard[row][col] != '-')
        return;

    hiddenBoard[row][col] = finalBoard[row][col];

    if (finalBoard[row][col] == 0)
    {
        uncoverAdjacentEmptyCells(row - 1, col - 1);
        uncoverAdjacentEmptyCells(row - 1, col);
        uncoverAdjacentEmptyCells(row - 1, col + 1);
        uncoverAdjacentEmptyCells(row, col - 1);
        uncoverAdjacentEmptyCells(row, col + 1);
        uncoverAdjacentEmptyCells(row + 1, col - 1);
        uncoverAdjacentEmptyCells(row + 1, col);
        uncoverAdjacentEmptyCells(row + 1, col + 1);
    }
    else if (finalBoard[row][col] != '*')
    {
        hiddenBoard[row][col] = '#'; // Change the symbol to '#' for cells with no mine

        // Uncover adjacent empty cells recursively
        uncoverAdjacentEmptyCells(row - 1, col - 1);
        uncoverAdjacentEmptyCells(row - 1, col);
        uncoverAdjacentEmptyCells(row - 1, col + 1);
        uncoverAdjacentEmptyCells(row, col - 1);
        uncoverAdjacentEmptyCells(row, col + 1);
        uncoverAdjacentEmptyCells(row + 1, col - 1);
        uncoverAdjacentEmptyCells(row + 1, col);
        uncoverAdjacentEmptyCells(row + 1, col + 1);
    }
}


void makeMove(void)
{
    int i, j, match = 0;

    printGameBoard();

    for (j = 0; j < numCols; j++)
    {
        for (i = 0; i < numRows; i++)
        {
            if (gameBoard[i][j] == hiddenBoard[i][j])
            {
                uncoverAdjacentEmptyCells(row, col);
                match++;
            }
        }
    }

    if (match == ((numRows * numCols) - totalMines))
    {
        handleWin();
    }

    printf("\nEnter the row value, followed by a space, then the column value: ");
    scanf("%d %d", &row, &col);
    FLUSH;

    if ((row >= numRows) || (row < 0) || (col < 0) || (col >= numCols))
    {
        printf("\nPlease enter a valid position within the grid.\n");
        makeMove();
    }

    if (gameBoard[row][col] == '*')
    {
        handleLoss();
    }

    if (hiddenBoard[row][col] != '-')
    {
        printf("\nPlease choose an unrevealed position.\n");
        makeMove();
    }
    else
    {
       uncoverAdjacentEmptyCells(row, col);
                // Change the symbol to '#' for cells with no mine
                hiddenBoard[row][col] = '#';
                if (hiddenBoard == '#'){
                    uncoverAdjacentEmptyCells(row, col);
                }
                
                
        
        printGameBoard();  // print the updated board after each move    
        makeMove();
    }
}

void handleLoss(void)
{
    printFinalBoard();
    printf("\n\t\tYou hit a mine at %d, %d\n\t\tYOU LOSE!!!!", row, col);
    askToPlayAgain();
}

void printFinalBoard(void)
{
    int i, j;

    printf("\n\t\tFinal Minesweeper Board (revealing mine locations):\n\n");

    for (i = 0; i < numRows; i++)
    {
        printf("|%d|\t", i);
    }
    printf("\n\n");

    for (j = 0; j < numCols; j++)
    {
        printf("|%d|\t", j);
        for (i = 0; i < numRows; i++)
        {
            printf("|%c|\t", finalBoard[i][j]);
        }
        printf("\n");
    }
}

void handleWin(void)
{
    printf("\n\n\n\t\t\tCongratulations! You Win!\n\n\n");
    askToPlayAgain();
}

void askToPlayAgain(void)
{
    char option[2];

    printf("\n\t\tWould you like to play again? (Y/N): ");
    scanf("%c", &option[0]);
    FLUSH;

    if ((option[0] == 'Y') || (option[0] == 'y'))
    {
        customizeGame();
        initializeMinesweeper();
        makeMove();
    }
    else if ((option[0] == 'N') || (option[0] == 'n'))
    {
        endGame();
    }
    else
    {
        printf("Please enter either Y or N.\n");
        askToPlayAgain();
    }
}

void endGame(void)
{
    printf("\n\n\t\tGame Over.\n");
    exit(1);
}                                         

// tried a lot of changes but was not able to get the recursive method to work

 
