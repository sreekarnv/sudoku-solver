/* Do not make any changes in the skeleton. Your submission will be invalidated if the skeleton is changed */


# include <stdio.h>
# include <stdlib.h>

void read_grid_from_file(int size, char *ip_file, int grid[36][36]) {
    FILE *fp;
    int i, j;
    fp = fopen(ip_file, "r");
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++) {
            fscanf(fp, "%d", &grid[i][j]);
        }
}

void print_grid(int size, int grid[36][36]) {
    int i, j;
    /* The segment below prints the grid in a standard format. Do not change */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++)
            printf("%d\t", grid[i][j]);
        printf("\n");
    }
}


int isPositionValid(int grid[36][36], int y, int x, int n)
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

int bruteForceSudokuRecurse(int sudoku[36][36], int y, int x)
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

int main(int argc, char *argv[]) {
    int grid[36][36], size, i, j;

    if (argc != 3) {
        printf("Usage: ./sudoku.out grid_size inputfile");
        exit(-1);
    }

    size = atoi(argv[1]);
    read_grid_from_file(size, argv[2], grid);

    /* Do your thing here */
    if (bruteForceSudokuRecurse(grid, 0, 0) == 0)
    {
        printf("No solution exists");
    }


    print_grid(size, grid);
}
