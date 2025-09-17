#include <stdio.h>
#include <stdlib.h>

#define NSTAT 100

int input_size(int *N, int *M, int opt);
int input_matrix_static(int matrix[NSTAT][NSTAT], int N, int M);
int input_matrix_dyn(int **matrix, int N, int M);
void output_matrix_static(int matrix[NSTAT][NSTAT], int N, int M);
void output_matrix_dyn(int **matrix, int N, int M);
int **alloc_dyn_one(int N, int M);
int **alloc_dyn_two(int N, int M);
int **alloc_dyn_three(int N, int M, int **data_ptr);
void free_dyn_two(int **matrix, int N);
void free_dyn_three(int **matrix, int *data);

int main(void)
{
    int opt, N, M;
    int success = 0;
    if (scanf("%d", &opt) != 1 || opt <= 0 || opt > 4)
    {
        printf("n/a");
        return 0;
    }
    if (!input_size(&N, &M, opt))
    {
        printf("n/a");
        return 0;
    }

    if (opt == 1)
    {
        int matrix[NSTAT][NSTAT] = {0};
        if (input_matrix_static(matrix, N, M))
        {
            output_matrix_static(matrix, N, M);
            success = 1;
        }
    }
    else if (opt == 2)
    {
        int **matrix = alloc_dyn_one(N, M);
        if (matrix)
        {
            if (input_matrix_dyn(matrix, N, M))
            {
                output_matrix_dyn(matrix, N, M);
                success = 1;
            }
            free(matrix);
        }
    }
    else if (opt == 3)
    {
        int **matrix = alloc_dyn_two(N, M);
        if (matrix)
        {
            if (input_matrix_dyn(matrix, N, M))
            {
                output_matrix_dyn(matrix, N, M);
                success = 1;
            }
            free_dyn_two(matrix, N);
        }
    }
    else
    {
        int *data = NULL;
        int **matrix = alloc_dyn_three(N, M, &data);
        if (matrix && data)
        {
            if (input_matrix_dyn(matrix, N, M))
            {
                output_matrix_dyn(matrix, N, M);
                success = 1;
            }
            free_dyn_three(matrix, data);
        }
        else
        {
            free(data);
            free(matrix);
        }
    }

    if (!success)
    {
        printf("n/a");
    }
    return 0;
}

int input_size(int *N, int *M, int opt)
{
    if (scanf("%d %d", N, M) != 2)
    {
        return 0;
    }
    int valid = (*N > 0 && *M > 0);
    if (opt == 1)
    {
        valid = valid && (*N <= NSTAT && *M <= NSTAT);
    }
    return valid;
}

int input_matrix_static(int matrix[NSTAT][NSTAT], int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (scanf("%d", &matrix[i][j]) != 1)
            {
                return 0;
            }
        }
    }
    return 1;
}

int input_matrix_dyn(int **matrix, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (scanf("%d", &matrix[i][j]) != 1)
            {
                return 0;
            }
        }
    }
    return 1;
}

void output_matrix_static(int matrix[NSTAT][NSTAT], int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        printf("%d", matrix[i][0]);
        for (int j = 1; j < M; j++)
        {
            printf(" %d", matrix[i][j]);
        }
        if (i != N - 1)
        {
            printf("\n");
        }
    }
}

void output_matrix_dyn(int **matrix, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        printf("%d", matrix[i][0]);
        for (int j = 1; j < M; j++)
        {
            printf(" %d", matrix[i][j]);
        }
        if (i != N - 1)
        {
            printf("\n");
        }
    }
}

int **alloc_dyn_one(int N, int M)
{
    int **matrix = malloc(N * sizeof(int *) + N * M * sizeof(int));
    if (!matrix)
        return NULL;
    int *data = (int *)(matrix + N);
    for (int i = 0; i < N; i++)
    {
        matrix[i] = data + i * M;
    }
    return matrix;
}

int **alloc_dyn_two(int N, int M)
{
    int **matrix = malloc(N * sizeof(int *));
    if (!matrix)
        return NULL;
    int i = 0;
    while (i < N)
    {
        matrix[i] = malloc(M * sizeof(int));
        if (!matrix[i])
        {
            int j = 0;
            while (j < i)
            {
                free(matrix[j]);
                j++;
            }
            free(matrix);
            return NULL;
        }
        i++;
    }
    return matrix;
}

int **alloc_dyn_three(int N, int M, int **data_ptr)
{
    int **matrix = malloc(N * sizeof(int *));
    if (!matrix)
        return NULL;
    *data_ptr = malloc(N * M * sizeof(int));
    if (!(*data_ptr))
    {
        free(matrix);
        return NULL;
    }
    for (int i = 0; i < N; i++)
    {
        matrix[i] = (*data_ptr) + i * M;
    }
    return matrix;
}

void free_dyn_two(int **matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void free_dyn_three(int **matrix, int *data)
{
    free(data);
    free(matrix);
}
