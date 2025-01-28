#include <stdio.h>
#include "validators/validator.h"

int main(int argc, char *argv[])
{
    char *key;
    int order_count, max_a, max_b, max_c;

    if (!validate_arguments(argc, argv, &order_count, &max_a, &max_b, &max_c))
    {
        return 1;
    }

    key = argv[1];

    printf("Key: %s\n", key);
    printf("Order Count: %d\n", order_count);
    printf("Max A: %d, Max B: %d, Max C: %d\n", max_a, max_b, max_c);

    return 0;
}