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

int main(void) {
  int op_code = 0;
  if (scanf("%d", &op_code) != 1 || op_code < 1 || op_code > 3) {
    printf("n/a");
    return 0;
  }

  int n1 = 0, m1 = 0, n2 = 0, m2 = 0; // Initialized to zero
  int **mat1 = NULL, **mat2 = NULL;
  int **result = NULL;
  int rn = 0, rm = 0;
  int err = 0;

  if (op_code == 1 || op_code == 2) {
    if (scanf("%d %d", &n1, &m1) != 2 || n1 <= 0 || m1 <= 0) {
      err = 1;
    }
    if (!err) {
      mat1 = alloc_matrix(n1, m1);
      if (!mat1 || !input_matrix(mat1, n1, m1))
        err = 1;
    }
    if (op_code == 1 && !err) {
      if (scanf("%d %d", &n2, &m2) != 2 || n2 != n1 || m2 != m1)
        err = 1;
      if (!err) {
        mat2 = alloc_matrix(n2, m2);
        if (!mat2 || !input_matrix(mat2, n2, m2))
          err = 1;
      }
    } else if (op_code == 2 && !err) {
      if (scanf("%d %d", &n2, &m2) != 2 || n2 <= 0 || m2 <= 0)
        err = 1;
      if (!err) {
        mat2 = alloc_matrix(n2, m2);
        if (!mat2 || !input_matrix(mat2, n2, m2))
          err = 1;
      }
    }
  } else {
    if (scanf("%d %d", &n1, &m1) != 2 || n1 <= 0 || m1 <= 0)
      err = 1;
    if (!err) {
      mat1 = alloc_matrix(n1, m1);
      if (!mat1 || !input_matrix(mat1, n1, m1))
        err = 1;
    }
  }

  if (!err) {
    if (op_code == 1) {
      err = sum_matrices(mat1, n1, m1, mat2, n2, m2, &result, &rn, &rm);
    } else if (op_code == 2) {
      err = mul_matrices(mat1, n1, m1, mat2, n2, m2, &result, &rn, &rm);
    } else {
      err = transpose_matrix(mat1, n1, m1, &result, &rn, &rm);
    }
  }

  if (err) {
    printf("n/a");
  } else {
    output_matrix(result, rn, rm);
  }

  free_matrix(mat1, n1);
  free_matrix(mat2, n2);
  free_matrix(result, rn);

  return 0;
}

int **alloc_matrix(int rows, int cols) {
  int **matrix = malloc(rows * sizeof(int *));
  if (!matrix)
    return NULL;
  for (int i = 0; i < rows; i++) {
    matrix[i] = malloc(cols * sizeof(int));
    if (!matrix[i]) {
      for (int j = 0; j < i; j++)
        free(matrix[j]);
      free(matrix);
      return NULL;
    }
  }
  return matrix;
}

void free_matrix(int **matrix, int rows) {
  if (!matrix)
    return;
  for (int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

int input_matrix(int **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      if (scanf("%d", &matrix[i][j]) != 1)
        return 0;
  return 1;
}

void output_matrix(int **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    printf("%d", matrix[i][0]);
    for (int j = 1; j < cols; j++)
      printf(" %d", matrix[i][j]);
    if (i != rows - 1)
      printf("\n");
  }
}

int sum_matrices(int **a, int n1, int m1, int **b, int n2, int m2, int ***res,
                 int *nr, int *mr) {
  if (n1 != n2 || m1 != m2)
    return 1;
  *nr = n1;
  *mr = m1;
  *res = alloc_matrix(*nr, *mr);
  if (!*res)
    return 1;

  for (int i = 0; i < *nr; i++) {
    for (int j = 0; j < *mr; j++) {
      (*res)[i][j] = a[i][j] + b[i][j];
    }
  }
  return 0;
}

int mul_matrices(int **a, int n1, int m1, int **b, int n2, int m2, int ***res,
                 int *nr, int *mr) {
  if (m1 != n2)
    return 1;
  *nr = n1;
  *mr = m2;
  *res = alloc_matrix(*nr, *mr);
  if (!*res)
    return 1;

  for (int i = 0; i < *nr; i++) {
    for (int j = 0; j < *mr; j++) {
      (*res)[i][j] = 0;
      for (int k = 0; k < m1; k++) {
        (*res)[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return 0;
}

int transpose_matrix(int **a, int n, int m, int ***res, int *nr, int *mr) {
  *nr = m;
  *mr = n;
  *res = alloc_matrix(*nr, *mr);
  if (!*res)
    return 1;

  for (int i = 0; i < *nr; i++) {
    for (int j = 0; j < *mr; j++) {
      (*res)[i][j] = a[j][i];
    }
  }
  return 0;
}
