/* Do not make any changes in the skeleton. Your submission will be invalidated if the skeleton is changed */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

int solved = 1;
int checkGrid[3] = {0};
int backtracks = 0;
struct Element {
    int x;
    int y;
    int possibleNumber;
    int size;
    int grid[36][36];
};
struct cell {
    int value;
    int possibleValues[37];
};

int changes = 0;

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

void * rowSolver(void* _args) {
    struct Element *element = (struct Element *) _args;

    int x = element->x;
    int y = element->y;
    int possibleNumber = element->possibleNumber;
    int size = element->size;
    if(solved == 0) {
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (element->grid[i][y] == possibleNumber) {
            solved = 0;
            return 0;
        }
    }
    checkGrid[0] = 1;
    return 0;

}

void * colSolver(void* _args) {
    struct Element *element = (struct Element *) _args;

    int x = element->x;
    int y = element->y;
    int possibleNumber = element->possibleNumber;
    int size = element->size;
if(solved == 0) {
        return 0;
    }
    for (int i = 0; i < size; i++) {
        if (element->grid[x][i] == possibleNumber) {

            solved = 0;
          return 0;
        }
    }
    checkGrid[1] = 1;
   return 0;
}

void * boxSolver(void* _args) {
    struct Element *element = (struct Element *) _args;
if(solved == 0) {
        return 0;
    }
    int x = element->x;
    int y = element->y;
    int possibleNumber = element->possibleNumber;
    int size = element->size;

    int k = sqrt(size);
    int startx = x - x % k;
    int starty = y - y % k;

    // check if the possibleNumber is present in the box
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (element->grid[i + startx][j + starty] == possibleNumber) {
            solved = 0;
               return 0;
            }
        }
    }
    checkGrid[2] = 1;
    return 0;
}

int backtracker_multithread(int sudoku[36][36], int y, int x, int size) {
    if (y == (size - 1) && x == (size - 1)) {
        // Base case breaks at the end of the sudoku board - (size-1, size-1)
        return 1;
    }

    if (y == (size)) {
        // reset the board back to first position in y direction
        // and move to next row
        x++;
        y = 0;
    }

    if (sudoku[x][y] > 0) {
        // Resembles already filled box, move to next box
        return backtracker_multithread(sudoku, y + 1, x, size);
    }

    for (int possibleNumber = 1; possibleNumber <= size; possibleNumber++) {
        pthread_t rowThread;
        pthread_t colThread;
        pthread_t boxThread;

        struct Element *rowElement = malloc(sizeof(struct Element));
        struct Element *colElement = malloc(sizeof(struct Element));
        struct Element *boxElement = malloc(sizeof(struct Element));

        rowElement->x = x;
        colElement->x = x;
        boxElement->x = x;

        rowElement->y = y;
        colElement->y = y;
        boxElement->y = y;

        rowElement->possibleNumber = possibleNumber;
        colElement->possibleNumber = possibleNumber;
        boxElement->possibleNumber = possibleNumber;

        rowElement->size = size;
        colElement->size = size;
        boxElement->size = size;

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                rowElement->grid[i][j] = sudoku[i][j];
                colElement->grid[i][j] = sudoku[i][j];
                boxElement->grid[i][j] = sudoku[i][j];
            }
        }

        pthread_create(&rowThread, NULL, rowSolver, rowElement);
        pthread_create(&colThread, NULL, colSolver, colElement);
        pthread_create(&boxThread, NULL, boxSolver, boxElement);

        pthread_join(rowThread, NULL);
        pthread_join(colThread, NULL);
        pthread_join(boxThread, NULL);

        int flag = 1;
        for (int k = 0; k < 3; ++k) {
            if (checkGrid[k] == 0) {
                flag = 0;
                break;
            }
        }

        for (int k = 0; k < 3; ++k) {
            checkGrid[k] = 0;
        }



        if (flag) {
            sudoku[x][y] = possibleNumber;

            if (backtracker_multithread(sudoku, y + 1, x, size) == 1) {
                return 1;
            }
        }
        backtracks++;
        sudoku[x][y] = 0;
    }
    return 0;
}

int isPositionValidOpt(struct cell optSudoku[36][36], int x, int y, int possibleNumber, int size) {

    // check if the possibleNumber is present in the column
    for (int i = 0; i < size; i++) {
        if (optSudoku[i][y].value == possibleNumber) {
            return 0;
        }
    }

    // check if the possibleNumber is present in the row
    for (int i = 0; i < size; i++) {
        if (optSudoku[x][i].value == possibleNumber) {
            return 0;
        }
    }

    int k = sqrt(size);
    int startx = x - x % k;
    int starty = y - y % k;

    // check if the possibleNumber is present in the box
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            if (optSudoku[i + startx][j + starty].value == possibleNumber) {
                return 0;
            }
        }
    }

    // return 1 if the possibleNumber is a valid Number
    return 1;
}
void print_gridOpt(int size, struct cell optSudoku[36][36]) {
    int i, j;
    /* The segment below prints the grid in a standard format. Do not change */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++)
            printf("%d\t", optSudoku[i][j].value);
        printf("\n");
    }
}



