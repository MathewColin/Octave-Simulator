// Copyright 2021-2022 Ceausu Matei Calin - grupa 311CA
#include <stdio.h>
#include <stdlib.h>

#define MODULO 10007
#define DIM 50

// Definition of struct:
struct matrix {
	int line, column;
	int **values;
};

#include "my_octave.h"

int main(void)
{
	char operation = ' ';
	int nr_matrix = 0, capacity = DIM;
	struct matrix *v = (struct matrix *)calloc(DIM, sizeof(struct matrix));
	if (!v)
		mem_error();
	while (operation != 'Q') {
		operation = getchar();
		int ok = 1;
		if (operation == 'L') { // adaugarea in memorie a unei matrici
			ok = 0;
			scanf("%d%d", &v[nr_matrix].line, &v[nr_matrix].column);
			int l, c;
			l = v[nr_matrix].line;
			c = v[nr_matrix].column;
			v[nr_matrix].values = matrix_allocation(l, c);
			for (int i = 0; i < l; i++) // citirea matricei
				for (int j = 0; j < c; j++)
					scanf("%d", &v[nr_matrix].values[i][j]);
			nr_matrix++; // cresterea numarului de matrici
			if (nr_matrix % DIM == 0) {
				int size = sizeof(struct matrix) * (nr_matrix + DIM);
				v = (struct matrix *)realloc(v, size);
				capacity += DIM;
			}
		}
		if (operation == 'P') // afisarea unei matrici
			print_matrix(v, nr_matrix, &ok);
		if (operation == 'Q') // eliberarea tuturor resurselor
			quit_and_free(nr_matrix, v, &ok);
		if (operation == 'D') // afisarea dimensiunilor unei matrici
			print_dimensions(nr_matrix, v, &ok);
		if (operation == 'C') { // redimensionarea unei matrici
			ok = 0;
			resize_matrix(nr_matrix, v);
		}
		if (operation == 'M') { // inmultirea a doua matrici
			ok = 0;
			int matrix1, matrix2;
			scanf("%d%d", &matrix1, &matrix2);
			int m1 = matrix1, m2 = matrix2;
			if (m1 > nr_matrix || m2 > nr_matrix || m1 < 0 || m2 < 0) {
				printf("No matrix with the given index\n");
			} else {
				if (v[matrix1].column == v[matrix2].line) {
					multiply(matrix1, matrix2, nr_matrix, v);
					nr_matrix++;
					if (nr_matrix % DIM == 0) {
						int nr = nr_matrix + DIM;
						int size = sizeof(struct matrix) * nr;
						v = (struct matrix *)realloc(v, size);
						capacity += DIM;
					}
				} else {
					printf("Cannot perform matrix multiplication\n");
				}
			}
		}
		if (operation == 'O') // sortarea in functie de suma elementelor
			sort_sum(nr_matrix, v, &ok);
		if (operation == 'T') // transpunerea unei matrici
			transpose(nr_matrix, v, &ok);
		if (operation == 'F') { // eliberarea unei matrici
			ok = 0;
			free_and_move(&nr_matrix, v, &capacity);
		}
		if (operation == 'S') { // algoritmul lui Strassen
			ok = 0;
			operation_S(&nr_matrix, v, &capacity);
		}
		if (ok == 1) // caz particular: comanda nerecunoscuta
			printf("Unrecognized command\n");
		getchar(); // citirea caracterului '\n'
	}
	return 0;
}

