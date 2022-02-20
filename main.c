#include <stdio.h>

void displaySudokuGrid(int arr[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0)
            {
                printf("|");
            }

            printf("%d", arr[i][j]);
        }

        printf("\n");

        if (i != 0 && (i + 1) % 3 == 0)
        {
            printf("---------------------------------\n");
        }
    }
}

int isPositionValid(int grid[9][9], int y, int x, int n)
{

    for (int i = 0; i <= 8; i++)
    {
        if (grid[i][y] == n)
        {
            return 0;
        }
    }

    for (int i = 0; i <= 8; i++)
    {
        if (grid[x][i] == n)
        {
            return 0;
        }
    }

    int startx = x - x % 3;
    int starty = y - y % 3;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[i + startx][j + starty] == n)
            {
                return 0;
            }
        }
    }

    return 1;
}

int bruteForceSudokuRecurse(int sudoku[9][9], int y, int x)
{
    if (y == 9 && x == 8)
    {
        return 1;
    }

    if (y == 9)
    {
        x++;
        y = 0;
    }

    if (sudoku[x][y] > 0)
    {
        return bruteForceSudokuRecurse(sudoku, y + 1, x);
    }

    for (int num = 1; num <= 9; num++)
    {

        if (isPositionValid(sudoku, y, x, num) == 1)
        {
            sudoku[x][y] = num;

            if (bruteForceSudokuRecurse(sudoku, y + 1, x) == 1)
            {
                return 1;
            }
        }

        sudoku[x][y] = 0;
    }
    return 0;
}

int main()
{
    int sudoku[9][9] = {
        {6, 0, 0, 8, 7, 0, 0, 0, 0},
        {0, 0, 9, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 8, 0, 0},
        {7, 9, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 8, 0, 0, 0, 0},
        {0, 0, 0, 4, 1, 0, 0, 0, 0},
        {0, 7, 0, 0, 5, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 7},
    };

    if (bruteForceSudokuRecurse(sudoku, 0, 0) == 0)
    {
        printf("No solution exists");
    }

    displaySudokuGrid(sudoku);

    return 0;
}
