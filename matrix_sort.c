#include <stdio.h>
#include <stdlib.h>

int **alloc_method_1(int N, int M);
int **alloc_method_2(int N, int M);
int **alloc_method_3(int N, int M);
void free_method_2(int **matrix);
void free_method_3(int **matrix, int N);

int input_matrix(int **matrix, int N, int M);
int sum_row(const int *row, int M);
void sort_matrix(int **matrix, int N, int M);
void print_matrix(int **matrix, int N, int M);

int main(void) {
  int method = 0, N = 0, M = 0;
  int **matrix = NULL;
  int success = 1;

  if (scanf("%d", &method) != 1 || method < 1 || method > 3)
    success = 0;
  if (success && (scanf("%d %d", &N, &M) != 2 || N <= 0 || M <= 0))
    success = 0;

  if (success) {
    if (method == 1)
      matrix = alloc_method_1(N, M);
    else if (method == 2)
      matrix = alloc_method_2(N, M);
    else
      matrix = alloc_method_3(N, M);
    success = (matrix != NULL);
  }

  if (success)
    success = input_matrix(matrix, N, M);

  if (success) {
    sort_matrix(matrix, N, M);
    print_matrix(matrix, N, M);
  } else {
    printf("n/a");
  }

  if (matrix != NULL) {
    if (method == 2)
      free_method_2(matrix);
    else if (method == 3)
      free_method_3(matrix, N);
    else
      free(matrix);
  }

  return 0;
}

int **alloc_method_1(int N, int M) {
  int **matrix = NULL;

  matrix = malloc(N * sizeof(int *) + N * M * sizeof(int));
  if (matrix != NULL) {
    int *data = (int *)(matrix + N);
    for (int i = 0; i < N; i++)
      matrix[i] = data + i * M;
  }
  return matrix;
}

int **alloc_method_2(int N, int M) {
  int **matrix = NULL;

  matrix = malloc(N * sizeof(int *));
  if (matrix != NULL) {
    int *data = malloc(N * M * sizeof(int));
    if (data != NULL) {
      for (int i = 0; i < N; i++)
        matrix[i] = data + i * M;
    } else {
      free(matrix);
      matrix = NULL;
    }
  }
  return matrix;
}

int **alloc_method_3(int N, int M) {
  int **matrix = NULL;

  matrix = malloc(N * sizeof(int *));
  if (matrix != NULL) {
    int success = 1;
    int i = 0;

    for (i = 0; i < N && success; i++) {
      matrix[i] = malloc(M * sizeof(int));
      if (matrix[i] == NULL)
        success = 0;
    }

    if (!success) {
      for (int j = 0; j < i; j++)
        free(matrix[j]);
      free(matrix);
      matrix = NULL;
    }
  }
  return matrix;
}

void free_method_2(int **matrix) {
  free(matrix[0]);
  free(matrix);
}

void free_method_3(int **matrix, int N) {
  for (int i = 0; i < N; i++)
    free(matrix[i]);
  free(matrix);
}

int input_matrix(int **matrix, int N, int M) {
  int success = 1;
  for (int i = 0; i < N && success; i++) {
    for (int j = 0; j < M && success; j++) {
      if (scanf("%d", &matrix[i][j]) != 1)
        success = 0;
    }
  }
  return success;
}

int sum_row(const int *row, int M) {
  int sum = 0;
  for (int i = 0; i < M; i++)
    sum += row[i];
  return sum;
}

void sort_matrix(int **matrix, int N, int M) {
  for (int i = 0; i < N - 1; i++) {
    for (int j = 0; j < N - 1 - i; j++) {
      if (sum_row(matrix[j], M) > sum_row(matrix[j + 1], M)) {
        int *tmp = matrix[j];
        matrix[j] = matrix[j + 1];
        matrix[j + 1] = tmp;
      }
    }
  }
}

void print_matrix(int **matrix, int N, int M) {
  for (int i = 0; i < N; i++) {
    printf("%d", matrix[i][0]);
    for (int j = 1; j < M; j++)
      printf(" %d", matrix[i][j]);
    if (i != N - 1)
      printf("\n");
  }
}