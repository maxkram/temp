#include <stdio.h>
#include <stdlib.h>

#define NSTAT 100

int input_size(int *N, int *M, int opt);
int input_matrix_static(int matrix[NSTAT][NSTAT], int N, int M);
int input_matrix_dyn(int **matrix, int N, int M);
void output_matrix_static(const int matrix[NSTAT][NSTAT], int N, int M);
void output_matrix_dyn(int **matrix, int N, int M);
int **alloc_dyn_one(int N, int M);
int **alloc_dyn_two(int N, int M);
int **alloc_dyn_three(int N, int M, int **data_ptr);
void free_dyn_two(int **matrix, int N);
void free_dyn_three(int **matrix, int *data);

int handle_static(int N, int M);
int handle_dyn_one(int N, int M);
int handle_dyn_two(int N, int M);
int handle_dyn_three(int N, int M);

int main(void) {
  int opt, N, M;
  int success = 0;
  int valid_input = 0;

  if (scanf("%d", &opt) == 1 && opt > 0 && opt <= 4) {
    if (input_size(&N, &M, opt)) {
      valid_input = 1;
    }
  }
  if (!valid_input) {
    printf("n/a\n");
    return 0;
  }

  if (opt == 1) {
    success = handle_static(N, M);
  } else if (opt == 2) {
    success = handle_dyn_one(N, M);
  } else if (opt == 3) {
    success = handle_dyn_two(N, M);
  } else {
    success = handle_dyn_three(N, M);
  }

  if (!success) {
    printf("n/a\n");
  }
  return 0;
}

int handle_static(int N, int M) {
  int matrix[NSTAT][NSTAT] = {0};
  int success = 1;

  if (!input_matrix_static(matrix, N, M)) {
    success = 0;
  } else {
    output_matrix_static((const int(*)[NSTAT])matrix, N, M);
  }
  return success;
}

int handle_dyn_one(int N, int M) {
  int **matrix = alloc_dyn_one(N, M);
  int success = 0;

  if (matrix != NULL) {
    if (input_matrix_dyn(matrix, N, M)) {
      output_matrix_dyn(matrix, N, M);
      success = 1;
    }
    free(matrix);
  }
  return success;
}

int handle_dyn_two(int N, int M) {
  int **matrix = alloc_dyn_two(N, M);
  int success = 0;

  if (matrix != NULL) {
    if (input_matrix_dyn(matrix, N, M)) {
      output_matrix_dyn(matrix, N, M);
      success = 1;
    }
    free_dyn_two(matrix, N);
  }
  return success;
}

int handle_dyn_three(int N, int M) {
  int *data = NULL;
  int **matrix = alloc_dyn_three(N, M, &data);
  int success = 0;

  if (matrix != NULL && data != NULL) {
    if (input_matrix_dyn(matrix, N, M)) {
      output_matrix_dyn(matrix, N, M);
      success = 1;
    }
    free_dyn_three(matrix, data);
  } else {
    free(data);
    free(matrix);
  }
  return success;
}

int input_size(int *N, int *M, int opt) {
  int success = 0;
  if (scanf("%d %d", N, M) == 2) {
    success = (*N > 0 && *M > 0);
    if (opt == 1) {
      success = success && (*N <= NSTAT && *M <= NSTAT);
    }
  }
  return success;
}

int input_matrix_static(int matrix[NSTAT][NSTAT], int N, int M) {
  int success = 1;
  int i = 0;

  while (i < N && success) {
    int j = 0;
    while (j < M && success) {
      if (scanf("%d", &matrix[i][j]) != 1) {
        success = 0;
      }
      j++;
    }
    i++;
  }
  return success;
}

int input_matrix_dyn(int **matrix, int N, int M) {
  int success = 1;
  int i = 0;

  while (i < N && success) {
    int j = 0;
    while (j < M && success) {
      if (scanf("%d", &matrix[i][j]) != 1) {
        success = 0;
      }
      j++;
    }
    i++;
  }
  return success;
}

void output_matrix_static(const int matrix[NSTAT][NSTAT], int N, int M) {
  int i = 0;
  while (i < N) {
    printf("%d", matrix[i][0]);
    int j = 1;
    while (j < M) {
      printf(" %d", matrix[i][j]);
      j++;
    }
    if (i != N - 1) {
      printf("\n");
    }
    i++;
  }
}

void output_matrix_dyn(int **matrix, int N, int M) {
  int i = 0;
  while (i < N) {
    printf("%d", matrix[i][0]);
    int j = 1;
    while (j < M) {
      printf(" %d", matrix[i][j]);
      j++;
    }
    if (i != N - 1) {
      printf("\n");
    }
    i++;
  }
}

int **alloc_dyn_one(int N, int M) {
  int **matrix = malloc(N * sizeof(int *) + N * M * sizeof(int));
  if (matrix == NULL) {
    return NULL;
  }
  int *data = (int *)(matrix + N);
  int i = 0;
  while (i < N) {
    matrix[i] = data + i * M;
    i++;
  }
  return matrix;
}

int **alloc_dyn_two(int N, int M) {
  int **matrix = malloc(N * sizeof(int *));
  int success = (matrix != NULL);
  int i = 0;

  while (i < N && success) {
    matrix[i] = malloc(M * sizeof(int));
    if (matrix[i] == NULL) {
      success = 0;
      int j = 0;
      while (j < i) {
        free(matrix[j]);
        j++;
      }
      free(matrix);
      matrix = NULL;
    } else {
      i++;
    }
  }
  return matrix;
}

int **alloc_dyn_three(int N, int M, int **data_ptr) {
  int **matrix = NULL;
  int success = 1;

  matrix = malloc(N * sizeof(int *));
  if (matrix == NULL) {
    success = 0;
  }

  if (success) {
    *data_ptr = malloc(N * M * sizeof(int));
    if (*data_ptr == NULL) {
      free(matrix);
      matrix = NULL;
      success = 0;
    }
  } else {
    *data_ptr = NULL;
  }

  if (success) {
    int i = 0;
    while (i < N) {
      matrix[i] = (*data_ptr) + i * M;
      i++;
    }
  }
  return matrix;
}

void free_dyn_two(int **matrix, int N) {
  int i = 0;
  while (i < N) {
    free(matrix[i]);
    i++;
  }
  free(matrix);
}

void free_dyn_three(int **matrix, int *data) {
  free(data);
  free(matrix);
}
