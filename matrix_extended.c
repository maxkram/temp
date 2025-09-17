#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100

int inputProperties(int *type, int *rows, int *cols);
int **alloc_single(int rows, int cols, int *error);
int **alloc_double(int rows, int cols, int *error);
int **alloc_multiple(int rows, int cols, int *error);
void free_matrix(int **matrix, int rows, int type);
int input_matrix(int **matrix, int rows, int cols, int type);
void output_matrix(int **matrix, int rows, int cols, int *rowmax, int *colmin, int type, int error);
int compute_min_max(int **matrix, int rows, int cols, int **rowmax, int **colmin, int type);

int main(void)
{
    int type = 0, rows = 0, cols = 0, error = 0, success = 0;
    int **matrix = NULL;
    int *rowmax = NULL, *colmin = NULL;

    if (inputProperties(&type, &rows, &cols))
    {
        if (type == 1)
        {
            static int stat_matrix[MAXSIZE][MAXSIZE];
            matrix = (int **)stat_matrix;
        }
        else if (type == 2)
        {
            matrix = alloc_single(rows, cols, &error);
        }
        else if (type == 3)
        {
            matrix = alloc_double(rows, cols, &error);
        }
        else if (type == 4)
        {
            matrix = alloc_multiple(rows, cols, &error);
        }

        if (error == 0 && matrix != NULL)
        {
            if (input_matrix(matrix, rows, cols, type))
            {
                if (compute_min_max(matrix, rows, cols, &rowmax, &colmin, type) == 0)
                {
                    output_matrix(matrix, rows, cols, rowmax, colmin, type, 0);
                    success = 1;
                }
            }
        }
    }

    if (!success)
    {
        printf("n/a");
    }

    free(rowmax);
    free(colmin);
    if (type != 1)
    {
        free_matrix(matrix, rows, type);
    }

    return 0;
}

int inputProperties(int *type, int *rows, int *cols)
{
    int err = 0;
    if (scanf("%d", type) != 1 || *type < 1 || *type > 4)
    {
        err = 1;
    }
    if (err == 0 && scanf("%d %d", rows, cols) != 2)
    {
        err = 1;
    }
    if (err == 0 && (*rows <= 0 || *cols <= 0))
    {
        err = 1;
    }
    if (err == 0 && *type == 1 && (*rows > MAXSIZE || *cols > MAXSIZE))
    {
        err = 1;
    }
    return (err == 0);
}

int **alloc_single(int rows, int cols, int *error)
{
    int **matrix = malloc(rows * sizeof(int *) + rows * cols * sizeof(int));
    if (!matrix)
    {
        *error = 1;
        return NULL;
    }
    int *data = (int *)(matrix + rows);
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = data + i * cols;
    }
    return matrix;
}

int **alloc_double(int rows, int cols, int *error)
{
    int **matrix = malloc(rows * sizeof(int *));
    int *data = malloc(rows * cols * sizeof(int));
    if (!matrix || !data)
    {
        free(matrix);
        free(data);
        *error = 1;
        return NULL;
    }
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = data + i * cols;
    }
    return matrix;
}

int **alloc_multiple(int rows, int cols, int *error)
{
    int **matrix = malloc(rows * sizeof(int *));
    if (!matrix)
    {
        *error = 1;
        return NULL;
    }
    int i = 0;
    int err_flag = 0;
    while (i < rows && err_flag == 0)
    {
        matrix[i] = malloc(cols * sizeof(int));
        if (!matrix[i])
        {
            err_flag = 1;
        }
        i++;
    }
    if (err_flag)
    {
        for (int j = 0; j < i - 1; j++)
        {
            free(matrix[j]);
        }
        free(matrix);
        *error = 1;
        return NULL;
    }
    return matrix;
}

void free_matrix(int **matrix, int rows, int type)
{
    if (type == 2)
    {
        free(matrix);
    }
    else if (type == 3)
    {
        free(matrix[0]);
        free(matrix);
    }
    else if (type == 4)
    {
        for (int i = 0; i < rows; i++)
        {
            free(matrix[i]);
        }
        free(matrix);
    }
}

int input_matrix(int **matrix, int rows, int cols, int type)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int *elem = NULL;
            if (type == 1)
            {
                elem = (int *)(matrix + i * cols + j);
            }
            else
            {
                elem = &(matrix[i][j]);
            }
            if (scanf("%d", elem) != 1)
            {
                return 0;
            }
        }
    }
    return 1;
}

int compute_min_max(int **matrix, int rows, int cols, int **rowmax, int **colmin, int type)
{
    *rowmax = malloc(rows * sizeof(int));
    *colmin = malloc(cols * sizeof(int));
    if (!*rowmax || !*colmin)
    {
        free(*rowmax);
        free(*colmin);
        return 1;
    }

    for (int i = 0; i < rows; i++)
    {
        int first_element = (type == 1) ? *((int *)(matrix) + i * cols) : matrix[i][0];
        (*rowmax)[i] = first_element;
    }

    for (int j = 0; j < cols; j++)
    {
        int first_element = (type == 1) ? *((int *)(matrix) + j) : matrix[0][j];
        (*colmin)[j] = first_element;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int current = (type == 1) ? *((int *)(matrix) + i * cols + j) : matrix[i][j];
            if (current > (*rowmax)[i])
            {
                (*rowmax)[i] = current;
            }
            if (current < (*colmin)[j])
            {
                (*colmin)[j] = current;
            }
        }
    }
    return 0;
}

void output_matrix(int **matrix, int rows, int cols, int *rowmax, int *colmin, int type, int error)
{
    if (error)
    {
        printf("n/a");
        return;
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int *elem = (type == 1) ? (int *)(matrix + i * cols + j) : &(matrix[i][j]);
            printf("%d", *elem);
            if (j != cols - 1)
            {
                printf(" ");
            }
        }
        if (i != rows - 1)
        {
            printf("\n");
        }
    }

    printf("\n");

    for (int i = 0; i < rows; i++)
    {
        printf("%d", rowmax[i]);
        if (i != rows - 1)
        {
            printf(" ");
        }
    }

    printf("\n");

    for (int j = 0; j < cols; j++)
    {
        printf("%d", colmin[j]);
        if (j != cols - 1)
        {
            printf(" ");
        }
    }
}
