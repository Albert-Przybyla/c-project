#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "utils.h"

bool validate_arguments(int argc, char *argv[], int *order_count, int *max_a, int *max_b, int *max_c)
{
    if (argc != 6)
    {
        printf("Użycie: %s <string> <int> <int> <int> <int>\n", argv[0]);
        return false;
    }

    for (int i = 2; i < argc; i++)
    {
        if (!is_number(argv[i]))
        {
            printf("Błąd: Argument %d ('%s') nie jest liczbą.\n", i, argv[i]);
            return false;
        }
    }

    *order_count = atoi(argv[2]);
    *max_a = atoi(argv[3]);
    *max_b = atoi(argv[4]);
    *max_c = atoi(argv[5]);

    if (*order_count <= 0 || *max_a <= 0 || *max_b <= 0 || *max_c <= 0)
    {
        printf("Błąd: Wszystkie liczby muszą być większe od 0.\n");
        return false;
    }

    return true;
}