int countSingleEntriedCells(struct cell optSudoku[36][36], int size) {
    // count if the entries are only single
    int counter  = 0;
    for (int k = 0; k < size; ++k) {
        for (int l = 0; l < size; ++l) {
            if (optSudoku[k][l].value == 0) {
                // each grid item cell
                int count = 0;
                for (int m = 1; m <= size; ++m) {
                    if (optSudoku[k][l].possibleValues[m] != 0) {
                        count++;
                    }
                }
                if (count == 1) {
                    counter++;
                }
            }

        }
    }
    return counter;
}

void find_single_cell(struct cell optSudoku[36][36], int size, int *x, int *y) {
    for (int k = *x; k < size; k++) {
        for (int l = *y; l < size; l++) {
            if (optSudoku[k][l].value == 0) {

                // each grid item cell
                int count = 0;
                for (int m = 1; m <= size; ++m) {
                    if (optSudoku[k][l].possibleValues[m] != 0) {
                        count++;
                    }
                }
                if (count == 1) {
                    *x = k;
                    *y = l;
                    return;
                }
            }
        }
    }
    return;
}


int getEntry(struct cell optSudoku[36][36], int size, int k, int l) {
    int val  = 0;

    if (optSudoku[k][l].value == 0) {
        // each grid item cell
        for (int m = 1; m <= size; ++m) {
            if (optSudoku[k][l].possibleValues[m] != 0) {
                return m;
            }
        }
    }
    return -1;
}



int main(int argc, char *argv[]) {

    // Uncomment the below lines to start the timer
    clock_t t;
    t = clock();

    int grid[36][36], size;

    if (argc != 3) {
        printf("Usage: ./sudoku.out grid_size inputfile");
        exit(-1);
    }
    size = atoi(argv[1]);
    read_grid_from_file(size, argv[2], grid);

    struct cell optSudoku[36][36];

    for (int k = 0; k < size; ++k) {
        for (int l = 0; l < size; ++l) {
            optSudoku[k][l].value = grid[k][l];
            for (int m = 0; m <= 36; ++m)
                optSudoku[k][l].possibleValues[m] = 0;
        }
    }

    for (int k = 0; k < size; ++k) {
        for (int l = 0; l < size; ++l) {
            for (int m = 1; m <= size; ++m) {
                if (isPositionValidOpt(optSudoku, k, l, m, size))
                    optSudoku[k][l].possibleValues[m] = 1;
            }
        }
    }

    while (countSingleEntriedCells(optSudoku, size) != 0) {
        int x = 0 ; int y = 0;
        find_single_cell(optSudoku, size, &x, &y);
        int e = getEntry(optSudoku, size, x , y);
        optSudoku[x][y].value =  e;
        optSudoku[x][y].possibleValues[e] = 0;

        changes++;

        // remove in the whole row
        for (int k = 0; k < size; ++k) {
            optSudoku[k][y].possibleValues[e] = 0;
        }

        // remove in col
        for (int k = 0; k < size; ++k) {
            optSudoku[x][k].possibleValues[e] = 0;
        }

        //remove in box
        int k = sqrt(size);
        int startx = x - x % k;
        int starty = y - y % k;

        // check if the possibleNumber is present in the box
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                optSudoku[i + startx][j + starty].possibleValues[e] = 0;
            }
        }
    }

    for (int k = 0; k < size; ++k) {
        for (int l = 0; l < size; ++l) {
            grid[k][l] = optSudoku[k][l].value;
        }
    }


    /* Do your thing here */
    if (backtracker_multithread(grid, 0, 0, size) == 0) {
        printf("No solution exists\n");
    }
    if (grid[size - 1][size - 1] == 0) {
        int sum = 0;

        for (int i = 0; i < size; ++i) {
            sum += grid[size - 1][i];
        }
        int hh = (size) * (size + 1);
        hh = hh / 2;
        grid[size - 1][size - 1] = hh - sum;

    }

    print_grid(size, grid);

    // Uncomment the below lines to get Debug Logs
    printf("[log]: Total Backtracks : %d\n", backtracks);
    printf("[log]: Total Changes because of optimisation : %d\n", changes);
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("[log]: Code took %f seconds to execute\n", time_taken);

}
