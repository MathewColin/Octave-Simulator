// Copyright 2021-2022 Ceausu Matei Calin - grupa 311CA
// Header for my_octave.c

#ifndef HEADER_H_
#define HEADER_H_

void swap_integers(int *a, int *b);

int **matrix_allocation(int n, int m);

void print_matrix(struct matrix *v, int nr_matrix, int *ok);

void mem_error(void);

void multiply(int matrix1, int matrix2, int nr_matrix, struct matrix v[]);

void sort_sum(int nr_matrix, struct matrix v[], int *ok);

void print_dimensions(int nr_matrix, struct matrix v[], int *ok);

void resize_matrix(int nr_matrix, struct matrix v[]);

void transpose(int nr_matrix, struct matrix v[], int *ok);

void freematrix(int **v, int n);

void free_and_move(int *nr_matrix, struct matrix v[], int *capacity);

void quit_and_free(int nr_matrix, struct matrix v[], int *ok);

int **add(int **m1, int **m2, int n);

int **subtract(int **m1, int **m2, int n);

void copy_matrix(int **m1, int **m2, int n);

int **strassen_multiply(int **A, int **B, int n);

void operation_S(int *nr_matrix, struct matrix v[], int *capacity);

#endif
