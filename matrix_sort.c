#include <stdio.h>
#include <stdlib.h>

int **alloc_method_1(int N, int M); // malloc одного блока для указателей и данных
int **alloc_method_2(int N, int M); // malloc массив указателей и malloc один блок для данных
int **alloc_method_3(int N, int M); // malloc массив указателей и для каждой строки отдельный malloc
void free_method_2(int **matrix);
void free_method_3(int **matrix, int N);

int input_matrix(int **matrix, int N, int M);
int sum_row(int *row, int M);
void sort_matrix(int **matrix, int N, int M);
void print_matrix(int **matrix, int N, int M);

int main(void)
{
    int method = 0, N = 0, M = 0;
    int **matrix = NULL;
    if (scanf("%d", &method) != 1 || method < 1 || method > 3)
    {
        printf("n/a");
        return 0;
    }
    if (scanf("%d %d", &N, &M) != 2 || N <= 0 || M <= 0)
    {
        printf("n/a");
        return 0;
    }

    if (method == 1)
    {
        matrix = alloc_method_1(N, M);
    }
    else if (method == 2)
    {
        matrix = alloc_method_2(N, M);
    }
    else
    {
        matrix = alloc_method_3(N, M);
    }
    if (!matrix)
    {
        printf("n/a");
        return 0;
    }
    if (!input_matrix(matrix, N, M))
    {
        printf("n/a");
        if (method == 2)
            free_method_2(matrix);
        else if (method == 3)
            free_method_3(matrix, N);
        else
            free(matrix);
        return 0;
    }

    sort_matrix(matrix, N, M);
    print_matrix(matrix, N, M);

    // очистка памяти
    if (method == 2)
        free_method_2(matrix);
    else if (method == 3)
        free_method_3(matrix, N);
    else
        free(matrix);
    return 0;
}

int **alloc_method_1(int N, int M)
{
    int **matrix = malloc(N * sizeof(int *) + N * M * sizeof(int));
    if (!matrix)
        return NULL;
    int *data = (int *)(matrix + N);
    for (int i = 0; i < N; i++)
        matrix[i] = data + i * M;
    return matrix;
}

int **alloc_method_2(int N, int M)
{
    int **matrix = malloc(N * sizeof(int *));
    int *data = malloc(N * M * sizeof(int));
    if (!matrix || !data)
    {
        free(matrix);
        free(data);
        return NULL;
    }
    for (int i = 0; i < N; i++)
        matrix[i] = data + i * M;
    return matrix;
}

int **alloc_method_3(int N, int M)
{
    int **matrix = malloc(N * sizeof(int *));
    if (!matrix)
        return NULL;
    for (int i = 0; i < N; i++)
    {
        matrix[i] = malloc(M * sizeof(int));
        if (!matrix[i])
        {
            for (int j = 0; j < i; j++)
                free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_method_2(int **matrix)
{
    free(matrix[0]);
    free(matrix);
}

void free_method_3(int **matrix, int N)
{
    for (int i = 0; i < N; i++)
        free(matrix[i]);
    free(matrix);
}

int input_matrix(int **matrix, int N, int M)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (scanf("%d", &matrix[i][j]) != 1)
                return 0;
    return 1;
}

int sum_row(int *row, int M)
{
    int sum = 0;
    for (int i = 0; i < M; i++)
        sum += row[i];
    return sum;
}

void sort_matrix(int **matrix, int N, int M)
{
    // Сортируем строки матрицы по сумме элементов методом пузырька (по указателям)
    for (int i = 0; i < N - 1; i++)
    {
        for (int j = 0; j < N - 1 - i; j++)
        {
            if (sum_row(matrix[j], M) > sum_row(matrix[j + 1], M))
            {
                int *tmp = matrix[j];
                matrix[j] = matrix[j + 1];
                matrix[j + 1] = tmp;
            }
        }
    }
}

void print_matrix(int **matrix, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        printf("%d", matrix[i][0]);
        for (int j = 1; j < M; j++)
            printf(" %d", matrix[i][j]);
        if (i != N - 1)
            printf("\n");
    }
}
