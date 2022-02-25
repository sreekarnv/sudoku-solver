#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct args {
	int grid[36][36];
	int size;
};

int solved = 0;

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

int find_empty_cell(int *row, int *col, int grid[36][36], int size) {
	int i, j;
	for (i = *row; i < size; i++) {
		for (j = *col; j < size; j++) {
			if (grid[i][j] == 0) {
				*col = j;
				*row = i;
				return 1;
			}
		}
	}
	return 0;
}

int is_valid(int n, int r, int c, int grid[36][36], int size) {
	int i, j;
	int box_size = sqrt(size);
	int valid_num[size];
	for (i = 0; i < size; i++) {
		if (grid[r][i] == n) return 0;
	}
	for (i = 0; i < size; i++) {
		if (grid[i][c] == n) return 0;
	}
	int row_start = (r / box_size) * box_size;
	int col_start = (c / box_size) * box_size;
	for (i = row_start; i < row_start + box_size; i++) {
		for (j = col_start; j < col_start + box_size; j++) {
			if (grid[i][j] == n) return 0;
		}
	}
	return 1;
}

void *solve_sudoku(void *args) {
	struct args *a;
	a = (struct args *)args;

	int row = 0;
	int col = 0;

	if (find_empty_cell(&row, &col, a->grid, a->size) == 0) {
		solved = 1;
		print_grid(a->size, a->grid);
		return 0;
	}

	for (int i = 0; i <= a->size; i++) {
		if (is_valid(i, row, col, a->grid, a->size)) {
			a->grid[row][col] = i;

			struct args new_a;
			memcpy(new_a.grid, a->grid, sizeof(a->grid));
			new_a.size = a->size;

			solve_sudoku(&new_a);
			if (solved) {
				return 0;
			}
			a->grid[row][col] = 0;
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {
	int grid[36][36], size;

	if (argc != 3) {
		printf("Usage: ./sudoku.out grid_size inputfile");
		exit(-1);
	}

	size = atoi(argv[1]);
	read_grid_from_file(size, argv[2], grid);

	/* Do your thing here */
	int row = 0;
	int col = 0;

	find_empty_cell(&row, &col, grid, size);

	int k;
	int valid[size];
	int index = 0;

	for (k = 1; k <= size; k++) {
		if (is_valid(k, row, col, grid, size)) {
			valid[index] = k;
			index++;
		}
	}

	pthread_t threads[index];
	pthread_t t;

	struct args args_array[index];

	for (int i = 0; i < index; i++) {
		grid[row][col] = valid[i];
		struct args new_a;
		memcpy(new_a.grid, grid, sizeof(grid));
		new_a.size = size;

		args_array[i] = new_a;
	}

	for (int th = 0; th < index; th++) {
		int result = pthread_create(&threads[th], NULL, solve_sudoku, &args_array[th]);
		assert(!result);
	}

	for (int th = 0; th < index; th++) {
		int result = pthread_join(threads[th], NULL);
		assert(!result);
	}

	if (solved == 0) printf("No solution");
	return 0;
}
