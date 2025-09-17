#include <stdio.h>
#include <stdlib.h>

int read_int(int *n);
int input_array(int *a, int n);
void sort_array(int *a, int n);
void print_array(const int *a, int n);

int main(void)
{
    int n;
    int success = 0;
    int *data = NULL;
    if (read_int(&n))
    {
        data = malloc(n * sizeof(int));
        if (data != NULL)
        {
            if (input_array(data, n))
            {
                sort_array(data, n);
                print_array(data, n);
                success = 1;
            }
            free(data);
        }
    }
    if (!success)
    {
        printf("n/a");
    }
    return 0;
}

int read_int(int *n)
{
    int res = 0;
    if (n != NULL && scanf("%d", n) == 1 && *n > 0)
    {
        res = 1;
    }
    return res;
}

int input_array(int *a, int n)
{
    int ok = 1;
    for (int i = 0; i < n && ok; i++)
    {
        if (scanf("%d", &a[i]) != 1)
        {
            ok = 0;
        }
    }
    return ok;
}

void sort_array(int *a, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }
}

void print_array(const int *a, int n)
{
    if (n > 0)
    {
        printf("%d", a[0]);
        for (int i = 1; i < n; i++)
        {
            printf(" %d", a[i]);
        }
    }
}
