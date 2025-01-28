#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_config_file(const char *file_path, const char *key, int *a, int *b, int *c)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        printf("Nie można otworzyć pliku: %s\n", file_path);
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, "x");
        if (token)
        {
            int quantity = atoi(token);
            token = strtok(NULL, "\n");

            if (token && strcmp(token, "A") == 0)
            {
                *a = quantity;
            }
            else if (token && strcmp(token, "B") == 0)
            {
                *b = quantity;
            }
            else if (token && strcmp(token, "C") == 0)
            {
                *c = quantity;
            }
        }
    }

    fclose(file);
    return 1;
}