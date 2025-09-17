#include <stdio.h>
#include <stdlib.h>

int read_int(int *n);
int input_array(int *a, int n);
void sort_array(int *a, int n);
void print_array(const int *a, int n);

int main(void)
{
    int n;
    int ret = 0;
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
                ret = 1;
            }
            free(data);
        }
    }
    if (!ret)
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
    int i = 0;
    while (i < n && ok)
    {
        if (scanf("%d", &a[i]) != 1)
        {
            ok = 0;
        }
        i++;
    }
    return ok;
}

void sort_array(int *a, int n)
{
    int i = 0;
    while (i < n - 1)
    {
        int j = 0;
        int swapped = 0;
        while (j < n - i - 1 && !swapped)
        {
            if (a[j] > a[j + 1])
            {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
            j++;
        }
        i++;
    }
}

void print_array(const int *a, int n)
{
    int i = 0;
    if (n > 0)
    {
        printf("%d", a[0]);
        i = 1;
        while (i < n)
        {
            printf(" %d", a[i]);
            i++;
        }
    }
}
