#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../validators/validator.h"

void runCouriers()
{
    printf("Courier %d statred\n", 1);
}

int main(int argc, char *argv[])
{
    if (!validate_storage(argc, argv))
    {
        return 1;
    }

    const char *config_file_path = argv[1];
    const char *key = argv[2];
    int a = 0, b = 0, c = 0;
    int price_a = 0, price_b = 0, price_c = 0;

    if (!read_config_file(config_file_path, key, &a, &b, &c, &price_a, &price_b, &price_c))
    {
        return 1;
    }

    printf("Storage start state:\n");
    printf("A: %d\n", a);
    printf("B: %d\n", b);
    printf("C: %d\n", c);
    printf("Price A: %d\n", price_a);
    printf("Price B: %d\n", price_b);
    printf("Price C: %d\n", price_c);

    return 0;
}