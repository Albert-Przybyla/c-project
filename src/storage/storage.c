#include <stdio.h>
#include <stdlib.h>
#include "../validators/validator.h"

int main(int argc, char *argv[])
{
    if (!validate_storage(argc, argv))
    {
        return 1;
    }

    const char *config_file_path = argv[1];
    const char *key = argv[2];
    int a = 0, b = 0, c = 0;

    if (!read_config_file(config_file_path, key, &a, &b, &c))
    {
        return 1;
    }

    printf("Surowce w magazynach:\n");
    printf("A: %d\n", a);
    printf("B: %d\n", b);
    printf("C: %d\n", c);

    return 0;
}