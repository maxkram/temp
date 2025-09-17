#include <stdio.h>
#include <stdlib.h>

int **alloc_matrix(int rows, int cols);
void free_matrix(int **matrix, int rows);
int input_matrix(int **matrix, int rows, int cols);
void output_matrix(int **matrix, int rows, int cols);

int sum_matrices(int **a, int n1, int m1, int **b, int n2, int m2, int ***res,
                 int *nr, int *mr);
int mul_matrices(int **a, int n1, int m1, int **b, int n2, int m2, int ***res,
                 int *nr, int *mr);
int transpose_matrix(int **a, int n, int m, int ***res, int *nr, int *mr);

int read_one_matrix(int ***mat, int *rows, int *cols, int exp_n, int exp_m);
int compute_element(int *row_a, int **b, int m1, int col_b);

int main(void) {
  int op_code = 0, n1 = 0, m1 = 0, n2 = 0, m2 = 0;
  int **mat1 = NULL, **mat2 = NULL, **result = NULL;
  int rn = 0, rm = 0;
  int err = 0;

  if (scanf("%d", &op_code) != 1 || op_code < 1 || op_code > 3) {
    err = 1;
  } else {
    if (op_code == 1 || op_code == 2) {
      if (!read_one_matrix(&mat1, &n1, &m1, 0, 0))
        err = 1;
      else if (op_code == 1 && !read_one_matrix(&mat2, &n2, &m2, n1, m1))
        err = 1;
      else if (op_code == 2 && !read_one_matrix(&mat2, &n2, &m2, 0, 0))
        err = 1;
    } else {
      if (!read_one_matrix(&mat1, &n1, &m1, 0, 0))
        err = 1;
    }
  }

  if (!err) {
    if (op_code == 1)
      err = sum_matrices(mat1, n1, m1, mat2, n2, m2, &result, &rn, &rm);
    else if (op_code == 2)
      err = mul_matrices(mat1, n1, m1, mat2, n2, m2, &result, &rn, &rm);
    else
      err = transpose_matrix(mat1, n1, m1, &result, &rn, &rm);
  }

  if (err)
    printf("n/a");
  else
    output_matrix(result, rn, rm);

  free_matrix(mat1, n1);
  free_matrix(mat2, n2);
  free_matrix(result, rn);

  return 0;
}

int read_one_matrix(int ***mat, int *rows, int *cols, int exp_n, int exp_m) {
  int n = 0, m = 0;
  int success = 1;

  if (scanf("%d %d", &n, &m) != 2 || n <= 0 || m <= 0)
    success = 0;
  else if (exp_n > 0 && (n != exp_n || m != exp_m))
    success = 0;
  else {
    *mat = alloc_matrix(n, m);
    if (*mat == NULL)
      success = 0;
    else {
      if (!input_matrix(*mat, n, m)) {
        free_matrix(*mat, n);
        *mat = NULL;
        success = 0;
      } else {
        *rows = n;
        *cols = m;
      }
    }
  }
  return success;
}

int **alloc_matrix(int rows, int cols) {
  int **matrix = malloc(rows * sizeof(int *));
  if (!matrix)
    return NULL;

  int success = 1;
  int i;
  for (i = 0; i < rows && success; i++) {
    matrix[i] = malloc(cols * sizeof(int));
    if (!matrix[i])
      success = 0;
  }

  if (!success) {
    for (int j = 0; j < i; j++) {
      free(matrix[j]);
    }
    free(matrix);
    matrix = NULL;
  }

  return matrix;
}

void free_matrix(int **matrix, int rows) {
  if (matrix != NULL) {
    for (int i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

int input_matrix(int **matrix, int rows, int cols) {
  int success = 1;

  for (int i = 0; i < rows && success; i++) {
    for (int j = 0; j < cols && success; j++) {
      if (scanf("%d", &matrix[i][j]) != 1)
        success = 0;
    }
  }

  return success;
}

void output_matrix(int **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    printf("%d", matrix[i][0]);
    for (int j = 1; j < cols; j++) {
      printf(" %d", matrix[i][j]);
    }
    if (i != rows - 1)
      printf("\n");
  }
}

int sum_matrices(int **a, int n1, int m1, int **b, int n2, int m2, int ***res,
                 int *nr, int *mr) {
  int retval = 0;

  if (n1 != n2 || m1 != m2)
    retval = 1;

  if (retval == 0) {
    *nr = n1;
    *mr = m1;
    *res = alloc_matrix(*nr, *mr);
    if (*res == NULL)
      retval = 1;
  }

  if (retval == 0) {
    for (int i = 0; i < *nr; i++) {
      for (int j = 0; j < *mr; j++) {
        (*res)[i][j] = a[i][j] + b[i][j];
      }
    }
  }
  return retval;
}

int compute_element(int *row_a, int **b, int m1, int col_b) {
  int sum = 0;
  for (int k = 0; k < m1; k++) {
    sum += row_a[k] * b[k][col_b];
  }
  return sum;
}

int mul_matrices(int **a, int n1, int m1, int **b, int n2, int m2, int ***res,
                 int *nr, int *mr) {
  int retval = 0;
  if (m1 != n2)
    retval = 1;

  if (retval == 0) {
    *nr = n1;
    *mr = m2;
    *res = alloc_matrix(*nr, *mr);
    if (*res == NULL)
      retval = 1;
  }

  if (retval == 0) {
    for (int i = 0; i < *nr; i++) {
      for (int j = 0; j < *mr; j++) {
        (*res)[i][j] = compute_element(a[i], b, m1, j);
      }
    }
  }
  return retval;
}

int transpose_matrix(int **a, int n, int m, int ***res, int *nr, int *mr) {
  int retval = 0;

  *nr = m;
  *mr = n;
  *res = alloc_matrix(*nr, *mr);
  if (*res == NULL)
    retval = 1;
  else {
    for (int i = 0; i < *nr; i++) {
      for (int j = 0; j < *mr; j++) {
        (*res)[i][j] = a[j][i];
      }
    }
  }
  return retval;
}