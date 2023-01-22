#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0
#define EPS 0.0000001
#define OK 0
#define INCORRECT 1
#define CALC_ERR 2

typedef struct {
  double **matrix;
  int rows;
  int columns;
} Matrix_t;

int create_matrix(int rows, int columns, Matrix_t *result);
void remove_matrix(Matrix_t *A);
int eq_matrix(Matrix_t *A, Matrix_t *B);
int sum_matrix(Matrix_t *A, Matrix_t *B, Matrix_t *result);
int sub_matrix(Matrix_t *A, Matrix_t *B, Matrix_t *result);
int mult_number(Matrix_t *A, double number, Matrix_t *result);
int mult_matrix(Matrix_t *A, Matrix_t *B, Matrix_t *result);
int transpose(Matrix_t *A, Matrix_t *result);
int calc_complements(Matrix_t *A, Matrix_t *result);
int determinant(Matrix_t *A, double *result);
int inverse_matrix(Matrix_t *A, Matrix_t *result);
void print_matrix(Matrix_t *A);
void minor_matrix(Matrix_t *parent, Matrix_t *minor, size_t using_row,
                  size_t using_col);

#endif  //  SRC_MATRIX_H_