void swap_integers(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

int **matrix_allocation(int n, int m)
{
	int **a = (int **)calloc(n, sizeof(int *));
	if (!a) {
		fprintf(stderr, "Out of memory\n");
		exit(0);
	}
	for (int i = 0; i < n; i++) {
		a[i] = (int *)calloc(m, sizeof(int));
		if (!a[i]) {
			fprintf(stderr, "Out of memory\n");
			exit(0);
		}
	}
	return a;
}

void print_matrix(struct matrix *v, int nr_matrix, int *ok)
{
	*ok = 0;
	int matrix_index, i, j;
	scanf("%d", &matrix_index);
	// cazul in care matricea nu se afla in memorie
	if (matrix_index >= nr_matrix || matrix_index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	for (i = 0; i < v[matrix_index].line; i++) {
		for (j = 0; j < v[matrix_index].column; j++)
			printf("%d ", v[matrix_index].values[i][j]);
		printf("\n");
	}
}

void mem_error(void)
{
	fprintf(stderr, "Out of memory\n");
	exit(0);
}

void multiply(int matrix1, int matrix2, int nr_matrix, struct matrix v[])
{
	int cl = v[matrix1].line;
	int cc = v[matrix2].column;
	int c_aux = v[matrix1].column;
	// alocarea matricei produs
	int **product = matrix_allocation(cl, cc);
	for (int i = 0; i < cl; i++) {
		for (int j = 0; j < cc; j++) {
			int s = 0;
			for (int k = 0; k < c_aux; k++) {
				int a = v[matrix1].values[i][k];
				int b = v[matrix2].values[k][j];
				s += a * b;
				s = s % MODULO;
				if (s < 0)
					s += MODULO;
			}
			product[i][j] = s;
		}
	}
	// adaugarea matricei in memorie
	v[nr_matrix].values = product;
	v[nr_matrix].line = cl;
	v[nr_matrix].column = cc;
}

void sort_sum(int nr_matrix, struct matrix v[], int *ok)
{
	*ok = 0;
	// alocarea vectorului s
	int *s = (int *)calloc(nr_matrix, sizeof(int));
	if (!s)
		mem_error();
	// determinarea sumei elementelor
	for (int z = 0; z < nr_matrix; z++)
		for (int i = 0; i < v[z].line; i++)
			for (int j = 0; j < v[z].column; j++) {
				s[z] += v[z].values[i][j];
				s[z] = s[z] % MODULO;
				if (s[z] < 0)
					s[z] += MODULO;
			}
	// sortarea
	for (int i = 0; i < nr_matrix - 1; i++)
		for (int j = i + 1; j < nr_matrix; j++)
			if (s[i] > s[j]) {
				swap_integers(&s[i], &s[j]);
				int **c;
				c = v[i].values;
				v[i].values = v[j].values;
				v[j].values = c;
				swap_integers(&v[i].line, &v[j].line);
				swap_integers(&v[i].column, &v[j].column);
			}
	//eliberarea resurselor
	free(s);
}

void print_dimensions(int nr_matrix, struct matrix v[], int *ok)
{
	*ok = 0;
	int matrix_index;
	scanf("%d", &matrix_index);
	if (matrix_index < nr_matrix && matrix_index >= 0) {
		int l = v[matrix_index].line;
		int c = v[matrix_index].column;
		printf("%d %d\n", l, c);
	} else { // cazul in care matricea nu exista in memorie
		printf("No matrix with the given index\n");
	}
}

void resize_matrix(int nr_matrix, struct matrix v[])
{
	int matrix_index, nr_l, nr_c;
	scanf("%d", &matrix_index);
	scanf("%d", &nr_l);
	// alocarea vectorului de linii
	int *lines = (int *)calloc(nr_l, sizeof(int));
	if (!lines)
		mem_error();
	for (int i = 0; i < nr_l; i++)
		scanf("%d", &lines[i]);
	scanf("%d", &nr_c);
	// alocarea vectorului de coloane
	int *columns = (int *)calloc(nr_c, sizeof(int));
	if (!columns)
		mem_error();
	for (int i = 0; i < nr_c; i++)
		scanf("%d", &columns[i]);
	if (matrix_index < nr_matrix && matrix_index >= 0) {
		int **new_matrix = matrix_allocation(nr_l, nr_c);
		for (int i = 0; i < nr_l; i++) {
			for (int j = 0; j < nr_c; j++) {
				int l = lines[i];
				int c = columns[j];
				new_matrix[i][j] = v[matrix_index].values[l][c];
			}
		}
		for (int i = 0; i < v[matrix_index].line; i++)
			free(v[matrix_index].values[i]);
		free(v[matrix_index].values);
		v[matrix_index].values = new_matrix;
		v[matrix_index].line = nr_l;
		v[matrix_index].column = nr_c;
	} else { // cazul in care nu exista matricea in memorie
		printf("No matrix with the given index\n");
	}
	free(lines); // eliberarea vectorului de linii
	free(columns); // eliberarea vectorului de coloane
}

void transpose(int nr_matrix, struct matrix v[], int *ok)
{
	*ok = 0;
	int matrix_index;
	scanf("%d", &matrix_index);
	if (matrix_index < nr_matrix && matrix_index >= 0) {
		int t_line = v[matrix_index].column;
		int t_column = v[matrix_index].line;
		int **transpose = matrix_allocation(t_line, t_column);
		for (int i = 0; i < t_line; i++)
			for (int j = 0; j < t_column; j++)
				transpose[i][j] = v[matrix_index].values[j][i];
		for (int i = 0; i < t_column; i++)
			free(v[matrix_index].values[i]);
		free(v[matrix_index].values);
		v[matrix_index].values = transpose;
		v[matrix_index].line = t_line;
		v[matrix_index].column = t_column;
	} else { // cazul in care nu exista
		printf("No matrix with the given index\n");
	}
}

void freematrix(int **v, int n)
{
	for (int i = 0; i < n; i++)
		free(v[i]);
	free(v);
}

void free_and_move(int *nr_matrix, struct matrix v[], int *capacity)
{
	int matrix_index;
	scanf("%d", &matrix_index);
	if (matrix_index < *nr_matrix && matrix_index >= 0) {
		for (int i = 0; i < v[matrix_index].line; i++)
			free(v[matrix_index].values[i]);
		free(v[matrix_index].values);
		// stergerea matricei din memorie:
		for (int i = matrix_index; i < *nr_matrix - 1; i++) {
			v[i].values = v[i + 1].values;
			v[i].line = v[i + 1].line;
			v[i].column = v[i + 1].column;
		}
		*nr_matrix = *nr_matrix - 1;
	} else { // cazul in care matricea nu exista
		printf("No matrix with the given index\n");
	}
	if (*capacity - *nr_matrix > DIM) {
		int size = sizeof(struct matrix) * (*nr_matrix - DIM);
		v = (struct matrix *)realloc(v, size);
		*capacity = *capacity - DIM;
	}
}

void quit_and_free(int nr_matrix, struct matrix v[], int *ok)
{
	*ok = 0;
	// eliberarea resurselor unei matrici
	for (int i = 0; i < nr_matrix; i++) {
		for (int j = 0; j < v[i].line; j++)
			free(v[i].values[j]);
		free(v[i].values);
	}
	free(v);
}

int **add(int **m1, int **m2, int n)
{
	int **aux = matrix_allocation(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			aux[i][j] = m1[i][j] + m2[i][j];
	return aux;
}

int **subtract(int **m1, int **m2, int n)
{
	int **aux = matrix_allocation(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			aux[i][j] = m1[i][j] - m2[i][j];
	return aux;
}

void copy_matrix(int **m1, int **m2, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			m1[i][j] = m2[i][j];
}

int **strassen_multiply(int **A, int **B, int n)
{
	if (n == 1) {
		int **C = matrix_allocation(1, 1);
		C[0][0] = A[0][0] * B[0][0];
		return C;
	}
	int **C = matrix_allocation(n, n), k = n / 2;
	int **A11 = matrix_allocation(k, k), **A12 = matrix_allocation(k, k);
	int **A21 = matrix_allocation(k, k), **A22 = matrix_allocation(k, k);
	int **B11 = matrix_allocation(k, k), **B12 = matrix_allocation(k, k);
	int **B21 = matrix_allocation(k, k), **B22 = matrix_allocation(k, k);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			A11[i][j] = A[i][j], A12[i][j] = A[i][k + j];
			A21[i][j] = A[k + i][j], A22[i][j] = A[k + i][k + j];
			B11[i][j] = B[i][j], B12[i][j] = B[i][k + j];
			B21[i][j] = B[k + i][j], B22[i][j] = B[k + i][k + j];
		}
	int **aux = subtract(B12, B22, k), **P1 = strassen_multiply(A11, aux, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	aux = add(A11, A12, k);
	int **P2 = strassen_multiply(aux, B22, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	aux = add(A21, A22, k);
	int **P3 = strassen_multiply(aux, B11, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	aux = subtract(B21, B11, k);
	int **P4 = strassen_multiply(A22, aux, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	aux = add(A11, A22, k);
	int **aux1 = add(B11, B22, k), **P5 = strassen_multiply(aux, aux1, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	freematrix(aux1, k); // eliberarea matricei aux1 pentru a fi refolosita
	aux = subtract(A12, A22, k), aux1 = add(B21, B22, k);
	int **P6 = strassen_multiply(aux, aux1, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	freematrix(aux1, k); // eliberarea matricei aux1 pentru a fi refolosita
	aux = subtract(A11, A21, k), aux1 = add(B11, B12, k);
	int **P7 = strassen_multiply(aux, aux1, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	freematrix(aux1, k); // eliberarea matricei aux1 pentru a fi refolosita
	aux = add(P5, P4, k), aux1 = add(aux, P6, k);
	int **aux2 = subtract(aux1, P2, k), **C11 = matrix_allocation(k, k);
	copy_matrix(C11, aux2, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	freematrix(aux1, k); // eliberarea matricei aux1 pentru a fi refolosita
	freematrix(aux2, k); // eliberarea matricei aux2 pentru a fi refolosita
	aux = add(P1, P2, k);
	int **C12 = matrix_allocation(k, k);
	copy_matrix(C12, aux, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	aux = add(P3, P4, k);
	int **C21 = matrix_allocation(k, k);
	copy_matrix(C21, aux, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	aux = add(P5, P1, k), aux1 = subtract(aux, P3, k);
	aux2 = subtract(aux1, P7, k);
	int **C22 = matrix_allocation(k, k);
	copy_matrix(C22, aux2, k);
	freematrix(aux, k); // eliberarea matricei aux pentru a fi refolosita
	freematrix(aux1, k); // eliberarea matricei aux1 pentru a fi refolosita
	freematrix(aux2, k); // eliberarea matricei aux2 pentru a fi refolosita
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++) {
			C[i][j] = C11[i][j], C[i][k + j] = C12[i][j];
			C[k + i][j] = C21[i][j], C[k + i][k + j] = C22[i][j];
		}
	for (int i = 0; i < k; i++) {
		free(A11[i]), free(A12[i]), free(A21[i]), free(A22[i]), free(B11[i]);
		free(B12[i]), free(B21[i]), free(B22[i]), free(P1[i]), free(P2[i]);
		free(P3[i]), free(P4[i]), free(P5[i]), free(P6[i]), free(P7[i]);
		free(C11[i]), free(C12[i]), free(C21[i]), free(C22[i]);
	}
	free(A11), free(A12), free(A21), free(A22), free(B11), free(B12);
	free(B21), free(B22), free(P1), free(P2), free(P3), free(P4);
	free(P5), free(P6), free(P7), free(C11), free(C12), free(C21), free(C22);
	return C;
}

void operation_S(int *nr_matrix, struct matrix v[], int *capacity)
{
	int m1, m2;
	scanf("%d%d", &m1, &m2);
	// cazul in care nu exista matricile in memorie
	if (m1 > *nr_matrix || m2 > *nr_matrix || m1 < 0 || m2 < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	// cazul in care inmultirea nu este posibila
	if (v[m1].line != v[m2].line && v[m1].column != v[m2].column) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	int l = v[m1].line;
	int **product;
	product = strassen_multiply(v[m1].values, v[m2].values, l);
	for (int i = 0; i < l; i++)
		for (int j = 0; j < l; j++) {
			product[i][j] %= MODULO;
			if (product[i][j] < 0)
				product[i][j] += MODULO;
		}
	v[*nr_matrix].values = product;
	v[*nr_matrix].line = l;
	v[*nr_matrix].column = l;
	*nr_matrix = *nr_matrix + 1;
	if (*nr_matrix % DIM == 0) {
		int size = sizeof(struct matrix) * (*nr_matrix + DIM);
		v = (struct matrix *)realloc(v, size);
		*capacity = *capacity + DIM;
	}
}
