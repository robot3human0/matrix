#include "matrix.h"

int create_matrix(int row, int col, Matrix_t *res) {
  int res_code = OK;
  if (row > 0 && col > 0) {
    res->rows = row;
    res->columns = col;
    res->matrix = (double **)calloc(row, sizeof(double *));
    for (size_t i = 0; i < row; i++) {
      res->matrix[i] = (double *)calloc(col, sizeof(double));
    }
    if (res->matrix == NULL) {
      res_code = INCORRECT;
      res->rows = 0;
      res->columns = 0;
    }
  } else {
    res_code = INCORRECT;
    res->matrix = NULL;
  }
  return res_code;
}

void remove_matrix(Matrix_t *A) {
  if (A->matrix != NULL) {
    for (size_t i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->rows = 0;
    A->columns = 0;
  }
}

int eq_matrix(Matrix_t *A, Matrix_t *B) {
  int res_code = SUCCESS;
  if (A->matrix != NULL && B->matrix != NULL) {
    if (A->rows == B->rows && A->columns == B->columns) {
      for (size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < A->columns; j++) {
          if (fabs(A->matrix[i][j] - B->matrix[i][j]) >= EPS) {
            res_code = FAILURE;
            break;
          }
        }
      }
    } else {
      res_code = FAILURE;
    }
  } else {
    res_code = FAILURE;
  }
  return res_code;
}

int sum_matrix(Matrix_t *A, Matrix_t *B, Matrix_t *result) {
  int res_code = OK;
  if (A->matrix == NULL || B->matrix == NULL) {
    res_code = INCORRECT;
  } else {
    if (A->rows == B->rows && A->columns == B->columns) {
      if (!create_matrix(A->rows, A->columns, result)) {
        for (size_t i = 0; i < A->rows; i++) {
          for (size_t j = 0; j < A->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
          }
        }
      } else {
        res_code = INCORRECT;
      }
    } else {
      res_code = CALC_ERR;
    }
  }
  return res_code;
}

int sub_matrix(Matrix_t *A, Matrix_t *B, Matrix_t *result) {
  int res_code = OK;
  if (A->matrix == NULL || B->matrix == NULL) {
    res_code = INCORRECT;
  } else {
    if (A->rows == B->rows && A->columns == B->columns) {
      if (!create_matrix(A->rows, A->columns, result)) {
        for (size_t i = 0; i < A->rows; i++) {
          for (size_t j = 0; j < A->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
          }
        }
      } else {
        res_code = INCORRECT;
      }
    } else {
      res_code = CALC_ERR;
    }
  }
  return res_code;
}

int mult_number(Matrix_t *A, double number, Matrix_t *result) {
  int res_code = OK;
  if (A->matrix == NULL) {
    res_code = INCORRECT;
  } else {
    if (!create_matrix(A->rows, A->columns, result)) {
      for (size_t i = 0; i < A->rows; i++) {
        for (size_t j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] * number;
        }
      }
    }
  }
  return res_code;
}

int mult_matrix(Matrix_t *A, Matrix_t *B, Matrix_t *result) {
  int res_code = OK;
  if (A->matrix == NULL || B->matrix == NULL) {
    res_code = INCORRECT;
  } else if (A->columns == B->rows) {
    if (!create_matrix(A->rows, B->columns, result)) {
      for (size_t i = 0; i < result->rows; i++) {
        for (size_t j = 0; j < result->columns; j++) {
          for (size_t f = 0; f < A->columns; f++) {
            result->matrix[i][j] += A->matrix[i][f] * B->matrix[f][j];
          }
        }
      }
    } else {
      res_code = INCORRECT;
    }
  } else {
    res_code = CALC_ERR;
  }
  return res_code;
}

int transpose(Matrix_t *A, Matrix_t *result) {
  int res_code = OK;
  if (A->matrix == NULL) {
    res_code = INCORRECT;
  } else if ((A->rows < 1) || (A->columns < 1)) {
    res_code = INCORRECT;
  } else {
    int len = A->rows * A->columns;
    double buff[len];
    int count = 0;
    if (!create_matrix(A->columns, A->rows, result)) {
      for (size_t i = 0; i < A->columns; i++) {
        for (size_t j = 0; j < A->rows; j++) {
          result->matrix[i][j] = A->matrix[j][i];
          count++;
        }
      }
    } else {
      res_code = INCORRECT;
    }
  }
  return res_code;
}

