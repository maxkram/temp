#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100

int inputProperties(int *type, int *rows, int *cols);
int **alloc_single(int rows, int cols, int *error);
int **alloc_double(int rows, int cols, int *error);
int **alloc_multiple(int rows, int cols, int *error);
void free_matrix(int **matrix, int rows, int type);
int input_matrix(int **matrix, int rows, int cols, int type);
void output_matrix(int **matrix, int rows, int cols, const int *rowmax,
                   const int *colmin, int type, int error);
int compute_min_max(int **matrix, int rows, int cols, int **rowmax,
                    int **colmin, int type);

int main(void) {
  int type = 0, rows = 0, cols = 0, error = 0, success = 0;
  int **matrix = NULL;
  int *rowmax = NULL, *colmin = NULL;

  success = inputProperties(&type, &rows, &cols);

  if (success) {
    if (type == 1) {
      static int stat_matrix[MAXSIZE][MAXSIZE];
      matrix = (int **)stat_matrix;
    } else if (type == 2)
      matrix = alloc_single(rows, cols, &error);
    else if (type == 3)
      matrix = alloc_double(rows, cols, &error);
    else if (type == 4)
      matrix = alloc_multiple(rows, cols, &error);

    success = (error == 0 && matrix != NULL);
  }

  if (success)
    success = input_matrix(matrix, rows, cols, type);
  if (success)
    success =
        (compute_min_max(matrix, rows, cols, &rowmax, &colmin, type) == 0);
  if (success)
    output_matrix(matrix, rows, cols, rowmax, colmin, type, 0);
  if (!success)
    printf("n/a");

  free(rowmax);
  free(colmin);
  if (type != 1)
    free_matrix(matrix, rows, type);

  return 0;
}

int inputProperties(int *type, int *rows, int *cols) {
  int result = 1;

  if (scanf("%d", type) != 1 || *type < 1 || *type > 4)
    result = 0;
  if (result && scanf("%d %d", rows, cols) != 2)
    result = 0;
  if (result && (*rows <= 0 || *cols <= 0))
    result = 0;
  if (result && *type == 1 && (*rows > MAXSIZE || *cols > MAXSIZE))
    result = 0;

  return result;
}

int **alloc_single(int rows, int cols, int *error) {
  int **matrix = malloc(rows * sizeof(int *) + rows * cols * sizeof(int));

  if (matrix != NULL) {
    int *data = (int *)(matrix + rows);
    for (int i = 0; i < rows; i++)
      matrix[i] = data + i * cols;
  } else {
    *error = 1;
  }

  return matrix;
}

int **alloc_double(int rows, int cols, int *error) {
  int **matrix = NULL;
  int *data = NULL;

  matrix = malloc(rows * sizeof(int *));
  if (matrix != NULL) {
    data = malloc(rows * cols * sizeof(int));
    if (data != NULL) {
      for (int i = 0; i < rows; i++)
        matrix[i] = data + i * cols;
    } else {
      free(matrix);
      matrix = NULL;
    }
  }

  if (matrix == NULL)
    *error = 1;
  return matrix;
}

int **alloc_multiple(int rows, int cols, int *error) {
  int **matrix = NULL;
  int i = 0;
  int err_flag = 0;

  matrix = malloc(rows * sizeof(int *));
  if (matrix != NULL) {
    for (i = 0; i < rows && !err_flag; i++) {
      matrix[i] = malloc(cols * sizeof(int));
      if (matrix[i] == NULL)
        err_flag = 1;
    }

    if (err_flag) {
      for (int j = 0; j < i; j++)
        free(matrix[j]);
      free(matrix);
      matrix = NULL;
    }
  }

  if (matrix == NULL)
    *error = 1;
  return matrix;
}

void free_matrix(int **matrix, int rows, int type) {
  if (type == 2)
    free(matrix);
  else if (type == 3) {
    free(matrix[0]);
    free(matrix);
  } else if (type == 4) {
    for (int i = 0; i < rows; i++)
      free(matrix[i]);
    free(matrix);
  }
}

int input_matrix(int **matrix, int rows, int cols, int type) {
  int result = 1;

  for (int i = 0; i < rows && result; i++) {
    for (int j = 0; j < cols && result; j++) {
      int *elem =
          (type == 1) ? (int *)(matrix + i * cols + j) : &(matrix[i][j]);
      if (scanf("%d", elem) != 1)
        result = 0;
    }
  }

  return result;
}

int compute_min_max(int **matrix, int rows, int cols, int **rowmax,
                    int **colmin, int type) {
  int result = 0;

  *rowmax = malloc(rows * sizeof(int));
  *colmin = malloc(cols * sizeof(int));

  if (*rowmax == NULL || *colmin == NULL) {
    free(*rowmax);
    free(*colmin);
    *rowmax = NULL;
    *colmin = NULL;
    result = 1;
  }

  if (result == 0) {
    for (int i = 0; i < rows; i++) {
      int first_element =
          (type == 1) ? *((int *)(matrix) + i * cols) : matrix[i][0];
      (*rowmax)[i] = first_element;
    }

    for (int j = 0; j < cols; j++) {
      int first_element = (type == 1) ? *((int *)(matrix) + j) : matrix[0][j];
      (*colmin)[j] = first_element;
    }

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        int current =
            (type == 1) ? *((int *)(matrix) + i * cols + j) : matrix[i][j];
        if (current > (*rowmax)[i])
          (*rowmax)[i] = current;
        if (current < (*colmin)[j])
          (*colmin)[j] = current;
      }
    }
  }

  return result;
}

void output_matrix(int **matrix, int rows, int cols, const int *rowmax,
                   const int *colmin, int type, int error) {
  if (error) {
    printf("n/a");
    return;
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      const int *elem =
          (type == 1) ? (int *)(matrix + i * cols + j) : &(matrix[i][j]);
      printf("%d", *elem);
      if (j != cols - 1)
        printf(" ");
    }
    if (i != rows - 1)
      printf("\n");
  }

  printf("\n");

  for (int i = 0; i < rows; i++) {
    printf("%d", rowmax[i]);
    if (i != rows - 1)
      printf(" ");
  }

  printf("\n");

  for (int j = 0; j < cols; j++) {
    printf("%d", colmin[j]);
    if (j != cols - 1)
      printf(" ");
  }
}