int calc_complements(Matrix_t *A, Matrix_t *result) {
  int res_code = OK;
  if (A->matrix == NULL) {
    res_code = INCORRECT;
  } else if ((A->rows < 1) || (A->rows != A->columns)) {
    res_code = CALC_ERR;
  } else {
    if (!create_matrix(A->rows, A->rows, result)) {
      if (A->rows == 1) {
        result->matrix[0][0] = A->matrix[0][0];
      } else {
        Matrix_t minor;
        create_matrix(A->rows - 1, A->rows - 1, &minor);
        double buff = 0;
        for (size_t i = 0; i < A->rows; i++) {
          for (size_t j = 0; j < A->rows; j++) {
            minor_matrix(A, &minor, i, j);
            determinant(&minor, &buff);
            if (((i + j) % 2) != 0) buff = -buff;
            result->matrix[i][j] = buff;
          }
        }
        remove_matrix(&minor);
      }
    } else {
      res_code = INCORRECT;
    }
  }
  return res_code;
}

int determinant(Matrix_t *A, double *result) {
  int res_code = OK;
  *result = 0;
  if (A->matrix == NULL) {
    res_code = INCORRECT;
  } else if (A->rows < 1 || A->rows != A->columns) {
    res_code = CALC_ERR;
  } else {
    if (A->rows == 1) {
      *result = A->matrix[0][0];
    } else if (A->rows == 2) {
      *result += A->matrix[0][0] * A->matrix[1][1];
      *result += A->matrix[0][1] * A->matrix[1][0] * -1;
    } else {
      double temp_result = 0;
      Matrix_t minor;
      char minus_flag = 1;
      create_matrix(A->rows - 1, A->rows - 1, &minor);
      for (size_t i = 0; i < A->rows; i++) {
        minor_matrix(A, &minor, 0, i);
        determinant(&minor, &temp_result);
        *result += temp_result * minus_flag * A->matrix[0][i];
        minus_flag = -minus_flag;
      }
      remove_matrix(&minor);
    }
  }
  return res_code;
}

int inverse_matrix(Matrix_t *A, Matrix_t *result) {
  int res_code = OK;
  if (A->matrix == NULL) {
    res_code = INCORRECT;
  } else if (A->rows < 1 || A->rows != A->columns) {
    res_code = CALC_ERR;
  } else if (A->rows == 1) {
    create_matrix(1, 1, result);
    result->matrix[0][0] = 1 / A->matrix[0][0];
  } else {
    double determinant_value = 0;
    determinant(A, &determinant_value);
    if (determinant_value != 0.0) {
      Matrix_t temp1;
      calc_complements(A, &temp1);
      Matrix_t temp2;
      transpose(&temp1, &temp2);
      res_code = mult_number(&temp2, 1 / determinant_value, result);
      remove_matrix(&temp1);
      remove_matrix(&temp2);
    } else {
      res_code = CALC_ERR;
    }
  }
  return res_code;
}

////  --===ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ===--  ////

void minor_matrix(Matrix_t *parent, Matrix_t *minor, size_t using_row,
                  size_t using_col) {
  size_t i, j, k, l;
  for (i = 0, k = 0; i < parent->rows, k < minor->rows; i++) {
    if (using_row == i) continue;
    for (j = 0, l = 0; j < parent->rows, l < minor->rows; j++) {
      if (using_col == j) continue;
      minor->matrix[k][l] = parent->matrix[i][j];
      l++;
    }
    k++;
  }
}

void print_matrix(Matrix_t *A) {
  for (size_t i = 0; i < A->rows; i++) {
    for (size_t j = 0; j < A->columns; j++) {
      printf("%.2f ", A->matrix[i][j]);
    }
    printf("\n");
  }
}
